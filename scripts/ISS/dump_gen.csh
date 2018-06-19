#! /bin/csh -f

if ("$1" == "")  then
  echo "ERROR: empty argument, need path to the BUILD directory"
  exit 1
endif

#Variables for working with ISS and RiscWatch
set ISS_PATH=/opt/pcad/RISCWatch/IBM/ppc-mc-iss/linux/model/
set ISS=${ISS_PATH}ppciss
set ICF_PATH=${ISS_PATH}iss_oi10.icf
set RW_PATH=/opt/pcad/RISCWatch
set RWCD=./rwcd

set BUILD_DIR=$1
set TEST_NAME=testiss
set CMD_PATH=${BUILD_DIR}/rumboot-oi10-Debug/ISS/${TEST_NAME}.cmd

set INIT_BIN_START_ADDR=0xFFFF0000
set BIN_START_ADDR=0xC0010000
set COMPARE_MEM_START_ADDR=0xC002E000
set COMPARE_MEM_LEN_BYTES=0x1000
set ASM_STEP=130000

echo "Delete old files"

mkdir ${BUILD_DIR}/rumboot-oi10-Debug/ISS/
rm ${CMD_PATH}
rm ${BUILD_DIR}"/rumboot-oi10-Debug/ISS/gold_mem_"${TEST_NAME}".dmp"

echo "Run simulator"

#run rwcd
xterm -e ${ISS} ${ICF_PATH} & 

echo "Create ISS commad file"

echo "exec sim_oi10.rwc" >> ${CMD_PATH}

echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-iss-iram-"${TEST_NAME}".bin "${BIN_START_ADDR} >> ${CMD_PATH}

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

echo "save mem "${BUILD_DIR}"/rumboot-oi10-Debug/ISS/gold_mem_"${TEST_NAME}".dmp "${COMPARE_MEM_START_ADDR} ${COMPARE_MEM_LEN_BYTES} >> ${CMD_PATH}

sleep 1
echo "Run RiscWatch..."
cd ${RW_PATH}
${RWCD} ${CMD_PATH}
exit 0