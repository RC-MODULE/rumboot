#! /bin/csh -f

#Variables for working with ISS and RiscWatch
ISS_PATH := /opt/pcad/RISCWatch/IBM/ppc-mc-iss/linux/model/
ISS := $(ISS_PATH)ppciss
ICF_PATH := ${ISS_PATH}iss_oi10.icf
RW_PATH := /opt/pcad/RISCWatch/
RWCD := ./rwcd
BUILD_DIR := /home/s.chernousov/workspace/OI-10-ISS/build
CMD_PATH := ${BUILD_DIR}"/rumboot-oi10-Debug/testiss.cmd"

INIT_BIN_START_ADDR := 0xFFFF0000
BIN_START_ADDR := 0xC0010000
COMPARE_MEM_START_ADDR := 0xC0028000
COMPARE_MEM_LEN_BYTES := 0x8000
ASM_STEP = 70000

#run rwcd
xterm -e "${ISS} ${ICF_PATH}"& 

#Start creating ISS command file...
#regs & tlb
echo "exec sim_ppc_isa.rwc" > ${CMD_PATH}
#load bin
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-bootrom-stub.bin "${INIT_BIN_START_ADDR} >> ${CMD_PATH}
echo "load bin "${BUILD_DIR}"/rumboot-oi10-Debug/rumboot-oi10-Debug-iss-iram-testiss.bin "${BIN_START_ADDR} >> ${CMD_PATH}

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

#save dump
echo "save mem "${BUILD_DIR}"/rumboot-oi10-Debug/iss_gold_mem.dmp " ${COMPARE_MEM_START_ADDR} ${COMPARE_MEM_LEN_BYTES} >> ${CMD_PATH}
#Successfully created ISS command file...

sleep 1
echo "Run RiscWatch..."
cd ${RW_PATH}
${RWCD} ${CMD_PATH}

exit 0
