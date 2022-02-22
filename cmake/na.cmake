#TODO:
# - Вычистить останки EXPERIMENT_STAGE_XXXXX из кода
# - Директории PPE_EXPER_DIR & MPE_TEST_SHEETS_DIR. Из-за них не будет работать standalone сборка№

macro(_na_init_variables DUT)
  set(MISALIGN_COUNT 0)
  # Extract The First Letter Of The DUT (We Pass It As A Define Into c-code)
  string(SUBSTRING "${DUT}" 0 1 DUT_LETTER)
  set(DUT_LETTER_QUOTED '${DUT_LETTER}')

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
  set(NA_TEST_op0_vec_file op0_vec.bin)
  set(NA_TEST_op0_cube_file op0_cube.bin)
  set(NA_TEST_op0_ameba_file op0_ameba.bin)
  set(NA_TEST_op0_with_unused_file op0_with_unused.bin)
  set(NA_TEST_op1_vec_file op1_vec.bin)
  set(NA_TEST_op1_cube_file op1_cube.bin)
  set(NA_TEST_op1_ameba_file op1_ameba.bin)
  set(NA_TEST_op1_with_unused_file op1_with_unused.bin)
  set(NA_TEST_op2_vec_file op2_vec.bin)
  set(NA_TEST_op2_cube_file op2_cube.bin)
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
  #set(NA_TEST_mpe_cfg_lut_file ${CMAKE_SOURCE_DIR}/src/lib/drivers/rcm_na/mpe_regs_table_macro.bin)
  set(NA_TEST_mpe_cfg_lut_file mpe_regs_table_macro.bin)

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

  set (NA_RM_KEYS_PPE
    --num_iterations_file      ${NA_TEST_num_iterations_file}
    --in_file                  ${NA_TEST_in_file}
    --etalon_file              ${NA_TEST_etalon_file}
    --cfg_ppe_file             ${NA_TEST_cfg_ppe_file}
    --status_regs_file         ${NA_TEST_status_regs_file}
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
  set(PLUSARG_metrics_lut1_tag metrics_lut1_file_tag)
  set(PLUSARG_metrics_lut2_tag metrics_lut2_file_tag)
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

                     +${PLUSARG_num_iterations_file_tag}=${NA_TEST_num_iterations_file}
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

  set(MERGE_BINS_4_LONG_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/merge_npe_rm_files.sh)

endmacro()

macro(misalign_increment)
if(MISALIGN_COUNT STREQUAL 15)
  set(MISALIGN_COUNT 1)
else()
  MATH(EXPR MISALIGN_COUNT "${MISALIGN_COUNT} + 1")
endif()
endmacro()


macro(na_testsuite_init DUT)
  rumboot_add_external_project(externals/npe_rm -DCOMPILE_FROM_ROMBOOT="YES")
  rumboot_add_external_project(externals/py_mpe_test)

        # CRUTCH - We Need This Submodule Content During The cmake First Run (From toplevel)
  set(directory externals/py_ppe_test)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule status ${directory}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_SUBMOD_STATUS_OUTPUT
  )
  if(GIT_SUBMOD_STATUS_OUTPUT MATCHES "^-")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init ${directory}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()
        #


  rumboot_add_external_project(externals/py_ppe_test)
  _na_init_variables(${DUT})
endmacro()



# Tests Without RM
macro(ADD_NPE_SIMPLE_TEST CONF name filename)
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES ${filename}
  )
endmacro()


      # Tests Use MPE+VPE (Without PPE)
      macro(ADD_NPE_MPE_VPE_TEST CONF name rm_bin_name make_tight)
      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
        CFLAGS -DDONT_USE_PPE=1
        PREPCMD 
          ${NA_RM_BIN_PATH}/${rm_bin_name} 
          ${NA_RM_KEYS} 
          ${RM_TF_KEYS}
          > ${RM_LOGFILE} &&

          ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
          &&
          ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

          || exit 1

        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:${rm_bin_name}
      )
      if("${make_tight}" STREQUAL "MAKE_TIGHT")
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME ${name}_tight
          FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_npe.c
          CFLAGS -DDONT_USE_PPE=1
          PREPCMD 
            ${NA_RM_BIN_PATH}/${rm_bin_name} 
            ${NA_RM_KEYS} 
            ${RM_TF_KEYS}
            > ${RM_LOGFILE} &&

            ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
            &&
            ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

            || exit 1

          IRUN_FLAGS ${NA_RM_PLUSARGS}
          SUBPROJECT_DEPS npe_rm:${rm_bin_name}
        )
      endif()  # MAKE_TIGHT
    endmacro()

    macro(ADD_NPE_MPE_RDMA_RDCH_VPE_TEST CONF name rm_bin_name)
      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
        CFLAGS -DDONT_USE_PPE=1 -DMPE_CFG_TESTPLAN_RDMA_RDCH
        PREPCMD 
          ${NA_RM_BIN_PATH}/${rm_bin_name} 
          ${NA_RM_KEYS} 
          ${RM_TF_KEYS}
          > ${RM_LOGFILE} &&

          ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
          &&
          ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

          || exit 1

        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:${rm_bin_name}
      )
    endmacro()

      # Tests Use All 3 Units
    macro(ADD_NPE_COMPLEX_TEST CONF name rm_bin_name make_tight)
      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
        PREPCMD 
          ${NA_RM_BIN_PATH}/${rm_bin_name} 
          ${NA_RM_KEYS} 
          ${RM_TF_KEYS}
          > ${RM_LOGFILE} &&

          ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
          &&
          ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

          || exit 1

        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:${rm_bin_name}
      )
      if("${make_tight}" STREQUAL "MAKE_TIGHT")
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME ${name}_tight
          FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_npe.c
          PREPCMD 
            ${NA_RM_BIN_PATH}/${rm_bin_name} 
            ${NA_RM_KEYS} 
            ${RM_TF_KEYS}
            > ${RM_LOGFILE} &&

            ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
            &&
            ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

            || exit 1

          IRUN_FLAGS ${NA_RM_PLUSARGS}
          SUBPROJECT_DEPS npe_rm:${rm_bin_name}
        )
      endif()  # MAKE_TIGHT
    endmacro()

macro (ADD_MPE_CONV_TEST CONF name trunc) # trunc=TRUNC0/TRUNC16
set(MPE_TEST_SHEET ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment2/${name}_CONV/mpe_arrays.txt)
add_rumboot_target(
  CONFIGURATION ${CONF}
  NAME ${name}_CONV
  FILES scr1/targets/simple-rom/nu/npe_mpe_stage2/mpe_conv.c
  CFLAGS -D${trunc}
  PREPCMD ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} ${trunc} < ${MPE_TEST_SHEET}
  IRUN_FLAGS ${NA_RM_PLUSARGS}
)
endmacro()


macro (ADD_MPE_SINGLE_TEST CONF name trunc) # trunc=TRUNC0/TRUNC16
set(MPE_TEST_SHEET ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment2/${name}/mpe_arrays.txt)
add_rumboot_target(
  CONFIGURATION ${CONF}
  NAME ${name}
  FILES scr1/targets/simple-rom/nu/npe_mpe_stage2/mpe_single.c
  CFLAGS -D${trunc}
  PREPCMD ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} ${trunc} < ${MPE_TEST_SHEET}
  IRUN_FLAGS ${NA_RM_PLUSARGS}
)
endmacro()


macro (ADD_MPE_VPE_BATCH_TEST CONF B C W H LP RP BP TP K S R SX SY)
set(TEST_BINS ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/BATCH/B${B})
add_rumboot_target(
  CONFIGURATION ${CONF}
  NAME MPE_VPE_BATCH${B}
  FILES scr1/targets/simple-rom/nu/npe/mpe_vpe_batch.c
  CFLAGS -DdB=${B} -DdC=${C} -DdW=${W} -DdH=${H} -DLP=${LP} -DRP=${RP} -DBP=${BP} -DTP=${TP} -DdK=${K} -DdS=${S} -DdR=${R} -DSX=${SX} -DSY=${SY} 
  PREPCMD cp ${TEST_BINS}/cmd.bin ${TEST_BINS}/cmd.bin.metrics ${TEST_BINS}/cube.bin ${TEST_BINS}/cube.bin.metrics ${TEST_BINS}/etalon.bin.metrics ${TEST_BINS}/warr.bin ${TEST_BINS}/warr.bin.metrics -t .
  IRUN_FLAGS ${NA_RM_PLUSARGS}
)
endmacro()


macro (ADD_MPE_COUPLED_TEST_LOOP CONF name rm_bin_name)
set (ADD_MPE_COUPLED_TEST_LOOP_ARGS cp ${MPE_TEST_SHEETS_DIR}/${name}/num_iterations.bin .)
foreach(i RANGE 0 15)
  set(MPE_TEST_SHEET ${MPE_TEST_SHEETS_DIR}/${name}/${rm_bin_name}_${i}/mpe_arrays_r.txt)
  set(ADD_MPE_COUPLED_TEST_LOOP_ARGS ${ADD_MPE_COUPLED_TEST_LOOP_ARGS} &&
  ${MPE_PARSE_TEST} ${NA_TEST_mpe_cmd_file}.${i} ${NA_TEST_in_file}.${i} ${NA_TEST_warr_file}.${i} ${NA_TEST_etalon_file}.${i} < ${MPE_TEST_SHEET})
endforeach()
add_rumboot_target(
  CONFIGURATION ${CONF}
  NAME ${name}
  FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_mpe.c
  PREPCMD ${ADD_MPE_COUPLED_TEST_LOOP_ARGS}
  IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
  TIMEOUT_CTEST 10000
)
endmacro()


macro (ADD_MPE_DEMO_TEST CONF mpe_demo_test_name show_perf)
  set(MPE_TEST_SHEET ${MPE_TEST_SHEETS_DIR}/${mpe_demo_test_name}/mpe_arrays.txt)
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${mpe_demo_test_name}
    FILES scr1/targets/simple-rom/nu/mpe_demo/mpe_demo_common.c
    CFLAGS -D${show_perf}
    PREPCMD ${MPE_PARSE_TEST} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} < ${MPE_TEST_SHEET}
    IRUN_FLAGS ${NA_RM_PLUSARGS}
    TIMEOUT_CTEST 10000
  )
endmacro()

macro (ADD_VPE_PPE_WKH_COMB_ALL CONF test_list_name)

set (ShowPerf "NotShowPerf")
set (TST_NMB 3)
set (i 0)

foreach(name_in ${${test_list_name}})
  foreach(i RANGE ${TST_NMB})

    if (i EQUAL 0)
      set (name "vpe_ppe_${name_in}_LIN_8b")
      set (rm_bin_name "main_vpe_ppe_IN_INT8")
      set (LBS "LIN")
      set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
    elseif (i EQUAL 1)
      set (name "vpe_ppe_${name_in}_LIN_16b")
      set (rm_bin_name "main_vpe_ppe_IN_INT16")
      set (LBS "LIN")
      set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
    elseif (i EQUAL 2)
      set (name "vpe_ppe_${name_in}_BOX_8b")
      set (rm_bin_name "main_vpe_ppe_box_IN_INT8")
      set (LBS "BOX")
      set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
    elseif (i EQUAL 3)
      set (name "vpe_ppe_${name_in}_BOX_16b")
      set (rm_bin_name "main_vpe_ppe_box_IN_INT16")
      set (LBS "BOX")
      set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
    endif()

    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe_long.c

      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO}} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

      CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

      IRUN_FLAGS ${NA_RM_PLUSARGS}

      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
    )

    if (i EQUAL TST_NMB)
      math (EXPR NU_SEED "${NU_SEED} + 1")
      set (i 0)
    endif()

  endforeach()
endforeach()
endmacro()

macro (ADD_PPE_TESTS CONF name_in rm_bin_name ShowPerf DataSrc LBS RM_CFG_PARAM)
set (TST_NMB 1)
set (name "${name_in}")
set (Sh_is_1 "--set_Sh 1 --Sh 1")
set (Sw_is_1 "--set_Sw 1 --Sw 1 --w_max 128")

if (EXPERIMENT_STAGE_2_SUB_2)
  string(REPLACE "ppe_i8_max_ml"    "PPE_2_i8_max_ml"     name  ${name})
  string(REPLACE "ppe_i16_max_ml"   "PPE_3_i16_max_ml"    name  ${name})
  string(REPLACE "ppe_fp16_max_ml"  "PPE_4_fp16_max_ml"   name  ${name})
  string(REPLACE "ppe_i8_min_ml"    "PPE_5_i8_min_ml"     name  ${name})
  string(REPLACE "ppe_i16_min_ml"   "PPE_6_i16_min_ml"    name  ${name})
  string(REPLACE "ppe_fp16_min_ml"  "PPE_7_fp16_min_ml"   name  ${name})
  string(REPLACE "ppe_i8_avg_ml"    "PPE_8_i8_avg_ml"     name  ${name})
  string(REPLACE "ppe_i16_avg_ml"   "PPE_9_i16_avg_ml"    name  ${name})
  string(REPLACE "ppe_fp16_avg_ml"  "PPE_10_fp16_avg_ml"  name  ${name})

  if (${name} STREQUAL "PPE_10_fp16_avg_ml")
    set (TST_NMB 1)
  else()
    set (TST_NMB 0)
  endif()

endif()

foreach(i RANGE ${TST_NMB})

  if (i EQUAL TST_NMB)
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} ${Sh_is_1} ${Sw_is_1}")

    string(REPLACE "PPE_10_fp16_avg_ml"  "PPE_11_fp16_avg_ml"  name  ${name})
  else()
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM}")
  endif()

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_${i}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  if(${LBS} STREQUAL "LIN")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight_${i}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_ppe.c

      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

      CFLAGS -DDUT=${DUT_LETTER_QUOTED}

      IRUN_FLAGS ${NA_RM_PLUSARGS}

      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
    )
  endif()

  if (i EQUAL TST_NMB)
    math (EXPR NU_SEED "${NU_SEED} + 1")
  endif()
endforeach()
endmacro()


macro(ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA CONF name rm_bin_name)
misalign_increment()
set(MISALIGN RANGE 0 ${MISALIGN_COUNT})
foreach(IntMisalign ${MISALIGN})
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_${IntMisalign}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DVPE_TraceMode_PPE=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
endforeach()
endmacro()

macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA CONF name rm_bin_name)
misalign_increment()
set(MISALIGN 0 ${MISALIGN_COUNT})
foreach(IntMisalign ${MISALIGN})
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_${IntMisalign}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight_${IntMisalign}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe.c
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      CFLAGS -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DFORCE_VPE_WDMA_EN=1 -DDUT=${DUT_LETTER_QUOTED}
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
endforeach()
endmacro()

macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN CONF name rm_bin_name axi_len)
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DAXI_LEN=1 -DAxiLen=${axi_len} -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
endmacro()

macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG CONF name rm_bin_name)
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
 endmacro()
 
 macro(ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_SRST_LONG CONF name rm_bin_name)
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight_rst
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_rst_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DVPE_CUBE_CMPL=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
 add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight_rst_fail
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_int_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DVPE_CUBE_CMPL=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight_pause
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_pause_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DVPE_CUBE_CMPL=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  ) 
 add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_na_tight_rst
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_na_vpe_rst_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1  -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  ) 
endmacro()


macro(ADD_VPE_COUPLED_TEST_CONTROL_CONS_FORCE_WDMA CONF name rm_bin_name)
misalign_increment()
set(MISALIGN 0 ${MISALIGN_COUNT})
foreach(IntMisalign ${MISALIGN})
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_${IntMisalign}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
endforeach()
endmacro()

macro(ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL_FORCE_WDMA CONF name rm_bin_name)
misalign_increment()
set(MISALIGN 0 ${MISALIGN_COUNT})
foreach(IntMisalign ${MISALIGN})
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_${IntMisalign}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DMISALIGN_EN=1 -DIntMisalign=${IntMisalign} -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
endforeach()
endmacro()  

macro(ADD_VPE_COUPLED_TEST_LOOP CONF name rm_bin_name)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
    CFLAGS -DDUT=${DUT_LETTER_QUOTED}
    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
)
endmacro()

macro(ADD_VPE_COUPLED_TEST_LOOP_LONG CONF name rm_bin_name)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long.c
    CFLAGS -DDUT=${DUT_LETTER_QUOTED}
    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}  || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
)
endmacro()

macro(ADD_VPE_COUPLED_TEST_CONTROL_CONS CONF name rm_bin_name)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
    CFLAGS -DFORCE_VPE_WDMA_EN=1
    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
)
endmacro()

macro(ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL CONF name rm_bin_name)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
    CFLAGS -DFORCE_VPE_WDMA_EN=1
    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
)
endmacro()


macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_PARALLEL CONF name bin_dir)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_parallel_vpe.c
    CFLAGS -DFORCE_VPE_WDMA_EN=1
    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
)
endmacro()

macro(ADD_VPE_FROM_BINARY_TEST_LOOP CONF name bin_dir)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
)
endmacro()

macro(ADD_VPE_FROM_BINARY_TEST_LOOP_FORCE_WDMA CONF name bin_dir)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    CFLAGS -DFORCE_VPE_WDMA_EN=1
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe.c
    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
)
endmacro()

macro(ADD_VPE_FROM_BINARY_TEST_CONTROL_CONS CONF name bin_dir)
add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_cons_vpe.c
    CFLAGS -DFORCE_VPE_WDMA_EN=1
    PREPCMD cp ${VPE_BINARIES_ROOT}/${bin_dir}/* . || exit 1
    IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
)
endmacro()

macro (ADD_VPE_PPE_TESTS CONF name_in RM_CFG_PARAM)
set (ShowPerf "NotShowPerf")
set (TST_NMB 3)

foreach(i RANGE ${TST_NMB})

  if (i EQUAL 0)
    set (name "${name_in}_LIN_8b")
    set (rm_bin_name "main_vpe_ppe_IN_INT8")
    set (LBS "LIN")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
  elseif (i EQUAL 1)
    set (name "${name_in}_LIN_16b")
    set (rm_bin_name "main_vpe_ppe_IN_INT16")
    set (LBS "LIN")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
  elseif (i EQUAL 2)
    set (name "${name_in}_BOX_8b")
    set (rm_bin_name "main_vpe_ppe_box_IN_INT8")
    set (LBS "BOX")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
  elseif (i EQUAL 3)
    set (name "${name_in}_BOX_16b")
    set (rm_bin_name "main_vpe_ppe_box_IN_INT16")
    set (LBS "BOX")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
  endif()

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  if (i EQUAL TST_NMB)
    math (EXPR NU_SEED "${NU_SEED} + 1")
  endif()

endforeach()
endmacro()

macro (ADD_PPE_DGTL_SLTN_TESTS CONF)

set (i8    0)
set (i16   1)
set (fp16  2)

set (dt_lst
  i8
  i16
  fp16
)

set (avg  0)
set (max  1)
set (min  2)

set (pm_lst
  avg
  max
  min
)

set (sw_lst_base 1 2)

foreach (dt ${dt_lst})
  foreach (pm ${pm_lst})
    foreach (kw RANGE 2 8)

      set (sw_lst)

      foreach (sw IN LISTS sw_lst_base)
        if (NOT(${kw} EQUAL ${sw}))
          set (sw_lst ${sw_lst} ${sw})
        endif()
      endforeach()

      set (sw_lst ${sw_lst} ${kw})

      foreach (sw IN LISTS sw_lst)
        foreach (w RANGE 8 16)

          set (kh ${kw})
          set (sh ${sw})
          set (h ${w})
          set (c 32)

          set (tp 0)
          set (bp 0)
          set (lp 0)
          set (rp 0)

          set (set_Kw_cfg "--set_Kw 1 --Kw ${kw}")
          set (set_Sw_cfg "--set_Sw 1 --Sw ${sw}")
          set (set_Kh_cfg "--set_Kh 1 --Kh ${kh}")
          set (set_Sh_cfg "--set_Sh 1 --Sh ${sh}")
          set (set_W_cfg "--w_min ${w} --w_max ${w}")
          set (set_H_cfg "--h_min ${h} --h_max ${h}")
          set (set_C_cfg "--c_min ${c} --c_max ${c}")
          set (set_Tp_cfg "--set_Tp 1 --Tp ${tp}")
          set (set_Bp_cfg "--set_Bp 1 --Bp ${bp}")
          set (set_Lp_cfg "--set_Lp 1 --Lp ${lp}")
          set (set_Rp_cfg "--set_Rp 1 --Rp ${rp}")

          set (set_pm_cfg "--set_meth 1 --pool_meth ${${pm}}")
          set (set_dt_cfg "--data_type ${${dt}}")

          set (rm_cfg_prm "${set_pm_cfg} ${set_dt_cfg} ${set_Kw_cfg} ${set_Sw_cfg} ${set_Kh_cfg} ${set_Sh_cfg} ${set_W_cfg} ${set_H_cfg} ${set_C_cfg} ${set_Tp_cfg} ${set_Bp_cfg} ${set_Lp_cfg} ${set_Rp_cfg}")
#          message("rm_cfg_prm ${rm_cfg_prm}")

          set (name "ppe_${dt}_${pm}_${kw}_${sw}_${kh}_${sh}_${w}_${h}_${c}")
#          message("${name}")

          set (LBS "LIN")
          set (DataSrc "MEMtoPPE")
          set (ShowPerf "DoNotShowPerf")
          set (main_c scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c)

          if (${dt} EQUAL i8)
            set(rm_bin_name "main_ppe_IN_INT8")
          elseif (${dt} EQUAL i16)
            set(rm_bin_name "main_ppe_IN_INT16")
          elseif (${dt} EQUAL fp16)
            set(rm_bin_name "main_ppe_IN_FP16")
          endif()

          add_rumboot_target(
            CONFIGURATION ${CONF}
            NAME ${name}
            FILES ${main_c}
          
            PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS_PPE} --it_nmb 1 --seed 1 ${rm_cfg_prm} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS_PPE} || exit 1
          
            CFLAGS -D${LBS} -D${DataSrc} -D${ShowPerf} -DDUT=${DUT_LETTER_QUOTED}
          
            IRUN_FLAGS ${NA_RM_PLUSARGS}
          
            SUBPROJECT_DEPS npe_rm:${rm_bin_name}
          )

        endforeach()
      endforeach()

    endforeach()
  endforeach()
endforeach()

endmacro()

macro (ADD_PPE_PY_TESTS CONF)
  set(rm_bin_name main_ppe_py)
  set(PPE_PY_GENERATE_TESTCASE_SCRIPT ${CMAKE_SOURCE_DIR}/externals/py_ppe_test/testplan/testcaseKW_engine.py)
  set(PPE_PY_TESTCASE_PREP_SCRIPT ${CMAKE_SOURCE_DIR}/externals/py_ppe_test/constraint_engine_bin.py)
  set(PPE_PY_TESTCASE_PREP_SCRIPT_OPTS -cm all_with_all -cf )
  execute_process(
    COMMAND 
      ${PYTHON_EXECUTABLE} -B ${PPE_PY_GENERATE_TESTCASE_SCRIPT}
    WORKING_DIRECTORY
      ${CMAKE_BINARY_DIR}
    OUTPUT_VARIABLE
      PPE_PY_TESTCASE_LIST
  )
  file(GLOB PPE_PY_TESTCASE_FILES ${CMAKE_BINARY_DIR}/data_constraint/*/*.py)
  foreach(file ${PPE_PY_TESTCASE_FILES})
    string(REGEX MATCH "[a-zA-Z0-9_\\.]+.py" test_file_name ${file})
    string(REGEX MATCH "[0-9]+_dc_ppe_[a-zA-Z0-9_]+" test_dir ${file})
    #string(REGEX MATCH "^[a-zA-Z0-9_\\.]+" test_name ${test_file_name})
    string(REPLACE "." "_" test_name ${test_file_name})
    set(test_name "${test_dir}-${test_name}")
    execute_process(
      COMMAND             grep scenario__path_mode ${file}
      WORKING_DIRECTORY   ${CMAKE_BINARY_DIR}
      OUTPUT_VARIABLE     SCENARIO_PATH_LINE
    )
    string(REGEX MATCH "mode_[a-z_]+" scenario__path_mode ${SCENARIO_PATH_LINE})
    if("${scenario__path_mode}" STREQUAL "mode_linear")
      set(LBS LIN)
    elseif("${scenario__path_mode}" STREQUAL "mode_boxed_mpe")
      set(LBS BOX)
    else()
      message("Warning: scenario__path_mode ${scenario__path_mode} For Test ${test_name} Not Supported")
      set(LBS LBS_NOT_SUPPORTED)
    endif()
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ppe_py_${test_name}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c
      CFLAGS -D${LBS} -DDUT=${DUT_LETTER_QUOTED} -DMEMtoPPE=1
      PREPCMD 
        cp ${file} . 
        &&
        ${PPE_PY_TESTCASE_PREP_SCRIPT} ${PPE_PY_TESTCASE_PREP_SCRIPT_OPTS} ${test_file_name}
        &&
        ${NA_RM_BIN_PATH}/${rm_bin_name}  ${NA_RM_KEYS} > ${RM_LOGFILE}
        &&
        ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}
        ||
        exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
    )
  endforeach()
endmacro()


macro (ADD_VPE_PPE_WKH_COMB CONF test_list_name)
set (ShowPerf "NotShowPerf")
set (i 0)

foreach(name_in ${${test_list_name}})

  math (EXPR j "${i} % 4")
  math (EXPR i "${i} + 1")

  set (RM_CFG_PARAM ${${name_in}})

  if (j EQUAL 0)
    set (name "vpe_ppe_${name_in}_LIN_8b")
    set (rm_bin_name "main_vpe_ppe_IN_INT8")
    set (LBS "LIN")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
  elseif (j EQUAL 1)
    set (name "vpe_ppe_${name_in}_LIN_16b")
    set (rm_bin_name "main_vpe_ppe_IN_INT16")
    set (LBS "LIN")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
  elseif (j EQUAL 2)
    set (name "vpe_ppe_${name_in}_BOX_8b")
    set (rm_bin_name "main_vpe_ppe_box_IN_INT8")
    set (LBS "BOX")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 0")
  elseif (j EQUAL 3)
    set (name "vpe_ppe_${name_in}_BOX_16b")
    set (rm_bin_name "main_vpe_ppe_box_IN_INT16")
    set (LBS "BOX")
    set (RM_CFG_PARAM_MACRO "${RM_CFG_PARAM} --data_type 1")
  endif()

  if (EXPERIMENT_STAGE_2_SUB_2)
    if (${name} MATCHES "_8b")
      set (name "NA_3_${name}")
    endif()
    if (${name} MATCHES "_16b")
      set (name "NA_4_${name}")
    endif()
  endif()

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_tight
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_ppe_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM_MACRO} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  if (j EQUAL 3)
    math (EXPR NU_SEED "${NU_SEED} + 1")
  endif()

endforeach()
endmacro()

macro(na_testsuite_add_npe_tests CONF)
  set(NPE_BINS ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/npe)
  set(MPE_DEMO_PATH src/platform/scr1/targets/simple-rom/nu/npe_mpe_stage2)
  set(MPE_PARSE_TEST_STAGE2 ${CMAKE_SOURCE_DIR}/${MPE_DEMO_PATH}/parse_mpe_arrays_stage2.pl)
  set(RM_TF_KEYS
  --cube_TF_file=${NPE_BINS}/resnet_bins/${name}/cube.bin.0
  --warr_TF_file=${NPE_BINS}/resnet_bins/${name}/warr.bin.0
  --op0_TF_file=${NPE_BINS}/resnet_bins/${name}/op0.bin.0
  )

  set (ConfigMPE_to_LUT ${CMAKE_SOURCE_DIR}/externals/py_mpe_test/ConfigMPE_to_LUT.py)
  set (ConfigMPE_to_LUT_LOGFILE ConfigMPE_to_LUT.log)

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME NPE_1
    FILES scr1/targets/simple-rom/nu/npe_regs/npe_regs.c
  )

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME simple_direct_WITH_PPE
    FILES scr1/targets/simple-rom/nu/simple_direct/nu_simple_direct.c
    PREPCMD cp ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/simple_direct/with_ppe/* .
    IRUN_FLAGS ${NA_RM_PLUSARGS}
  )

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME simple_direct_WITH_VPE
    CFLAGS -DDONT_USE_PPE=1
    FILES scr1/targets/simple-rom/nu/simple_direct/nu_simple_direct.c
    PREPCMD cp ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/simple_direct/with_vpe/* .
    IRUN_FLAGS ${NA_RM_PLUSARGS}
  )

  ################################
  ### MPE Tests
  ### FixMe: Duplication? 
  ADD_MPE_SINGLE_TEST(${CONF} MPE_2 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_3 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_4 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_5 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_6 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_7 TRUNC0)
  ADD_MPE_SINGLE_TEST(${CONF} MPE_8 TRUNC0) # mu int8 test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_9 TRUNC0) # mu int16 test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_10 TRUNC0) # mu fp16 test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_11 TRUNC0) # mu fp16 NaN test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_12 TRUNC16) # mu fp16 inf test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_13 TRUNC0) # mu+acc int8 test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_14 TRUNC0) # mu+acc int16 test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_15 TRUNC0) # mu int norm test
  ADD_MPE_SINGLE_TEST(${CONF} MPE_16 TRUNC16) # mu+acc fp16 test
  
            ###
          add_rumboot_target(
            CONFIGURATION ${CONF}
            NAME RESNET
            FILES scr1/targets/simple-rom/nu/npe/npe_resnet.c
            PREPCMD cp ${NPE_BINS}/resnet_bins/cmd.bin ${NPE_BINS}/resnet_bins/cmd.bin.metrics ${NPE_BINS}/resnet_bins/cube.bin ${NPE_BINS}/resnet_bins/cube.bin.metrics ${NPE_BINS}/resnet_bins/etalon.bin ${NPE_BINS}/resnet_bins/etalon.bin.metrics ${NPE_BINS}/resnet_bins/op0.bin ${NPE_BINS}/resnet_bins/op0.bin.metrics ${NPE_BINS}/resnet_bins/warr.bin ${NPE_BINS}/resnet_bins/warr.bin.metrics -t .
            IRUN_FLAGS ${NA_RM_PLUSARGS}
          )
    
          ADD_MPE_CONV_TEST(${CONF} MPE_2 TRUNC16)
              ## NOT EXPERIMENT_STAGE_2_SUB_2
    
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME MPE_PERFORMANCE
          FILES scr1/targets/simple-rom/nu/npe_mpe_stage2/mpe_performance.c
          PREPCMD ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} TRUNC16 < ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/tests/experiment2/MPE_PERFORMANCE/mpe_arrays.txt
          IRUN_FLAGS ${NA_RM_PLUSARGS}
        )
        
        ADD_MPE_VPE_BATCH_TEST(${CONF} 2 66 274 7 1 1 1 1 130 3 3 3 3) # B C W H LP RP BP TP K S R SX SY
        ADD_MPE_VPE_BATCH_TEST(${CONF} 3 66 274 7 1 1 1 1 130 3 3 3 3) # B C W H LP RP BP TP K S R SX SY

          ################################
          ### Control Unit Tests
        set(CU_TEST_DIR ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/na_cu)
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME NA_7 #na_cu_mpe_vpe
          FILES scr1/targets/simple-rom/nu/na_cu/cu_dpnd_mpe_vpe.c
          PREPCMD cp ${CU_TEST_DIR}/in_arr.bin ${CU_TEST_DIR}/in_arr.bin.metrics ${CU_TEST_DIR}/etalon.bin ${CU_TEST_DIR}/etalon.bin.metrics -t .
          IRUN_FLAGS +in_arr=in_arr.bin
                     +metrics_in_arr=in_arr.bin.metrics
                     +etalon_arr=etalon.bin
                     +metrics_etalon_arr=etalon.bin.metrics
        )
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME NA_8 #na_cu_buffers
          FILES scr1/targets/simple-rom/nu/na_cu/cu_buffers.c
          PREPCMD cp ${CU_TEST_DIR}/in_arr.bin ${CU_TEST_DIR}/in_arr.bin.metrics -t .
          IRUN_FLAGS +in_arr=in_arr.bin
                     +metrics_in_arr=in_arr.bin.metrics
        )
    
          ##########################################
          ## NPE Reg Test
        ADD_NPE_SIMPLE_TEST(${CONF} npe_regs scr1/targets/simple-rom/nu/npe_regs/npe_regs.c)
    
          ##########################################
          ## Direct Tests That Use Predefined Set Of MPE Configurations And Then Use Some VPE And PPE Functions
        foreach(label RANGE 25 48)  # Int16
          ADD_NPE_MPE_VPE_TEST(${CONF} NA_2_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT)
        endforeach()
        foreach(label RANGE 49 72)  # Int8
          ADD_NPE_MPE_VPE_TEST(${CONF} NA_1_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT)
        endforeach()
    
          foreach(label RANGE 1 24)
            ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label} main_mpe_direct_ex_MPE_CFG_${label} MAKE_TIGHT)
            ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT)
          endforeach()
          foreach(label RANGE 25 72)
            ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label} main_mpe_direct_ex_MPE_CFG_${label} MAKE_TIGHT)
          endforeach()
          foreach(label RANGE 1 48)
            ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP main_mpe_direct_ex_MPE_CFG_${label}_FP MAKE_TIGHT)
            ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE NO_TIGHT)
          endforeach()

          ADD_NPE_MPE_RDMA_RDCH_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_TESTPLAN_RDMA_RDCH main_mpe_direct_ex_MPE_CFG_TESTPLAN_RDMA_RDCH)
    
          ##################################
          ## Direct Complex Tests On Important Cube Sizes
        foreach(label RANGE 49 72)
          ADD_NPE_COMPLEX_TEST(${CONF} NA_5_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT)
        endforeach()
        foreach(label RANGE 25 48)
          ADD_NPE_COMPLEX_TEST(${CONF} NA_6_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT)
        endforeach()
    
          ###################################
          ## Some Other Direct Tests
          foreach(label RANGE 1 24)
            ADD_NPE_COMPLEX_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT)
          endforeach()
          foreach(label RANGE 1 48)
            ADD_NPE_COMPLEX_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE NO_TIGHT)
          endforeach()
    
          ADD_NPE_COMPLEX_TEST(${CONF} npe_all_ex_IN_INT16 main_npe_all_ex_IN_INT16 MAKE_TIGHT)
          
          ###################################################################
          # Resnet-test 
          ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE main_resnet_IN_FP16_LAYER0_MPE NO_TIGHT) # LAYER0_MPE
          ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN main_resnet_IN_FP16_LAYER0_MPE_BN NO_TIGHT) # LAYER0_MPE_BN
          ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN_RELU main_resnet_IN_FP16_LAYER0_MPE_BN_RELU NO_TIGHT) # LAYER0_MPE_BN_RELU
          ADD_NPE_COMPLEX_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE main_resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE NO_TIGHT) # LAYER0_MPE_BN_RELU_PPE
          ###################################################################
    

  na_testsuite_add_vpe_tests(${CONF})
  na_testsuite_add_ppe_tests(${CONF})

  #ADD_PPE_DGTL_SLTN_TESTS(${CONF})
endmacro()

  # Тесты в окру*ении, где только VPE (они не идут на сборке)
macro(na_testsuite_add_vpe_unit_tests CONF)
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_9_0_op0_relu_int32  main_vpe_9_0_op0_relu_int32  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_15_0_op1_relu_int32 main_vpe_15_0_op1_relu_int32 ) 
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_9_1_op0_relu_fp32   main_vpe_9_1_op0_relu_fp32   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_15_1_op1_relu_fp32  main_vpe_15_1_op1_relu_fp32  )

  # Tests on VPE::DEMUX::C3 pipeline mode-flow
  foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_3_c3_IN_INT32_${out_macro}  main_vpe_3_c3_IN_INT32_${out_macro} )
  endforeach()
  foreach(out_macro IN ITEMS OUT_INT16 OUT_FP16)
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_3_c3_IN_FP32_${out_macro}   main_vpe_3_c3_IN_FP32_${out_macro}  )
  endforeach()

  # Test on VPE::DEMUX OVERFLOW COUNTERS TESTS PIPELINE MODE-FLOW
  foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_overflow_counters_IN_INT32_${out_macro}  main_overflow_counters_IN_INT32_${out_macro} )
  endforeach()
  foreach(out_macro IN ITEMS OUT_INT16 OUT_FP16)
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_overflow_counters_IN_FP32_${out_macro}   main_overflow_counters_IN_FP32_${out_macro}  )
  endforeach()
  # не получилос(((
  # add_rumboot_target(
  #   CONFIGURATION ${CONF}
  #   FEATURES NOCODE
  #   TESTGROUP vpe_overflow_counters-group
  #   LOAD vpe_overflow_counters_IN_INT32_OUT_INT8,vpe_overflow_counters_IN_INT32_OUT_INT16
  #   # LOAD rumboot-top-rumboot-scr1-Debug-ROM-vpe_overflow_counters_IN_INT32_OUT_INT8,rumboot-top-rumboot-scr1-Debug-ROM-vpe_overflow_counters_IN_INT32_OUT_INT16
  #   NAME vpe_overflow_counters
  # )

  # Tests on VPE::InputConverters
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_18_op2_c1  main_vpe_18_op2_c1 )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_16_op2_c2  main_vpe_16_op2_c2 )

  # Tests on VPE::NORM
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_8_0_op0_norm       main_vpe_8_0_op0_norm      )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_14_0_op1_norm      main_vpe_14_0_op1_norm     )

  foreach(round_macro IN ITEMS 
      DOWN     TOWARDSZERO     UP     AWAYFROMZERO     HALFDOWN     HALFTOWARDSZERO     NEAREST     HALFAWAYFROMZERO
    )
      ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_8_0_op0_norm_ROUND_${round_macro}       main_vpe_8_0_op0_norm_ROUND_${round_macro}      )
      ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_14_0_op1_norm_ROUND_${round_macro}      main_vpe_14_0_op1_norm_ROUND_${round_macro}     )
  endforeach() # round_macro


  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_8_1_op0_norm_rnd   main_vpe_8_1_op0_norm_rnd  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_14_1_op1_norm_rnd  main_vpe_14_1_op1_norm_rnd )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_20_0_op2_norm      main_vpe_20_0_op2_norm     )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_20_1_op2_norm_rnd  main_vpe_20_1_op2_norm_rnd )

  # Tests on VPE::LSHIFT
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_4_op0_lshift   main_vpe_4_op0_lshift  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_10_op1_lshift  main_vpe_10_op1_lshift )

  # Tests on VPE::Formater
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_6_0_op0_f_int   main_vpe_6_0_op0_f_int  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_12_0_op1_f_int  main_vpe_12_0_op1_f_int )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_6_1_op0_f_fp    main_vpe_6_1_op0_f_fp   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_12_1_op1_f_fp   main_vpe_12_1_op1_f_fp  )

  # Tests on VPE::ALU
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_0_op0_alu_int8_low      main_vpe_5_0_op0_alu_int8_low      )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_1_op0_alu_int8_middle   main_vpe_5_1_op0_alu_int8_middle   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_2_op0_alu_int8_high     main_vpe_5_2_op0_alu_int8_high     )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_3_op0_alu_int16_low     main_vpe_5_3_op0_alu_int16_low     )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_4_op0_alu_int16_middle  main_vpe_5_4_op0_alu_int16_middle  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_5_op0_alu_int16_high    main_vpe_5_5_op0_alu_int16_high    )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_5_6_op0_alu_fp32          main_vpe_5_6_op0_alu_fp32          )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_0_op1_alu_int8_low     main_vpe_11_0_op1_alu_int8_low     )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_1_op1_alu_int8_middle  main_vpe_11_1_op1_alu_int8_middle  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_2_op1_alu_int8_high    main_vpe_11_2_op1_alu_int8_high    )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_3_op1_alu_int16_low    main_vpe_11_3_op1_alu_int16_low    )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_4_op1_alu_int16_middle main_vpe_11_4_op1_alu_int16_middle )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_5_op1_alu_int16_high   main_vpe_11_5_op1_alu_int16_high   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_11_6_op1_alu_fp32         main_vpe_11_6_op1_alu_fp32         )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_17_0_op2_alu_int8_low     main_vpe_17_0_op2_alu_int8_low     )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_17_1_op2_alu_int8_high    main_vpe_17_1_op2_alu_int8_high    )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_17_2_op2_alu_int16_low    main_vpe_17_2_op2_alu_int16_low    )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_17_3_op2_alu_int16_high   main_vpe_17_3_op2_alu_int16_high   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_17_4_op2_alu_fp32         main_vpe_17_4_op2_alu_fp32         )

  # Tests on VPE::MUL
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_7_0_op0_mul_int16   main_vpe_7_0_op0_mul_int16  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_7_1_op0_mul_int8    main_vpe_7_1_op0_mul_int8   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_7_2_op0_mul_fp32    main_vpe_7_2_op0_mul_fp32   )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_13_0_op1_mul_int16  main_vpe_13_0_op1_mul_int16 )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_13_1_op1_mul_int8   main_vpe_13_1_op1_mul_int8  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_13_2_op1_mul_fp32   main_vpe_13_2_op1_mul_fp32  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_19_0_op2_mul_int16  main_vpe_19_0_op2_mul_int16 )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_19_1_op2_mul_int8   main_vpe_19_1_op2_mul_int8  )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_19_2_op2_mul_fp32   main_vpe_19_2_op2_mul_fp32  )

  # Tests on VPE::OP2::LUT
  foreach(in_macro IN ITEMS IN_INT32 IN_FP32)
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_21_0_lut_${in_macro}              main_vpe_21_0_lut_${in_macro}              ) # VPE_21
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_21_1_lut_addition_${in_macro}     main_vpe_21_1_lut_addition_${in_macro}     ) # VPE_21_addition
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_21_2_lut_out_of_range_${in_macro} main_vpe_21_2_lut_out_of_range_${in_macro} ) # VPE_21_out_of_range
    ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_21_3_lut_offset_${in_macro}       main_vpe_21_3_lut_offset_${in_macro}       ) # VPE_21_offset
    foreach(mirror_type IN ITEMS 0 1)
      ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro} main_vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro} ) # VPE_21_offset. mirror mode
    endforeach()
  endforeach()

  # Test on VPE block's together work 
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_22_op0_together   main_vpe_22_op0_together   ) # VPE_22
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_23_op1_together   main_vpe_23_op1_together   ) # VPE_23
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_24_op2_together   main_vpe_24_op2_together   ) # VPE_24
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_25_op012_together main_vpe_25_op012_together ) # VPE_25

  # Test on VPE ???
  ADD_VPE_COUPLED_TEST_CONTROL_CONS(${CONF} vpe_27_0_control_cons main_vpe_27_0_control_cons   ) # VPE_27
  ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL(${CONF} vpe_27_1_control_par main_vpe_27_1_control_par ) # VPE_27
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_26_autonom_nowdma main_vpe_26_autonom)
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_28_perf main_28_perf) # VPE_28

  # Test on VPE special cases
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_special_cases_IN_FP32_OUT_FP16 main_vpe_special_cases_IN_FP32_OUT_FP16) # Test on special cases

endmacro()

macro(na_testsuite_add_vpe_tests CONF)
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME VPE_1
      FILES scr1/targets/simple-rom/nu/vpe_regs/regs_vpe.c
  )

  # Tests on VPE DMA
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_2_dma_int16_axi_len_0  main_vpe_2_dma_int16 0 )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_2_dma_int8_axi_len_1   main_vpe_2_dma_int8 1  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_2_dma_int16_axi_len_3  main_vpe_2_dma_int16 3 )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_2_dma_int8_axi_len_7   main_vpe_2_dma_int8 7  )

  # Tests on VPE::Formater
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_6_0_op0_f_int_dma  main_vpe_6_0_op0_f_int_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_6_1_op0_f_fp_dma   main_vpe_6_1_op0_f_fp_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_12_0_op1_f_int_dma main_vpe_12_0_op1_f_int_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_12_1_op1_f_fp_dma  main_vpe_12_1_op1_f_fp_dma  )

  # Tests on VPE::LSHIFT
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_4_op0_lshift_dma   main_vpe_4_op0_lshift_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_10_op1_lshift_dma  main_vpe_10_op1_lshift_dma )

  # Tests on VPE::InputConverters
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_18_op2_c1_dma      main_vpe_18_op2_c1_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_16_op2_c2_dma      main_vpe_16_op2_c2_dma )
  
  # Tests on VPE DMA
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_0_op0_alu_int8_low_dma      main_vpe_5_0_op0_alu_int8_low_dma     )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_1_op0_alu_int8_middle_dma   main_vpe_5_1_op0_alu_int8_middle_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_2_op0_alu_int8_high_dma     main_vpe_5_2_op0_alu_int8_high_dma    )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_3_op0_alu_int16_low_dma     main_vpe_5_3_op0_alu_int16_low_dma    )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_4_op0_alu_int16_middle_dma  main_vpe_5_4_op0_alu_int16_middle_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_5_op0_alu_int16_high_dma    main_vpe_5_5_op0_alu_int16_high_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_5_6_op0_alu_fp32_dma          main_vpe_5_6_op0_alu_fp32_dma         )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_0_op1_alu_int8_low_dma     main_vpe_11_0_op1_alu_int8_low_dma    )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_1_op1_alu_int8_middle_dma  main_vpe_11_1_op1_alu_int8_middle_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_2_op1_alu_int8_high_dma    main_vpe_11_2_op1_alu_int8_high_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_3_op1_alu_int16_low_dma    main_vpe_11_3_op1_alu_int16_low_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_4_op1_alu_int16_middle_dma main_vpe_11_4_op1_alu_int16_middle_dma)
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_5_op1_alu_int16_high_dma   main_vpe_11_5_op1_alu_int16_high_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_11_6_op1_alu_fp32_dma         main_vpe_11_6_op1_alu_fp32_dma        )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_17_0_op2_alu_int8_low_dma     main_vpe_17_0_op2_alu_int8_low_dma    )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_17_1_op2_alu_int8_high_dma    main_vpe_17_1_op2_alu_int8_high_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_17_2_op2_alu_int16_low_dma    main_vpe_17_2_op2_alu_int16_low_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_17_3_op2_alu_int16_high_dma   main_vpe_17_3_op2_alu_int16_high_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_17_4_op2_alu_fp32_dma         main_vpe_17_4_op2_alu_fp32_dma        )

  # Tests on VPE::DEMUX::C3 TESTS MEM MODE
  foreach(in_macro IN ITEMS IN_INT8 IN_INT16)
    foreach(out_macro IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
      ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_3_c3_${in_macro}_${out_macro}_dma       main_vpe_3_c3_${in_macro}_${out_macro}_dma  )
    endforeach()
  endforeach()
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_3_c3_IN_FP16_OUT_INT16_dma  main_vpe_3_c3_IN_FP16_OUT_INT16_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_3_c3_IN_FP16_OUT_FP16_dma   main_vpe_3_c3_IN_FP16_OUT_FP16_dma  )

  # Tests on VPE::RELU
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_9_0_op0_relu_int_dma   main_vpe_9_0_op0_relu_int_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_9_1_op0_relu_fp_dma    main_vpe_9_1_op0_relu_fp_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_15_0_op1_relu_int_dma  main_vpe_15_0_op1_relu_int_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_15_1_op1_relu_fp_dma   main_vpe_15_1_op1_relu_fp_dma  )

  # Tests on VPE::NORM
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_8_0_op0_norm_dma       main_vpe_8_0_op0_norm_dma      )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_8_1_op0_norm_rnd_dma   main_vpe_8_1_op0_norm_rnd_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_14_0_op1_norm_dma      main_vpe_14_0_op1_norm_dma     )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_14_1_op1_norm_rnd_dma  main_vpe_14_1_op1_norm_rnd_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_20_0_op2_norm_dma      main_vpe_20_0_op2_norm_dma     )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_20_1_op2_norm_rnd_dma  main_vpe_20_1_op2_norm_rnd_dma )

  # Tests on VPE channel mode
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op0_vec_ex_int_dma     main_vpe_op0_vec_ex_int )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op1_vec_ex_int_dma     main_vpe_op1_vec_ex_int )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op0_vec_ex_fp_dma      main_vpe_op0_vec_ex_fp  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op1_vec_ex_fp_dma      main_vpe_op1_vec_ex_fp  )

  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op0_vec_ex_int main_vpe_op0_vec_ex_int )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op1_vec_ex_int main_vpe_op1_vec_ex_int )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op0_vec_ex_fp  main_vpe_op0_vec_ex_fp  )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op1_vec_ex_fp  main_vpe_op1_vec_ex_fp  )

  # Tests on VPE dma
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_2_dma_int8_dma  main_vpe_2_dma_int8  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_2_dma_int16_dma main_vpe_2_dma_int16 )

    # Tests on VPE::DMA with and without SOFT RESET
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_SRST_LONG(${CONF}  vpe_5_6_op0_alu_fp32_dma  main_vpe_5_6_op0_alu_fp32_dma  )
 
    # Tests on VPE::MUL
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_7_0_op0_mul_int16_dma  main_vpe_7_0_op0_mul_int16_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_7_1_op0_mul_int8_dma   main_vpe_7_1_op0_mul_int8_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_7_2_op0_mul_fp32_dma   main_vpe_7_2_op0_mul_fp32_dma   )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_13_0_op1_mul_int16_dma main_vpe_13_0_op1_mul_int16_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_13_1_op1_mul_int8_dma  main_vpe_13_1_op1_mul_int8_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_13_2_op1_mul_fp32_dma  main_vpe_13_2_op1_mul_fp32_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_19_0_op2_mul_int16_dma main_vpe_19_0_op2_mul_int16_dma )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_19_1_op2_mul_int8_dma  main_vpe_19_1_op2_mul_int8_dma  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_19_2_op2_mul_fp32_dma  main_vpe_19_2_op2_mul_fp32_dma  )

  # Tests on VPE channel mode
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op0_ch_mode_int_dma  main_vpe_op0_ch_mode_int )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op1_ch_mode_int_dma  main_vpe_op1_ch_mode_int )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op0_ch_mode_fp_dma   main_vpe_op0_ch_mode_fp  )
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_op1_ch_mode_fp_dma   main_vpe_op1_ch_mode_fp  )

  #Tests on VPE::OP2::LUT in context NPE
  foreach(in_macro IN ITEMS IN_INT16 IN_INT8 IN_FP16)
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_21_0_lut_${in_macro}_dma               main_vpe_21_0_lut_${in_macro}_dma              )
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_21_1_lut_addition_${in_macro}_dma      main_vpe_21_1_lut_addition_${in_macro}_dma     )
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_21_2_lut_out_of_range_${in_macro}_dma  main_vpe_21_2_lut_out_of_range_${in_macro}_dma )
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_21_3_lut_offset_${in_macro}_dma        main_vpe_21_3_lut_offset_${in_macro}_dma       )
    foreach(mirror_type IN ITEMS 0 1)
      ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro}_dma main_vpe_21_3_lut_offset_mirror_${mirror_type}_${in_macro}_dma) # VPE_21_offset. mirror mode
    endforeach()
  endforeach()

  # Tests on VPE+PPE channel mode ???
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op0_ch_mode_int_dma  main_vpe_op0_ch_mode_int )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op1_ch_mode_int_dma  main_vpe_op1_ch_mode_int )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op0_ch_mode_fp_dma   main_vpe_op0_ch_mode_fp  )
  ADD_VPE_PPE_COUPLED_TEST_LOOP_FORCE_WDMA(${CONF} vpe_ppe_op1_ch_mode_fp_dma   main_vpe_op1_ch_mode_fp  )

  # Test on VPE block's together work 
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_22_op0_together_dma   main_vpe_22_op0_together_dma   ) # VPE_22
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_23_op1_together_dma   main_vpe_23_op1_together_dma   ) # VPE_23
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_24_op2_together_dma   main_vpe_24_op2_together_dma   ) # VPE_24
  ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_LONG(${CONF} vpe_25_op012_together_dma main_vpe_25_op012_together_dma ) # VPE_25
    
  # Test on VPE ???
  ADD_VPE_COUPLED_TEST_CONTROL_CONS_FORCE_WDMA(${CONF} vpe_26_0_control_cons_dma    main_vpe_26_0_control_cons_dma ) # VPE_27
  ADD_VPE_COUPLED_TEST_CONTROL_PARALLEL_FORCE_WDMA(${CONF} vpe_26_1_control_par_dma main_vpe_26_1_control_par_dma  ) # VPE_27
  
  
  # Tests on VPE batch-mode for int-type
  foreach(in IN ITEMS IN_INT8 IN_INT16)
    foreach(coef0 IN ITEMS COEF0_INT8 COEF0_INT16)
      foreach(coef1 IN ITEMS COEF1_INT8 COEF1_INT16)
        foreach(coef2 IN ITEMS COEF2_INT8 COEF2_INT16)
          foreach(out IN ITEMS OUT_INT8 OUT_INT16 OUT_FP16)
            foreach(number_testcase RANGE 1 4)
              add_rumboot_target(
                CONFIGURATION ${CONF}
                NAME vpe_batch_mode_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
                CFLAGS -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS}
                SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
              )
              add_rumboot_target(
                CONFIGURATION ${CONF}
                NAME vpe_batch_mode_with_element_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
                FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
                CFLAGS -DDUT=${DUT_LETTER_QUOTED}
                PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_with_element_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
                IRUN_FLAGS ${NA_RM_PLUSARGS}
                SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_with_element_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
              )
            endforeach()
          endforeach()
        endforeach()
      endforeach()
    endforeach()
  endforeach()

  # Tests on VPE batch-mode for fp-type
  foreach(out IN ITEMS OUT_INT16 OUT_FP16)
    foreach(number_testcase RANGE 1 4)
      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME vpe_batch_mode_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
        CFLAGS -DDUT=${DUT_LETTER_QUOTED}
        PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
      )
      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
        CFLAGS -DDUT=${DUT_LETTER_QUOTED}
        PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
      )
    endforeach()
  endforeach()

endmacro()

macro(na_testsuite_add_ppe_tests CONF)

  if(NOT DEFINED NU_SEED)
    set(NU_SEED 1)
  endif()

  if(NOT DEFINED NU_IT_NMB)
    set(NU_IT_NMB 32)
  endif()

  set (i8_max   "--set_meth 1 --pool_meth 1 --data_type 0 --c_min 32 --c_max 32")
  set (i16_max  "--set_meth 1 --pool_meth 1 --data_type 1 --c_min 32 --c_max 32")
  set (fp16_max "--set_meth 1 --pool_meth 1 --data_type 2 --c_min 32 --c_max 32")

  set (i8_min   "--set_meth 1 --pool_meth 2 --data_type 0 --c_min 32 --c_max 32")
  set (i16_min  "--set_meth 1 --pool_meth 2 --data_type 1 --c_min 32 --c_max 32")
  set (fp16_min "--set_meth 1 --pool_meth 2 --data_type 2 --c_min 32 --c_max 32")

  set (i8_avg   "--set_meth 1 --pool_meth 0 --data_type 0 --c_min 32 --c_max 32")
  set (i16_avg  "--set_meth 1 --pool_meth 0 --data_type 1 --c_min 32 --c_max 32")
  set (fp16_avg "--set_meth 1 --pool_meth 0 --data_type 2 --c_min 32 --c_max 32")

  set (w1_128_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w1_128_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w1_128_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w1_128_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w1_128_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w1_128_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w1_128_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w1_128_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w1_128_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w1_128_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w1_128_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w1_128_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w1_128_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w1_128_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w1_128_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w1_128_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w1_128_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w1_128_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w1_128_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w1_128_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w1_128_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w1_128_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w1_128_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w1_128_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w1_128_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w1_128_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w1_128_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w1_128_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w1_128_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w1_128_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w1_128_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w1_128_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w1_128_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w1_128_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w1_128_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w1_128_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w1_128_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w1_128_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w1_128_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w1_128_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w1_128_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w1_128_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 1 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w128_128_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w128_128_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w128_128_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w128_128_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w128_128_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w128_128_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w128_128_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w128_128_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w128_128_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w128_128_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w128_128_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w128_128_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w128_128_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w128_128_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w128_128_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w128_128_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w128_128_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w128_128_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w128_128_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w128_128_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w128_128_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w128_128_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w128_128_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w128_128_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w128_128_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w128_128_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w128_128_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w128_128_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w128_128_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w128_128_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w128_128_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w128_128_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w128_128_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w128_128_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w128_128_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w128_128_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w128_128_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w128_128_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w128_128_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w128_128_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w128_128_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w128_128_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 128 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w129_129_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w129_129_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w129_129_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w129_129_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w129_129_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w129_129_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w129_129_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w129_129_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w129_129_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w129_129_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w129_129_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w129_129_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w129_129_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w129_129_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w129_129_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w129_129_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w129_129_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w129_129_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w129_129_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w129_129_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w129_129_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w129_129_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w129_129_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w129_129_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w129_129_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w129_129_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w129_129_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w129_129_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w129_129_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w129_129_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w129_129_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w129_129_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w129_129_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w129_129_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w129_129_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w129_129_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w129_129_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w129_129_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w129_129_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w129_129_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w129_129_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w129_129_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 129 --w_max 129 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w128_256_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w128_256_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w128_256_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w128_256_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w128_256_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w128_256_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w128_256_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w128_256_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w128_256_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w128_256_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w128_256_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w128_256_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w128_256_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w128_256_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w128_256_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w128_256_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w128_256_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w128_256_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w128_256_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w128_256_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w128_256_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w128_256_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w128_256_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w128_256_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w128_256_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w128_256_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w128_256_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w128_256_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w128_256_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w128_256_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w128_256_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w128_256_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w128_256_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w128_256_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w128_256_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w128_256_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w128_256_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w128_256_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w128_256_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w128_256_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w128_256_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w128_256_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 128 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w255_255_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w255_255_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w255_255_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w255_255_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w255_255_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w255_255_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w255_255_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w255_255_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w255_255_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w255_255_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w255_255_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w255_255_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w255_255_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w255_255_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w255_255_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w255_255_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w255_255_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w255_255_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w255_255_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w255_255_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w255_255_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w255_255_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w255_255_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w255_255_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w255_255_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w255_255_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w255_255_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w255_255_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w255_255_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w255_255_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w255_255_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w255_255_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w255_255_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w255_255_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w255_255_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w255_255_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w255_255_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w255_255_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w255_255_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w255_255_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w255_255_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w255_255_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 255 --w_max 255 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w256_256_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w256_256_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w256_256_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w256_256_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w256_256_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w256_256_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w256_256_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w256_256_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w256_256_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w256_256_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w256_256_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w256_256_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w256_256_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w256_256_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w256_256_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w256_256_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w256_256_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w256_256_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w256_256_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w256_256_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w256_256_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w256_256_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w256_256_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w256_256_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w256_256_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w256_256_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w256_256_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w256_256_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w256_256_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w256_256_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w256_256_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w256_256_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w256_256_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w256_256_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w256_256_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w256_256_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w256_256_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w256_256_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w256_256_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w256_256_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w256_256_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w256_256_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 256 --c_min 256 --c_max 288 --h_min 2 --h_max 16")
  set (w256_288_k1_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 1 --h_max 1")
  set (w256_288_k1_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 2 --h_max 2")
  set (w256_288_k1_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 16 --h_min 2 --h_max 16")
  set (w256_288_k1_1_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 1 --h_max 1")
  set (w256_288_k1_1_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 2 --h_max 2")
  set (w256_288_k1_1_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 1 --c_max 1 --h_min 2 --h_max 16")
  set (w256_288_k15_15_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 1 --h_max 1")
  set (w256_288_k15_15_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 2 --h_max 2")
  set (w256_288_k15_15_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 15 --c_max 15 --h_min 2 --h_max 16")
  set (w256_288_k16_16_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 1 --h_max 1")
  set (w256_288_k16_16_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 2 --h_max 2")
  set (w256_288_k16_16_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 16 --h_min 2 --h_max 16")
  set (w256_288_k16_32_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 1 --h_max 1")
  set (w256_288_k16_32_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 2 --h_max 2")
  set (w256_288_k16_32_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 16 --c_max 32 --h_min 2 --h_max 16")
  set (w256_288_k32_48_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 1 --h_max 1")
  set (w256_288_k32_48_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 2 --h_max 2")
  set (w256_288_k32_48_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 32 --c_max 48 --h_min 2 --h_max 16")
  set (w256_288_k48_64_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 1 --h_max 1")
  set (w256_288_k48_64_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 2 --h_max 2")
  set (w256_288_k48_64_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 48 --c_max 64 --h_min 2 --h_max 16")
  set (w256_288_k64_80_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 1 --h_max 1")
  set (w256_288_k64_80_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 2 --h_max 2")
  set (w256_288_k64_80_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 64 --c_max 80 --h_min 2 --h_max 16")
  set (w256_288_k80_96_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 1 --h_max 1")
  set (w256_288_k80_96_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 2 --h_max 2")
  set (w256_288_k80_96_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 80 --c_max 96 --h_min 2 --h_max 16")
  set (w256_288_k96_112_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 1 --h_max 1")
  set (w256_288_k96_112_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 2 --h_max 2")
  set (w256_288_k96_112_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 96 --c_max 112 --h_min 2 --h_max 16")
  set (w256_288_k112_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 1 --h_max 1")
  set (w256_288_k112_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 2 --h_max 2")
  set (w256_288_k112_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 112 --c_max 128 --h_min 2 --h_max 16")
  set (w256_288_k128_128_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 1 --h_max 1")
  set (w256_288_k128_128_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 2 --h_max 2")
  set (w256_288_k128_128_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 128 --h_min 2 --h_max 16")
  set (w256_288_k128_256_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 1 --h_max 1")
  set (w256_288_k128_256_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 2 --h_max 2")
  set (w256_288_k128_256_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 128 --c_max 256 --h_min 2 --h_max 16")
  set (w256_288_k256_288_h1_1 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 1 --h_max 1")
  set (w256_288_k256_288_h2_2 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 2 --h_max 2")
  set (w256_288_k256_288_h2_16 "--set_meth 1 --pool_meth 1 --w_min 256 --w_max 288 --c_min 256 --c_max 288 --h_min 2 --h_max 16")

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

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME PPE_1
    FILES scr1/targets/simple-rom/nu/ppe_regs/regs_ppe.c
  )

  ADD_PPE_TESTS(${CONF} ppe_i8_max_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_max})
  ADD_PPE_TESTS(${CONF} ppe_i16_max_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_max})
  ADD_PPE_TESTS(${CONF} ppe_fp16_max_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_max})

  ADD_PPE_TESTS(${CONF} ppe_i8_min_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_min})
  ADD_PPE_TESTS(${CONF} ppe_i16_min_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_min})
  ADD_PPE_TESTS(${CONF} ppe_fp16_min_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_min})

  ADD_PPE_TESTS(${CONF} ppe_i8_avg_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_avg})
  ADD_PPE_TESTS(${CONF} ppe_i16_avg_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_avg})
  ADD_PPE_TESTS(${CONF} ppe_fp16_avg_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_avg})

  if("${USE_PPE_PY_TESTS}" STREQUAL "Yes")  
    ADD_PPE_PY_TESTS(${CONF})
  endif()

  ADD_VPE_PPE_WKH_COMB(${CONF} vpe_ppe_wkh_comb)
  #ADD_VPE_PPE_WKH_COMB_ALL(vpe_ppe_wkh_comb)

  #ADD_PPE_DGTL_SLTN_TESTS(${CONF})
endmacro()
