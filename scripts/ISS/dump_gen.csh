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

set DMPOBJ_PATH="${LOG_DIR}/../rumboot-oi10-Debug/${TEST_NAME}.dmp"
set DMPOBJ_BOOT_PATH="${LOG_DIR}/../rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.dmp"

if ( -f ${DMPOBJ_PATH}) then
set MEMSET_ADDR=`grep "<memset>:" ${DMPOBJ_PATH} | cut -c 1-8`
set MEMCPY_ADDR=`grep "<memcpy>:" ${DMPOBJ_PATH} | cut -c 1-8`
set EXIT_ADDR=`grep "<_exit+" ${DMPOBJ_PATH} | cut -c 1-8`
else
    echo "ERROR: ${DMPOBJ_PATH} not found"
    exit 1
endif
if ( -f ${DMPOBJ_BOOT_PATH}) then
set MEMSET_BOOT_ADDR=`grep "<memset>:" ${DMPOBJ_BOOT_PATH} | cut -c 1-8`
else
    echo "ERROR: ${DMPOBJ_BOOT_PATH} not found"
    exit 1
endif

if ("${EXIT_ADDR}" == "") then
    echo "ERROR: exit address not found"
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

${ISS} ${ICF_PATH} & 

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

set TEST1_PATH=${LOG_DIR}/test_data1.dmp
set TEST2_PATH=${LOG_DIR}/test_data2.dmp

rm -f ${TEST1_PATH}
rm -f ${TEST2_PATH}

echo "" >> ${CMD_PATH}
echo "create src" >> ${CMD_PATH}
echo "create dst" >> ${CMD_PATH}
echo "create len" >> ${CMD_PATH}
echo "create val" >> ${CMD_PATH}
echo "create addr" >> ${CMD_PATH}
if ( "${MEMSET_BOOT_ADDR}" != "" ) then
echo "create rom_memset" >> ${CMD_PATH}
endif
if ( "${MEMSET_ADDR}" != "" ) then
echo "create memset" >> ${CMD_PATH}
endif
if ( "${MEMCPY_ADDR}" != "" ) then
echo "create memcpy" >> ${CMD_PATH}
endif
echo "create exit_addr" >> ${CMD_PATH}
echo "" >> ${CMD_PATH}
if ( "${MEMSET_BOOT_ADDR}" != "" ) then
echo "set rom_memset = 0x${MEMSET_BOOT_ADDR}" >> ${CMD_PATH}
endif
if ( "${MEMSET_ADDR}" != "" ) then
echo "set memset = 0x${MEMSET_ADDR}" >> ${CMD_PATH}
endif
if ( "${MEMCPY_ADDR}" != "" ) then
echo "set memcpy = 0x${MEMCPY_ADDR}" >> ${CMD_PATH}
endif
echo "set exit_addr = 0x${EXIT_ADDR}" >> ${CMD_PATH}
echo "" >> ${CMD_PATH}
echo "bpmode hw step" >> ${CMD_PATH}
if ( "${MEMSET_BOOT_ADDR}" != "" ) then
echo "bp set ihw rom_memset" >> ${CMD_PATH}
endif
if ( "${MEMSET_ADDR}" != "" ) then
echo "bp set ihw memset" >> ${CMD_PATH}
endif
if ( "${MEMCPY_ADDR}" != "" ) then
echo "bp set ihw memcpy" >> ${CMD_PATH}
endif
echo "bp set ihw exit_addr" >> ${CMD_PATH}
echo "run" >> ${CMD_PATH}
echo "" >> ${CMD_PATH}
echo "set addr = IAR" >> ${CMD_PATH}
echo "while (addr != exit_addr)" >> ${CMD_PATH}
if ( "${MEMSET_BOOT_ADDR}" != "" ) then
echo " if (addr == rom_memset)" >> ${CMD_PATH}
echo "  set dst = R3" >> ${CMD_PATH}
echo "  set val = R4" >> ${CMD_PATH}
echo "  set len = R5" >> ${CMD_PATH}
echo "  memfill dst len val" >> ${CMD_PATH}
echo " endif" >> ${CMD_PATH}
endif
if ( "${MEMSET_ADDR}" != "" ) then
echo " if (addr == memset)" >> ${CMD_PATH}
echo "  set dst = R3" >> ${CMD_PATH}
echo "  set val = R4" >> ${CMD_PATH}
echo "  set len = R5" >> ${CMD_PATH}
echo "  memfill dst len val" >> ${CMD_PATH}
echo " endif" >> ${CMD_PATH}
endif
if ( "${MEMCPY_ADDR}" != "" ) then
echo " if (addr == memcpy)" >> ${CMD_PATH}
echo "  set dst = R3" >> ${CMD_PATH}
echo "  set src = R4" >> ${CMD_PATH}
echo "  set len = R5" >> ${CMD_PATH}
echo "  memcopy src dst len" >> ${CMD_PATH}
echo " endif" >> ${CMD_PATH}
endif
echo " run" >> ${CMD_PATH}
echo " set addr = IAR" >> ${CMD_PATH}
echo "endwhile" >> ${CMD_PATH}
echo "" >> ${CMD_PATH}
echo "save mem "${TEST1_PATH}" 0x80002a8c 0x80" >> ${CMD_PATH}
echo "save mem "${TEST2_PATH}" 0x80002b0c 0x80" >> ${CMD_PATH}
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


