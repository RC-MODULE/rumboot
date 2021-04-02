SET(RUMBOOT_ARCH riscv)
SET(RUMBOOT_PLATFORM scr1)

set(RUMBOOT_PLATFORM_DEFAULT_LDS scr1/rom.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT top)
set(RUMBOOT_NO_SELFTEST true)

if (RUMBOOT_BUILD_TYPE STREQUAL "Production")
  set(BOOTROM_IFLAGS ${RTLFLAGS} +BOOTMGR_KEEP_DRIVING=1 +GTUBE_ONLY_PRODUCTION_OPCODES)
else()
  set(BOOTROM_IFLAGS ${RTLFLAGS})
endif()

#These are configurations for our binaries
rumboot_add_configuration(
  ROM
  DEFAULT
  SNAPSHOT top
  LDS scr1/rom.lds
  LDFLAGS "-e _start"
  CFLAGS -DRUMBOOT_MAIN_NORETURN
  PREFIX ROM
  TIMEOUT_CTEST 200000
  TIMEOUT 1500 ms
  LOAD IRAM_IMAGE SELF
  IRUN_FLAGS ${BOOTROM_IFLAGS}
  FEATURES ROMGEN
  # FILES scr1/targets/simple-rom/common_for_tests.c
)


macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/tools/print-heaps.c
      )
    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom/timer.c
    )

    # quad_spi tests
    # if(DUT STREQUAL "QUAD_SPI_SIMPLE")
    add_rumboot_target(
      CONFIGURATION ROM
      FILES 
        scr1/targets/simple-rom/quad_spi/qspi_write_read_m25p.c # test on quad_spi+flash_m25p10a
      IRUN_FLAGS
        +SPI_FLASH=m25p
    )
    add_rumboot_target(
      CONFIGURATION ROM
      FILES 
        scr1/targets/simple-rom/quad_spi/qspi_loopback.c # loopback test on quad_spi
      IRUN_FLAGS
        +TESTNAME=loopback
    )
    # endif()

    # mdma tests
    # if(DUT STREQUAL "MDMA_SIMPLE")
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/mdma_simple_test/mdma_simple_test.c 
      IRUN_FLAGS +myfile=${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/mdma_simple_test/data
    )
    # endif()

    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/load_bin_example/hello_load_bin.c
      IRUN_FLAGS +myfile=${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/load_bin_example/data
    )

    # VPE example 
    # if(DUT STREQUAL "VPE")
    set(VPE_SIMPLE_PATH src/platform/scr1/targets/simple-rom/nu/vpe_simple)
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/vpe_simple/hello_vpe.c
      IRUN_FLAGS +in_file_tag=${CMAKE_SOURCE_DIR}/${VPE_SIMPLE_PATH}/in_data.bin 
                 +etalon_file_tag=${CMAKE_SOURCE_DIR}/${VPE_SIMPLE_PATH}/etalon.bin
                 +cfg_file_tag=${CMAKE_SOURCE_DIR}/${VPE_SIMPLE_PATH}/Config_vpe1.bin
    )
    # endif()

    set(NA_RM_BIN_PATH ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/npe_rm/rm_core/rtl-tests)
    set(NA_RM_PLUSARGS +in_file_tag=cube.bin
                       +in_ameba_file_tag=cube_ameba.bin
                       +in_with_unused_file_tag=cube_with_unused.bin
                       +warr_file_tag=warr.bin
                       +warr_with_unused_file_tag=warr_with_unused.bin
                       +etalon_file_tag=res.bin
                       +etalon_ameba_file_tag=res_ameba.bin
                       +etalon_with_unused_file_tag=res_with_unused.bin
                       +op0_vec_file_tag=op0.bin
                       +op0_cube_file_tag=op0.bin
                       +op0_ameba_file_tag=op0_ameba.bin
                       +op0_with_unused_file_tag=op0_with_unused.bin
                       +op1_vec_file_tag=op1.bin
                       +op1_cube_file_tag=op1.bin
                       +op1_ameba_file_tag=op1_ameba.bin
                       +op1_with_unused_file_tag=op1_with_unused.bin
                       +op2_vec_file_tag=op2.bin
                       +op2_cube_file_tag=op2.bin
                       +op2_ameba_file_tag=op2_ameba.bin
                       +op2_with_unused_file_tag=op2_with_unused.bin

                       +cfg_file_tag=config_vpe.bin
                       +cfg_mpe_file_tag=config_mpe.bin

                       +metrics_in_tag=cube.bin.metrics
                       +metrics_in_ameba_tag=cube_ameba.bin.metrics
                       +metrics_in_with_unused_tag=cube_with_unused.bin.metrics
                       +metrics_warr_tag=warr.bin.metrics
                       +metrics_warr_with_unused_tag=warr_with_unused.bin.metrics
                       +metrics_etalon_tag=res.bin.metrics
                       +metrics_etalon_ameba_tag=res_ameba.bin.metrics
                       +metrics_etalon_with_unused_tag=res_with_unused.bin.metrics
                       +metrics_op0_vec_tag=op0.bin.metrics
                       +metrics_op0_cube_tag=op0.bin.metrics
                       +metrics_op0_ameba_tag=op0_ameba.bin.metrics
                       +metrics_op0_with_unused_tag=op0_with_unused.bin.metrics
                       +metrics_op1_vec_tag=op1.bin.metrics
                       +metrics_op1_cube_tag=op1.bin.metrics
                       +metrics_op1_ameba_tag=op1_ameba.bin.metrics
                       +metrics_op1_with_unused_tag=op1_with_unused.bin.metrics
                       +metrics_op2_vec_tag=op2.bin.metrics
                       +metrics_op2_cube_tag=op2.bin.metrics
                       +metrics_op2_ameba_tag=op2_ameba.bin.metrics
                       +metrics_op2_with_unused_tag=op2_with_unused.bin.metrics
    )
    set(RM_LOGFILE npe_rm.log)
    set(VPE_COUPLED_PATH src/platform/scr1/targets/simple-rom/nu/vpe_first_coupled)
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/vpe_first_coupled/first_coupled_vpe.c
      PREPCMD ${NA_RM_BIN_PATH}/main_sqared_try_all_ops > ${RM_LOGFILE}
      IRUN_FLAGS ${NA_RM_PLUSARGS}
    )

    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/vpe_first_coupled/first_coupled_mpe.c
      PREPCMD ${NA_RM_BIN_PATH}/main_generate_first_coupled_mpe > ${RM_LOGFILE}
      IRUN_FLAGS ${NA_RM_PLUSARGS}
    )

    # MPE example 
    # if(DUT STREQUAL "MPE")
    set(MPE_SIMPLE_PATH src/platform/scr1/targets/simple-rom/nu/mpe_simple)
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/mpe_simple/hello_mpe.c
      IRUN_FLAGS +in_file_tag=${CMAKE_SOURCE_DIR}/${MPE_SIMPLE_PATH}/in_data.bin 
                 +etalon_file_tag=${CMAKE_SOURCE_DIR}/${MPE_SIMPLE_PATH}/etalon.bin
                 +cfg_file_tag=${CMAKE_SOURCE_DIR}/${MPE_SIMPLE_PATH}/Config_mpe1.bin
    )
    # endif()

    # PPE example 
    # if(DUT STREQUAL "PPE")
    set(PPE_SIMPLE_PATH src/platform/scr1/targets/simple-rom/nu/ppe_simple)
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/ppe_simple/hello_ppe.c
      IRUN_FLAGS +in_file_tag=${CMAKE_SOURCE_DIR}/${PPE_SIMPLE_PATH}/in_data.bin 
                 +etalon_file_tag=${CMAKE_SOURCE_DIR}/${PPE_SIMPLE_PATH}/etalon.bin
                 +cfg_file_tag=${CMAKE_SOURCE_DIR}/${PPE_SIMPLE_PATH}/Config_ppe1.bin
    )
    # endif()

    # demonstrator tests

    # if(DUT STREQUAL "DEMONSTRATOR_SIMPLE")
    set(DEMONSTRATOR_TESTS_DIR ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/demonstrator)
      # Script That  Generate config.ini 
    set(CREATE_INI ${DEMONSTRATOR_TESTS_DIR}/input/create_configini.pl)

    add_rumboot_target(
      CONFIGURATION ROM
      FILES
        ${DEMONSTRATOR_TESTS_DIR}/first_test.c
      IRUN_FLAGS
        +input_file=${DEMONSTRATOR_TESTS_DIR}/input/input_rtl_data_X.bin
      PREPCMD
        ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/matrix_rm/matrix_rm 
          --config=${DEMONSTRATOR_TESTS_DIR}/input/config.ini
          --input_dir=${DEMONSTRATOR_TESTS_DIR}/input/
    )

    add_rumboot_target(
      CONFIGURATION ROM
      FILES
        ${DEMONSTRATOR_TESTS_DIR}/demonstrator_simple_test.c
      IRUN_FLAGS
        +input_data=HoWoRdSdC.bin
        +input_weigth=KRdSdC.bin
        +etalon=HoWoK.bin
      PREPCMD
        cp ${DEMONSTRATOR_TESTS_DIR}/input/config_base.ini . &&
        # perl ${CREATE_INI} 15 32 64 16 all_ones &&
        perl ${CREATE_INI} 15 32 64 16 &&
        ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/matrix_rm/matrix_rm 
          --config=config.ini
          --input_dir=./
    )

    add_rumboot_target(
      CONFIGURATION ROM
      FILES
        ${DEMONSTRATOR_TESTS_DIR}/test_on_vec_op.c
      IRUN_FLAGS
         +input_coef=coef.bin
         +input_data=data.bin
         # +etalon_data=${DEMONSTRATOR_TESTS_DIR}/input/etalon_data.bin
      PREPCMD
        cp ${DEMONSTRATOR_TESTS_DIR}/input/vec_data_template.ini . 
        && ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/bin_generator/bin_generator
            --ini=vec_data_template.ini
            --length=16
            --seed=0
        && cp ${DEMONSTRATOR_TESTS_DIR}/input/vec_coef_template.ini . 
        && ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/bin_generator/bin_generator
            --ini=vec_coef_template.ini
            --length=16
            --seed=0
    )

    add_rumboot_target(
      CONFIGURATION ROM
      FILES
        ${DEMONSTRATOR_TESTS_DIR}/test_on_vec_op_cycle.c
    )

		add_rumboot_target(
      CONFIGURATION ROM
      FILES
        ${DEMONSTRATOR_TESTS_DIR}/demonstrator_pooling_test.c
      IRUN_FLAGS
        +input_data=${DEMONSTRATOR_TESTS_DIR}/input/data.bin
        +etalon_data=${DEMONSTRATOR_TESTS_DIR}/input/etalon_data.bin
    )
	
    # endif()
endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.S
    ${CMAKE_SOURCE_DIR}/src/lib/eventsystem-memory.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/quad_spi.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/lava_demonstrator.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/nu_cpdmac_lib.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/nu_lib.c
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/lib/drivers/nu_test_lib.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-D__RVE_EXT -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las -march=rv32imc -mabi=ilp32")
    SET(CMAKE_C_FLAGS "${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    SET(CMAKE_ASM_FLAGS ${RUMBOOT_COMMON_FLAGS})
    SET(CMAKE_OBJCOPY_FLAGS --gap-fill 0x00 --pad-to 16384)
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -static -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

endmacro()


if (NOT CROSS_COMPILE)
  SET(CROSS_COMPILE riscv32-ncrmnt-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/riscv32-ncrmnt-elf/bin/riscv32-ncrmnt-elf-gcc)
      set(CROSS_COMPILE "/opt/r42/toolchains/riscv32-ncrmnt-elf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
