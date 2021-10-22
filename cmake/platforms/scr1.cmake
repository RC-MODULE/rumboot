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
    # Extract The First Letter Of The DUT (We Pass It As A Define Into c-code)
    string(SUBSTRING "${DUT}" 0 1 DUT_LETTER)
    set(DUT_LETTER_QUOTED '${DUT_LETTER}')

    rumboot_add_external_project(externals/npe_rm -DCOMPILE_FROM_ROMBOOT="YES")

    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/tools/print-heaps.c
      )
    add_rumboot_target(
        CONFIGURATION ROM
        FILES common/bootrom/timer.c
    )

    ################################################################
    ######################## quad_spi tests ########################
    ################################################################
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

    ################################################################
    ######################### mdma tests ###########################
    ################################################################
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
    ################ common for tests with npe_rm ##################
    ################################################################
    if(DUT STREQUAL "MPE" OR DUT STREQUAL "VPE" OR DUT STREQUAL "PPE" OR DUT STREQUAL "NPE")
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
      set(NA_TEST_mpe_cfg_lut_file ${CMAKE_SOURCE_DIR}/src/platform/scr1/lib/drivers/mpe_regs_table_macro.bin)

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
      set(PLUSARG_mpe_cfg_lut_file_tag mpe_cfg_lut_file_tag)


      set(NA_RM_BIN_PATH ${CMAKE_BINARY_DIR}/${rumboot_dirname}/npe_rm/rm_core/rtl-tests)
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
                         +${PLUSARG_mpe_cfg_lut_file_tag}=${NA_TEST_mpe_cfg_lut_file}
      )
      set(NA_RM_PLUSARGS_LOOP 
        +${PLUSARG_num_iterations_file_tag}=${NA_TEST_num_iterations_file}
        +${PLUSARG_mpe_cfg_lut_file_tag}=${NA_TEST_mpe_cfg_lut_file}
      )
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

      if(DISABLE_NPE_RM_LOG)
        set(RM_LOGFILE /dev/null)
      else()
        set(RM_LOGFILE npe_rm.log)
      endif()

      ##################################################################
      ############# VPE_TESTS general settings and macros ##############
      ##################################################################
      if(DUT STREQUAL "VPE" OR DUT STREQUAL "NPE")
        # For The Misaligned Malloc In VPE DMA Tests
        set(MISALIGN_COUNT 0)
        macro(misalign_increment)
          if(MISALIGN_COUNT STREQUAL 15)
            set(MISALIGN_COUNT 1)
          else()
            MATH(EXPR MISALIGN_COUNT "${MISALIGN_COUNT} + 1")
          endif()
        endmacro()

        macro(ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA name rm_bin_name)
          misalign_increment()
          set(MISALIGN RANGE 0 ${MISALIGN_COUNT})
          foreach(IntMisalign ${MISALIGN})
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}_${IntMisalign}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DVPE_TraceMode_PPE=1 -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
          endforeach()
        endmacro()

        macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA name rm_bin_name)
          misalign_increment()
          set(MISALIGN 0 ${MISALIGN_COUNT})
          foreach(IntMisalign ${MISALIGN})
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}_${IntMisalign}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}_tight_${IntMisalign}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe.c
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                CFLAGS -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DFORCE_VPE_WDMA_EN=1 -DDUT=${DUT_LETTER_QUOTED}
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
          endforeach()
        endmacro()
      
        macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN name rm_bin_name axi_len)
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1 -DAXI_LEN=1 -DAxiLen=${axi_len} -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
        endmacro()
        
        macro(ADD_VPE_COUPLED_TEST_CONTROL_CONS_FORCE_WDMA name rm_bin_name)
          misalign_increment()
          set(MISALIGN 0 ${MISALIGN_COUNT})
          foreach(IntMisalign ${MISALIGN})
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}_${IntMisalign}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
          endforeach()
        endmacro()
        
        macro(ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL_FORCE_WDMA name rm_bin_name)
          misalign_increment()
          set(MISALIGN 0 ${MISALIGN_COUNT})
          foreach(IntMisalign ${MISALIGN})
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}_${IntMisalign}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
          endforeach()
        endmacro()  
        
        add_rumboot_target(
            CONFIGURATION ROM
            NAME VPE_1
            FILES scr1/targets/simple-rom/nu/vpe_regs/regs_vpe.c
        )  
      endif() # DUT STREQUAL VPE OR NPE

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
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
        endmacro()

        macro(ADD_VPE_COUPLED_TEST_CONTROL_CONS name rm_bin_name)
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
        endmacro()
      
        if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
            macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_CONS name bin_dir)
                add_rumboot_target(
                    CONFIGURATION ROM
                    NAME ${name}
                    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
                    CFLAGS -DFORCE_VPE_WDMA_EN=1
                    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
                    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                )
            endmacro()
        endif() #### EXPERIMENT_STAGE_2_SUB_1
      
        macro(ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL name rm_bin_name)
            add_rumboot_target(
                CONFIGURATION ROM
                NAME ${name}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
                CFLAGS -DFORCE_VPE_WDMA_EN=1
                PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )
        endmacro()

        if(DEFINED EXPERIMENT_STAGE_2_SUB_1) ####
            macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_PARALLEL name bin_dir)
                add_rumboot_target(
                    CONFIGURATION ROM
                    NAME ${name}
                    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
                    CFLAGS -DFORCE_VPE_WDMA_EN=1
                    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
                    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                )
            endmacro()

            macro(ADD_VPE_FROM_BINARY_TEST_LOOP name bin_dir)
                add_rumboot_target(
                    CONFIGURATION ROM
                    NAME ${name}
                    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
                    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                )
            endmacro()

            macro(ADD_VPE_FROM_BINARY_TEST_LOOP_FORCE_WDMA name bin_dir)
                add_rumboot_target(
                    CONFIGURATION ROM
                    NAME ${name}
                    CFLAGS -DFORCE_VPE_WDMA_EN=1
                    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
                    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
                    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                )
            endmacro()
        endif() #### EXPERIMENT_STAGE_2_SUB_1

        ##################################################################
        ########################### VPE_TESTS ############################
        ##################################################################
        # Tests on VPE::RELU
        ADD_VPE_COUPLED_TEST_LOOP(vpe_9_0_op0_relu_int32  main_vpe_9_0_op0_relu_int32  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_15_0_op1_relu_int32 main_vpe_15_0_op1_relu_int32 ) 
        ADD_VPE_COUPLED_TEST_LOOP(vpe_9_1_op0_relu_fp32   main_vpe_9_1_op0_relu_fp32   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_15_1_op1_relu_fp32  main_vpe_15_1_op1_relu_fp32  )
      
        # Tests on VPE::DEMUX::C3 pipeline mode-flow
        foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
          ADD_VPE_COUPLED_TEST_LOOP(vpe_3_c3_IN_INT32_${out_macro}  main_vpe_3_c3_IN_INT32_${out_macro} )
        endforeach()
        foreach(out_macro IN ITEMS OUT_INT16 OUT_FP16)
          ADD_VPE_COUPLED_TEST_LOOP(vpe_3_c3_IN_FP32_${out_macro}   main_vpe_3_c3_IN_FP32_${out_macro}  )
        endforeach()

        # Test on VPE::DEMUX OVERFLOW COUNTERS TESTS PIPELINE MODE-FLOW
        foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
          ADD_VPE_COUPLED_TEST_LOOP(vpe_overflow_counters_IN_INT32_${out_macro}  main_overflow_counters_IN_INT32_${out_macro} )
        endforeach()
        foreach(out_macro IN ITEMS OUT_INT16 OUT_FP16)
          ADD_VPE_COUPLED_TEST_LOOP(vpe_overflow_counters_IN_FP32_${out_macro}   main_overflow_counters_IN_FP32_${out_macro}  )
        endforeach()
        # не получилос(((
        # add_rumboot_target(
        #   CONFIGURATION ROM
        #   FEATURES NOCODE
        #   TESTGROUP vpe_overflow_counters-group
        #   LOAD vpe_overflow_counters_IN_INT32_OUT_INT8,vpe_overflow_counters_IN_INT32_OUT_INT16
        #   # LOAD rumboot-top-rumboot-scr1-Debug-ROM-vpe_overflow_counters_IN_INT32_OUT_INT8,rumboot-top-rumboot-scr1-Debug-ROM-vpe_overflow_counters_IN_INT32_OUT_INT16
        #   NAME vpe_overflow_counters
        # )

        # Tests on VPE::InputConverters
        ADD_VPE_COUPLED_TEST_LOOP(vpe_18_op2_c1  main_vpe_18_op2_c1 )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_16_op2_c2  main_vpe_16_op2_c2 )
      
        # Tests on VPE::NORM
        ADD_VPE_COUPLED_TEST_LOOP(vpe_8_0_op0_norm       main_vpe_8_0_op0_norm      )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_14_0_op1_norm      main_vpe_14_0_op1_norm     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_8_1_op0_norm_rnd   main_vpe_8_1_op0_norm_rnd  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_14_1_op1_norm_rnd  main_vpe_14_1_op1_norm_rnd )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_20_0_op2_norm      main_vpe_20_0_op2_norm     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_20_1_op2_norm_rnd  main_vpe_20_1_op2_norm_rnd )

        # Tests on VPE::LSHIFT
        ADD_VPE_COUPLED_TEST_LOOP(vpe_4_op0_lshift   main_vpe_4_op0_lshift  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_10_op1_lshift  main_vpe_10_op1_lshift )

        # Tests on VPE::Formater
        ADD_VPE_COUPLED_TEST_LOOP(vpe_6_0_op0_f_int   main_vpe_6_0_op0_f_int  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_12_0_op1_f_int  main_vpe_12_0_op1_f_int )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_6_1_op0_f_fp    main_vpe_6_1_op0_f_fp   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_12_1_op1_f_fp   main_vpe_12_1_op1_f_fp  )
      
        # Tests on VPE::ALU
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_0_op0_alu_int8_low      main_vpe_5_0_op0_alu_int8_low      )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_1_op0_alu_int8_middle   main_vpe_5_1_op0_alu_int8_middle   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_2_op0_alu_int8_high     main_vpe_5_2_op0_alu_int8_high     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_3_op0_alu_int16_low     main_vpe_5_3_op0_alu_int16_low     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_4_op0_alu_int16_middle  main_vpe_5_4_op0_alu_int16_middle  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_5_op0_alu_int16_high    main_vpe_5_5_op0_alu_int16_high    )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_5_6_op0_alu_fp32          main_vpe_5_6_op0_alu_fp32          )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_0_op1_alu_int8_low     main_vpe_11_0_op1_alu_int8_low     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_1_op1_alu_int8_middle  main_vpe_11_1_op1_alu_int8_middle  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_2_op1_alu_int8_high    main_vpe_11_2_op1_alu_int8_high    )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_3_op1_alu_int16_low    main_vpe_11_3_op1_alu_int16_low    )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_4_op1_alu_int16_middle main_vpe_11_4_op1_alu_int16_middle )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_5_op1_alu_int16_high   main_vpe_11_5_op1_alu_int16_high   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_11_6_op1_alu_fp32         main_vpe_11_6_op1_alu_fp32         )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_17_0_op2_alu_int8_low     main_vpe_17_0_op2_alu_int8_low     )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_17_1_op2_alu_int8_high    main_vpe_17_1_op2_alu_int8_high    )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_17_2_op2_alu_int16_low    main_vpe_17_2_op2_alu_int16_low    )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_17_3_op2_alu_int16_high   main_vpe_17_3_op2_alu_int16_high   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_17_4_op2_alu_fp32         main_vpe_17_4_op2_alu_fp32         )

        # Tests on VPE::MUL
        ADD_VPE_COUPLED_TEST_LOOP(vpe_7_0_op0_mul_int16   main_vpe_7_0_op0_mul_int16  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_7_1_op0_mul_int8    main_vpe_7_1_op0_mul_int8   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_7_2_op0_mul_fp32    main_vpe_7_2_op0_mul_fp32   )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_13_0_op1_mul_int16  main_vpe_13_0_op1_mul_int16 )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_13_1_op1_mul_int8   main_vpe_13_1_op1_mul_int8  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_13_2_op1_mul_fp32   main_vpe_13_2_op1_mul_fp32  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_19_0_op2_mul_int16  main_vpe_19_0_op2_mul_int16 )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_19_1_op2_mul_int8   main_vpe_19_1_op2_mul_int8  )
        ADD_VPE_COUPLED_TEST_LOOP(vpe_19_2_op2_mul_fp32   main_vpe_19_2_op2_mul_fp32  )

        # Tests on VPE::OP2::LUT
        foreach(in_macro IN ITEMS IN_INT32 IN_FP32)
          ADD_VPE_COUPLED_TEST_LOOP(vpe_21_0_lut_${in_macro}              main_vpe_21_0_lut_${in_macro}              ) # VPE_21
          ADD_VPE_COUPLED_TEST_LOOP(vpe_21_1_lut_addition_${in_macro}     main_vpe_21_1_lut_addition_${in_macro}     ) # VPE_21_addition
          ADD_VPE_COUPLED_TEST_LOOP(vpe_21_2_lut_out_of_range_${in_macro} main_vpe_21_2_lut_out_of_range_${in_macro} ) # VPE_21_out_of_range
          ADD_VPE_COUPLED_TEST_LOOP(vpe_21_3_lut_offset_${in_macro}       main_vpe_21_3_lut_offset_${in_macro}       ) # VPE_21_offset
          foreach(mirror_type IN ITEMS 0 1)
            ADD_VPE_COUPLED_TEST_LOOP(vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro} main_vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro} ) # VPE_21_offset. mirror mode
          endforeach()
        endforeach()
  
        # Test on VPE block's together work 
        ADD_VPE_COUPLED_TEST_LOOP(vpe_22_op0_together   main_vpe_22_op0_together   ) # VPE_22
        ADD_VPE_COUPLED_TEST_LOOP(vpe_23_op1_together   main_vpe_23_op1_together   ) # VPE_23
        ADD_VPE_COUPLED_TEST_LOOP(vpe_24_op2_together   main_vpe_24_op2_together   ) # VPE_24
        ADD_VPE_COUPLED_TEST_LOOP(vpe_25_op012_together main_vpe_25_op012_together ) # VPE_25

        # Test on VPE ???
        ADD_VPE_COUPLED_TEST_CONTROL_CONS(vpe_27_0_control_cons main_vpe_27_0_control_cons   ) # VPE_27
        ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL(vpe_27_1_control_par main_vpe_27_1_control_par ) # VPE_27
        ADD_VPE_COUPLED_TEST_LOOP(vpe_26_autonom_nowdma main_vpe_26_autonom)
        ADD_VPE_COUPLED_TEST_LOOP(vpe_28_perf main_28_perf) # VPE_28

        # Test on VPE special cases
        ADD_VPE_COUPLED_TEST_LOOP(vpe_special_cases_IN_FP32_OUT_FP16 main_vpe_special_cases_IN_FP32_OUT_FP16) # Test on special cases
      endif()  # if(DUT STREQUAL "VPE")

      ###################################################################
      ############### VPE_TESTS adapted for NPE assembly ################
      ###################################################################
      if(DUT STREQUAL "VPE" OR DUT STREQUAL "NPE")
        
        # Tests on VPE DMA
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(vpe_2_dma_int16_axi_len_0  main_vpe_2_dma_int16 0 )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(vpe_2_dma_int8_axi_len_1   main_vpe_2_dma_int8 1  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(vpe_2_dma_int16_axi_len_3  main_vpe_2_dma_int16 3 )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(vpe_2_dma_int8_axi_len_7   main_vpe_2_dma_int8 7  )
        
        # Tests on VPE::Formater
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_6_0_op0_f_int_dma  main_vpe_6_0_op0_f_int_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_6_1_op0_f_fp_dma   main_vpe_6_1_op0_f_fp_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_12_0_op1_f_int_dma main_vpe_12_0_op1_f_int_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_12_1_op1_f_fp_dma  main_vpe_12_1_op1_f_fp_dma  )
        
        # Tests on VPE::LSHIFT
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_4_op0_lshift_dma   main_vpe_4_op0_lshift_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_10_op1_lshift_dma  main_vpe_10_op1_lshift_dma )
        
        # Tests on VPE::InputConverters
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_18_op2_c1_dma      main_vpe_18_op2_c1_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_16_op2_c2_dma      main_vpe_16_op2_c2_dma )
      
        # Tests on VPE::DEMUX::C3 TESTS MEM MODE
        foreach(in_macro IN ITEMS IN_INT8 IN_INT16)
          foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
            ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_3_c3_${in_macro}_${out_macro}_dma       main_vpe_3_c3_${in_macro}_${out_macro}_dma  )
          endforeach()
        endforeach()
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_3_c3_IN_FP16_OUT_INT16_dma  main_vpe_3_c3_IN_FP16_OUT_INT16_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_3_c3_IN_FP16_OUT_FP16_dma   main_vpe_3_c3_IN_FP16_OUT_FP16_dma  )
      
        # Tests on VPE::RELU
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_9_0_op0_relu_int_dma   main_vpe_9_0_op0_relu_int_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_9_1_op0_relu_fp_dma    main_vpe_9_1_op0_relu_fp_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_15_0_op1_relu_int_dma  main_vpe_15_0_op1_relu_int_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_15_1_op1_relu_fp_dma   main_vpe_15_1_op1_relu_fp_dma  )

        # Tests on VPE::NORM
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_8_0_op0_norm_dma       main_vpe_8_0_op0_norm_dma      )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_8_1_op0_norm_rnd_dma   main_vpe_8_1_op0_norm_rnd_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_14_0_op1_norm_dma      main_vpe_14_0_op1_norm_dma     )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_14_1_op1_norm_rnd_dma  main_vpe_14_1_op1_norm_rnd_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_20_0_op2_norm_dma      main_vpe_20_0_op2_norm_dma     )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_20_1_op2_norm_rnd_dma  main_vpe_20_1_op2_norm_rnd_dma )
      
        # Tests on VPE channel mode
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_vec_ex_int_dma     main_vpe_op0_vec_ex_int )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_vec_ex_int_dma     main_vpe_op1_vec_ex_int )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_vec_ex_fp_dma      main_vpe_op0_vec_ex_fp  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_vec_ex_fp_dma      main_vpe_op1_vec_ex_fp  )
      
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_vec_ex_int main_vpe_op0_vec_ex_int )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_vec_ex_int main_vpe_op1_vec_ex_int )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_vec_ex_fp  main_vpe_op0_vec_ex_fp  )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_vec_ex_fp  main_vpe_op1_vec_ex_fp  )

        # Tests on VPE dma
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_2_dma_int16_dma main_vpe_2_dma_int16 )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_2_dma_int8_dma  main_vpe_2_dma_int8  )

        # Tests on VPE::ALU
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_0_op0_alu_int8_low_dma      main_vpe_5_0_op0_alu_int8_low_dma     )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_1_op0_alu_int8_middle_dma   main_vpe_5_1_op0_alu_int8_middle_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_2_op0_alu_int8_high_dma     main_vpe_5_2_op0_alu_int8_high_dma    )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_3_op0_alu_int16_low_dma     main_vpe_5_3_op0_alu_int16_low_dma    )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_4_op0_alu_int16_middle_dma  main_vpe_5_4_op0_alu_int16_middle_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_5_op0_alu_int16_high_dma    main_vpe_5_5_op0_alu_int16_high_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_5_6_op0_alu_fp32_dma          main_vpe_5_6_op0_alu_fp32_dma         )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_0_op1_alu_int8_low_dma     main_vpe_11_0_op1_alu_int8_low_dma    )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_1_op1_alu_int8_middle_dma  main_vpe_11_1_op1_alu_int8_middle_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_2_op1_alu_int8_high_dma    main_vpe_11_2_op1_alu_int8_high_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_3_op1_alu_int16_low_dma    main_vpe_11_3_op1_alu_int16_low_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_4_op1_alu_int16_middle_dma main_vpe_11_4_op1_alu_int16_middle_dma)
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_5_op1_alu_int16_high_dma   main_vpe_11_5_op1_alu_int16_high_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_11_6_op1_alu_fp32_dma         main_vpe_11_6_op1_alu_fp32_dma        )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_0_op2_alu_int8_low_dma     main_vpe_17_0_op2_alu_int8_low_dma    )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_1_op2_alu_int8_high_dma    main_vpe_17_1_op2_alu_int8_high_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_2_op2_alu_int16_low_dma    main_vpe_17_2_op2_alu_int16_low_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_3_op2_alu_int16_high_dma   main_vpe_17_3_op2_alu_int16_high_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_17_4_op2_alu_fp32_dma         main_vpe_17_4_op2_alu_fp32_dma        )

        # Tests on VPE::MUL
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_0_op0_mul_int16_dma  main_vpe_7_0_op0_mul_int16_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_1_op0_mul_int8_dma   main_vpe_7_1_op0_mul_int8_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_7_2_op0_mul_fp32_dma   main_vpe_7_2_op0_mul_fp32_dma   )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_0_op1_mul_int16_dma main_vpe_13_0_op1_mul_int16_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_1_op1_mul_int8_dma  main_vpe_13_1_op1_mul_int8_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_13_2_op1_mul_fp32_dma  main_vpe_13_2_op1_mul_fp32_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_0_op2_mul_int16_dma main_vpe_19_0_op2_mul_int16_dma )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_1_op2_mul_int8_dma  main_vpe_19_1_op2_mul_int8_dma  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_19_2_op2_mul_fp32_dma  main_vpe_19_2_op2_mul_fp32_dma  )
      
        # Tests on VPE channel mode
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_ch_mode_int_dma  main_vpe_op0_ch_mode_int )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_ch_mode_int_dma  main_vpe_op1_ch_mode_int )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op0_ch_mode_fp_dma   main_vpe_op0_ch_mode_fp  )
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_op1_ch_mode_fp_dma   main_vpe_op1_ch_mode_fp  )

        #Tests on VPE::OP2::LUT in context NPE
        foreach(in_macro IN ITEMS IN_INT16 IN_INT8 IN_FP16)
          ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_21_0_lut_${in_macro}_dma               main_vpe_21_0_lut_${in_macro}_dma              )
          ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_21_1_lut_addition_${in_macro}_dma      main_vpe_21_1_lut_addition_${in_macro}_dma     )
          ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_21_2_lut_out_of_range_${in_macro}_dma  main_vpe_21_2_lut_out_of_range_${in_macro}_dma )
          ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_21_3_lut_offset_${in_macro}_dma        main_vpe_21_3_lut_offset_${in_macro}_dma       )
          foreach(mirror_type IN ITEMS 0 1)
            ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro}_dma main_vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro}_dma) # VPE_21_offset. mirror mode
          endforeach()
        endforeach()

        # Tests on VPE+PPE channel mode ???
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_ch_mode_int_dma  main_vpe_op0_ch_mode_int )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_ch_mode_int_dma  main_vpe_op1_ch_mode_int )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op0_ch_mode_fp_dma   main_vpe_op0_ch_mode_fp  )
        ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_ppe_op1_ch_mode_fp_dma   main_vpe_op1_ch_mode_fp  )

        # Test on VPE block's together work 
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_22_op0_together_dma   main_vpe_22_op0_together_dma   ) # VPE_22
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_23_op1_together_dma   main_vpe_23_op1_together_dma   ) # VPE_23
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_24_op2_together_dma   main_vpe_24_op2_together_dma   ) # VPE_24
        ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_25_op012_together_dma main_vpe_25_op012_together_dma ) # VPE_25
        
        # Test on VPE ???
        ADD_VPE_COUPLED_TEST_CONTROL_CONS_FORCE_WDMA(vpe_26_0_control_cons_dma    main_vpe_26_0_control_cons_dma ) # VPE_27
        ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL_FORCE_WDMA(vpe_26_1_control_par_dma main_vpe_26_1_control_par_dma  ) # VPE_27
        
        # Tests on VPE ???
        #ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_26_autonom_dma  main_vpe_26_autonom )
        #ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_26_autonom_nowdma_dma main_vpe_26_autonom        ) # VPE_26  ?????????????
        #ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA(vpe_28_perf_dma           main_28_perf               ) # VPE_28  ?????????????


        # Какие-то переименования ???
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
      endif()  # if(DUT STREQUAL "VPE" OR "NPE")


      ##################################################################
      ############# PPE_TESTS general settings and macros ##############
      ##################################################################
      if(DUT STREQUAL "PPE" OR DUT STREQUAL "NPE")
        add_rumboot_target(
            CONFIGURATION ROM
            NAME PPE_1
            FILES scr1/targets/simple-rom/nu/ppe_regs/regs_ppe.c
        )
  	
        add_rumboot_target(
            CONFIGURATION ROM
            NAME NPE_1
            FILES scr1/targets/simple-rom/nu/npe_regs/npe_regs.c
        )
		add_rumboot_target(
            CONFIGURATION ROM
            NAME MPE_1
            FILES scr1/targets/simple-rom/nu/mpe_regs/regs_mpe.c
        )
      endif()

      if(NOT DEFINED NU_SEED)
        set(NU_SEED 1)
      endif()

      if(NOT DEFINED NU_IT_NMB)
        set(NU_IT_NMB 32)
      endif()

      set (i8_max   "--pool_meth 1 --data_type 0")
      set (i16_max  "--pool_meth 1 --data_type 1")
      set (fp16_max "--pool_meth 1 --data_type 2")

      set (i8_min   "--pool_meth 2 --data_type 0")
      set (i16_min  "--pool_meth 2 --data_type 1")
      set (fp16_min "--pool_meth 2 --data_type 2")

      set (i8_avg   "--pool_meth 0 --data_type 0")
      set (i16_avg  "--pool_meth 0 --data_type 1")
      set (fp16_avg "--pool_meth 0 --data_type 2")

      macro (ADD_PPE_TESTS name rm_bin_name ShowPerf DataSrc LBS RM_CFG_PARAM)

        set (TST_NMB 1)
        set (Sh_is_1 "--set_Sh 1 --Sh 1")
        set (Sw_is_1 "--set_Sw 1 --Sw 1 --w_max 128")

        foreach(i RANGE ${TST_NMB})

          if (i EQUAL TST_NMB)
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} ${Sh_is_1} ${Sw_is_1}")
          else()
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM}")
          endif()

          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${i}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe.c

            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} || exit 1

            CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}

            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )

          if (i EQUAL TST_NMB)
            math (EXPR NU_SEED "${NU_SEED} + 1")
          endif()
        endforeach()
      endmacro()

      set (w1_128_k1_16_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w1_128_k1_16_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w1_128_k1_16_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w1_128_k1_1_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w1_128_k1_1_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w1_128_k1_1_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w1_128_k15_15_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w1_128_k15_15_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w1_128_k15_15_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w1_128_k16_16_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w1_128_k16_16_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w1_128_k16_16_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w1_128_k16_32_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w1_128_k16_32_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w1_128_k16_32_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w1_128_k32_48_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w1_128_k32_48_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w1_128_k32_48_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w1_128_k48_64_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w1_128_k48_64_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w1_128_k48_64_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w1_128_k64_80_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w1_128_k64_80_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w1_128_k64_80_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w1_128_k80_96_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w1_128_k80_96_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w1_128_k80_96_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w1_128_k96_112_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w1_128_k96_112_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w1_128_k96_112_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w1_128_k112_128_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w1_128_k112_128_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w1_128_k112_128_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w1_128_k128_128_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w1_128_k128_128_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w1_128_k128_128_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w1_128_k128_256_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w1_128_k128_256_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w1_128_k128_256_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w1_128_k256_288_h1_1 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w1_128_k256_288_h2_2 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w1_128_k256_288_h2_16 "--pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w128_128_k1_16_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w128_128_k1_16_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w128_128_k1_16_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w128_128_k1_1_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w128_128_k1_1_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w128_128_k1_1_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w128_128_k15_15_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w128_128_k15_15_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w128_128_k15_15_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w128_128_k16_16_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w128_128_k16_16_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w128_128_k16_16_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w128_128_k16_32_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w128_128_k16_32_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w128_128_k16_32_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w128_128_k32_48_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w128_128_k32_48_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w128_128_k32_48_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w128_128_k48_64_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w128_128_k48_64_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w128_128_k48_64_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w128_128_k64_80_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w128_128_k64_80_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w128_128_k64_80_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w128_128_k80_96_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w128_128_k80_96_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w128_128_k80_96_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w128_128_k96_112_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w128_128_k96_112_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w128_128_k96_112_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w128_128_k112_128_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w128_128_k112_128_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w128_128_k112_128_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w128_128_k128_128_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w128_128_k128_128_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w128_128_k128_128_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w128_128_k128_256_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w128_128_k128_256_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w128_128_k128_256_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w128_128_k256_288_h1_1 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w128_128_k256_288_h2_2 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w128_128_k256_288_h2_16 "--pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w129_129_k1_16_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w129_129_k1_16_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w129_129_k1_16_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w129_129_k1_1_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w129_129_k1_1_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w129_129_k1_1_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w129_129_k15_15_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w129_129_k15_15_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w129_129_k15_15_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w129_129_k16_16_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w129_129_k16_16_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w129_129_k16_16_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w129_129_k16_32_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w129_129_k16_32_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w129_129_k16_32_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w129_129_k32_48_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w129_129_k32_48_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w129_129_k32_48_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w129_129_k48_64_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w129_129_k48_64_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w129_129_k48_64_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w129_129_k64_80_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w129_129_k64_80_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w129_129_k64_80_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w129_129_k80_96_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w129_129_k80_96_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w129_129_k80_96_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w129_129_k96_112_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w129_129_k96_112_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w129_129_k96_112_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w129_129_k112_128_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w129_129_k112_128_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w129_129_k112_128_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w129_129_k128_128_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w129_129_k128_128_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w129_129_k128_128_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w129_129_k128_256_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w129_129_k128_256_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w129_129_k128_256_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w129_129_k256_288_h1_1 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w129_129_k256_288_h2_2 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w129_129_k256_288_h2_16 "--pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w128_256_k1_16_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w128_256_k1_16_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w128_256_k1_16_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w128_256_k1_1_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w128_256_k1_1_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w128_256_k1_1_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w128_256_k15_15_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w128_256_k15_15_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w128_256_k15_15_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w128_256_k16_16_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w128_256_k16_16_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w128_256_k16_16_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w128_256_k16_32_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w128_256_k16_32_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w128_256_k16_32_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w128_256_k32_48_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w128_256_k32_48_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w128_256_k32_48_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w128_256_k48_64_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w128_256_k48_64_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w128_256_k48_64_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w128_256_k64_80_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w128_256_k64_80_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w128_256_k64_80_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w128_256_k80_96_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w128_256_k80_96_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w128_256_k80_96_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w128_256_k96_112_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w128_256_k96_112_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w128_256_k96_112_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w128_256_k112_128_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w128_256_k112_128_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w128_256_k112_128_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w128_256_k128_128_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w128_256_k128_128_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w128_256_k128_128_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w128_256_k128_256_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w128_256_k128_256_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w128_256_k128_256_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w128_256_k256_288_h1_1 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w128_256_k256_288_h2_2 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w128_256_k256_288_h2_16 "--pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w255_255_k1_16_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w255_255_k1_16_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w255_255_k1_16_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w255_255_k1_1_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w255_255_k1_1_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w255_255_k1_1_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w255_255_k15_15_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w255_255_k15_15_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w255_255_k15_15_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w255_255_k16_16_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w255_255_k16_16_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w255_255_k16_16_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w255_255_k16_32_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w255_255_k16_32_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w255_255_k16_32_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w255_255_k32_48_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w255_255_k32_48_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w255_255_k32_48_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w255_255_k48_64_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w255_255_k48_64_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w255_255_k48_64_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w255_255_k64_80_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w255_255_k64_80_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w255_255_k64_80_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w255_255_k80_96_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w255_255_k80_96_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w255_255_k80_96_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w255_255_k96_112_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w255_255_k96_112_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w255_255_k96_112_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w255_255_k112_128_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w255_255_k112_128_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w255_255_k112_128_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w255_255_k128_128_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w255_255_k128_128_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w255_255_k128_128_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w255_255_k128_256_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w255_255_k128_256_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w255_255_k128_256_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w255_255_k256_288_h1_1 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w255_255_k256_288_h2_2 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w255_255_k256_288_h2_16 "--pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w256_256_k1_16_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w256_256_k1_16_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w256_256_k1_16_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w256_256_k1_1_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w256_256_k1_1_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w256_256_k1_1_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w256_256_k15_15_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w256_256_k15_15_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w256_256_k15_15_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w256_256_k16_16_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w256_256_k16_16_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w256_256_k16_16_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w256_256_k16_32_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w256_256_k16_32_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w256_256_k16_32_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w256_256_k32_48_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w256_256_k32_48_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w256_256_k32_48_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w256_256_k48_64_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w256_256_k48_64_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w256_256_k48_64_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w256_256_k64_80_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w256_256_k64_80_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w256_256_k64_80_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w256_256_k80_96_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w256_256_k80_96_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w256_256_k80_96_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w256_256_k96_112_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w256_256_k96_112_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w256_256_k96_112_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w256_256_k112_128_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w256_256_k112_128_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w256_256_k112_128_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w256_256_k128_128_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w256_256_k128_128_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w256_256_k128_128_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w256_256_k128_256_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w256_256_k128_256_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w256_256_k128_256_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w256_256_k256_288_h1_1 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w256_256_k256_288_h2_2 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w256_256_k256_288_h2_16 "--pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
      set (w256_288_k1_16_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
      set (w256_288_k1_16_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
      set (w256_288_k1_16_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
      set (w256_288_k1_1_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
      set (w256_288_k1_1_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
      set (w256_288_k1_1_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
      set (w256_288_k15_15_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
      set (w256_288_k15_15_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
      set (w256_288_k15_15_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
      set (w256_288_k16_16_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
      set (w256_288_k16_16_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
      set (w256_288_k16_16_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
      set (w256_288_k16_32_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
      set (w256_288_k16_32_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
      set (w256_288_k16_32_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
      set (w256_288_k32_48_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
      set (w256_288_k32_48_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
      set (w256_288_k32_48_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
      set (w256_288_k48_64_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
      set (w256_288_k48_64_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
      set (w256_288_k48_64_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
      set (w256_288_k64_80_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
      set (w256_288_k64_80_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
      set (w256_288_k64_80_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
      set (w256_288_k80_96_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
      set (w256_288_k80_96_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
      set (w256_288_k80_96_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
      set (w256_288_k96_112_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
      set (w256_288_k96_112_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
      set (w256_288_k96_112_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
      set (w256_288_k112_128_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
      set (w256_288_k112_128_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
      set (w256_288_k112_128_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
      set (w256_288_k128_128_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
      set (w256_288_k128_128_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
      set (w256_288_k128_128_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
      set (w256_288_k128_256_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
      set (w256_288_k128_256_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
      set (w256_288_k128_256_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
      set (w256_288_k256_288_h1_1 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
      set (w256_288_k256_288_h2_2 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
      set (w256_288_k256_288_h2_16 "--pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 2 --h_max 16")

      set (vpe_ppe_wkh_comb
        w1_128_k1_16_h1_1
        w1_128_k1_16_h2_2
        w1_128_k1_16_h2_16
        w1_128_k1_1_h1_1
        w1_128_k1_1_h2_2
        w1_128_k1_1_h2_16
        w1_128_k15_15_h1_1
        w1_128_k15_15_h2_2
        w1_128_k15_15_h2_16
        w1_128_k16_16_h1_1
        w1_128_k16_16_h2_2
        w1_128_k16_16_h2_16
        w1_128_k16_32_h1_1
        w1_128_k16_32_h2_2
        w1_128_k16_32_h2_16
        w1_128_k32_48_h1_1
        w1_128_k32_48_h2_2
        w1_128_k32_48_h2_16
        w1_128_k48_64_h1_1
        w1_128_k48_64_h2_2
        w1_128_k48_64_h2_16
        w1_128_k64_80_h1_1
        w1_128_k64_80_h2_2
        w1_128_k64_80_h2_16
        w1_128_k80_96_h1_1
        w1_128_k80_96_h2_2
        w1_128_k80_96_h2_16
        w1_128_k96_112_h1_1
        w1_128_k96_112_h2_2
        w1_128_k96_112_h2_16
        w1_128_k112_128_h1_1
        w1_128_k112_128_h2_2
        w1_128_k112_128_h2_16
        w1_128_k128_128_h1_1
        w1_128_k128_128_h2_2
        w1_128_k128_128_h2_16
        w1_128_k128_256_h1_1
        w1_128_k128_256_h2_2
        w1_128_k128_256_h2_16
        w1_128_k256_288_h1_1
        w1_128_k256_288_h2_2
        w1_128_k256_288_h2_16
        w128_128_k1_16_h1_1
        w128_128_k1_16_h2_2
        w128_128_k1_16_h2_16
        w128_128_k1_1_h1_1
        w128_128_k1_1_h2_2
        w128_128_k1_1_h2_16
        w128_128_k15_15_h1_1
        w128_128_k15_15_h2_2
        w128_128_k15_15_h2_16
        w128_128_k16_16_h1_1
        w128_128_k16_16_h2_2
        w128_128_k16_16_h2_16
        w128_128_k16_32_h1_1
        w128_128_k16_32_h2_2
        w128_128_k16_32_h2_16
        w128_128_k32_48_h1_1
        w128_128_k32_48_h2_2
        w128_128_k32_48_h2_16
        w128_128_k48_64_h1_1
        w128_128_k48_64_h2_2
        w128_128_k48_64_h2_16
        w128_128_k64_80_h1_1
        w128_128_k64_80_h2_2
        w128_128_k64_80_h2_16
        w128_128_k80_96_h1_1
        w128_128_k80_96_h2_2
        w128_128_k80_96_h2_16
        w128_128_k96_112_h1_1
        w128_128_k96_112_h2_2
        w128_128_k96_112_h2_16
        w128_128_k112_128_h1_1
        w128_128_k112_128_h2_2
        w128_128_k112_128_h2_16
        w128_128_k128_128_h1_1
        w128_128_k128_128_h2_2
        w128_128_k128_128_h2_16
        w128_128_k128_256_h1_1
        w128_128_k128_256_h2_2
        w128_128_k128_256_h2_16
        w128_128_k256_288_h1_1
        w128_128_k256_288_h2_2
        w128_128_k256_288_h2_16
        w129_129_k1_16_h1_1
        w129_129_k1_16_h2_2
        w129_129_k1_16_h2_16
        w129_129_k1_1_h1_1
        w129_129_k1_1_h2_2
        w129_129_k1_1_h2_16
        w129_129_k15_15_h1_1
        w129_129_k15_15_h2_2
        w129_129_k15_15_h2_16
        w129_129_k16_16_h1_1
        w129_129_k16_16_h2_2
        w129_129_k16_16_h2_16
        w129_129_k16_32_h1_1
        w129_129_k16_32_h2_2
        w129_129_k16_32_h2_16
        w129_129_k32_48_h1_1
        w129_129_k32_48_h2_2
        w129_129_k32_48_h2_16
        w129_129_k48_64_h1_1
        w129_129_k48_64_h2_2
        w129_129_k48_64_h2_16
        w129_129_k64_80_h1_1
        w129_129_k64_80_h2_2
        w129_129_k64_80_h2_16
        w129_129_k80_96_h1_1
        w129_129_k80_96_h2_2
        w129_129_k80_96_h2_16
        w129_129_k96_112_h1_1
        w129_129_k96_112_h2_2
        w129_129_k96_112_h2_16
        w129_129_k112_128_h1_1
        w129_129_k112_128_h2_2
        w129_129_k112_128_h2_16
        w129_129_k128_128_h1_1
        w129_129_k128_128_h2_2
        w129_129_k128_128_h2_16
        w129_129_k128_256_h1_1
        w129_129_k128_256_h2_2
        w129_129_k128_256_h2_16
        w129_129_k256_288_h1_1
        w129_129_k256_288_h2_2
        w129_129_k256_288_h2_16
        w128_256_k1_16_h1_1
        w128_256_k1_16_h2_2
        w128_256_k1_16_h2_16
        w128_256_k1_1_h1_1
        w128_256_k1_1_h2_2
        w128_256_k1_1_h2_16
        w128_256_k15_15_h1_1
        w128_256_k15_15_h2_2
        w128_256_k15_15_h2_16
        w128_256_k16_16_h1_1
        w128_256_k16_16_h2_2
        w128_256_k16_16_h2_16
        w128_256_k16_32_h1_1
        w128_256_k16_32_h2_2
        w128_256_k16_32_h2_16
        w128_256_k32_48_h1_1
        w128_256_k32_48_h2_2
        w128_256_k32_48_h2_16
        w128_256_k48_64_h1_1
        w128_256_k48_64_h2_2
        w128_256_k48_64_h2_16
        w128_256_k64_80_h1_1
        w128_256_k64_80_h2_2
        w128_256_k64_80_h2_16
        w128_256_k80_96_h1_1
        w128_256_k80_96_h2_2
        w128_256_k80_96_h2_16
        w128_256_k96_112_h1_1
        w128_256_k96_112_h2_2
        w128_256_k96_112_h2_16
        w128_256_k112_128_h1_1
        w128_256_k112_128_h2_2
        w128_256_k112_128_h2_16
        w128_256_k128_128_h1_1
        w128_256_k128_128_h2_2
        w128_256_k128_128_h2_16
        w128_256_k128_256_h1_1
        w128_256_k128_256_h2_2
        w128_256_k128_256_h2_16
        w128_256_k256_288_h1_1
        w128_256_k256_288_h2_2
        w128_256_k256_288_h2_16
        w255_255_k1_16_h1_1
        w255_255_k1_16_h2_2
        w255_255_k1_16_h2_16
        w255_255_k1_1_h1_1
        w255_255_k1_1_h2_2
        w255_255_k1_1_h2_16
        w255_255_k15_15_h1_1
        w255_255_k15_15_h2_2
        w255_255_k15_15_h2_16
        w255_255_k16_16_h1_1
        w255_255_k16_16_h2_2
        w255_255_k16_16_h2_16
        w255_255_k16_32_h1_1
        w255_255_k16_32_h2_2
        w255_255_k16_32_h2_16
        w255_255_k32_48_h1_1
        w255_255_k32_48_h2_2
        w255_255_k32_48_h2_16
        w255_255_k48_64_h1_1
        w255_255_k48_64_h2_2
        w255_255_k48_64_h2_16
        w255_255_k64_80_h1_1
        w255_255_k64_80_h2_2
        w255_255_k64_80_h2_16
        w255_255_k80_96_h1_1
        w255_255_k80_96_h2_2
        w255_255_k80_96_h2_16
        w255_255_k96_112_h1_1
        w255_255_k96_112_h2_2
        w255_255_k96_112_h2_16
        w255_255_k112_128_h1_1
        w255_255_k112_128_h2_2
        w255_255_k112_128_h2_16
        w255_255_k128_128_h1_1
        w255_255_k128_128_h2_2
        w255_255_k128_128_h2_16
        w255_255_k128_256_h1_1
        w255_255_k128_256_h2_2
        w255_255_k128_256_h2_16
        w255_255_k256_288_h1_1
        w255_255_k256_288_h2_2
        w255_255_k256_288_h2_16
        w256_256_k1_16_h1_1
        w256_256_k1_16_h2_2
        w256_256_k1_16_h2_16
        w256_256_k1_1_h1_1
        w256_256_k1_1_h2_2
        w256_256_k1_1_h2_16
        w256_256_k15_15_h1_1
        w256_256_k15_15_h2_2
        w256_256_k15_15_h2_16
        w256_256_k16_16_h1_1
        w256_256_k16_16_h2_2
        w256_256_k16_16_h2_16
        w256_256_k16_32_h1_1
        w256_256_k16_32_h2_2
        w256_256_k16_32_h2_16
        w256_256_k32_48_h1_1
        w256_256_k32_48_h2_2
        w256_256_k32_48_h2_16
        w256_256_k48_64_h1_1
        w256_256_k48_64_h2_2
        w256_256_k48_64_h2_16
        w256_256_k64_80_h1_1
        w256_256_k64_80_h2_2
        w256_256_k64_80_h2_16
        w256_256_k80_96_h1_1
        w256_256_k80_96_h2_2
        w256_256_k80_96_h2_16
        w256_256_k96_112_h1_1
        w256_256_k96_112_h2_2
        w256_256_k96_112_h2_16
        w256_256_k112_128_h1_1
        w256_256_k112_128_h2_2
        w256_256_k112_128_h2_16
        w256_256_k128_128_h1_1
        w256_256_k128_128_h2_2
        w256_256_k128_128_h2_16
        w256_256_k128_256_h1_1
        w256_256_k128_256_h2_2
        w256_256_k128_256_h2_16
        w256_256_k256_288_h1_1
        w256_256_k256_288_h2_2
        w256_256_k256_288_h2_16
        w256_288_k1_16_h1_1
        w256_288_k1_16_h2_2
        w256_288_k1_16_h2_16
        w256_288_k1_1_h1_1
        w256_288_k1_1_h2_2
        w256_288_k1_1_h2_16
        w256_288_k15_15_h1_1
        w256_288_k15_15_h2_2
        w256_288_k15_15_h2_16
        w256_288_k16_16_h1_1
        w256_288_k16_16_h2_2
        w256_288_k16_16_h2_16
        w256_288_k16_32_h1_1
        w256_288_k16_32_h2_2
        w256_288_k16_32_h2_16
        w256_288_k32_48_h1_1
        w256_288_k32_48_h2_2
        w256_288_k32_48_h2_16
        w256_288_k48_64_h1_1
        w256_288_k48_64_h2_2
        w256_288_k48_64_h2_16
        w256_288_k64_80_h1_1
        w256_288_k64_80_h2_2
        w256_288_k64_80_h2_16
        w256_288_k80_96_h1_1
        w256_288_k80_96_h2_2
        w256_288_k80_96_h2_16
        w256_288_k96_112_h1_1
        w256_288_k96_112_h2_2
        w256_288_k96_112_h2_16
        w256_288_k112_128_h1_1
        w256_288_k112_128_h2_2
        w256_288_k112_128_h2_16
        w256_288_k128_128_h1_1
        w256_288_k128_128_h2_2
        w256_288_k128_128_h2_16
        w256_288_k128_256_h1_1
        w256_288_k128_256_h2_2
        w256_288_k128_256_h2_16
        w256_288_k256_288_h1_1
        w256_288_k256_288_h2_2
        w256_288_k256_288_h2_16
      )

      macro (ADD_VPE_PPE_TESTS_OLD name rm_bin_name ShowPerf LBS RM_CFG_PARAM)

        set(TST_NMB 1)

        foreach(i RANGE ${TST_NMB})

          set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type ${i}")

          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_${i}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe.c

            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} || exit 1

            CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}

            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )

          if (i EQUAL TST_NMB)
            math (EXPR NU_SEED "${NU_SEED} + 1")
          endif()
        endforeach()
      endmacro()

      macro (ADD_VPE_PPE_TESTS name_in RM_CFG_PARAM)

        set (ShowPerf "NotShowPerf")
        set (TST_NMB 3)

        foreach(i RANGE ${TST_NMB})

          if (i EQUAL 0)
            set (name "${name_in}_LIN_8b")
            set (rm_bin_name "main_vpe_ppe")
            set (LBS "LIN")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
          elseif (i EQUAL 1)
            set (name "${name_in}_LIN_16b")
            set (rm_bin_name "main_vpe_ppe")
            set (LBS "LIN")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
          elseif (i EQUAL 2)
            set (name "${name_in}_BOX_8b")
            set (rm_bin_name "main_vpe_ppe_box")
            set (LBS "BOX")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
          elseif (i EQUAL 3)
            set (name "${name_in}_BOX_16b")
            set (rm_bin_name "main_vpe_ppe_box")
            set (LBS "BOX")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
          endif()

          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe.c

            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} || exit 1

            CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}

            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )

          if (i EQUAL TST_NMB)
            math (EXPR NU_SEED "${NU_SEED} + 1")
          endif()

        endforeach()
      endmacro()

      macro (ADD_VPE_PPE_WKH_COMB test_list_name)

        set (ShowPerf "NotShowPerf")
        set (i 0)

        foreach(name_in ${${test_list_name}})

          math (EXPR j "${i} % 4")
          math (EXPR i "${i} + 1")

          set (RM_CFG_PARAM ${${name_in}})

          if (j EQUAL 0)
            set (name "vpe_ppe_${name_in}_LIN_8b")
            set (rm_bin_name "main_vpe_ppe")
            set (LBS "LIN")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
          elseif (j EQUAL 1)
            set (name "vpe_ppe_${name_in}_LIN_16b")
            set (rm_bin_name "main_vpe_ppe")
            set (LBS "LIN")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
          elseif (j EQUAL 2)
            set (name "vpe_ppe_${name_in}_BOX_8b")
            set (rm_bin_name "main_vpe_ppe_box")
            set (LBS "BOX")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
          elseif (j EQUAL 3)
            set (name "vpe_ppe_${name_in}_BOX_16b")
            set (rm_bin_name "main_vpe_ppe_box")
            set (LBS "BOX")
            set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
          endif()
        
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe.c
        
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} || exit 1
        
            CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}
        
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
        
            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )
        
          if (j EQUAL 3)
            math (EXPR NU_SEED "${NU_SEED} + 1")
          endif()
        
        endforeach()
      endmacro()

      macro (ADD_VPE_PPE_WKH_COMB_ALL test_list_name)

        set (ShowPerf "NotShowPerf")
        set (TST_NMB 3)
        set (i 0)

        foreach(name_in ${${test_list_name}})
          foreach(i RANGE ${TST_NMB})

            if (i EQUAL 0)
              set (name "vpe_ppe_${name_in}_LIN_8b")
              set (rm_bin_name "main_vpe_ppe")
              set (LBS "LIN")
              set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
            elseif (i EQUAL 1)
              set (name "vpe_ppe_${name_in}_LIN_16b")
              set (rm_bin_name "main_vpe_ppe")
              set (LBS "LIN")
              set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
            elseif (i EQUAL 2)
              set (name "vpe_ppe_${name_in}_BOX_8b")
              set (rm_bin_name "main_vpe_ppe_box")
              set (LBS "BOX")
              set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
            elseif (i EQUAL 3)
              set (name "vpe_ppe_${name_in}_BOX_16b")
              set (rm_bin_name "main_vpe_ppe_box")
              set (LBS "BOX")
              set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
            endif()

            add_rumboot_target(
              CONFIGURATION ROM
              NAME ${name}
              FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe.c

              PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} || exit 1

              CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

              IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}

              SUBPROJECT_DEPS npe_rm:${rm_bin_name}
            )

            if (i EQUAL TST_NMB)
              math (EXPR NU_SEED "${NU_SEED} + 1")
              set (i 0)
            endif()

          endforeach()
        endforeach()
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

      ##################################################################
      ########################### PPE_TESTS ############################
      ##################################################################
      if(DUT STREQUAL "PPE" OR DUT STREQUAL "NPE")

        ADD_PPE_TESTS(ppe_i8_max_ml main_ppe NotShowPerf MEMtoPPE LIN ${i8_max})
        ADD_PPE_TESTS(ppe_i16_max_ml main_ppe NotShowPerf MEMtoPPE LIN ${i16_max})
        ADD_PPE_TESTS(ppe_fp16_max_ml main_ppe NotShowPerf MEMtoPPE LIN ${fp16_max})

        ADD_PPE_TESTS(ppe_i8_min_ml main_ppe NotShowPerf MEMtoPPE LIN ${i8_min})
        ADD_PPE_TESTS(ppe_i16_min_ml main_ppe NotShowPerf MEMtoPPE LIN ${i16_min})
        ADD_PPE_TESTS(ppe_fp16_min_ml main_ppe NotShowPerf MEMtoPPE LIN ${fp16_min})

        ADD_PPE_TESTS(ppe_i8_avg_ml main_ppe NotShowPerf MEMtoPPE LIN ${i8_avg})
        ADD_PPE_TESTS(ppe_i16_avg_ml main_ppe NotShowPerf MEMtoPPE LIN ${i16_avg})
        ADD_PPE_TESTS(ppe_fp16_avg_ml main_ppe NotShowPerf MEMtoPPE LIN ${fp16_avg})

        #ADD_VPE_PPE_TESTS_OLD(vpe_ppe_w129_129_k32_48_h2_2_l  main_vpe_ppe      NotShowPerf LIN ${w129_129_k32_48_h2_2} )
        #ADD_VPE_PPE_TESTS_OLD(vpe_ppe_w129_129_k32_48_h2_2_b  main_vpe_ppe_box  NotShowPerf BOX ${w129_129_k32_48_h2_2} )
        #ADD_VPE_PPE_TESTS(vpe_ppe_w1_128_k1_16_h1_1 ${w1_128_k1_16_h1_1})

        ADD_VPE_PPE_WKH_COMB(vpe_ppe_wkh_comb)
        #ADD_VPE_PPE_WKH_COMB_ALL(vpe_ppe_wkh_comb)

      endif()

      if(DUT STREQUAL "PPE")
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

      ###################################################################
      ############# MPE_TESTS general settings and macros ###############
      ###################################################################
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

        ###################################################################
        ########################### MPE_TESTS #############################
        ###################################################################
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

      ###################################################################
      ############# NPE_TESTS general settings and macros ###############
      ###################################################################
      if(DUT STREQUAL "NPE")
        set(NPE_BINS ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/npe)
        add_rumboot_target(
            CONFIGURATION ROM
            NAME RESNET
            FILES scr1/targets/simple-rom/nu/npe/npe_resnet.c
            PREPCMD cp ${NPE_BINS}/resnet_bins/cmd.bin ${NPE_BINS}/resnet_bins/cmd.bin.metrics ${NPE_BINS}/resnet_bins/cube.bin ${NPE_BINS}/resnet_bins/cube.bin.metrics ${NPE_BINS}/resnet_bins/etalon.bin ${NPE_BINS}/resnet_bins/etalon.bin.metrics ${NPE_BINS}/resnet_bins/op0.bin ${NPE_BINS}/resnet_bins/op0.bin.metrics ${NPE_BINS}/resnet_bins/warr.bin ${NPE_BINS}/resnet_bins/warr.bin.metrics -t .
            IRUN_FLAGS ${NA_RM_PLUSARGS}
         )
      
        set(MPE_DEMO_PATH src/platform/scr1/targets/simple-rom/nu/npe_mpe_stage2)
        set(MPE_PARSE_TEST_STAGE2 ${CMAKE_SOURCE_DIR}/${MPE_DEMO_PATH}/parse_mpe_arrays_stage2.pl)
        
        macro (ADD_MPE_SINGLE_TEST name trunc) # trunc=TRUNC0/TRUNC16
          set(MPE_TEST_SHEET ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment2/${name}/mpe_arrays.txt)
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES scr1/targets/simple-rom/nu/npe_mpe_stage2/mpe_single.c
            CFLAGS -D${trunc}
            PREPCMD ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} ${trunc} < ${MPE_TEST_SHEET}
            IRUN_FLAGS ${NA_RM_PLUSARGS}
          )
        endmacro()
        
        ###################################################################
        ########################### NPE_TESTS #############################
        ###################################################################

        ADD_MPE_SINGLE_TEST(MPE_2 TRUNC0)
        ADD_MPE_SINGLE_TEST(MPE_3 TRUNC0)
        ADD_MPE_SINGLE_TEST(MPE_4 TRUNC0)
        ADD_MPE_SINGLE_TEST(MPE_5 TRUNC0)
        ADD_MPE_SINGLE_TEST(MPE_6 TRUNC0)
        ADD_MPE_SINGLE_TEST(MPE_7 TRUNC0)

        ADD_MPE_SINGLE_TEST(MPE_8 TRUNC0) # mu int8 test
        ADD_MPE_SINGLE_TEST(MPE_9 TRUNC0) # mu int16 test
        ADD_MPE_SINGLE_TEST(MPE_10 TRUNC0) # mu fp16 test
        ADD_MPE_SINGLE_TEST(MPE_11 TRUNC0) # mu fp16 NaN test
        ADD_MPE_SINGLE_TEST(MPE_12 TRUNC16) # mu fp16 inf test
        ADD_MPE_SINGLE_TEST(MPE_13 TRUNC0) # mu+acc int8 test
        ADD_MPE_SINGLE_TEST(MPE_14 TRUNC0) # mu+acc int16 test
        ADD_MPE_SINGLE_TEST(MPE_15 TRUNC0) # mu int norm test
        ADD_MPE_SINGLE_TEST(MPE_16 TRUNC16) # mu+acc fp16 test

        macro (ADD_MPE_CONV_TEST name trunc) # trunc=TRUNC0/TRUNC16
          set(MPE_TEST_SHEET ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment2/${name}_CONV/mpe_arrays.txt)
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}_CONV
            FILES scr1/targets/simple-rom/nu/npe_mpe_stage2/mpe_conv.c
            CFLAGS -D${trunc}
            PREPCMD ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} ${trunc} < ${MPE_TEST_SHEET}
            IRUN_FLAGS ${NA_RM_PLUSARGS}
          )
        endmacro()
        ADD_MPE_CONV_TEST(MPE_2 TRUNC16)

        set(CU_TEST_DIR ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/na_cu)
        add_rumboot_target(
          CONFIGURATION ROM
          NAME NA_7 #na_cu_mpe_vpe
          FILES scr1/targets/simple-rom/nu/na_cu/cu_dpnd_mpe_vpe.c
          PREPCMD cp ${CU_TEST_DIR}/in_arr.bin ${CU_TEST_DIR}/in_arr.bin.metrics ${CU_TEST_DIR}/etalon.bin ${CU_TEST_DIR}/etalon.bin.metrics -t .
          IRUN_FLAGS +in_arr=in_arr.bin
                     +metrics_in_arr=in_arr.bin.metrics
                     +etalon_arr=etalon.bin
                     +metrics_etalon_arr=etalon.bin.metrics
        )
        add_rumboot_target(
          CONFIGURATION ROM
          NAME NA_8 #na_cu_buffers
          FILES scr1/targets/simple-rom/nu/na_cu/cu_buffers.c
          PREPCMD cp ${CU_TEST_DIR}/in_arr.bin ${CU_TEST_DIR}/in_arr.bin.metrics -t .
          IRUN_FLAGS +in_arr=in_arr.bin
                     +metrics_in_arr=in_arr.bin.metrics
        )

        macro(ADD_NPE_SIMPLE_TEST name filename)
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES ${filename}
          )
        endmacro()

        macro(ADD_NPE_MPE_VPE_TEST name rm_bin_name)
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe.c
            CFLAGS -DDONT_USE_PPE=1
            PREPCMD 
              ${NA_RM_BIN_PATH}/${rm_bin_name} 
              ${NA_RM_KEYS} 
              --cube_TF_file=${NPE_BINS}/resnet_bins/${name}/cube.bin.0
              --warr_TF_file=${NPE_BINS}/resnet_bins/${name}/warr.bin.0
              --op0_TF_file=${NPE_BINS}/resnet_bins/${name}/op0.bin.0
              > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )
        endmacro()

        ADD_NPE_SIMPLE_TEST(npe_regs scr1/targets/simple-rom/nu/npe_regs/npe_regs.c)

        # foreach(in_macro IN ITEMS IN_INT8 IN_INT16 IN_FP16)
        #   ADD_NPE_MPE_VPE_TEST(npe_mpe_only_rnd_${in_macro} main_mpe_rnd_${in_macro})
        # endforeach()

        foreach(label RANGE 1 47)
          ADD_NPE_MPE_VPE_TEST(npe_mpe_direct_ex_MPE_CFG_${label} main_mpe_direct_ex_MPE_CFG_${label})
          ADD_NPE_MPE_VPE_TEST(npe_mpe_direct_ex_MPE_CFG_${label}_FP main_mpe_direct_ex_MPE_CFG_${label}_FP)
          ADD_NPE_MPE_VPE_TEST(npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE)
          ADD_NPE_MPE_VPE_TEST(npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE)
        endforeach()


        macro(ADD_NPE_COMPLEX_TEST name rm_bin_name)
          add_rumboot_target(
            CONFIGURATION ROM
            NAME ${name}
            FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe.c
            PREPCMD 
              ${NA_RM_BIN_PATH}/${rm_bin_name} 
              ${NA_RM_KEYS} 
              --cube_TF_file=${NPE_BINS}/resnet_bins/${name}/cube.bin.0
              --warr_TF_file=${NPE_BINS}/resnet_bins/${name}/warr.bin.0
              --op0_TF_file=${NPE_BINS}/resnet_bins/${name}/op0.bin.0
              > ${RM_LOGFILE} || exit 1
            IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )
        endmacro()

        foreach(label RANGE 1 47)
          ADD_NPE_COMPLEX_TEST(npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE)
          ADD_NPE_COMPLEX_TEST(npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE)
        endforeach()

        ADD_NPE_COMPLEX_TEST(npe_all_ex_IN_INT16 main_npe_all_ex_IN_INT16)
        
        ###################################################################
        # Resnet-test 
        ADD_NPE_MPE_VPE_TEST(resnet_IN_FP16_LAYER0_MPE main_resnet_IN_FP16_LAYER0_MPE) # LAYER0_MPE
        ADD_NPE_MPE_VPE_TEST(resnet_IN_FP16_LAYER0_MPE_BN main_resnet_IN_FP16_LAYER0_MPE_BN) # LAYER0_MPE_BN
        ADD_NPE_MPE_VPE_TEST(resnet_IN_FP16_LAYER0_MPE_BN_RELU main_resnet_IN_FP16_LAYER0_MPE_BN_RELU) # LAYER0_MPE_BN_RELU
        ADD_NPE_COMPLEX_TEST(resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE main_resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE) # LAYER0_MPE_BN_RELU_PPE
        ###################################################################
      endif() # if(DUT STREQUAL "NPE")
    endif()  # if(DUT STREQUAL MPE,VPE,PPE,NPE)
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

