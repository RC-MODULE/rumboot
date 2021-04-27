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
  add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/ppe_regs/regs_ppe.c
	)
    # files transfered from RM to simulation environment
    set(NA_TEST_num_iterations_file num_iterations.bin)
    set(NA_TEST_in_file cube.bin)
    set(NA_TEST_in_ameba_file cube_ameba.bin)
    set(NA_TEST_in_with_unused_file cube_with_unused.bin)
    set(NA_TEST_warr_file warr.bin)
    set(NA_TEST_warr_with_unused_file warr_with_unused.bin)
    set(NA_TEST_etalon_file etalon.bin)
    set(NA_TEST_etalon_ameba_file etalon_ameba.bin)
    set(NA_TEST_etalon_with_unused_file etalon_with_unused.bin)
    set(NA_TEST_op0_vec_file op0.bin)
    set(NA_TEST_op0_cube_file op0.bin)
    set(NA_TEST_op0_ameba_file op0_ameba.bin)
    set(NA_TEST_op0_with_unused_file op0_with_unused.bin)
    set(NA_TEST_op1_vec_file op1.bin)
    set(NA_TEST_op1_cube_file op1.bin)
    set(NA_TEST_op1_ameba_file op1_ameba.bin)
    set(NA_TEST_op1_with_unused_file op1_with_unused.bin)
    set(NA_TEST_op2_vec_file op2.bin)
    set(NA_TEST_op2_cube_file op2.bin)
    set(NA_TEST_op2_ameba_file op2_ameba.bin)
    set(NA_TEST_op2_with_unused_file op2_with_unused.bin)
    set(NA_TEST_lut1_file lut1.bin)
    set(NA_TEST_lut2_file lut2.bin)

    set(NA_TEST_cfg_file config_vpe.bin)
    set(NA_TEST_cfg_mpe_file config_mpe.bin)
    set(NA_TEST_cfg_ppe_file config_ppe.bin)
    set(NA_TEST_mpe_cmd_file cmd.bin)

    set(NA_TEST_metrics_in ${NA_TEST_in_file}.metrics)
    set(NA_TEST_metrics_in_ameba ${NA_TEST_in_ameba_file}.metrics)
    set(NA_TEST_metrics_in_with_unused ${NA_TEST_in_with_unused_file}.metrics)
    set(NA_TEST_metrics_warr ${NA_TEST_warr_file}.metrics)
    set(NA_TEST_metrics_warr_with_unused ${NA_TEST_warr_with_unused_file}.metrics)
    set(NA_TEST_metrics_etalon ${NA_TEST_etalon_file}.metrics)
    set(NA_TEST_metrics_etalon_ameba ${NA_TEST_etalon_ameba_file}.metrics)
    set(NA_TEST_metrics_etalon_with_unused ${NA_TEST_etalon_with_unused_file}.metrics)
    set(NA_TEST_metrics_op0_vec ${NA_TEST_op0_vec_file}.metrics)
    set(NA_TEST_metrics_op0_cube ${NA_TEST_op0_cube_file}.metrics)
    set(NA_TEST_metrics_op0_ameba ${NA_TEST_op0_ameba_file}.metrics)
    set(NA_TEST_metrics_op0_with_unused ${NA_TEST_op0_with_unused_file}.metrics)
    set(NA_TEST_metrics_op1_vec ${NA_TEST_op1_vec_file}.metrics)
    set(NA_TEST_metrics_op1_cube ${NA_TEST_op1_cube_file}.metrics)
    set(NA_TEST_metrics_op1_ameba ${NA_TEST_op1_ameba_file}.metrics)
    set(NA_TEST_metrics_op1_with_unused ${NA_TEST_op1_with_unused_file}.metrics)
    set(NA_TEST_metrics_op2_vec ${NA_TEST_op2_vec_file}.metrics)
    set(NA_TEST_metrics_op2_cube ${NA_TEST_op2_cube_file}.metrics)
    set(NA_TEST_metrics_op2_ameba ${NA_TEST_op2_ameba_file}.metrics)
    set(NA_TEST_metrics_op2_with_unused ${NA_TEST_op2_with_unused_file}.metrics)
    set(NA_TEST_metrics_lut1 ${NA_TEST_lut1_file}.metrics)
    set(NA_TEST_metrics_lut2 ${NA_TEST_lut2_file}.metrics)
    set(NA_TEST_metrics_mpe_cmd ${NA_TEST_mpe_cmd_file}.metrics)
    ###################

    set(NA_RM_KEYS
      --num_iterations_file      ${NA_TEST_num_iterations_file}
      --in_file                  ${NA_TEST_in_file}
      --in_ameba_file            ${NA_TEST_in_ameba_file}
      --in_with_unused_file      ${NA_TEST_in_with_unused_file}
      --warr_file                ${NA_TEST_warr_file}
      --warr_with_unused_file    ${NA_TEST_warr_with_unused_file}
      --etalon_file              ${NA_TEST_etalon_file}
      --etalon_ameba_file        ${NA_TEST_etalon_ameba_file}
      --etalon_with_unused_file  ${NA_TEST_etalon_with_unused_file}
      --op0_vec_file             ${NA_TEST_op0_vec_file}
      --op0_cube_file            ${NA_TEST_op0_cube_file}
      --op0_ameba_file           ${NA_TEST_op0_ameba_file}
      --op0_with_unused_file     ${NA_TEST_op0_with_unused_file}
      --op1_vec_file             ${NA_TEST_op1_vec_file}
      --op1_cube_file            ${NA_TEST_op1_cube_file}
      --op1_ameba_file           ${NA_TEST_op1_ameba_file}
      --op1_with_unused_file     ${NA_TEST_op1_with_unused_file}
      --op2_vec_file             ${NA_TEST_op2_vec_file}
      --op2_cube_file            ${NA_TEST_op2_cube_file}
      --op2_ameba_file           ${NA_TEST_op2_ameba_file}
      --op2_with_unused_file     ${NA_TEST_op2_with_unused_file}
      --lut1_file                ${NA_TEST_lut1_file}
      --lut2_file                ${NA_TEST_lut2_file}
      --cfg_file                 ${NA_TEST_cfg_file}
      --cfg_mpe_file             ${NA_TEST_cfg_mpe_file}
      --cfg_ppe_file             ${NA_TEST_cfg_ppe_file}
      # '.metrics' added by RM itself
     )

    set(PLUSARG_num_iterations_file_tag num_iterations_file_tag)
    set(PLUSARG_in_file_tag in_file_tag)
    set(PLUSARG_in_ameba_file_tag in_ameba_file_tag)
    set(PLUSARG_in_with_unused_file_tag in_with_unused_file_tag)
    set(PLUSARG_warr_file_tag warr_file_tag)
    set(PLUSARG_warr_with_unused_file_tag warr_with_unused_file_tag)
    set(PLUSARG_etalon_file_tag etalon_file_tag)
    set(PLUSARG_etalon_ameba_file_tag etalon_ameba_file_tag)
    set(PLUSARG_etalon_with_unused_file_tag etalon_with_unused_file_tag)
    set(PLUSARG_op0_vec_file_tag op0_vec_file_tag)
    set(PLUSARG_op0_cube_file_tag op0_cube_file_tag)
    set(PLUSARG_op0_ameba_file_tag op0_ameba_file_tag)
    set(PLUSARG_op0_with_unused_file_tag op0_with_unused_file_tag)
    set(PLUSARG_op1_vec_file_tag op1_vec_file_tag)
    set(PLUSARG_op1_cube_file_tag op1_cube_file_tag)
    set(PLUSARG_op1_ameba_file_tag op1_ameba_file_tag)
    set(PLUSARG_op1_with_unused_file_tag op1_with_unused_file_tag)
    set(PLUSARG_op2_vec_file_tag op2_vec_file_tag)
    set(PLUSARG_op2_cube_file_tag op2_cube_file_tag)
    set(PLUSARG_op2_ameba_file_tag op2_ameba_file_tag)
    set(PLUSARG_op2_with_unused_file_tag op2_with_unused_file_tag)
    set(PLUSARG_lut1_file_tag lut1_file_tag)
    set(PLUSARG_lut2_file_tag lut2_file_tag)

    set(PLUSARG_cfg_file_tag cfg_file_tag)
    set(PLUSARG_cfg_mpe_file_tag cfg_mpe_file_tag)
    set(PLUSARG_cfg_ppe_file_tag cfg_ppe_file_tag)
    set(PLUSARG_mpe_cmd_file_tag mpe_cmd_file_tag)

    set(PLUSARG_metrics_in_tag metrics_in_tag)
    set(PLUSARG_metrics_in_ameba_tag metrics_in_ameba_tag)
    set(PLUSARG_metrics_in_with_unused_tag metrics_in_with_unused_tag)
    set(PLUSARG_metrics_warr_tag metrics_warr_tag)
    set(PLUSARG_metrics_warr_with_unused_tag metrics_warr_with_unused_tag)
    set(PLUSARG_metrics_etalon_tag metrics_etalon_tag)
    set(PLUSARG_metrics_etalon_ameba_tag metrics_etalon_ameba_tag)
    set(PLUSARG_metrics_etalon_with_unused_tag metrics_etalon_with_unused_tag)
    set(PLUSARG_metrics_op0_vec_tag metrics_op0_vec_tag)
    set(PLUSARG_metrics_op0_cube_tag metrics_op0_cube_tag)
    set(PLUSARG_metrics_op0_ameba_tag metrics_op0_ameba_tag)
    set(PLUSARG_metrics_op0_with_unused_tag metrics_op0_with_unused_tag)
    set(PLUSARG_metrics_op1_vec_tag metrics_op1_vec_tag)
    set(PLUSARG_metrics_op1_cube_tag metrics_op1_cube_tag)
    set(PLUSARG_metrics_op1_ameba_tag metrics_op1_ameba_tag)
    set(PLUSARG_metrics_op1_with_unused_tag metrics_op1_with_unused_tag)
    set(PLUSARG_metrics_op2_vec_tag metrics_op2_vec_tag)
    set(PLUSARG_metrics_op2_cube_tag metrics_op2_cube_tag)
    set(PLUSARG_metrics_op2_ameba_tag metrics_op2_ameba_tag)
    set(PLUSARG_metrics_op2_with_unused_tag metrics_op2_with_unused_tag)
    set(PLUSARG_metrics_lut1_tag metrics_lut1_tag)
    set(PLUSARG_metrics_lut2_tag metrics_lut2_tag)
    set(PLUSARG_metrics_mpe_cmd_tag metrics_mpe_cmd_tag)


    set(NA_RM_BIN_PATH ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils/npe_rm/rm_core/rtl-tests)
    set(NA_RM_PLUSARGS +${PLUSARG_in_file_tag}=${NA_TEST_in_file}
                       +${PLUSARG_in_ameba_file_tag}=${NA_TEST_in_ameba_file}
                       +${PLUSARG_in_with_unused_file_tag}=${NA_TEST_in_with_unused_file}
                       +${PLUSARG_warr_file_tag}=${NA_TEST_warr_file}
                       +${PLUSARG_warr_with_unused_file_tag}=${NA_TEST_warr_with_unused_file}
                       +${PLUSARG_etalon_file_tag}=${NA_TEST_etalon_file}
                       +${PLUSARG_etalon_ameba_file_tag}=${NA_TEST_etalon_ameba_file}
                       +${PLUSARG_etalon_with_unused_file_tag}=${NA_TEST_etalon_with_unused_file}
                       +${PLUSARG_op0_vec_file_tag}=${NA_TEST_op0_vec_file}
                       +${PLUSARG_op0_cube_file_tag}=${NA_TEST_op0_cube_file}
                       +${PLUSARG_op0_ameba_file_tag}=${NA_TEST_op0_with_unused_file}
                       +${PLUSARG_op0_with_unused_file_tag}=${NA_TEST_op0_with_unused_file}
                       +${PLUSARG_op1_vec_file_tag}=${NA_TEST_op1_vec_file}
                       +${PLUSARG_op1_cube_file_tag}=${NA_TEST_op1_cube_file}
                       +${PLUSARG_op1_ameba_file_tag}=${NA_TEST_op1_ameba_file}
                       +${PLUSARG_op1_with_unused_file_tag}=${NA_TEST_op1_with_unused_file}
                       +${PLUSARG_op2_vec_file_tag}=${NA_TEST_op2_vec_file}
                       +${PLUSARG_op2_cube_file_tag}=${NA_TEST_op2_cube_file}
                       +${PLUSARG_op2_ameba_file_tag}=${NA_TEST_op2_ameba_file}
                       +${PLUSARG_op2_with_unused_file_tag}=${NA_TEST_op2_with_unused_file}
                       +${PLUSARG_lut1_file_tag}=${NA_TEST_lut1_file}
                       +${PLUSARG_lut2_file_tag}=${NA_TEST_lut2_file}

                       +${PLUSARG_cfg_file_tag}=${NA_TEST_cfg_file}
                       +${PLUSARG_cfg_mpe_file_tag}=${NA_TEST_cfg_mpe_file}
                       +${PLUSARG_cfg_ppe_file_tag}=${NA_TEST_cfg_ppe_file}
                       +${PLUSARG_mpe_cmd_file_tag}=${NA_TEST_mpe_cmd_file}

                       +${PLUSARG_metrics_in_tag}=${NA_TEST_metrics_in}
                       +${PLUSARG_metrics_in_ameba_tag}=${NA_TEST_metrics_in_ameba}
                       +${PLUSARG_metrics_in_with_unused_tag}=${NA_TEST_metrics_in_with_unused}
                       +${PLUSARG_metrics_warr_tag}=${NA_TEST_metrics_warr}
                       +${PLUSARG_metrics_warr_with_unused_tag}=${NA_TEST_metrics_warr_with_unused}
                       +${PLUSARG_metrics_etalon_tag}=${NA_TEST_metrics_etalon}
                       +${PLUSARG_metrics_etalon_ameba_tag}=${NA_TEST_metrics_etalon_ameba}
                       +${PLUSARG_metrics_etalon_with_unused_tag}=${NA_TEST_metrics_etalon_with_unused}
                       +${PLUSARG_metrics_op0_vec_tag}=${NA_TEST_metrics_op0_vec}
                       +${PLUSARG_metrics_op0_cube_tag}=${NA_TEST_metrics_op0_cube}
                       +${PLUSARG_metrics_op0_ameba_tag}=${NA_TEST_metrics_op0_ameba}
                       +${PLUSARG_metrics_op0_with_unused_tag}=${NA_TEST_metrics_op0_with_unused}
                       +${PLUSARG_metrics_op1_vec_tag}=${NA_TEST_metrics_op1_vec}
                       +${PLUSARG_metrics_op1_cube_tag}=${NA_TEST_metrics_op1_cube}
                       +${PLUSARG_metrics_op1_ameba_tag}=${NA_TEST_metrics_op1_ameba}
                       +${PLUSARG_metrics_op1_with_unused_tag}=${NA_TEST_metrics_op1_with_unused}
                       +${PLUSARG_metrics_op2_vec_tag}=${NA_TEST_metrics_op2_vec}
                       +${PLUSARG_metrics_op2_cube_tag}=${NA_TEST_metrics_op2_cube}
                       +${PLUSARG_metrics_op2_ameba_tag}=${NA_TEST_metrics_op2_ameba}
                       +${PLUSARG_metrics_op2_with_unused_tag}=${NA_TEST_metrics_op2_with_unused}
                       +${PLUSARG_metrics_lut1_tag}=${NA_TEST_metrics_lut1}
                       +${PLUSARG_metrics_lut2_tag}=${NA_TEST_metrics_lut2}
                       +${PLUSARG_metrics_mpe_cmd_tag}=${NA_TEST_metrics_mpe_cmd}
    )
    set(NA_RM_PLUSARGS_LOOP +${PLUSARG_num_iterations_file_tag}=${NA_TEST_num_iterations_file})
    foreach(i RANGE 0 31)
      set(NA_RM_PLUSARGS_LOOP ${NA_RM_PLUSARGS_LOOP} 
        +${PLUSARG_in_file_tag}_${i}_=${NA_TEST_in_file}.${i}
        +${PLUSARG_in_ameba_file_tag}_${i}_=${NA_TEST_in_ameba_file}.${i}
        +${PLUSARG_in_with_unused_file_tag}_${i}_=${NA_TEST_in_with_unused_file}.${i}
        +${PLUSARG_warr_file_tag}_${i}_=${NA_TEST_warr_file}.${i}
        +${PLUSARG_warr_with_unused_file_tag}_${i}_=${NA_TEST_warr_with_unused_file}.${i}
        +${PLUSARG_etalon_file_tag}_${i}_=${NA_TEST_etalon_file}.${i}
        +${PLUSARG_etalon_ameba_file_tag}_${i}_=${NA_TEST_etalon_ameba_file}.${i}
        +${PLUSARG_etalon_with_unused_file_tag}_${i}_=${NA_TEST_etalon_with_unused_file}.${i}
        +${PLUSARG_op0_vec_file_tag}_${i}_=${NA_TEST_op0_vec_file}.${i}
        +${PLUSARG_op0_cube_file_tag}_${i}_=${NA_TEST_op0_cube_file}.${i}
        +${PLUSARG_op0_ameba_file_tag}_${i}_=${NA_TEST_op0_with_unused_file}.${i}
        +${PLUSARG_op0_with_unused_file_tag}_${i}_=${NA_TEST_op0_with_unused_file}.${i}
        +${PLUSARG_op1_vec_file_tag}_${i}_=${NA_TEST_op1_vec_file}.${i}
        +${PLUSARG_op1_cube_file_tag}_${i}_=${NA_TEST_op1_cube_file}.${i}
        +${PLUSARG_op1_ameba_file_tag}_${i}_=${NA_TEST_op1_ameba_file}.${i}
        +${PLUSARG_op1_with_unused_file_tag}_${i}_=${NA_TEST_op1_with_unused_file}.${i}
        +${PLUSARG_op2_vec_file_tag}_${i}_=${NA_TEST_op2_vec_file}.${i}
        +${PLUSARG_op2_cube_file_tag}_${i}_=${NA_TEST_op2_cube_file}.${i}
        +${PLUSARG_op2_ameba_file_tag}_${i}_=${NA_TEST_op2_ameba_file}.${i}
        +${PLUSARG_op2_with_unused_file_tag}_${i}_=${NA_TEST_op2_with_unused_file}.${i}
        +${PLUSARG_lut1_file_tag}_${i}_=${NA_TEST_lut1_file}.${i}
        +${PLUSARG_lut2_file_tag}_${i}_=${NA_TEST_lut2_file}.${i}

        +${PLUSARG_cfg_file_tag}_${i}_=${NA_TEST_cfg_file}.${i}
        +${PLUSARG_cfg_mpe_file_tag}_${i}_=${NA_TEST_cfg_mpe_file}.${i}

        +${PLUSARG_cfg_ppe_file_tag}_${i}_=${NA_TEST_cfg_ppe_file}.${i}
        +${PLUSARG_cfg_ppe_pyfile_tag}_${i}_=${NA_TEST_cfg_ppe_file}.${i}

        +${PLUSARG_mpe_cmd_file_tag}_${i}_=${NA_TEST_mpe_cmd_file}.${i}

        +${PLUSARG_metrics_in_tag}_${i}_=${NA_TEST_in_file}.${i}.metrics
        +${PLUSARG_metrics_in_ameba_tag}_${i}_=${NA_TEST_in_ameba_file}.${i}.metrics
        +${PLUSARG_metrics_in_with_unused_tag}_${i}_=${NA_TEST_in_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_warr_tag}_${i}_=${NA_TEST_warr_file}.${i}.metrics
        +${PLUSARG_metrics_warr_with_unused_tag}_${i}_=${NA_TEST_warr_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_etalon_tag}_${i}_=${NA_TEST_etalon_file}.${i}.metrics
        +${PLUSARG_metrics_etalon_ameba_tag}_${i}_=${NA_TEST_etalon_ameba_file}.${i}.metrics
        +${PLUSARG_metrics_etalon_with_unused_tag}_${i}_=${NA_TEST_etalon_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_op0_vec_tag}_${i}_=${NA_TEST_op0_vec_file}.${i}.metrics
        +${PLUSARG_metrics_op0_cube_tag}_${i}_=${NA_TEST_op0_cube_file}.${i}.metrics
        +${PLUSARG_metrics_op0_ameba_tag}_${i}_=${NA_TEST_op0_ameba_file}.${i}.metrics
        +${PLUSARG_metrics_op0_with_unused_tag}_${i}_=${NA_TEST_op0_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_op1_vec_tag}_${i}_=${NA_TEST_op1_vec_file}.${i}.metrics
        +${PLUSARG_metrics_op1_cube_tag}_${i}_=${NA_TEST_op1_cube_file}.${i}.metrics
        +${PLUSARG_metrics_op1_ameba_tag}_${i}_=${NA_TEST_op1_ameba_file}.${i}.metrics
        +${PLUSARG_metrics_op1_with_unused_tag}_${i}_=${NA_TEST_op1_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_op2_vec_tag}_${i}_=${NA_TEST_op2_vec_file}.${i}.metrics
        +${PLUSARG_metrics_op2_cube_tag}_${i}_=${NA_TEST_op2_cube_file}.${i}.metrics
        +${PLUSARG_metrics_op2_ameba_tag}_${i}_=${NA_TEST_op2_ameba_file}.${i}.metrics
        +${PLUSARG_metrics_op2_with_unused_tag}_${i}_=${NA_TEST_op2_with_unused_file}.${i}.metrics
        +${PLUSARG_metrics_lut1_tag}_${i}_=${NA_TEST_lut1_file}.${i}.metrics
        +${PLUSARG_metrics_lut2_tag}_${i}_=${NA_TEST_lut2_file}.${i}.metrics
        +${PLUSARG_metrics_mpe_cmd_tag}_${i}_=${NA_TEST_mpe_cmd_file}.${i}.metrics
      )
    endforeach()
    set(RM_LOGFILE npe_rm.log)

    macro(ADD_VPE_COUPLED_TEST name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/first_coupled_vpe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()

    macro(ADD_MPE_COUPLED_TEST name rm_bin_name) # name - символическое имя теста, rm_bin_name - имя executable и3 npe_rm/rtl-tests/CMakeLists.txt
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/first_coupled_mpe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()

    macro(ADD_PPE_FIRST_COUPLED_TEST name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/first_coupled_ppe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()

    macro(ADD_PPE_NEXT_COUPLED_TEST name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/one_coupled_ppe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()

    # Add new tests here
    ADD_VPE_COUPLED_TEST(vpe_first_coupled main_sqared_try_all_ops)
    ADD_VPE_COUPLED_TEST(vpe_first_coupled_float main_sqared_try_all_ops_float)
    ADD_VPE_COUPLED_TEST(vpe_nothing_to_do main_nothing_to_do)
    ADD_VPE_COUPLED_TEST(vpe_dma_test main_vpe_dma)

    ADD_MPE_COUPLED_TEST(mpe_first_coupled main_sqared_simple_mpe)

    ADD_PPE_FIRST_COUPLED_TEST(ppe_first_coupled main_simple_ppe_coupled)
    ADD_PPE_FIRST_COUPLED_TEST(ppe_first_coupled_float main_simple_ppe_coupled_float)

    ADD_PPE_NEXT_COUPLED_TEST(ppe_one_coupled main_i8_ppe_coupled)
    ADD_PPE_NEXT_COUPLED_TEST(ppe_probe_coupled main_probe_ppe_coupled)

    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/dma_coupled_vpe.c
      PREPCMD ${NA_RM_BIN_PATH}/main_vpe_dma ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
    )

    
    macro(ADD_VPE_COUPLED_TEST_LOOP name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      )
    endmacro()

    macro(ADD_PPE_COUPLED_TEST_LOOP name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      )
    endmacro()

    ADD_VPE_COUPLED_TEST_LOOP(vpe_try_loop main_nothing_to_do_x4)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_relu_int32 main_relu_int32)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_relu_fp32  main_relu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_c3 main_vpe_c3)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_c1 main_vpe_c1)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_c2 main_vpe_c2)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_norm_op0 main_norm_op0)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_norm_op1 main_norm_op1)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_lshift_op0 main_vpe_lshift_op0)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_lshift_op1 main_vpe_lshift_op1)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_f_op0 main_vpe_f_op0)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_f_op1 main_vpe_f_op1)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_lut main_lut)

    ADD_PPE_COUPLED_TEST_LOOP(ppe_loop_coupled main_loop_ppe_coupled)

    ADD_PPE_COUPLED_TEST_LOOP(ppe_2_i8_max    main_ppe_2_i8_max   )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_3_i16_max   main_ppe_3_i16_max  )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_4_fp16_max  main_ppe_4_fp16_max )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_5_i8_avg    main_ppe_5_i8_avg   )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_6_i16_avg   main_ppe_6_i16_avg  )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_7_fp16_avg  main_ppe_7_fp16_avg )

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

    set(MPE_DEMO_PATH src/platform/scr1/targets/simple-rom/nu/mpe_demo)
    set(MPE_PARSE_TEST ${CMAKE_SOURCE_DIR}/${MPE_DEMO_PATH}/parse_mpe_arrays.pl)
    set(MPE_TEST_SHEETS_DIR ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests)     # Outside The RumBoot!!!!

    macro (ADD_MPE_DEMO_TEST mpe_demo_test_name)
      set(MPE_TEST_SHEET ${MPE_TEST_SHEETS_DIR}/${mpe_demo_test_name}/mpe_arrays.txt)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME mpe_demo_${mpe_demo_test_name}
        FILES scr1/targets/simple-rom/nu/mpe_demo/mpe_demo_common.c
        PREPCMD ${MPE_PARSE_TEST} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} < ${MPE_TEST_SHEET}
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()

    ADD_MPE_DEMO_TEST(test1)
    ADD_MPE_DEMO_TEST(test2)
    ADD_MPE_DEMO_TEST(test3)
    ADD_MPE_DEMO_TEST(test4)
    ADD_MPE_DEMO_TEST(test5)
    ADD_MPE_DEMO_TEST(test_MAX_calculation)
    ADD_MPE_DEMO_TEST(test-int8)
    ADD_MPE_DEMO_TEST(test-int16)

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
