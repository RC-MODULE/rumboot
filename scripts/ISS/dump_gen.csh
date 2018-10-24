#! /bin/csh -f

if ("$1" == "")  then
  echo "ERROR: first argument is empty, need set path to the BUILD_DIR"
  exit 1
endif

if ("$2" == "")  then
  echo "ERROR: second argument is empty, need set TEST_NAME"
  exit 1
endif

if ("$3" == "")  then
  echo "ERROR: third argument is empty, need set SCRIPT_DIR"
  exit 1
endif

set BUILD_DIR=$1
set TEST_NAME=$2
set SCRIPT_DIR=$3
set LOG_DIR=$PWD
set CMD_PATH=${LOG_DIR}/${TEST_NAME}.cmd
set DMP_PATH=${LOG_DIR}/test_iss_data_gold.dmp

if ( -f test_iss_data.dmp) then
    set lines=`cat test_iss_data.dmp`
    set COMPARE_MEM_START_ADDR=$lines[1]
    set COMPARE_MEM_LEN_BYTES=$lines[2]
    sed -i '1,2d' test_iss_data.dmp
else
    echo "ERROR: test iss data dump not found"
    exit 1
endif

echo "Start Script"

#Variables for working with ISS and RiscWatch
set ISS_PATH=/opt/pcad/RISCWatch/IBM/ppc-mc-iss/linux/model
set ISS=${ISS_PATH}/ppciss
set ICF_PATH=${SCRIPT_DIR}/iss_oi10.icf
set RW_PATH=/opt/pcad/RISCWatch
set RWCD=./rwcd


set INIT_BIN_START_ADDR=0xFFFF0000
set BIN_START_ADDR=0x80000000
set ASM_STEP=130000

rm -f ${CMD_PATH}
rm -f ${DMP_PATH}

echo "Run simulator"

xterm -e ${ISS} ${ICF_PATH} & 

echo "Create ISS command file"

echo "exec "${SCRIPT_DIR}"/sim_oi10.rwc" >> ${CMD_PATH}

echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/"${TEST_NAME}".bin "${BIN_START_ADDR} >> ${CMD_PATH}

#triple bootrom (workaround)
echo "write tlb 0 0 V 0x0" >> ${CMD_PATH}
#for ROM mirror
echo "write tlb 0 0 EPN 0xFFFF0000 V 0x1 TS 0x0 S 0x03 T 0x0000 E+RPN 0x01FFFFF0000 IL1ID b11 U0123 b0000 WIMG b0111 E 0x0 UXWR b000 SXWR b111 BE0_5 0x-" >> ${CMD_PATH}
echo "memacc add 0xFFFF0000 0xFFFFFFFF RW 1 MEM 0x1FFFFF0000" >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
#for NOR mirror
echo "write tlb 0 0 EPN 0xFFFF0000 V 0x1 TS 0x0 S 0x03 T 0x0000 E+RPN 0x0007FFF0000 IL1ID b11 U0123 b0000 WIMG b0111 E 0x0 UXWR b000 SXWR b111 BE0_5 0x-" >> ${CMD_PATH}
echo "memacc add 0x7FFF0000 0xFFFFFFFF RW 1 MEM 0x007FFF0000" >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "exec "${SCRIPT_DIR}"/postload_oi10.rwc" >> ${CMD_PATH}
#end workaround

@ MAX_ASMSTEP = 65535
@ tmp = ${ASM_STEP}
while ($tmp > 0)
    if ($tmp > $MAX_ASMSTEP) then
        echo "asmstep "$MAX_ASMSTEP >> ${CMD_PATH}
        @ tmp = ${ASM_STEP} - $MAX_ASMSTEP
    else
        echo "asmstep "$tmp >> ${CMD_PATH}
        @ tmp = 0
    endif
end

echo "save mem "${DMP_PATH}" "${COMPARE_MEM_START_ADDR} ${COMPARE_MEM_LEN_BYTES} >> ${CMD_PATH}

sleep 1
echo "Run RiscWatch"
cd ${RW_PATH}
${RWCD} ${CMD_PATH}

sleep 1
echo "Compare dumps..."
cd ${LOG_DIR}

sed -i '1,4d' test_iss_data_gold.dmp
diff -Biw test_iss_data_gold.dmp test_iss_data.dmp > diff_log.txt
if ( -z diff_log.txt )  then
    echo "SUCCESSFULL: dumps are equal"
    exit 0
  else
    echo "ERROR: dumps not equal, see diff_log.txt"
    exit 1
endif


