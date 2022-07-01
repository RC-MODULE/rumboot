
macro(_na_init_variables DUT)
  set(MISALIGN_COUNT 0)
  # Extract The First Letter Of The DUT (We Pass It As A Define Into c-code)
  string(SUBSTRING "${DUT}" 0 1 DUT_LETTER)
  set(DUT_LETTER_QUOTED '${DUT_LETTER}')

  # files transfered from RM to simulation environment
  set(NA_TEST_num_iterations_file num_iterations.bin)
  set(NA_TEST_dep_table_file dep_table.bin)
  set(NA_TEST_in_file cube.bin)
  set(NA_TEST_warr_file warr.bin)
  set(NA_TEST_etalon_file etalon.bin)
  set(NA_TEST_op0_vec_file op0_vec.bin)
  set(NA_TEST_op0_cube_file op0_cube.bin)
  set(NA_TEST_op1_vec_file op1_vec.bin)
  set(NA_TEST_op1_cube_file op1_cube.bin)
  set(NA_TEST_op2_vec_file op2_vec.bin)
  set(NA_TEST_op2_cube_file op2_cube.bin)
  set(NA_TEST_lut1_file lut1.bin)
  set(NA_TEST_lut2_file lut2.bin)

  set(NA_TEST_cfg_file config_vpe.bin)
  set(NA_TEST_cfg_mpe_file config_mpe.bin)
  set(NA_TEST_cfg_ppe_file config_ppe.bin)
  set(NA_TEST_mpe_cmd_file cmd.bin)

  set(NA_TEST_metrics_in ${NA_TEST_in_file}.metrics)
  set(NA_TEST_metrics_warr ${NA_TEST_warr_file}.metrics)
  set(NA_TEST_metrics_etalon ${NA_TEST_etalon_file}.metrics)
  set(NA_TEST_metrics_op0_vec ${NA_TEST_op0_vec_file}.metrics)
  set(NA_TEST_metrics_op0_cube ${NA_TEST_op0_cube_file}.metrics)
  set(NA_TEST_metrics_op1_vec ${NA_TEST_op1_vec_file}.metrics)
  set(NA_TEST_metrics_op1_cube ${NA_TEST_op1_cube_file}.metrics)
  set(NA_TEST_metrics_op2_vec ${NA_TEST_op2_vec_file}.metrics)
  set(NA_TEST_metrics_op2_cube ${NA_TEST_op2_cube_file}.metrics)
  set(NA_TEST_metrics_lut1 ${NA_TEST_lut1_file}.metrics)
  set(NA_TEST_metrics_lut2 ${NA_TEST_lut2_file}.metrics)
  set(NA_TEST_metrics_mpe_cmd ${NA_TEST_mpe_cmd_file}.metrics)

  set(NA_TEST_metrics_dep_table ${NA_TEST_dep_table_file}.metrics)

  set(NA_TEST_status_regs_file status_regs.bin)
  #set(NA_TEST_mpe_cfg_lut_file ${CMAKE_SOURCE_DIR}/src/lib/drivers/rcm_na/mpe_regs_table_macro.bin)
  set(NA_TEST_mpe_cfg_lut_file mpe_regs_table_macro.bin)
  set(NA_TEST_metrics_mpe_cfg_lut ${NA_TEST_mpe_cfg_lut_file}.metrics)

  ###################

  set(NA_RM_KEYS
    --num_iterations_file      ${NA_TEST_num_iterations_file}
    --dep_table_file           ${NA_TEST_dep_table_file}      
    --in_file                  ${NA_TEST_in_file}
    --warr_file                ${NA_TEST_warr_file}
    --etalon_file              ${NA_TEST_etalon_file}
    --op0_vec_file             ${NA_TEST_op0_vec_file}
    --op0_cube_file            ${NA_TEST_op0_cube_file}
    --op1_vec_file             ${NA_TEST_op1_vec_file}
    --op1_cube_file            ${NA_TEST_op1_cube_file}
    --op2_vec_file             ${NA_TEST_op2_vec_file}
    --op2_cube_file            ${NA_TEST_op2_cube_file}
    --lut1_file                ${NA_TEST_lut1_file}
    --lut2_file                ${NA_TEST_lut2_file}
    --cfg_file                 ${NA_TEST_cfg_file}
    --cfg_mpe_file             ${NA_TEST_cfg_mpe_file}
    --cfg_ppe_file             ${NA_TEST_cfg_ppe_file}
    --status_regs_file         ${NA_TEST_status_regs_file}
    # '.metrics' added by RM itself
  )

  set(PLUSARG_num_iterations_file_tag num_iterations_file_tag)
  set(PLUSARG_dep_table_file_tag dep_table_file_tag)
  set(PLUSARG_in_file_tag in_file_tag)
  set(PLUSARG_warr_file_tag warr_file_tag)
  set(PLUSARG_etalon_file_tag etalon_file_tag)
  set(PLUSARG_op0_vec_file_tag op0_vec_file_tag)
  set(PLUSARG_op0_cube_file_tag op0_cube_file_tag)
  set(PLUSARG_op1_vec_file_tag op1_vec_file_tag)
  set(PLUSARG_op1_cube_file_tag op1_cube_file_tag)
  set(PLUSARG_op2_vec_file_tag op2_vec_file_tag)
  set(PLUSARG_op2_cube_file_tag op2_cube_file_tag)
  set(PLUSARG_lut1_file_tag lut1_file_tag)
  set(PLUSARG_lut2_file_tag lut2_file_tag)

  set(PLUSARG_cfg_file_tag cfg_file_tag)
  set(PLUSARG_cfg_mpe_file_tag cfg_mpe_file_tag)
  set(PLUSARG_cfg_ppe_file_tag cfg_ppe_file_tag)
  set(PLUSARG_mpe_cmd_file_tag mpe_cmd_file_tag)

  set(PLUSARG_metrics_in_tag metrics_in_tag)
  set(PLUSARG_metrics_warr_tag metrics_warr_tag)
  set(PLUSARG_metrics_etalon_tag metrics_etalon_tag)
  set(PLUSARG_metrics_op0_vec_tag metrics_op0_vec_tag)
  set(PLUSARG_metrics_op0_cube_tag metrics_op0_cube_tag)
  set(PLUSARG_metrics_op1_vec_tag metrics_op1_vec_tag)
  set(PLUSARG_metrics_op1_cube_tag metrics_op1_cube_tag)
  set(PLUSARG_metrics_op2_vec_tag metrics_op2_vec_tag)
  set(PLUSARG_metrics_op2_cube_tag metrics_op2_cube_tag)
  set(PLUSARG_metrics_lut1_tag metrics_lut1_file_tag)
  set(PLUSARG_metrics_lut2_tag metrics_lut2_file_tag)
  set(PLUSARG_metrics_mpe_cmd_tag metrics_mpe_cmd_tag)

  set(PLUSARG_metrics_dep_table_tag metrics_dep_table_tag)

  set(PLUSARG_status_regs_file_tag status_regs_file_tag)
  set(PLUSARG_mpe_cfg_lut_file_tag mpe_cfg_lut_file_tag)
  set(PLUSARG_metrics_mpe_cfg_lut_tag metrics_mpe_cfg_lut_tag)

  set(NA_RM_BIN_PATH ${CMAKE_BINARY_DIR}/${rumboot_dirname}/npe_rm/rm_core/rtl-tests)
  set(NA_RM_PLUSARGS +${PLUSARG_in_file_tag}=${NA_TEST_in_file}
                     +${PLUSARG_warr_file_tag}=${NA_TEST_warr_file}
                     +${PLUSARG_etalon_file_tag}=${NA_TEST_etalon_file}
                     +${PLUSARG_op0_vec_file_tag}=${NA_TEST_op0_vec_file}
                     +${PLUSARG_op0_cube_file_tag}=${NA_TEST_op0_cube_file}
                     +${PLUSARG_op1_vec_file_tag}=${NA_TEST_op1_vec_file}
                     +${PLUSARG_op1_cube_file_tag}=${NA_TEST_op1_cube_file}
                     +${PLUSARG_op2_vec_file_tag}=${NA_TEST_op2_vec_file}
                     +${PLUSARG_op2_cube_file_tag}=${NA_TEST_op2_cube_file}
                     +${PLUSARG_lut1_file_tag}=${NA_TEST_lut1_file}
                     +${PLUSARG_lut2_file_tag}=${NA_TEST_lut2_file}

                     +${PLUSARG_cfg_file_tag}=${NA_TEST_cfg_file}
                     +${PLUSARG_cfg_mpe_file_tag}=${NA_TEST_cfg_mpe_file}
                     +${PLUSARG_cfg_ppe_file_tag}=${NA_TEST_cfg_ppe_file}
                     +${PLUSARG_mpe_cmd_file_tag}=${NA_TEST_mpe_cmd_file}

                     +${PLUSARG_metrics_in_tag}=${NA_TEST_metrics_in}
                     +${PLUSARG_metrics_warr_tag}=${NA_TEST_metrics_warr}
                     +${PLUSARG_metrics_etalon_tag}=${NA_TEST_metrics_etalon}
                     +${PLUSARG_metrics_op0_vec_tag}=${NA_TEST_metrics_op0_vec}
                     +${PLUSARG_metrics_op0_cube_tag}=${NA_TEST_metrics_op0_cube}
                     +${PLUSARG_metrics_op1_vec_tag}=${NA_TEST_metrics_op1_vec}
                     +${PLUSARG_metrics_op1_cube_tag}=${NA_TEST_metrics_op1_cube}
                     +${PLUSARG_metrics_op2_vec_tag}=${NA_TEST_metrics_op2_vec}
                     +${PLUSARG_metrics_op2_cube_tag}=${NA_TEST_metrics_op2_cube}
                     +${PLUSARG_metrics_lut1_tag}=${NA_TEST_metrics_lut1}
                     +${PLUSARG_metrics_lut2_tag}=${NA_TEST_metrics_lut2}
                     +${PLUSARG_metrics_mpe_cmd_tag}=${NA_TEST_metrics_mpe_cmd}

                     +${PLUSARG_metrics_dep_table_tag}=${NA_TEST_metrics_dep_table}

                     +${PLUSARG_status_regs_file_tag}=${NA_TEST_status_regs_file}
                     +${PLUSARG_mpe_cfg_lut_file_tag}=${NA_TEST_mpe_cfg_lut_file}
                     +${PLUSARG_metrics_mpe_cfg_lut_tag}=${NA_TEST_metrics_mpe_cfg_lut}

                     +${PLUSARG_num_iterations_file_tag}=${NA_TEST_num_iterations_file}
                     +${PLUSARG_dep_table_file_tag}=${NA_TEST_dep_table_file}
  )
  set(NA_RM_PLUSARGS_LOOP 
    +${PLUSARG_num_iterations_file_tag}=${NA_TEST_num_iterations_file}
    +${PLUSARG_dep_table_file_tag}=${NA_TEST_dep_table_file}
    +${PLUSARG_metrics_dep_table_tag}=${NA_TEST_metrics_dep_table}
    +${PLUSARG_mpe_cfg_lut_file_tag}=${NA_TEST_mpe_cfg_lut_file}
    +${PLUSARG_metrics_mpe_cfg_lut_tag}=${NA_TEST_metrics_mpe_cfg_lut}
  )
  foreach(i RANGE 0 31)
    set(NA_RM_PLUSARGS_LOOP ${NA_RM_PLUSARGS_LOOP} 
      +${PLUSARG_in_file_tag}_${i}_=${NA_TEST_in_file}.${i}
      +${PLUSARG_warr_file_tag}_${i}_=${NA_TEST_warr_file}.${i}
      +${PLUSARG_etalon_file_tag}_${i}_=${NA_TEST_etalon_file}.${i}
      +${PLUSARG_op0_vec_file_tag}_${i}_=${NA_TEST_op0_vec_file}.${i}
      +${PLUSARG_op0_cube_file_tag}_${i}_=${NA_TEST_op0_cube_file}.${i}
      +${PLUSARG_op1_vec_file_tag}_${i}_=${NA_TEST_op1_vec_file}.${i}
      +${PLUSARG_op1_cube_file_tag}_${i}_=${NA_TEST_op1_cube_file}.${i}
      +${PLUSARG_op2_vec_file_tag}_${i}_=${NA_TEST_op2_vec_file}.${i}
      +${PLUSARG_op2_cube_file_tag}_${i}_=${NA_TEST_op2_cube_file}.${i}
      +${PLUSARG_lut1_file_tag}_${i}_=${NA_TEST_lut1_file}.${i}
      +${PLUSARG_lut2_file_tag}_${i}_=${NA_TEST_lut2_file}.${i}

      +${PLUSARG_cfg_file_tag}_${i}_=${NA_TEST_cfg_file}.${i}
      +${PLUSARG_cfg_mpe_file_tag}_${i}_=${NA_TEST_cfg_mpe_file}.${i}

      +${PLUSARG_cfg_ppe_file_tag}_${i}_=${NA_TEST_cfg_ppe_file}.${i}

      +${PLUSARG_mpe_cmd_file_tag}_${i}_=${NA_TEST_mpe_cmd_file}.${i}

      +${PLUSARG_metrics_in_tag}_${i}_=${NA_TEST_in_file}.${i}.metrics
      +${PLUSARG_metrics_warr_tag}_${i}_=${NA_TEST_warr_file}.${i}.metrics
      +${PLUSARG_metrics_etalon_tag}_${i}_=${NA_TEST_etalon_file}.${i}.metrics
      +${PLUSARG_metrics_op0_vec_tag}_${i}_=${NA_TEST_op0_vec_file}.${i}.metrics
      +${PLUSARG_metrics_op0_cube_tag}_${i}_=${NA_TEST_op0_cube_file}.${i}.metrics
      +${PLUSARG_metrics_op1_vec_tag}_${i}_=${NA_TEST_op1_vec_file}.${i}.metrics
      +${PLUSARG_metrics_op1_cube_tag}_${i}_=${NA_TEST_op1_cube_file}.${i}.metrics
      +${PLUSARG_metrics_op2_vec_tag}_${i}_=${NA_TEST_op2_vec_file}.${i}.metrics
      +${PLUSARG_metrics_op2_cube_tag}_${i}_=${NA_TEST_op2_cube_file}.${i}.metrics
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
  rumboot_add_external_project(externals/npe_rm 
    CONFIGURE_FLAGS -DCOMPILE_FROM_ROMBOOT="YES"
    NOBUILD
  )
  rumboot_add_external_project(externals/py_mpe_test
    NOCONFIGURE
  )

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


  # rumboot_add_external_project(externals/py_ppe_test)
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
macro(ADD_NPE_MPE_VPE_TEST CONF name rm_bin_name make_tight comparer)
  ADD_NPE_MPE_VPE_TEST_SEED(${CONF} ${name} ${rm_bin_name} ${make_tight} ${comparer} 64)  
endmacro()

macro(ADD_MPE_PRM CONF sfx prm_min prm_max)

  if(NOT DEFINED NU_SEED)
    set(NU_SEED 1)
  endif()

  if(NOT DEFINED NU_IT_NMB OR NU_IT_NMB EQUAL 1)
    set(NU_IT_NMB 32)
  endif()

  if("${sfx}" MATCHES "krnl")
    set (set_PyPx "--set_R 1 --set_S 1")
    set (Py_n "R")
    set (Px_n "S")
  elseif("${sfx}" MATCHES "strd")
    set (set_PyPx "--set_Sh 1 --set_Sw 1")
    set (Py_n "Sh")
    set (Px_n "Sw")
  elseif ("${sfx}" MATCHES "dltn")
    set (set_PyPx "--set_Dr 1 --set_Ds 1")
    set (Py_n "Dr")
    set (Px_n "Ds")
  else()
    message(FATAL_ERROR "Undefined test type ${sfx}")
  endif()

  if ("${sfx}" MATCHES "IN_INT8" OR "${sfx}" MATCHES "IN_INT16" OR "${sfx}" MATCHES "IN_FP16")
    string(REGEX MATCH "IN_.+[0-9]+" in_dt ${sfx})
    set (rm_bin_name "main_mpe_rnd_${in_dt}")

    if ("${in_dt}" MATCHES "IN_FP16")
      set (bitwise "EPS")
    else()
      set (bitwise "BITWISE")
    endif()
  else()
    message(FATAL_ERROR "Undefined DataType ${sfx}")
  endif()

  set (hwck_min_max "--h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 33 --c_max 33 --k_min 1 --k_max 1")

  set (set_it_nmb "--it_nmb ${NU_IT_NMB}")

  set (NA_RM_KEYS_SWP ${NA_RM_KEYS})

  foreach(Py RANGE ${prm_min} ${prm_max})
    foreach(Px RANGE ${prm_min} ${prm_max})
      set (RM_CFG_PARAM "${set_PyPx} --${Py_n} ${Py} --${Px_n} ${Px}")

      set (NA_RM_KEYS "${NA_RM_KEYS} ${set_it_nmb} ${RM_CFG_PARAM} ${hwck_min_max}")

      set (name "${sfx}_${Py_n}_${Py}_${Px_n}_${Px}_${NU_SEED}")

      ADD_NPE_MPE_VPE_TEST_SEED(${CONF} ${name} ${rm_bin_name} NO_TIGHT ${bitwise} ${NU_SEED})

      set (NA_RM_KEYS ${NA_RM_KEYS_SWP})

    endforeach()
  endforeach()

endmacro()

macro(ADD_NPE_MPE_VPE_TEST_SEED CONF name rm_bin_name make_tight comparer seed_value)
  if("${comparer}" STREQUAL "EPS")
    set(COMPARER_OPT -DUSE_NU_HALF_COMPARE_EPS=1)
  elseif("${comparer}" STREQUAL "MPE_PERFORMANCE")
    set(COMPARER_OPT -DMPE_CFG_PERFORMANCE=1)
  elseif("${comparer}" STREQUAL "MPE_PERFORMANCE_DO")
    set(COMPARER_OPT -DMPE_CFG_PERFORMANCE_DO=1)
  else()
    set(COMPARER_OPT)
  endif()

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
    CFLAGS -DDONT_USE_PPE=1 ${COMPARER_OPT}
    PREPCMD 
      ${NA_RM_BIN_PATH}/${rm_bin_name} 
      ${NA_RM_KEYS} 
      ${RM_TF_KEYS}
      --seed ${seed_value}
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
      CFLAGS -DDONT_USE_PPE=1 ${COMPARER_OPT}
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

macro(ADD_NPE_MPE_TEST CONF name number rm_bin_name)
  if (${number} EQUAL 0)
    set(TESTNAME ${name})
  else()
    set(TESTNAME "MPE_${number}")
  endif()
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${TESTNAME}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
    CFLAGS -DDONT_USE_PPE=1 -D${name}
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


macro(ADD_NPE_MPE_BATCH_TEST CONF name number rm_bin_name)
  if (${number} EQUAL 0)
    set(TESTNAME ${name})
  else()
    set(TESTNAME "MPE_VPE_LONG_BATCH_${number}")
  endif()
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${TESTNAME}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long_batch_mode.c
    CFLAGS -DDONT_USE_PPE=1 -D${name}
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

macro(ADD_NPE_MPE_VPE_RST_TEST CONF name rm_bin_name make_tight comparer)
  if("${comparer}" STREQUAL "EPS")
    set(COMPARER_OPT -DUSE_NU_HALF_COMPARE_EPS=1)
  else()
    set(COMPARER_OPT)
  endif()
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_rst
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_mpe_rst_long.c
    CFLAGS -DDONT_USE_PPE=1 ${COMPARER_OPT}
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

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_pause
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_mpe_pause_long.c
    CFLAGS -DDONT_USE_PPE=1 ${COMPARER_OPT}
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

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_rst_fail
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_mpe_int_long.c
    CFLAGS -DDONT_USE_PPE=1 ${COMPARER_OPT}
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
macro(ADD_NPE_COMPLEX_TEST CONF name rm_bin_name make_tight comparer)
  if("${comparer}" STREQUAL "EPS")
    set(COMPARER_OPT -DUSE_NU_HALF_COMPARE_EPS=1)
  else()
    set(COMPARER_OPT)
  endif()
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_long.c
    CFLAGS ${COMPARER_OPT}
    PREPCMD 
      ${NA_RM_BIN_PATH}/${rm_bin_name} 
      ${NA_RM_KEYS} 
      ${RM_TF_KEYS}
      > ${RM_LOGFILE} &&

      ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
      &&
      ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

      || exit 1

    IRUN_FLAGS ${NA_RM_PLUSARGS} ${COMP}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )  
 
  if("${make_tight}" STREQUAL "MAKE_TIGHT")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_npe.c
      CFLAGS ${COMPARER_OPT}
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

macro(ADD_NPE_COMPLEX_TEST_RST CONF name rm_bin_name make_tight comparer)
  if("${comparer}" STREQUAL "EPS")
    set(COMPARER_OPT -DUSE_NU_HALF_COMPARE_EPS=1)
  else()
    set(COMPARER_OPT)
  endif()	
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_rst
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_npe_rst_long.c
    CFLAGS ${COMPARER_OPT}
    PREPCMD 
      ${NA_RM_BIN_PATH}/${rm_bin_name} 
      ${NA_RM_KEYS} 
      ${RM_TF_KEYS}
      > ${RM_LOGFILE} &&

      ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}
      &&
      ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}

      || exit 1

    IRUN_FLAGS ${NA_RM_PLUSARGS} ${COMP}
    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
 
endmacro()

macro(ADD_NPE_COMPLEX_TEST_TIGHT CONF name rm_bin_name make_tight comparer)
  if("${comparer}" STREQUAL "EPS")
    set(COMPARER_OPT -DUSE_NU_HALF_COMPARE_EPS=1)
  else()
    set(COMPARER_OPT)
  endif()
  if("${make_tight}" STREQUAL "MAKE_TIGHT")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_complex_npe.c
      CFLAGS ${COMPARER_OPT}
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


macro (ADD_NKBVS_TEST CONF name arrays_path d vpe_in_c vpe_in_h vpe_in_w)
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME nkbvs_${name}
    FILES scr1/targets/simple-rom/nu/npe/nkbvs.c
    CFLAGS -D${d} -Dvpe_in_c=${vpe_in_c} -Dvpe_in_w=${vpe_in_w} -Dvpe_in_h=${vpe_in_h}
    PREPCMD
        ${NA_RM_BIN_PATH}/main_nkbvs_${name}
        ${NA_RM_KEYS}
        ${RM_TF_KEYS}
        > ${RM_LOGFILE} &&

        ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} TRUNC16 < ${arrays_path}/mpe_arrays.txt

        #&& cp ${arrays_path}/config_vpe.bin .

        && mv ./config_vpe.bin.0 ./config_vpe.bin

        || exit 1

    IRUN_FLAGS ${NA_RM_PLUSARGS}
    SUBPROJECT_DEPS npe_rm:main_nkbvs_${name}
  )
endmacro()

macro (ADD_NKBVS_DW_TRANSPOSE_TEST CONF name arrays_path d stride_h stride_w pad_h pad_w vpe_in_c vpe_in_h vpe_in_w)
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME nkbvs_${name}
    FILES scr1/targets/simple-rom/nu/npe/nkbvs_dw_transpose.c
    CFLAGS -D${d} -Dstride_h=${stride_h} -Dstride_w=${stride_w} -Dpad_h=${pad_h} -Dpad_w=${pad_w} -Dvpe_in_c=${vpe_in_c} -Dvpe_in_w=${vpe_in_w} -Dvpe_in_h=${vpe_in_h}
    PREPCMD
        ${NA_RM_BIN_PATH}/main_nkbvs_${name}
        ${NA_RM_KEYS}
        ${RM_TF_KEYS}
        > ${RM_LOGFILE} &&

        ${MPE_PARSE_TEST_STAGE2} ${NA_TEST_mpe_cmd_file} ${NA_TEST_in_file} ${NA_TEST_warr_file} ${NA_TEST_etalon_file} TRUNC16 < ${arrays_path}/mpe_arrays.txt

        #&& cp ${arrays_path}/config_vpe.bin .

        && mv ./config_vpe.bin.0 ./config_vpe.bin

        || exit 1

    IRUN_FLAGS ${NA_RM_PLUSARGS}
    SUBPROJECT_DEPS npe_rm:main_nkbvs_${name}
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

macro (ADD_PPE_TESTS CONF name rm_bin_name ShowPerf DataSrc LBS RM_CFG_PARAM)

  if(NOT DEFINED NU_SEED)
    set(NU_SEED 1)
  endif()

  if(NOT DEFINED NU_IT_NMB OR NU_IT_NMB EQUAL 1)
    set(NU_IT_NMB 32)
  endif()

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  if(${LBS} STREQUAL "LIN")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_tight
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_ppe.c

      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

      CFLAGS -DDUT=${DUT_LETTER_QUOTED}

      IRUN_FLAGS ${NA_RM_PLUSARGS}

      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
    )
  endif()

endmacro()

macro (ADD_PPE_RST_TESTS CONF name rm_bin_name ShowPerf DataSrc LBS RM_CFG_PARAM)

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_na_rst_fail
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_na_ppe_int_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_na_rst
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_na_ppe_rst_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
  
  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}_pause
    FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_na_ppe_pause_long.c

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${ShowPerf} -D${DataSrc} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

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
      NAME ${name}_tight_vpe_rst
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
      NAME ${name}_tight_vpe_pause
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_pause_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1 -DVPE_CUBE_CMPL=1 -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  ) 
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_na_tight_vpe_rst
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_na_vpe_rst_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1  -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  ) 
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_na_tight_vpe_rst_fail
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_na_vpe_int_long.c
      CFLAGS -DFORCE_VPE_WDMA_EN=1  -DDUT=${DUT_LETTER_QUOTED}
      PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )
  add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ${name}_na_tight_vpe_pause
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_na_vpe_pause_long.c
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
      #message("Warning: scenario__path_mode ${scenario__path_mode} For Test ${test_name} Not Supported")
      set(LBS SPL)
    endif()

    execute_process(
      COMMAND             grep reduction ${file}
      WORKING_DIRECTORY   ${CMAKE_BINARY_DIR}
      OUTPUT_VARIABLE     REDUCTION_LINE
    )
    string(REGEX MATCH "\\[[12]\\]" reduction_value ${REDUCTION_LINE})
    if ("${reduction_value}" STREQUAL "[1]")
      set(MAX_RED MAX_RED_0)
    elseif("${reduction_value}" STREQUAL "[2]")
      set(MAX_RED MAX_RED_1)
    else()
      message(FATAL_ERROR "Undefined MAX_RED ${reduction_value} in file ${file}")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_blocking_KW") 
      set (AXI_BLOCKINGS "+axi128_latency=12 +axi128_blockings")
    else()
      set (AXI_BLOCKINGS "")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_pause_KW") 
      set (PPE_PAUSE "PPE_PAUSE")
    else()
      set (PPE_PAUSE "NO_PPE_PAUSE")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_sw_rst") 
      set (PPE_SWRST "PPE_SWRST")
    else()
      set (PPE_SWRST "NO_PPE_SWRST")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_a_few_cube") 
      set (PPE_CFG_CONST "PPE_CFG_CONST")
    else()
      set (PPE_CFG_CONST "NO_PPE_CFG_CONST")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_mark_cube") 
      set (PPE_MARK_CUBE "PPE_MARK_CUBE")
    else()
      set (PPE_MARK_CUBE "NO_PPE_MARK_CUBE")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_perform") 
      set (ShowPerf "ShowPerf")
    else()
      set (ShowPerf "NotShowPerf")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_data_fp_nan" OR
        "${test_name}" MATCHES "dc_ppe_data_fp_inf") 
      set (PPE_NAN_INF "PPE_NAN_INF")
    else()
      set (PPE_NAN_INF "NoPPE_NAN_INF")
    endif()

    if ("${test_name}" MATCHES "dc_ppe_axi_error") 
      set (AXI128_RESPERR "+axi128_resperr")
      set (RESP_ERR "RESP_ERR")
    else()
      set (AXI128_RESPERR "")
      set (RESP_ERR "NO_RESP_ERR")
    endif()

    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME ppe_py_${test_name}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c
 
      CFLAGS -D${LBS} -D${MAX_RED} -DDUT=${DUT_LETTER_QUOTED} -DMEMtoPPE=1 -D${PPE_PAUSE} -D${PPE_SWRST} -D${PPE_CFG_CONST} -D${PPE_MARK_CUBE} -D${ShowPerf} -D${PPE_NAN_INF} -D${RESP_ERR}

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
      IRUN_FLAGS ${NA_RM_PLUSARGS} ${AXI_BLOCKINGS} ${AXI128_RESPERR}
      SUBPROJECT_DEPS npe_rm:${rm_bin_name}
    )

  endforeach()
endmacro()

macro (ADD_VPE_PPE_WKH_COMB CONF test_name_list ALL_COMB)
  set (i 0)
  set (j 0)

  foreach(wkh_var ${${test_name_list}})

    string(REGEX REPLACE "w([0-9]+)_.*" \\1 w_min ${wkh_var})
    string(REGEX REPLACE "w[0-9]+_([0-9]+)_.*" \\1 w_max ${wkh_var})   

    string(REGEX REPLACE "w.*k([0-9]+)_.*" \\1 k_min ${wkh_var})
    string(REGEX REPLACE "w.*k[0-9]+_([0-9]+)_.*" \\1 k_max ${wkh_var})

    string(REGEX REPLACE "w.*h([0-9]+)_.*" \\1 h_min ${wkh_var})
    string(REGEX REPLACE "w.*h[0-9]+_([0-9]+)" \\1 h_max ${wkh_var})

    set (wkh_opt "--w_min ${w_min} --w_max ${w_max} --c_min ${k_min} --c_max ${k_max} --h_min ${h_min} --h_max ${h_max}")

    set (nxt_trgt 1)

    while (${nxt_trgt})

      if (NOT ${ALL_COMB})
        math (EXPR j "${i} % 4")
        math (EXPR i "${i} + 1")
      endif()

      if (j EQUAL 0)
        set (name "vpe_ppe_${wkh_var}_LIN_8b")
        set (rm_bin_name "main_vpe_ppe_IN_INT8")
        set (LBS "LIN")

        set (wkh_opt "${wkh_opt} --data_type 0")
      elseif (j EQUAL 1)
        set (name "vpe_ppe_${wkh_var}_LIN_16b")
        set (rm_bin_name "main_vpe_ppe_IN_INT16")
        set (LBS "LIN")

        set (wkh_opt "${wkh_opt} --data_type 1")
      elseif (j EQUAL 2)
        set (name "vpe_ppe_${wkh_var}_BOX_8b")
        set (rm_bin_name "main_vpe_ppe_IN_INT8_BOX")
        set (LBS "BOX")

        set (wkh_opt "${wkh_opt} --data_type 0")
      elseif (j EQUAL 3)
        set (name "vpe_ppe_${wkh_var}_BOX_16b")
        set (rm_bin_name "main_vpe_ppe_IN_INT16_BOX")
        set (LBS "BOX")

        set (wkh_opt "${wkh_opt} --data_type 1")
      endif()

      add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME ${name}
        FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe_long.c
  
        PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} ${wkh_opt} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
  
        CFLAGS -D${LBS} -DDUT=${DUT_LETTER_QUOTED}
  
        IRUN_FLAGS ${NA_RM_PLUSARGS}
  
        SUBPROJECT_DEPS npe_rm:${rm_bin_name}
      )
  
      if (NOT ${All_COMB})
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME ${name}_tight
          FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_tight_vpe_ppe_long.c
  
          PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} ${wkh_opt} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
  
          CFLAGS -D${ShowPerf} -D${LBS} -DDUT=${DUT_LETTER_QUOTED}
  
          IRUN_FLAGS ${NA_RM_PLUSARGS}
  
          SUBPROJECT_DEPS npe_rm:${rm_bin_name}
        )
      endif()

      if (${ALL_COMB})
        if (j EQUAL 3)
          set (j 0)
          set (nxt_trgt 0)
          math (EXPR NU_SEED "${NU_SEED} + 1")
        else()
          math (EXPR j "${j} + 1")
        endif()
      else()
        set (nxt_trgt 0)

        if (j EQUAL 3)
          math (EXPR NU_SEED "${NU_SEED} + 1")
        endif()
      endif()

    endwhile()

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

  if(NA_TESTGROUP STREQUAL "POWER")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME NPE_POWER
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_power.c
      PREPCMD 
        ${NA_RM_BIN_PATH}/main_npe_power 
        ${NA_RM_KEYS} 
        > ${RM_LOGFILE} &&

        ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE}

        || exit 1

      IRUN_FLAGS ${NA_RM_PLUSARGS_LOOP}
                 +clk_div_en
      SUBPROJECT_DEPS npe_rm:main_npe_power
    )
  endif()

  #add_rumboot_target(
  # CONFIGURATION ${CONF}
  #   NAME NPE_1
  #   FILES scr1/targets/simple-rom/nu/npe_regs/npe_regs.c
  # )

  ADD_NPE_SIMPLE_TEST(${CONF} ID_check scr1/targets/simple-rom/nu/npe_regs/id_check.c)
    
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
  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "MPE")
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
    
    ADD_NPE_MPE_TEST(${CONF} MPE_CFG_TESTPLAN_RDMA_RDCH 19 main_mpe_direct_ex_MPE_CFG_TESTPLAN_RDMA_RDCH)
    ADD_NPE_MPE_TEST(${CONF} MPE_CFG_TESTPLAN_RDMA_WRCH 20 main_mpe_direct_ex_MPE_CFG_TESTPLAN_RDMA_WRCH)
    #ADD_NPE_MPE_TEST(${CONF} MPE_CFG_TESTPLAN_RDMA_THRE 0 main_mpe_direct_ex_MPE_CFG_TESTPLAN_RDMA_THRE)
    ADD_NPE_MPE_TEST(${CONF} MPE_CFG_POWER 0 main_mpe_direct_ex_MPE_CFG_POWER)
    
    foreach(geometry IN ITEMS GEOMETRY_1 GEOMETRY_2 GEOMETRY_3 GEOMETRY_4 GEOMETRY_5 GEOMETRY_6 GEOMETRY_7)
     foreach(data IN ITEMS INT8 INT16 FP16)
      foreach(batch_size RANGE 1 31)
       ADD_NPE_MPE_BATCH_TEST(${CONF} MPE_VPE_LONG_BATCH ${batch_size}_${data}_${geometry} main_npe_batch_${batch_size}_${data}_${geometry})
      endforeach()
     endforeach()
    endforeach()
    
        ###
      #add_rumboot_target(
      #  CONFIGURATION ${CONF}
      #  NAME RESNET
      #  FILES scr1/targets/simple-rom/nu/npe/npe_resnet.c
      #  PREPCMD cp ${NPE_BINS}/resnet_bins/cmd.bin ${NPE_BINS}/resnet_bins/cmd.bin.metrics ${NPE_BINS}/resnet_bins/cube.bin ${NPE_BINS}/resnet_bins/cube.bin.metrics ${NPE_BINS}/resnet_bins/etalon.bin ${NPE_BINS}/resnet_bins/etalon.bin.metrics ${NPE_BINS}/resnet_bins/op0.bin ${NPE_BINS}/resnet_bins/op0.bin.metrics ${NPE_BINS}/resnet_bins/warr.bin ${NPE_BINS}/resnet_bins/warr.bin.metrics -t .
      #  IRUN_FLAGS ${NA_RM_PLUSARGS}
      #)

      #ADD_MPE_CONV_TEST(${CONF} MPE_2 TRUNC16)
          ## NOT EXPERIMENT_STAGE_2_SUB_2

    ADD_NPE_MPE_VPE_TEST(${CONF} MPE_PERFORMANCE main_mpe_performance NOT_MAKE_TIGHT MPE_PERFORMANCE)
    ADD_NPE_MPE_VPE_TEST(${CONF} MPE_PERFORMANCE_DO main_mpe_performance_do NOT_MAKE_TIGHT MPE_PERFORMANCE_DO)
    
    ADD_MPE_VPE_BATCH_TEST(${CONF} 2 66 274 7 1 1 1 1 130 3 3 3 3) # B C W H LP RP BP TP K S R SX SY
    ADD_MPE_VPE_BATCH_TEST(${CONF} 3 66 274 7 1 1 1 1 130 3 3 3 3) # B C W H LP RP BP TP K S R SX SY

    # Test on MPE::ALU
    foreach(in_macro IN ITEMS IN_INT16 IN_INT8)
      foreach(walking_item IN ITEMS 64 1 2 3)
        ADD_NPE_MPE_VPE_TEST_SEED(
          ${CONF} 
          npe_mpe_alu_${in_macro}_${walking_item} 
          main_mpe_alu_${in_macro} # not-zero walking in data
          NO_TIGHT BITWISE 
          ${walking_item}
        )
        ADD_NPE_MPE_VPE_TEST_SEED(
          ${CONF} 
          npe_mpe_alu_${in_macro}_ZEROWARR_${walking_item} 
          main_mpe_alu_${in_macro}_ZEROWARR # not-zero walking in kernels
          NO_TIGHT BITWISE 
          ${walking_item}
        )
      endforeach()
    endforeach()

    # Test on MPE. FP16. special cases, null column.
    ADD_NPE_MPE_VPE_TEST(
      ${CONF} 
      npe_mpe_null_column
      main_mpe_null_column
      NO_TIGHT EPS 
    )

  endif()  # NA_TESTGROUP MPE

  ### MPE KERNEL STRIDE DILATION NA_TESTGROUP MPE_KRNL MPE_STRD MPE_DLTN
  if(NOT DEFINED NA_TESTGROUP OR
    "${NA_TESTGROUP}" STREQUAL "MPE_KRNL" OR
    "${NA_TESTGROUP}" STREQUAL "MPE_STRD" OR
    "${NA_TESTGROUP}" STREQUAL "MPE_DLTN"
  )

    set (krnl_min "1")
    set (krnl_max "16")
    set (strd_min "1")
    set (strd_max "16")
    set (dltn_min "1")
    set (dltn_max "8")

    if(NOT DEFINED NA_TESTGROUP)
      set(prm_list krnl strd dltn)
    elseif ("${NA_TESTGROUP}" STREQUAL "MPE_KRNL")
      set(prm_list "krnl")
    elseif ("${NA_TESTGROUP}" STREQUAL "MPE_STRD")
      set(prm_list "strd")
    elseif ("${NA_TESTGROUP}" STREQUAL "MPE_DLTN")
      set(prm_list "dltn")
    endif()

    if(NOT DEFINED NA_TESTGROUP)
      set (dt_list IN_INT16)
    elseif (NOT DEFINED NA_IN_DATATYPE)
      set (dt_list IN_INT8 IN_INT16 IN_FP16)
    elseif ("${NA_IN_DATATYPE}" STREQUAL "IN_INT8")
      set (dt_list "IN_INT8")
    elseif ("${NA_IN_DATATYPE}" STREQUAL "IN_INT16")
      set (dt_list "IN_INT16")
    elseif ("${NA_IN_DATATYPE}" STREQUAL "IN_FP16")
      set (dt_list "IN_FP16")
    else()
      message(FATAL_ERROR "Undefined NA_IN_DATATYPE: ${NA_IN_DATATYPE}")
    endif()

    foreach (prm IN ITEMS ${prm_list})
      foreach (in_dt IN ITEMS ${dt_list})

        set (sfx "npe_mpe_${prm}_${in_dt}")

        if (prm STREQUAL "krnl")
          set (prm_min "${krnl_min}")
          set (prm_max "${krnl_max}")
        elseif (prm STREQUAL "strd")
          set (prm_min "${strd_min}")
          set (prm_max "${strd_max}")
        elseif (prm STREQUAL "dltn")
          set (prm_min "${dltn_min}")
          set (prm_max "${dltn_max}")
        else()
          message(FATAL_ERROR "Undefined prm: ${prm}")
        endif()

        ADD_MPE_PRM(${CONF} ${sfx} ${prm_min} ${prm_max})

      endforeach()
    endforeach()

  endif() # NA_TESTGROUP MPE_KRNL MPE_STRD MPE_DLTN

      ################################
      ### Control Unit Tests
  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "CU")
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
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME NPE_1
      FILES scr1/targets/simple-rom/nu/npe_regs/npe_regs.c
    )	  
  # ADD_NPE_SIMPLE_TEST(${CONF} npe_regs scr1/targets/simple-rom/nu/npe_regs/npe_regs.c)    	  
      ##########################################       
  	   ## NPE CMD DMA MPE Test
  	add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME NPE_CU_CMD_DMA_MPE
        FILES scr1/targets/simple-rom/nu/na_cu/cu_cmd_dma_mpe.c
      )	   	

    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME debug_dep_table
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_with_dep.c
      PREPCMD
        ${NA_RM_BIN_PATH}/main_na_cu ${NA_RM_KEYS} > ${RM_LOGFILE} && 
        ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} 
          > ${ConfigMPE_to_LUT_LOGFILE} &&
        ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || 
        exit 1
      SUBPROJECT_DEPS npe_rm:main_na_cu
      IRUN_FLAGS ${NA_RM_PLUSARGS}
    )
  endif() # NA_TESTGROUP CU
  
  
   ##########################################
      ## MPE Reg Test
  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "MPE")
    add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME MPE_1
        FILES scr1/targets/simple-rom/nu/mpe_regs/regs_mpe.c
    )	
  endif()
  
      ##########################################
      ## Direct Tests That Use Predefined Set Of MPE Configurations And Then Use Some VPE And PPE Functions
  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "MPE_CFG")
    foreach(label RANGE 25 48)  # Int16
      ADD_NPE_MPE_VPE_TEST(${CONF} NA_2_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT BITWISE)
    endforeach()
    foreach(label RANGE 49 72)  # Int8
      ADD_NPE_MPE_VPE_TEST(${CONF} NA_1_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT BITWISE)
    endforeach()

    foreach(label RANGE 1 24)
      ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label} main_mpe_direct_ex_MPE_CFG_${label} MAKE_TIGHT BITWISE)
      ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_VPE NO_TIGHT BITWISE)
    endforeach()
    foreach(label RANGE 25 72)
      ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label} main_mpe_direct_ex_MPE_CFG_${label} MAKE_TIGHT BITWISE)
    endforeach()
    foreach(label RANGE 1 48)
      ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP main_mpe_direct_ex_MPE_CFG_${label}_FP MAKE_TIGHT EPS)
      ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_VPE NO_TIGHT EPS)
    endforeach()
    #foreach(label RANGE 0 1)
    #  ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_batch main_npe_batch_${label} NO_TIGHT BITWISE)
    #endforeach()
    #ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_0_batch main_npe_batch_0 NO_TIGHT BITWISE)
    #ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_1_batch main_npe_batch_1 NO_TIGHT BITWISE)
    #ADD_NPE_MPE_VPE_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_2_batch main_npe_batch_2 NO_TIGHT BITWISE)

    ADD_NPE_MPE_VPE_RST_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_24_FP main_mpe_direct_ex_MPE_CFG_24_FP MAKE_TIGHT EPS)
      ##################################
      ## Direct Complex Tests On Important Cube Sizes
    foreach(label RANGE 49 72)
      ADD_NPE_COMPLEX_TEST(${CONF} NA_5_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT BITWISE)
    endforeach()
    foreach(label RANGE 25 48)
      ADD_NPE_COMPLEX_TEST(${CONF} NA_6_npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT BITWISE)
    endforeach()
    ###################################
    ## Some Other Direct Tests
    foreach(label RANGE 1 24)
      ADD_NPE_COMPLEX_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_WITH_PPE NO_TIGHT BITWISE)
    endforeach()
    foreach(label RANGE 1 48)
      ADD_NPE_COMPLEX_TEST(${CONF} npe_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE main_mpe_direct_ex_MPE_CFG_${label}_FP_WITH_PPE NO_TIGHT BITWISE) #BITWISE - Because it passes Yet
    endforeach()

    ADD_NPE_MPE_VPE_TEST(${CONF} npe_all_ex_IN_INT16 main_npe_all_ex_IN_INT16 MAKE_TIGHT BITWISE)

    ADD_NPE_COMPLEX_TEST_TIGHT(${CONF} npe_tight_complex main_npe_tight_complex MAKE_TIGHT BITWISE)
	ADD_NPE_COMPLEX_TEST_RST (${CONF} npe_mpe_direct_ex_MPE_CFG_24_WITH_PPE main_mpe_direct_ex_MPE_CFG_24_WITH_PPE NO_TIGHT BITWISE)
  endif() # NA_TESTGROUP MPE_CFG
      ###################################################################
      # Resnet-test 
      # ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE main_resnet_IN_FP16_LAYER0_MPE NO_TIGHT BITWISE) # LAYER0_MPE
      # ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN main_resnet_IN_FP16_LAYER0_MPE_BN NO_TIGHT BITWISE) # LAYER0_MPE_BN
      # ADD_NPE_MPE_VPE_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN_RELU main_resnet_IN_FP16_LAYER0_MPE_BN_RELU NO_TIGHT BITWISE) # LAYER0_MPE_BN_RELU
      # ADD_NPE_COMPLEX_TEST(${CONF} resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE main_resnet_IN_FP16_LAYER0_MPE_BN_RELU_PPE NO_TIGHT BITWISE) # LAYER0_MPE_BN_RELU_PPE
      ###################################################################

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY1")
    foreach(iter_start RANGE 0 13400 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP1_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP1_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY2")
    foreach(iter_start RANGE 0 10700 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP2_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP2_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY3")
    foreach(iter_start RANGE 0 8000 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP3_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP3_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY4")
    foreach(iter_start RANGE 0 35712 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP4_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP4_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY5")
    foreach(iter_start RANGE 0 68112 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP5_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP5_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY6")
    foreach(iter_start RANGE 0 138240 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP6_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP6_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MPE_GEOMETRY7")
    foreach(iter_start RANGE 0 32300 100)
      math(EXPR iter_end "${iter_start}+99")
      ADD_NPE_MPE_VPE_TEST(${CONF} mpe_cfg_auto_GROUP7_${iter_start}_${iter_end} main_mpe_auto_tests_GROUP7_${iter_start}_${iter_end} MAKE_TIGHT BITWISE)
    endforeach()
  endif()

  if(NA_TESTGROUP STREQUAL "MICRO_TESTS")
  SET (MICRO_TESTS ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/simple-rom/nu/mpe_micro)
    add_rumboot_target(
        CONFIGURATION ${CONF}
        NAME MICRO_TESTS
        FILES scr1/targets/simple-rom/nu/mpe_micro/mpe_micro.c
        CFLAGS -Wno-unused-variable
        PREPCMD 
            #cp ${MICRO_TESTS}/cmd.bin ${MICRO_TESTS}/cmd.bin.metrics -t . && 
            ${NA_RM_BIN_PATH}/main_mpe_batch_micro ${NA_RM_KEYS} > ${RM_LOGFILE} && 
            ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} 
            ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE} &&
            ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} &&
            ${CMAKE_SOURCE_DIR}/../units/rcm_lava_mpe/macro_comm/macro_microgen F || exit 1
        IRUN_FLAGS ${NA_RM_PLUSARGS}
        SUBPROJECT_DEPS npe_rm:main_mpe_batch_micro
    )
  endif()

  macro(ADD_NKBVS_FROM_BINARY_TEST CONF label c_program comparer)
    if("${comparer}" STREQUAL "WITH_PPE")
      set(COMPARER_OPT)
    else()
      set(COMPARER_OPT -DDONT_USE_PPE=1)
    endif()
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME nkbvs_${label}
      FILES scr1/targets/simple-rom/nu/coupled_with_rm/${c_program}
      CFLAGS ${COMPARER_OPT}
      PREPCMD 
        cp ${NKBVS_TEST_DIR}/${label}/*.bin* . &&
        ${PYTHON_EXECUTABLE} -B ${ConfigMPE_to_LUT} ${NA_TEST_num_iterations_file} ${NA_TEST_cfg_mpe_file} ${NA_TEST_mpe_cfg_lut_file} > ${ConfigMPE_to_LUT_LOGFILE} &&
        ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS}
        || exit 1
      IRUN_FLAGS ${NA_RM_PLUSARGS} 
    )  
  endmacro()

  if(NA_TESTGROUP STREQUAL "NKBVS")
    if(NOT DEFINED NKBVS_TEST_DIR)
      set(NKBVS_TEST_DIR /opt/lib_h31/LAVA_lib/nkbvs_tests_bins/conv2dplus)
    endif()
    ADD_NPE_MPE_VPE_TEST(${CONF} nkbvs_1 main_nkbvs_1 NOT_MAKE_TIGHT BITWISE)
    ADD_NPE_MPE_VPE_TEST(${CONF} nkbvs_2 main_nkbvs_2 NOT_MAKE_TIGHT BITWISE)
    ADD_NPE_MPE_VPE_TEST(${CONF} nkbvs_3 main_nkbvs_3 NOT_MAKE_TIGHT BITWISE)
    ADD_NKBVS_FROM_BINARY_TEST(${CONF} 4 coupled_loop_npe_long.c WO_PPE)
    #ADD_NPE_MPE_VPE_TEST(${CONF} nkbvs_4 main_nkbvs_4 NOT_MAKE_TIGHT BITWISE)
    ADD_NKBVS_FROM_BINARY_TEST(${CONF} 5 coupled_loop_npe_long.c WITH_PPE)
    #ADD_NPE_COMPLEX_TEST(${CONF} nkbvs_5 main_nkbvs_5 NOT_MAKE_TIGHT BITWISE)
    ADD_NKBVS_FROM_BINARY_TEST(${CONF} 6 coupled_loop_npe_long.c WITH_PPE)
    #ADD_NPE_COMPLEX_TEST(${CONF} nkbvs_6 main_nkbvs_6 NOT_MAKE_TIGHT BITWISE)
    ADD_NKBVS_FROM_BINARY_TEST(${CONF} 7 coupled_loop_npe_long.c WO_PPE)
    #ADD_NPE_MPE_VPE_TEST(${CONF} nkbvs_7 main_nkbvs_7 NOT_MAKE_TIGHT BITWISE)

    ADD_NKBVS_TEST(${CONF} 2d_deconv_1 /home/v.gordeev/na_arrays/nkbvs_2d_deconv_1 No 32 421 419)

    ADD_NKBVS_TEST(${CONF} depthwise_1 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_1 VPE_TraceMode_MPE_DW 32 416 408)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_deconv_2 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/nkbvs_2d_deconv_2 No 2 2 1 0 64 827 838)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_1_small /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_1_small VPE_TraceMode_MPE_DW 1 1 0 1 32 9 165)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_1 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_1 VPE_TraceMode_MPE_DW 1 1 0 1 32 421 419)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_2_small /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_2_small VPE_TraceMode_MPE_DW 2 2 1 0 64 2 99)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_2 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_2 VPE_TraceMode_MPE_DW 2 2 1 0 64 413 419)
    #ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_3_opt_small /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_3_opt_small VPE_TraceMode_MPE_DW 3 3 1 1 32 2 163)
    #ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_3_opt /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_3_opt VPE_TraceMode_MPE_DW 3 3 1 1 32 2 163)
    #ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_3_small /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_3_small VPE_TraceMode_MPE_DW 1 1 1 1 32 6 159)
    #ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_3 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_3 VPE_TraceMode_MPE_DW 1 1 1 1 32 6 159)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_7_small /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_7_small VPE_TraceMode_MPE_DW 1 1 0 3 32 16 171)
    ADD_NKBVS_DW_TRANSPOSE_TEST(${CONF} 2d_dw_deconv_7 /home/a.yarovikov/projects/rcm_lava_mpe/py/manual_tests/depthwise_deconvolution_7 VPE_TraceMode_MPE_DW 1 1 0 3 2048 427 421)
  endif()

  na_testsuite_add_vpe_tests(${CONF})
  na_testsuite_add_ppe_tests(${CONF})
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

  # Tests on VPE::InputConverters
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_18_op2_c1  main_vpe_18_op2_c1 )
  ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_16_op2_c2  main_vpe_16_op2_c2 )

  # Tests on VPE::NORM
  # Hm, Replaced By The Version With Round Mode Setting But Not Deleted From Here
  #ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_8_0_op0_norm       main_vpe_8_0_op0_norm      )
  #ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_14_0_op1_norm      main_vpe_14_0_op1_norm     )

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

  # Test on VPE ALU according to testplan with Vitia's targets
  foreach(coef IN ITEMS INT16 INT8)
    foreach(block IN ITEMS OP0 OP1)  
      ADD_VPE_COUPLED_TEST_LOOP_LONG(${CONF} vpe_alu_IN_INT32_COEF0_${coef}_COEF1_${coef}_COEF2_${coef}_OUT_INT16_${block} main_alu_IN_INT32_COEF0_${coef}_COEF1_${coef}_COEF2_${coef}_OUT_INT16_${block})
    endforeach()
  endforeach()
  
endmacro()

macro(na_testsuite_add_vpe_tests CONF)
  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "VPE_DMA_BASE")
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
    
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_29_power_IN_FP16_dma main_vpe_29_power_IN_FP16_dma 15) #VPE29
    ADD_VPE_COUPLED_TEST_LOOP_FORCE_WDMA_AXI_LEN(${CONF} vpe_29_power_IN_INT8_dma main_vpe_29_power_IN_INT8_dma 15) #VPE29
    
    
  endif() # NA_TESTGROUP VPE_DMA_BASE

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "VPE_DMA_BATCH")
    
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
                  CFLAGS -DDUT=${DUT_LETTER_QUOTED} -DFORCE_VPE_WDMA_EN=1
                  PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
                  IRUN_FLAGS ${NA_RM_PLUSARGS}
                  SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
                )
                add_rumboot_target(
                  CONFIGURATION ${CONF}
                  NAME vpe_batch_mode_with_element_${in}_${coef0}_${coef1}_${coef2}_${out}_${number_testcase}
                  FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
                  CFLAGS -DDUT=${DUT_LETTER_QUOTED} -DFORCE_VPE_WDMA_EN=1
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
          CFLAGS -DDUT=${DUT_LETTER_QUOTED} -DFORCE_VPE_WDMA_EN=1
          PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
          IRUN_FLAGS ${NA_RM_PLUSARGS}
          SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
        )
        add_rumboot_target(
          CONFIGURATION ${CONF}
          NAME vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
          FILES scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_long_batch_mode.c
          CFLAGS -DDUT=${DUT_LETTER_QUOTED} -DFORCE_VPE_WDMA_EN=1
          PREPCMD ${NA_RM_BIN_PATH}/main_vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase} ${NA_RM_KEYS} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1
          IRUN_FLAGS ${NA_RM_PLUSARGS}
          SUBPROJECT_DEPS npe_rm:main_vpe_batch_mode_with_element_IN_FP16_COEF0_FP16_COEF1_FP16_COEF2_FP16_${out}_${number_testcase}
        )
      endforeach()
    endforeach()
  endif() # NA_TESTGROUP VPE_DMA_BATCH

endmacro()

macro(ADD_PPE_RNDM CONF sfx rm_bin_name RM_CFG_PARAM)

  if(NOT DEFINED NU_SEED)
    set(NU_SEED 1)
  endif()

  if(NOT DEFINED NU_IT_NMB OR NU_IT_NMB EQUAL 1)
    set(NU_IT_NMB 32)
  endif()

  if ("${rm_bin_name}" MATCHES "main_vpe_ppe")
    set (src_c "scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_vpe_ppe_long.c")
    set (DataSrc "VPEtoPPE")
  elseif("${rm_bin_name}" MATCHES "main_ppe")
    set (src_c "scr1/targets/simple-rom/nu/coupled_with_rm/coupled_loop_ppe_long.c")
    set (DataSrc "MEMtoPPE")
  else()
    message(FATAL_ERROR "Undefined source file name ${rm_bin_name}")
  endif()

  if ("${rm_bin_name}" MATCHES "BOX")
    set (LBS "BOX")
  elseif ("${rm_bin_name}" MATCHES "SPL")
    set (LBS "LIN_SPL")
  else()
    set (LBS "LIN")
  endif()

  set (name "${rm_bin_name}_${sfx}_${NU_SEED}")

  add_rumboot_target(
    CONFIGURATION ${CONF}
    NAME ${name}
    FILES ${src_c}

    PREPCMD ${NA_RM_BIN_PATH}/${rm_bin_name} ${NA_RM_KEYS} --seed ${NU_SEED} --it_nmb ${NU_IT_NMB} ${RM_CFG_PARAM} > ${RM_LOGFILE} && ${MERGE_BINS_4_LONG_SCRIPT} ${NA_RM_KEYS} || exit 1

    CFLAGS -D${LBS} -D${DataSrc} -DDUT=${DUT_LETTER_QUOTED}

    IRUN_FLAGS ${NA_RM_PLUSARGS}

    SUBPROJECT_DEPS npe_rm:${rm_bin_name}
  )

endmacro()

macro(na_testsuite_add_ppe_tests CONF)

  if(NOT DEFINED NU_SEED)
    set(NU_SEED 1)
  endif()

  if(NOT DEFINED NU_IT_NMB OR NU_IT_NMB EQUAL 1)
    set(NU_IT_NMB 32)
  endif()

  set (i8_max   "--set_meth 1 --pool_meth 1 --data_type 0 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (i16_max  "--set_meth 1 --pool_meth 1 --data_type 1 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (fp16_max "--set_meth 1 --pool_meth 1 --data_type 2 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")

  set (i8_min   "--set_meth 1 --pool_meth 2 --data_type 0 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (i16_min  "--set_meth 1 --pool_meth 2 --data_type 1 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (fp16_min "--set_meth 1 --pool_meth 2 --data_type 2 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")

  set (i8_avg   "--set_meth 1 --pool_meth 0 --data_type 0 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (i16_avg  "--set_meth 1 --pool_meth 0 --data_type 1 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")
  set (fp16_avg "--set_meth 1 --pool_meth 0 --data_type 2 --h_min 1 --h_max 256 --w_min 1 --w_max 256 --c_min 32 --c_max 32")

  set (w_min_max
    1   128
    128 128
    129 129
    128 256
    255 255
    256 256
    256 288
  )

  set (k_min_max
    1   1
    1   16
    15  15
    16  16
    16  32
    32  48
    48  64
    64  80
    80  96
    96  112
    112 128
    128 128
    128 256
    256 288
  )

  set (h_min_max
    1 1 
    2 2 
    2 16
  )

  list(LENGTH w_min_max w_ln)
  list(LENGTH k_min_max k_ln)
  list(LENGTH h_min_max h_ln)

  math (EXPR w_prs "${w_ln}/2-1")
  math (EXPR k_prs "${k_ln}/2-1")
  math (EXPR h_prs "${h_ln}/2-1")

  set (vpe_ppe_wkh_var)

  foreach (wp RANGE 0 ${w_prs})
    math (EXPR w0 "${wp}*2")
    list(GET w_min_max ${w0} w_min)

    math (EXPR w1 "${w0}+1")
    list(GET w_min_max ${w1} w_max)

    foreach(kp RANGE 0 ${k_prs})
      math (EXPR k0 "${kp}*2")
      list(GET k_min_max ${k0} k_min)

      math (EXPR k1 "${k0}+1")
      list(GET k_min_max ${k1} k_max)

      foreach(hp RANGE 0 ${h_prs})
        math (EXPR h0 "${hp}*2")
        list(GET h_min_max ${h0} h_min)

        math (EXPR h1 "${h0}+1")
        list(GET h_min_max ${h1} h_max)

        set (wkh_var "w${w_min}_${w_max}_k${k_min}_${k_max}_h${h_min}_${h_max}")

        list(APPEND vpe_ppe_wkh_var ${wkh_var})

      endforeach()
    endforeach()
  endforeach()

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "PPE_LA")
    add_rumboot_target(
      CONFIGURATION ${CONF}
      NAME PPE_1
      FILES scr1/targets/simple-rom/nu/ppe_regs/regs_ppe.c
    )

    ADD_PPE_RST_TESTS(${CONF} ppe_fp16_max_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_max})
  endif() # NA_TESTGROUP PPE_LA

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "PPE_BASE")

    ADD_PPE_TESTS(${CONF} ppe_i8_max_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_max})
    ADD_PPE_TESTS(${CONF} ppe_i16_max_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_max})
    ADD_PPE_TESTS(${CONF} ppe_fp16_max_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_max})

    ADD_PPE_TESTS(${CONF} ppe_i8_min_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_min})
    ADD_PPE_TESTS(${CONF} ppe_i16_min_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_min})
    ADD_PPE_TESTS(${CONF} ppe_fp16_min_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_min})

    ADD_PPE_TESTS(${CONF} ppe_i8_avg_ml main_ppe_IN_INT8 NotShowPerf MEMtoPPE LIN ${i8_avg})
    ADD_PPE_TESTS(${CONF} ppe_i16_avg_ml main_ppe_IN_INT16 NotShowPerf MEMtoPPE LIN ${i16_avg})
    ADD_PPE_TESTS(${CONF} ppe_fp16_avg_ml main_ppe_IN_FP16 NotShowPerf MEMtoPPE LIN ${fp16_avg})

  endif() # NA_TESTGROUP PPE_BASE

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "PPE_PY")
    ADD_PPE_PY_TESTS(${CONF})
  endif()

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "VPE_PPE")
    ADD_VPE_PPE_WKH_COMB(${CONF} vpe_ppe_wkh_var 0)
  endif() # NA_TESTGROUP VPE_PPE

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "PPE_RNDM")
    set (hwc_min 1)
    set (hwc_max 512)

    set (h_min_max "--h_min ${hwc_min} --h_max ${hwc_max}")
    set (w_min_max "--w_min ${hwc_min} --w_max ${hwc_max}")
    set (c_min_max "--c_min ${hwc_min} --c_max ${hwc_max}")

    set (RM_CFG_PARAM "h_min_max w_min_max c_min_max")

    set (sfx "${hwc_min}_${hwc_max}")

    foreach(in_macro IN ITEMS IN_INT8 IN_INT16 IN_FP16)
      ADD_PPE_RNDM(${CONF} ${sfx} main_ppe_${in_macro} RM_CFG_PARAM)
      ADD_PPE_RNDM(${CONF} ${sfx} main_ppe_${in_macro}_SPL RM_CFG_PARAM)
      ADD_PPE_RNDM(${CONF} ${sfx} main_vpe_ppe_${in_macro} RM_CFG_PARAM)
      ADD_PPE_RNDM(${CONF} ${sfx} main_vpe_ppe_${in_macro}_BOX RM_CFG_PARAM)
    endforeach()

  endif() # NA_TESTGROUP PPE_RNDM

  if(NOT DEFINED NA_TESTGROUP OR "${NA_TESTGROUP}" STREQUAL "PPE_KRNL")
    set (hwc_min 1)
    set (hwc_max 512)

    set (h_min_max "--h_min ${hwc_min} --h_max ${hwc_max}")
    set (w_min_max "--w_min ${hwc_min} --w_max ${hwc_max}")
    set (c_min_max "--c_min ${hwc_min} --c_max ${hwc_max}")

    foreach(Kh RANGE 1 8)
      foreach(Kw RANGE 1 8)

        set (RM_CFG_PARAM "${h_min_max} ${w_min_max} ${c_min_max} --set_Kh 1 --Kh ${Kh} --set_Kw 1 --Kw ${Kw}")

        set (sfx "${hwc_min}_${hwc_max}_Kh_${Kh}_Kw_${Kw}")

        foreach(in_macro IN ITEMS IN_INT8 IN_INT16 IN_FP16)
          ADD_PPE_RNDM(${CONF} ${sfx} main_ppe_${in_macro} RM_CFG_PARAM)
          ADD_PPE_RNDM(${CONF} ${sfx} main_ppe_${in_macro}_SPL RM_CFG_PARAM)
          ADD_PPE_RNDM(${CONF} ${sfx} main_vpe_ppe_${in_macro} RM_CFG_PARAM)
          ADD_PPE_RNDM(${CONF} ${sfx} main_vpe_ppe_${in_macro}_BOX RM_CFG_PARAM)
        endforeach()

      endforeach()
    endforeach()

  endif() # NA_TESTGROUP PPE_RNDM

endmacro()
