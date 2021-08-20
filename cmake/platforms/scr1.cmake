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

    ########################################################
    # quad_spi tests
    if(DUT STREQUAL "QUAD_SPI_SIMPLE")
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
    endif()

    ######################################################################
    # mdma tests
    if(DUT STREQUAL "MDMA_SIMPLE")
    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/simple-rom/mdma_simple_test/mdma_simple_test.c 
      IRUN_FLAGS +myfile=${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/mdma_simple_test/data
    )
    endif()

    add_rumboot_target(
      CONFIGURATION ROM
      FILES scr1/targets/load_bin_example/hello_load_bin.c
      IRUN_FLAGS +myfile=${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/load_bin_example/data
    )

    ################################################################
    # common for tests with npe_rm
    if(DUT STREQUAL "MPE" OR DUT STREQUAL "VPE" OR DUT STREQUAL "PPE")

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

    set(NA_TEST_status_regs_file status_regs.bin)

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
      --status_regs_file         ${NA_TEST_status_regs_file}
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

    set(PLUSARG_status_regs_file_tag status_regs_file_tag)


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

                       +${PLUSARG_status_regs_file_tag}=${NA_TEST_status_regs_file}
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

        +${PLUSARG_status_regs_file_tag}_${i}_=${NA_TEST_status_regs_file}.${i}
      )
    endforeach()
    set(RM_LOGFILE npe_rm.log)

    ###################################
    if(DUT STREQUAL "VPE")
      
    add_rumboot_target(
      CONFIGURATION ROM
        NAME VPE_1
        FILES scr1/targets/simple-rom/nu/vpe_regs/regs_vpe.c
    )


if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
    if(NOT DEFINED EXPERIMENT_STAGE_2_DIR)
      set(EXPERIMENT_STAGE_2_DIR /opt/lib_h31/LAVA_lib/experiment_stage_2)
    endif()
    message("EXPERIMENT_STAGE_2_DIR = ${EXPERIMENT_STAGE_2_DIR}")


    if(NOT DEFINED VPE_BINARIES_ROOT)
      if(EXISTS ${CMAKE_SOURCE_DIR}/../../VPE)
        set(VPE_BINARIES_ROOT ${CMAKE_SOURCE_DIR}/../../VPE)
      elseif(EXISTS ${EXPERIMENT_STAGE_2_DIR}/VPE)
        set(VPE_BINARIES_ROOT ${EXPERIMENT_STAGE_2_DIR}/VPE)
      else()
        message(FATAL_ERROR "VPE_BINARIES_ROOT test binaries Directory not found")
      endif()
    endif()
    message("VPE_BINARIES_ROOT = ${VPE_BINARIES_ROOT}")
endif() #### EXPERIMENT_STAGE_2_SUB_1

    
    macro(ADD_VPE_COUPLED_TEST_LOOP name rm_bin_name)
      set(MISALIGN RANGE 0 7)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
            CFLAGS -DIntMisalign=${IntMisalign}
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()

    macro(ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA name rm_bin_name)
      set(MISALIGN RANGE 0 7)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()

    macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA name rm_bin_name)
      set(MISALIGN RANGE 0 7)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()
    
    macro(ADD_VPE_COUPLED_TEST_CONTROL_CONS name rm_bin_name)
      set(MISALIGN RANGE 0 7)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()
    
if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
    macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_CONS name bin_dir)
      set(MISALIGN 0)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()
endif() #### EXPERIMENT_STAGE_2_SUB_1
    
    macro(ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL name rm_bin_name)
      set(MISALIGN RANGE 0 7)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()

if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
    macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_PARALLEL name bin_dir)
      set(MISALIGN 0)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()

    macro(ADD_VPE_FROM_BINARY_TEST_LOOP name bin_dir)
      set(MISALIGN 0)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
            CFLAGS -DIntMisalign=${IntMisalign}
            PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()

    macro(ADD_VPE_FROM_BINARY_TEST_LOOP_FORCE_WDMA name bin_dir)
      set(MISALIGN 0)
      foreach(IntMisalign ${MISALIGN})
        add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${IntMisalign}
            CFLAGS -DFORCE_VPE_WDMA_EN=1 -DIntMisalign=${IntMisalign}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
            PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )
      endforeach()
    endmacro()
endif() #### EXPERIMENT_STAGE_2_SUB_1

    ADD_VPE_COUPLED_TEST_LOOP(vpe_try_loop main_nothing_to_do_x4)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_2_dma_int16 main_vpe_2_dma_int16)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_2_dma_int8 main_vpe_2_dma_int8)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_9_0_op0_relu_int32 main_vpe_9_0_op0_relu_int32)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_15_0_op1_relu_int32 main_vpe_15_0_op1_relu_int32) 
    ADD_VPE_COUPLED_TEST_LOOP(vpe_9_1_op0_relu_fp32  main_vpe_9_1_op0_relu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_15_1_op1_relu_fp32  main_vpe_15_1_op1_relu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_3_c3 main_vpe_3_c3)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_18_op2_c1 main_vpe_18_op2_c1)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_16_op2_c2 main_vpe_16_op2_c2)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_8_0_op0_norm main_vpe_8_0_op0_norm)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_14_0_op1_norm main_vpe_14_0_op1_norm)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_8_1_op0_norm_rnd main_vpe_8_1_op0_norm_rnd)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_14_1_op1_norm_rnd main_vpe_14_1_op1_norm_rnd)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_20_0_op2_norm main_vpe_20_0_op2_norm)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_20_1_op2_norm_rnd main_vpe_20_1_op2_norm_rnd)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_4_op0_lshift main_vpe_4_op0_lshift)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_10_op1_lshift main_vpe_10_op1_lshift)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_6_0_op0_f_int  main_vpe_6_0_op0_f_int)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_12_0_op1_f_int main_vpe_12_0_op1_f_int)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_6_1_op0_f_fp main_vpe_6_1_op0_f_fp)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_12_1_op1_f_fp main_vpe_12_1_op1_f_fp)
    
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_0_op0_alu_int8_low main_vpe_5_0_op0_alu_int8_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_0_op0_alu_int8_low_dma main_vpe_5_0_op0_alu_int8_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_1_op0_alu_int8_middle main_vpe_5_1_op0_alu_int8_middle)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_1_op0_alu_int8_middle_dma main_vpe_5_1_op0_alu_int8_middle_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_2_op0_alu_int8_high main_vpe_5_2_op0_alu_int8_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_2_op0_alu_int8_high_dma main_vpe_5_2_op0_alu_int8_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_3_op0_alu_int16_low main_vpe_5_3_op0_alu_int16_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_3_op0_alu_int16_low_dma main_vpe_5_3_op0_alu_int16_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_4_op0_alu_int16_middle main_vpe_5_4_op0_alu_int16_middle)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_4_op0_alu_int16_middle_dma main_vpe_5_4_op0_alu_int16_middle_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_5_op0_alu_int16_high main_vpe_5_5_op0_alu_int16_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_5_op0_alu_int16_high_dma main_vpe_5_5_op0_alu_int16_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_5_6_op0_alu_fp32 main_vpe_5_6_op0_alu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_6_op0_alu_fp32_dma main_vpe_5_6_op0_alu_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_0_op1_alu_int8_low main_vpe_11_0_op1_alu_int8_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_0_op1_alu_int8_low_dma main_vpe_11_0_op1_alu_int8_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_1_op1_alu_int8_middle main_vpe_11_1_op1_alu_int8_middle)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_1_op1_alu_int8_middle_dma main_vpe_11_1_op1_alu_int8_middle_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_2_op1_alu_int8_high main_vpe_11_2_op1_alu_int8_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_2_op1_alu_int8_high_dma main_vpe_11_2_op1_alu_int8_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_3_op1_alu_int16_low main_vpe_11_3_op1_alu_int16_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_3_op1_alu_int16_low_dma main_vpe_11_3_op1_alu_int16_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_4_op1_alu_int16_middle main_vpe_11_4_op1_alu_int16_middle)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_4_op1_alu_int16_middle_dma main_vpe_11_4_op1_alu_int16_middle_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_5_op1_alu_int16_high main_vpe_11_5_op1_alu_int16_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_5_op1_alu_int16_high_dma main_vpe_11_5_op1_alu_int16_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_11_6_op1_alu_fp32 main_vpe_11_6_op1_alu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_6_op1_alu_fp32_dma main_vpe_11_6_op1_alu_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_17_0_op2_alu_int8_low main_vpe_17_0_op2_alu_int8_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_0_op2_alu_int8_low_dma main_vpe_17_0_op2_alu_int8_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_17_1_op2_alu_int8_high main_vpe_17_1_op2_alu_int8_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_1_op2_alu_int8_high_dma main_vpe_17_1_op2_alu_int8_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_17_2_op2_alu_int16_low main_vpe_17_2_op2_alu_int16_low)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_2_op2_alu_int16_low_dma main_vpe_17_2_op2_alu_int16_low_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_17_3_op2_alu_int16_high main_vpe_17_3_op2_alu_int16_high)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_3_op2_alu_int16_high_dma main_vpe_17_3_op2_alu_int16_high_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_17_4_op2_alu_fp32 main_vpe_17_4_op2_alu_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_4_op2_alu_fp32_dma main_vpe_17_4_op2_alu_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_7_0_op0_mul_int16 main_vpe_7_0_op0_mul_int16)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_0_op0_mul_int16_dma main_vpe_7_0_op0_mul_int16_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_7_1_op0_mul_int8 main_vpe_7_1_op0_mul_int8)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_1_op0_mul_int8_dma main_vpe_7_1_op0_mul_int8_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_7_2_op0_mul_fp32 main_vpe_7_2_op0_mul_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_2_op0_mul_fp32_dma main_vpe_7_2_op0_mul_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_13_0_op1_mul_int16 main_vpe_13_0_op1_mul_int16)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_0_op1_mul_int16_dma main_vpe_13_0_op1_mul_int16_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_13_1_op1_mul_int8 main_vpe_13_1_op1_mul_int8)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_1_op1_mul_int8_dma main_vpe_13_1_op1_mul_int8_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_13_2_op1_mul_fp32 main_vpe_13_2_op1_mul_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_2_op1_mul_fp32_dma main_vpe_13_2_op1_mul_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_19_0_op2_mul_int16 main_vpe_19_0_op2_mul_int16)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_0_op2_mul_int16_dma main_vpe_19_0_op2_mul_int16_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_19_1_op2_mul_int8 main_vpe_19_1_op2_mul_int8)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_1_op2_mul_int8_dma main_vpe_19_1_op2_mul_int8_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_19_2_op2_mul_fp32 main_vpe_19_2_op2_mul_fp32)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_2_op2_mul_fp32_dma main_vpe_19_2_op2_mul_fp32_dma)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_21_0_lut main_vpe_21_0_lut) # VPE_21
    ADD_VPE_COUPLED_TEST_LOOP(vpe_21_1_lut_addition main_vpe_21_1_lut_addition) # VPE_21_addition
    ADD_VPE_COUPLED_TEST_LOOP(vpe_22_op0_together main_vpe_22_op0_together) # VPE_22
    ADD_VPE_COUPLED_TEST_LOOP(vpe_23_op1_together main_vpe_23_op1_together) # VPE_23
    ADD_VPE_COUPLED_TEST_LOOP(vpe_24_op2_together main_vpe_24_op2_together) # VPE_24
    ADD_VPE_COUPLED_TEST_LOOP(vpe_25_op012_together main_vpe_25_op012_together) # VPE_25
    ADD_VPE_COUPLED_TEST_CONTROL_CONS(vpe_27_0_control_cons main_vpe_27_0_control_cons) # VPE_27
    ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL(vpe_27_1_control_par main_vpe_27_1_control_par) # VPE_27
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_26_autonom main_vpe_26_autonom)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_26_autonom_nowdma main_vpe_26_autonom)
    ADD_VPE_COUPLED_TEST_LOOP(vpe_28_perf main_28_perf) # VPE_28
    
    
    ################
    # VPE Tests adapted for NPE assembly
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op0_f_int main_npe_vpe_op0_f_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op0_f_fp main_npe_vpe_op0_f_fp)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op1_f_int main_npe_vpe_op1_f_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op1_f_fp main_npe_vpe_op1_f_fp)
    
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op0_lshift main_npe_vpe_op0_lshift)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op1_lshift main_npe_vpe_op1_lshift)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op2_c1 main_npe_vpe_op2_c1)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op2_c2 main_npe_vpe_op2_c2)
    
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_c3 main_npe_vpe_c3)
    
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op0_relu_int main_npe_vpe_op0_relu_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op0_relu_fp  main_npe_vpe_op0_relu_fp)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op1_relu_int main_npe_vpe_op1_relu_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(npe_vpe_op1_relu_fp  main_npe_vpe_op1_relu_fp)
    
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_vec_ex_int main_vpe_op0_vec_ex_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_vec_ex_int main_vpe_op1_vec_ex_int)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_vec_ex_fp main_vpe_op0_vec_ex_fp)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_vec_ex_fp main_vpe_op1_vec_ex_fp)
    
    ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_vec_ex_int main_vpe_op0_vec_ex_int)
    ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_vec_ex_int main_vpe_op1_vec_ex_int)
    ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_vec_ex_fp main_vpe_op0_vec_ex_fp)
    ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_vec_ex_fp main_vpe_op1_vec_ex_fp)

    ###############

if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
    ADD_VPE_FROM_BINARY_TEST_LOOP_FORCE_WDMA(VPE_2_0 VPE_2/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP_FORCE_WDMA(VPE_2_1 VPE_2/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_9_0 VPE_9/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_15_0 VPE_15/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_9_1 VPE_9/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_15_1 VPE_15/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_3 VPE_3)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_18 VPE_18)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_16 VPE_16)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_8_0 VPE_8/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_14_0 VPE_14/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_8_1 VPE_8/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_14_1 VPE_14/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_20_0 VPE_20/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_20_1 VPE_20/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_4 VPE_4)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_10 VPE_10)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_6_0 VPE_6/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_12_0 VPE_12/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_6_1 VPE_6/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_12_1 VPE_12/1)
   
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_0 VPE_5/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_1 VPE_5/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_2 VPE_5/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_3 VPE_5/3)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_4 VPE_5/4)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_5 VPE_5/5)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_5_6 VPE_5/6)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_0 VPE_11/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_1 VPE_11/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_2 VPE_11/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_3 VPE_11/3)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_4 VPE_11/4)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_5 VPE_11/5)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_11_6 VPE_11/6)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_17_0 VPE_17/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_17_1 VPE_17/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_17_2 VPE_17/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_17_3 VPE_17/3)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_17_4 VPE_17/4)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_7_0 VPE_7/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_7_1 VPE_7/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_7_2 VPE_7/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_13_0 VPE_13/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_13_1 VPE_13/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_13_2 VPE_13/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_19_0 VPE_19/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_19_1 VPE_19/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_19_2 VPE_19/2)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_21_0 VPE_21/0)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_21_1 VPE_21/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_22 VPE_22)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_23 VPE_23)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_24 VPE_24)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_25 VPE_25)
    ADD_VPE_FROM_BINARY_TEST_CONTROL_CONS(VPE_27_0 VPE_27/0)
    ADD_VPE_FROM_BINARY_TEST_CONTROL_PARALLEL(VPE_27_1 VPE_27/1)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_26 VPE_26)
    ADD_VPE_FROM_BINARY_TEST_LOOP(VPE_28 VPE_28)

endif() #### EXPERIMENT_STAGE_2_SUB_1

    endif()  # if(DUT STREQUAL "VPE")

    ###############
    if(DUT STREQUAL "PPE")

    add_rumboot_target(
        CONFIGURATION ROM
        NAME PPE_1
        FILES scr1/targets/simple-rom/nu/ppe_regs/regs_ppe.c
    )
    

    macro(ADD_PPE_NEXT_COUPLED_TEST name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/one_coupled_ppe.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
      )
    endmacro()


    ADD_PPE_NEXT_COUPLED_TEST(ppe_one_coupled main_i8_ppe_coupled)
    ADD_PPE_NEXT_COUPLED_TEST(ppe_probe_coupled main_probe_ppe_coupled)

    # works after rm CMakeCache.txt
    if(NOT DEFINED NU_SEED)
      set(NU_SEED 1)
    endif()

    macro(ADD_PPE_COUPLED_TEST_LOOP name rm_bin_name ShowPerf)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe.c
        PREPCMD
          ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} > ${RM_LOGFILE} &&
          cp ${RM_LOGFILE} ../npe_rm_${NU_SEED}.log ||
          exit 1
        CFLAGS -D${ShowPerf}
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      )
    endmacro()

    # works after rm CMakeCache.txt
    if(NOT DEFINED NU_TESTS_NMB)
      set(NU_TESTS_NMB 0)
    endif()

    macro (ADD_PPE_MANY_TESTS name rm_bin_name ShowPerf OpMode)
      foreach(i RANGE 3)
        add_rumboot_target(
          CONFIGURATION ROM
          NAME ${name}_${i}
          FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_new.c

          PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} > ${RM_LOGFILE} || exit 1

          CFLAGS -D${ShowPerf} -D${OpMode}
          IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        )

        math (EXPR NU_SEED "${NU_SEED} + 1")
      endforeach()
    endmacro()

    macro(ADD_PPE_V_EXPER_TEST name rm_bin_name ShowPerf OpMode)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_new.c
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} > ${RM_LOGFILE} || exit 1
        CFLAGS -D${ShowPerf} -D${OpMode}
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      )
    endmacro()

if(DEFINED EXPERIMENT_STAGE_2_SUB_1)
    if(NOT DEFINED PPE_EXPER_DIR)
      if(EXISTS ${CMAKE_SOURCE_DIR}/../../PPE)
        set(PPE_EXPER_DIR ${CMAKE_SOURCE_DIR}/../../PPE)
      elseif(EXISTS ${EXPERIMENT_STAGE_2_DIR}/PPE)
        set(PPE_EXPER_DIR ${EXPERIMENT_STAGE_2_DIR}/PPE/)
      else()
        message(FATAL_ERROR "PPE_EXPER_DIR test binaries Directory not found")
      endif()
    endif()
    message("PPE_EXPER_DIR = ${PPE_EXPER_DIR}")

    macro(ADD_PPE_EXPER_TEST name ShowPerf)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/experiment_ppe.c
        PREPCMD cp ${PPE_EXPER_DIR}/${name}/*bin* .
        CFLAGS -D${ShowPerf}
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      )
    endmacro()
endif()  ### EXPERIMENT_STAGE_2_SUB_1


    ADD_PPE_COUPLED_TEST_LOOP(ppe_loop_coupled main_loop_ppe_coupled NotShowPerf)

    ADD_PPE_COUPLED_TEST_LOOP(ppe_2_i8_max    main_ppe_2_i8_max   NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_3_i16_max   main_ppe_3_i16_max  NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_4_fp16_max  main_ppe_4_fp16_max NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_5_i8_avg    main_ppe_5_i8_avg   NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_6_i16_avg   main_ppe_6_i16_avg  NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_7_fp16_avg  main_ppe_7_fp16_avg NotShowPerf )
    ADD_PPE_COUPLED_TEST_LOOP(ppe_11_i16_max  main_ppe_3_i16_max  ShowPerf    )

    ADD_PPE_V_EXPER_TEST(ppe_8v_i16_max   main_ppe_3_i16_max  NotShowPerf VPEtoPPE)
    ADD_PPE_V_EXPER_TEST(ppe_8v_i16_max_p main_ppe_3_i16_max  ShowPerf    VPEtoPPE)
    ADD_PPE_V_EXPER_TEST(ppe_2v_i8_max    main_ppe_2_i8_max   NotShowPerf MEMtoMEM)
    ADD_PPE_V_EXPER_TEST(ppe_2v_i8_max_p  main_ppe_2_i8_max   ShowPerf MEMtoMEM)

    ADD_PPE_V_EXPER_TEST(ppe_5v_i8_min    main_ppe_2_i8_min   NotShowPerf MEMtoMEM)
    ADD_PPE_V_EXPER_TEST(ppe_6v_i16_min   main_ppe_3_i16_min  NotShowPerf MEMtoMEM)
    ADD_PPE_V_EXPER_TEST(ppe_7v_fp16_min  main_ppe_4_fp16_min NotShowPerf MEMtoMEM)

    ADD_PPE_MANY_TESTS(ppe_many_i8_max    main_ppe_2_i8_max   NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_i16_max   main_ppe_3_i16_max  NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_fp16_max  main_ppe_4_fp16_max NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_i8_avg    main_ppe_5_i8_avg   NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_i16_avg   main_ppe_6_i16_avg  NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_fp16_avg  main_ppe_7_fp16_avg NotShowPerf MEMtoMEM)

    ADD_PPE_MANY_TESTS(ppe_many_i8_min    main_ppe_2_i8_min   NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_i16_min   main_ppe_3_i16_min  NotShowPerf MEMtoMEM)
    ADD_PPE_MANY_TESTS(ppe_many_fp16_min  main_ppe_4_fp16_min NotShowPerf MEMtoMEM)

if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ###
    ADD_PPE_EXPER_TEST(PPE_2  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_3  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_4  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_5  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_6  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_7  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_8  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_9  NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_10 NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_11 NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_12 NotShowPerf)
    ADD_PPE_EXPER_TEST(PPE_13 ShowPerf)
    ADD_PPE_EXPER_TEST(PPE_14 NotShowPerf)
endif() ### EXPERIMENT_STAGE_2_SUB_1

    endif() # if(DUT STREQUAL "PPE")

    ###################################################
    # MPE 
    if(DUT STREQUAL "MPE")

    set(MPE_DEMO_PATH src/platform/scr1/targets/simple-rom/nu/mpe_demo)
    set(MPE_PARSE_TEST ${CMAKE_SOURCE_DIR}/${MPE_DEMO_PATH}/parse_mpe_arrays.pl)

    if(NOT DEFINED MPE_TEST_SHEETS_DIR)
      if(EXISTS ${CMAKE_SOURCE_DIR}/../../MPE)
        set (MPE_TEST_SHEETS_DIR ${CMAKE_SOURCE_DIR}/../../MPE)
      elseif(EXISTS ${EXPERIMENT_STAGE_2_DIR}/MPE)
        set (MPE_TEST_SHEETS_DIR ${EXPERIMENT_STAGE_2_DIR}/MPE)
      elseif(EXISTS ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment)
        set(MPE_TEST_SHEETS_DIR ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment)
      else()
        message(FATAL_ERROR "MPE_TEST_SHEETS_DIR test binaries not found")
      endif()
    endif()
    message ("MPE_TEST_SHEETS_DIR = ${MPE_TEST_SHEETS_DIR}")

    macro (ADD_MPE_DEMO_TEST mpe_demo_test_name show_perf)
      set(MPE_TEST_SHEET ${MPE_TEST_SHEETS_DIR}/${mpe_demo_test_name}/mpe_arrays.txt)
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${mpe_demo_test_name}
        FILES scr1/targets/simple-rom/nu/mpe_demo/mpe_demo_common.c
        CFLAGS -D${show_perf}
        PREPCMD ${MPE_PARSE_TEST} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} < ${MPE_TEST_SHEET}
        IRUN_FLAGS ${NA_RM_PLUSARGS}
        TIMEOUT_CTEST 10000
      )
    endmacro()

    ADD_MPE_DEMO_TEST(MPE_1 No)
    ADD_MPE_DEMO_TEST(MPE_2 No)
    ADD_MPE_DEMO_TEST(MPE_3 No)
    ADD_MPE_DEMO_TEST(MPE_4 No)
    ADD_MPE_DEMO_TEST(MPE_5 No)
    ADD_MPE_DEMO_TEST(MPE_6 No)
    ADD_MPE_DEMO_TEST(MPE_10 No)
    ADD_MPE_DEMO_TEST(MPE_11 No)
    ADD_MPE_DEMO_TEST(MPE_14 No)
    ADD_MPE_DEMO_TEST(MPE_16 SHOW_PERF)
    
    macro (ADD_MPE_COUPLED_TEST_LOOP name rm_bin_name)
      set (ADD_MPE_COUPLED_TEST_LOOP_ARGS cp ${MPE_TEST_SHEETS_DIR}/${name}/num_iterations.bin .)
      foreach(i RANGE 0 15)
        set(MPE_TEST_SHEET ${MPE_TEST_SHEETS_DIR}/${name}/${rm_bin_name}_${i}/mpe_arrays_r.txt)
        set(ADD_MPE_COUPLED_TEST_LOOP_ARGS ${ADD_MPE_COUPLED_TEST_LOOP_ARGS} &&
        ${MPE_PARSE_TEST} ${NA_TEST_mpe_cmd_file}.${i} ${NA_TEST_in_file}.${i} ${NA_TEST_warr_file}.${i} ${NA_TEST_etalon_file}.${i} < ${MPE_TEST_SHEET})
      endforeach()
      add_rumboot_target(
        CONFIGURATION ROM
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_mpe.c
        PREPCMD ${ADD_MPE_COUPLED_TEST_LOOP_ARGS}
        IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        TIMEOUT_CTEST 10000
      )
    endmacro()

    ADD_MPE_COUPLED_TEST_LOOP(MPE_7 test) # mu int8 test
    ADD_MPE_COUPLED_TEST_LOOP(MPE_8 test) # mu int16 test
    ADD_MPE_COUPLED_TEST_LOOP(MPE_9 test) # mu fp16 test

    ADD_MPE_COUPLED_TEST_LOOP(MPE_12 test) # mu+acc int8 test
    ADD_MPE_COUPLED_TEST_LOOP(MPE_13 test) # mu+acc int16 test
    ADD_MPE_COUPLED_TEST_LOOP(MPE_15 test) # mu+acc fp16 test

    endif()  # if(DUT STREQUAL "MPE")
    ########################################

    endif()  # if(DUT STREQUAL MPE,VPE,PPE)

    ###########################################################
    # demonstrator tests

    if(DUT STREQUAL "DEMONSTRATOR_SIMPLE")
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
    endif() # DEMONSTRATOR_SIMPLE
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
  SET(CROSS_COMPILE riscv32-rcm-elf)
  message(STATUS "No -DCROSS_COMPILE passed to cmake, attempting to detect ${CROSS_COMPILE}")
  if (EXISTS /opt/r42/toolchains/riscv32-ncrmnt-elf/bin/riscv32-ncrmnt-elf-gcc)
      set(CROSS_COMPILE "/opt/r42/toolchains/riscv32-rcm-elf/bin/${CROSS_COMPILE}")
  endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
