#! /bin/csh -f

if ("$1" == "")  then
  echo "ERROR: first argument is empty, need set path to the BUILD_DIR"
  exit 1
endif

if ("$2" == "")  then
  echo "ERROR: second argument is empty, need set TEST_NAME"
  exit 1
endif

#Variables for working with ISS and RiscWatch
set ISS_PATH=/opt/pcad/RISCWatch/IBM/ppc-mc-iss/linux/model/
set ISS=${ISS_PATH}ppciss
set ICF_PATH=${ISS_PATH}iss_oi10.icf
set RW_PATH=/opt/pcad/RISCWatch
set RWCD=./rwcd

set BUILD_DIR=$1
set TEST_NAME=$2
set CMD_PATH=${BUILD_DIR}/rumboot-oi10-Debug/ISS/${TEST_NAME}.cmd

set INIT_BIN_START_ADDR=0xFFFF0000
set BIN_START_ADDR=0x80000000
set COMPARE_MEM_START_ADDR=0x8001E000
set COMPARE_MEM_LEN_BYTES=0x1000
set ASM_STEP=130000

echo "Delete old files"

mkdir ${BUILD_DIR}/rumboot-oi10-Debug/ISS/
rm ${CMD_PATH}
rm ${BUILD_DIR}"/rumboot-oi10-Debug/ISS/"${TEST_NAME}"_gold.dmp"

echo "Run simulator"

xterm -e ${ISS} ${ICF_PATH} & 

echo "Create ISS command file"

echo "exec sim_oi10.rwc" >> ${CMD_PATH}

echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/"${TEST_NAME}".bin "${BIN_START_ADDR} >> ${CMD_PATH}

#double bootrom (workaround)
echo "write tlb 0 0 V 0x0" >> ${CMD_PATH}
echo "write tlb 0 0 EPN 0xFFFF0000 V 0x1 TS 0x0 S 0x03 T 0x0000 E+RPN 0x01FFFFF0000 IL1ID b11 U0123 b0000 WIMG b0111 E 0x0 UXWR b000 SXWR b111 BE0_5 0x-" >> ${CMD_PATH}
echo "memacc add 0xFFFF0000 0xFFFFFFFF RW 1 MEM 0x1FFFFF0000" >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "exec postload_oi10.rwc"

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

echo "save mem "${BUILD_DIR}"/rumboot-oi10-Debug/ISS/"${TEST_NAME}"_gold.dmp "${COMPARE_MEM_START_ADDR} ${COMPARE_MEM_LEN_BYTES} >> ${CMD_PATH}

sleep 1
echo "Run RiscWatch..."
cd ${RW_PATH}
${RWCD} ${CMD_PATH}
exit 0