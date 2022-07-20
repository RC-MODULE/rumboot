set(RUMBOOT_ONEVALUE_ARGS SNAPSHOT LDS PREFIX NAME BOOTROM CHECKPOINTS RESTORE TIMEOUT_CTEST VARIABLE CONFIGURATION APPEND COMBOIMAGE OPTIMIZE)
set(RUMBOOT_MULVALUE_ARGS OBJCOPY_FLAGS FILES IRUN_FLAGS CCFLAGS CFLAGS ASFLAGS CXXFLAGS TESTGROUP LDFLAGS PREPCMD CHECKCMD FEATURES TIMEOUT LOAD DEPENDS PACKIMAGE_FLAGS DUMPFLAGS SUBPROJECT_DEPS)

find_program(PYTHON_EXECUTABLE
  NAMES python3.9 python3.8 python3.7 python3.6 python3 python
  HINTS /usr/local/bin
)

cmake_policy(SET CMP0114 NEW)

if (NOT PYTHON_EXECUTABLE)
  message(FATAL_ERROR "Failed to find useable python3")
else()
  message(STATUS "Found python3: ${PYTHON_EXECUTABLE}")
endif()

# Broken on RHEL
#find_package(PythonInterp 3.0 REQUIRED)

macro(rumboot_add_configuration name)
  message(STATUS "Adding configuration ${name}")
  set(options DEFAULT)
  set(oneValueArgs   ${RUMBOOT_ONEVALUE_ARGS})
  set(multiValueArgs ${RUMBOOT_MULVALUE_ARGS})

  cmake_parse_arguments(CONFIGURATION_${name} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  LIST(APPEND RUMBOOT_CONFIGURATIONS ${name})
  if (CONFIGURATION_${name}_DEFAULT)
    set(RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION ${name})
    message(STATUS "Default configuration set to: ${RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION}")
  endif()

  #If configuration doesn't provide optimization level, use -Os
  if (NOT CONFIGURATION_${name}_OPTIMIZE)
    set(CONFIGURATION_${name}_OPTIMIZE -Os)
  endif()

  if (CONFIGURATION_${name}_CONFIGURATION)
    foreach(c ${oneValueArgs} ${multiValueArgs})
      config_load_param(CONFIGURATION_${name} ${CONFIGURATION_${name}_CONFIGURATION} ${c})
    endforeach()
  endif()

  list (FIND CONFIGURATION_${name}_FEATURES "COVERAGE" _index)
  if (${_index} GREATER -1 AND RUMBOOT_COVERAGE)
    message(STATUS "Enabling coverage instrumentation for: ${name}")
    set(CONFIGURATION_${name}_CFLAGS
      ${CONFIGURATION_${name}_CFLAGS} ${RUMBOOT_COVER_CFLAGS})
    set(CONFIGURATION_${name}_LDFLAGS
    ${CONFIGURATION_${name}_LDFLAGS} ${RUMBOOT_COVER_LFLAGS})

  endif()

endmacro()

macro(config_load_param prefix conf param)
  if (NOT ${prefix}_${param})
    set(${prefix}_${param} ${CONFIGURATION_${conf}_${param}})
  endif()
endmacro()

macro(config_load_param_append prefix conf param)
    set(${prefix}_${param} ${${prefix}_${param}} ${CONFIGURATION_${conf}_${param}})
endmacro()

macro(generate_product_name outvar name)
  set(${outvar} rumboot-${RUMBOOT_PLATFORM}-${RUMBOOT_BUILD_TYPE}-${name})
endmacro()

macro(fetch_sources dir)
  file(GLOB RUMBOOT_TARGETS_C ${dir}/*.c)
  file(GLOB RUMBOOT_TARGETS_S ${dir}/*.S)
  file(GLOB RUMBOOT_TARGETS_LUA ${dir}/*.lua)
endmacro()

macro(add_rumboot_target_dir dir)

  cmake_parse_arguments(TMPD "" "${RUMBOOT_ONEVALUE_ARGS}" "${RUMBOOT_MULVALUE_ARGS}" ${ARGN})
  string (REPLACE "FILES;${TMPD_FILES}" "" OUT "${ARGN}")

  foreach(ddir ${RUMBOOT_PLATFORM_COMMON_DIR}/${dir} ${RUMBOOT_PLATFORM_TARGET_DIR}/${dir})
    fetch_sources(${ddir})
    foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S} ${RUMBOOT_TARGETS_LUA})
    add_rumboot_target(
        ${OUT}
        FILES ${target} ${TMPD_FILES}
    )
    endforeach()
  endforeach()
endmacro()

macro(locate_source_file list f)
  #Extra seat-belts
  if (EXISTS ${RUMBOOT_PLATFORM_TARGET_DIR}/${f} AND
      EXISTS ${RUMBOOT_PLATFORM_COMMON_DIR}/${f})
      message(FATAL_ERROR "FATAL: Files with the same name: ${RUMBOOT_PLATFORM_TARGET_DIR}/${f} and ${RUMBOOT_PLATFORM_COMMON_DIR}/${f}")
    endif()

  if (EXISTS ${RUMBOOT_PLATFORM_TARGET_DIR}/${f})
        LIST(APPEND ${list} ${RUMBOOT_PLATFORM_TARGET_DIR}/${f})
  elseif(EXISTS ${RUMBOOT_PLATFORM_COMMON_DIR}/${f})
        LIST(APPEND ${list} ${RUMBOOT_PLATFORM_COMMON_DIR}/${f})
  elseif(EXISTS ${f})
        LIST(APPEND ${list} ${f})
  else()
    message(FATAL_ERROR "Can't find file ${f} for target ${TARGET_NAME}")
  endif()
endmacro()

function(expand_source_list outlist inlist)
  set(__list)
  foreach(f ${${inlist}})
    locate_source_file(__list ${f})
  endforeach()
  set(${outlist} "${__list}" PARENT_SCOPE)
endfunction()

function(extract_labels_from_source product)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/extract-labels.py ${ARGN}
    RESULT_VARIABLE RET
    OUTPUT_VARIABLE OUT
    )
  if (RET STREQUAL "0")
    get_test_property(${product} LABELS labels)
    SET_TESTS_PROPERTIES(${product} PROPERTIES LABELS "${labels};${OUT}")    
  endif()
endfunction()

function(feature_check feature featurelist outvar)
  list (FIND ${featurelist} "${feature}" _index)
  if (_index GREATER -1)
    set(${outvar} Yes PARENT_SCOPE)
  else()
    set(${outvar} No PARENT_SCOPE)
  endif()
endfunction()

function(expand_external_dependency var outvar)
  string(REPLACE ":" ";" var "${${var}}")
  list(LENGTH var _len)
  if (_len EQUAL "2")
    list(GET var 0 _eproject)
    list(GET var 1 _estub)
    set(_ext_rumboot    rumboot-${_eproject}-${RUMBOOT_BUILD_TYPE})        
    set(_ext_cmd    ${_ext_rumboot}-${name})
    set(_ext_target ${_ext_rumboot}-${_estub}.all)
    set(_ext_path ${CMAKE_BINARY_DIR}/${_ext_rumboot}/${_ext_rumboot}-${_estub})

    set(HDL_EXTRA_REBUILD_CMD 
        "${HDL_EXTRA_REBUILD_CMD}
        make rumboot-${_eproject}-${RUMBOOT_BUILD_TYPE}-configure
        make -C ${CMAKE_BINARY_DIR}/rumboot-${_eproject}-${RUMBOOT_BUILD_TYPE} ${_ext_target}
        " PARENT_SCOPE)
    set(${outvar} "${_ext_path}" PARENT_SCOPE)
  endif()  
endfunction()


function(rumboot_add_external_project directory)
#Dummy
endfunction()

if (CMAKE_VERILOG_RULES_DIR)
  get_filename_component(_dir ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
  include(${_dir}/integration.cmake)
  return()
endif()

##############################################################################

function(rumboot_add_external_project directory)
      ## Download The External If it is not inited
      set(_ext_opts)
      set(options NOCONFIGURE NOBUILD)
      set(oneValueArgs )
      set(multiValueArgs CONFIGURE_FLAGS)
      cmake_parse_arguments(EXTPRJ "${options}" "${oneValueArgs}"
                            "${multiValueArgs}" ${ARGN} )

      execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule status ${directory}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_SUBMOD_STATUS_OUTPUT
    )
    if(GIT_SUBMOD_STATUS_OUTPUT MATCHES "^-")
      execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init ${directory}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      )
    endif()

    get_filename_component(_name ${directory} NAME_WE)

    if (EXTPRJ_NOCONFIGURE)
      set(EXTPRJ_NOBUILD True)
      set(_ext_opts ${_ext_opts}
        CONFIGURE_COMMAND echo "${_name} needs no configure step"
        )
    else()
      set(_ext_opts ${_ext_opts}
        CONFIGURE_COMMAND  ${CMAKE_COMMAND} "${PROJECT_SOURCE_DIR}/${directory}" -G ${CMAKE_GENERATOR} ${EXTPRJ_CONFIGURE_FLAGS}
      )
    endif()

    if (EXTPRJ_NOBUILD)
      set(_ext_opts ${_ext_opts}
        BUILD_COMMAND echo "${_name} needs no build step"
      )
    else()
      set(_ext_opts ${_ext_opts}
        BUILD_COMMAND "$(MAKE)" -C "${CMAKE_BINARY_DIR}/${_name}"
      )
    endif()

    ExternalProject_Add(${_name}
      STEP_TARGETS      configure
      SOURCE_DIR        "${CMAKE_SOURCE_DIR}/${directory}"
      BINARY_DIR        "${CMAKE_BINARY_DIR}/${_name}"
      ${_ext_opts}
      INSTALL_COMMAND   ""
    )
    
endfunction()


function(add_source SRC)
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/${SRC}" PARENT_SCOPE)
endfunction()

function(add_boot_source KEY SRC)
    if (${${KEY}})
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/src/bootsrc/${SRC}" PARENT_SCOPE)
    endif()
endfunction()


macro(generate_stuff_for_target product)

  if (NOT RUMBOOT_PLATFORM STREQUAL "native")
    set(PACKIMAGE_EXT ".bin")
  endif()
  list (FIND TARGET_FEATURES "PACKIMAGE" _index)
  if (${_index} EQUAL -1)
    set(packimage_cmd)
  elseif (NOT TARGET_PACKIMAGE_FLAGS)
    set(packimage_cmd ${PYTHON_EXECUTABLE} ${RUMBOOT_PACKIMAGE} -f ${product}${PACKIMAGE_EXT} -C)
  else()
    set(packimage_cmd ${PYTHON_EXECUTABLE} ${RUMBOOT_PACKIMAGE} -f ${product}${PACKIMAGE_EXT} ${TARGET_PACKIMAGE_FLAGS})
  endif()

  list (FIND TARGET_FEATURES "CPACK" _index)
  if (NOT ${_index} EQUAL -1)
      #Package it!
      install(TARGETS ${product} RUNTIME DESTINATION rumboot/binaries)
  endif()

  set(_outfiles ${product})
  if (NOT RUMBOOT_PLATFORM STREQUAL "native")
      list (FIND TARGET_FEATURES "ROMAPIGEN" _romapigen)
      if (NOT ${_romapigen} EQUAL -1)
          add_custom_command(
            OUTPUT ${product}.h
            COMMAND echo '/* Automatically generated header for ${RUMBOOT_PLATFORM} from ${GIT_REVISION} */' > ${product}.h
            COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/romapigen.py ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}/${product} ${product} >> ${product}.h
            COMMENT "Generating ROM API header: ${product}.h"
            DEPENDS ${product}
          )
          install(FILES ${CMAKE_BINARY_DIR}/${product}.h DESTINATION rumboot/include)
          list(APPEND _outfiles ${product}.h)
      endif()

      add_custom_command(
        OUTPUT ${product}.dmp
        COMMAND ${CROSS_COMPILE}-objdump -D ${TARGET_DUMPFLAGS} ${CMAKE_DUMP_FLAGS} ${product} > ${product}.dmp
        COMMENT "Generating disassembly listing: ${product}.dmp"
        DEPENDS ${product}
      )
      list(APPEND _outfiles ${product}.dmp)

      add_custom_command(
        OUTPUT ${product}.bin
        COMMAND ${CROSS_COMPILE}-objcopy ${CMAKE_OBJCOPY_FLAGS} ${TARGET_OBJCOPY_FLAGS} -O binary ${product} ${product}.bin
        COMMENT "Generating binary image ${product}.bin"
        DEPENDS ${product}
      )
      list(APPEND _outfiles ${product}.bin)

      if (TARGET_APPEND)
      message(${product}.bin)
        add_custom_command(
          APPEND
          OUTPUT ${product}.bin
          COMMAND ${PYTHON_EXECUTABLE} ${RUMBOOT_COMBINE} -a ini -i ${product}.bin -i ${TARGET_APPEND} -o ${product}.bin
          COMMENT "Appending ${TARGET_APPEND} to ${product}"
        )
      endif()

      if (packimage_cmd)
        add_custom_command(
          APPEND
          OUTPUT ${product}.bin
          COMMAND ${packimage_cmd}
          COMMENT "Writing header checksums to ${product}"
        )
      endif()


      if (NOT ${_index} EQUAL -1)
        install(FILES ${CMAKE_BINARY_DIR}/${product}.dmp DESTINATION rumboot/binaries)
        install(FILES ${CMAKE_BINARY_DIR}/${product}.bin DESTINATION rumboot/binaries)
        install(FILES ${CMAKE_BINARY_DIR}/${product}.map DESTINATION rumboot/binaries)
      endif()
  else()
    add_custom_command(
      TARGET ${product}
      COMMAND ${packimage_cmd}
      COMMENT "Adding image header to ${product}"
    )
  endif()

  add_custom_target(
    ${product}.all ALL
    DEPENDS ${_outfiles}
  )    

  add_dependencies(${product}.all utils)

  if (NOT ${TARGET_BOOTROM} STREQUAL "" AND NOT ${TARGET_BOOTROM} MATCHES ":")
    add_dependencies(${product}.all ${bootrom})
  endif()

  populate_dependencies(${product}.all)
endmacro()


set(RUMBOOT_COVER_CFLAGS -DRUMBOOT_COVERAGE=1 -fprofile-arcs -ftest-coverage)
set(RUMBOOT_COVER_LFLAGS --coverage)


function(expand_target_load loadlist outvar listvar)
  set(v 0)
  set(loadflags "")
  set(seen)
  set(plus)
  foreach(l ${${loadlist}})
    if (v EQUAL 0)
      if (plus)
        list(APPEND seen ${plus})
      endif()
      set(plus ${l})
      set(v 1)
      list (FIND seen "${plus}" _index)
      #If we've seen this plusarg before, skip
      if (NOT ${_index} GREATER -1)
        set("${outvar}_${plus}")
        set("${outvar}_${plus}" PARENT_SCOPE)
        endif()
    else()
      if (NOT ${_index} GREATER -1)
        set(trgs ${l})
        string(REPLACE "," ";" trgs "${trgs}")
        foreach(trg ${trgs})
          generate_product_name(tproduct ${trg})
          if(TARGET ${tproduct})
            list(APPEND "${outvar}_${plus}" ${tproduct})
          else()
            list(APPEND "${outvar}_${plus}" ${trg})
          endif()
        endforeach()
        set(v 0)
      endif()
    endif()
  endforeach()

  #Left-over
  list(APPEND seen ${plus})

  #Prepare overall list
  foreach(s ${seen})
      list(APPEND ${listvar} ${outvar}_${s})
      set(${outvar}_${s} "${${outvar}_${s}}" PARENT_SCOPE)
  endforeach()
  if (${listvar})
    list(REMOVE_DUPLICATES ${listvar})
  endif()
  set(${listvar} "${${listvar}}" PARENT_SCOPE)
endfunction()

macro(populate_dependencies target)
    #Filter out dependencies
    foreach(dep ${TARGET_DEPENDS})
      generate_product_name(dep ${dep})
      add_dependencies(${target} ${dep}.all)
    endforeach()
    #Process TARGET_LOAD dependencies
    expand_target_load(TARGET_LOAD TARGET_LOAD TARGET_LOAD_LIST)
    foreach(mem ${TARGET_LOAD_LIST})
      foreach(bin ${${mem}})
        if(TARGET ${bin})
          add_dependencies(${target} ${bin}.all)
        endif()
      endforeach()
    endforeach()
endmacro()


function(add_rumboot_test target name)
  generate_product_name(product ${target})
  add_test(${name} ${product} ${ARGN})
  #SET_TESTS_PROPERTIES(${name} PROPERTIES TIMEOUT "15")
endfunction()

function(add_rumboot_target)
  set(options )

  set(oneValueArgs   ${RUMBOOT_ONEVALUE_ARGS})
  set(multiValueArgs ${RUMBOOT_MULVALUE_ARGS})

  cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if (NOT TARGET_CONFIGURATION)
    set(TARGET_CONFIGURATION ${RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION})
  endif()

  foreach(c ${oneValueArgs})
    config_load_param(TARGET ${TARGET_CONFIGURATION} ${c})
  endforeach()

  foreach(c ${multiValueArgs})
    config_load_param_append(TARGET ${TARGET_CONFIGURATION} ${c})
  endforeach()

  if (NOT TARGET_NAME)
    list(GET TARGET_FILES 0 TARGET_NAME)
    GET_FILENAME_COMPONENT(TARGET_NAME ${TARGET_NAME} NAME_WE)
    endif()

  list (FIND TARGET_FEATURES "NOCODE" _index2)
  if ((NOT ${_index2} GREATER -1) AND (NOT TARGET_FILES))
    message(FATAL_ERROR "add_rumboot_target() requires TARGET_FILES to contain at least one file")
  endif()

  generate_product_name(product ${TARGET_PREFIX}-${TARGET_NAME})
  generate_product_name(bootrom ${TARGET_BOOTROM}.all)
  if (TARGET_VARIABLE)
    set(${TARGET_VARIABLE} ${PROJECT_BINARY_DIR}/${product} PARENT_SCOPE)
  endif()

  set(name rumboot-${TARGET_SNAPSHOT}-${product})

  if (${name})
    message(STATUS "NOT Adding rumboot target: ${name} - already exists")
    return()
  endif()

  if (TARGET_COMBOIMAGE)
    message("Generating combo image ${product}.bin from plusarg ${TARGET_COMBOIMAGE}")
    expand_target_load(TARGET_LOAD TARGET_LOAD TARGET_LOAD_LIST)
    set(_deps)
    list(APPEND _cmds COMMAND ${PYTHON_EXECUTABLE} ${RUMBOOT_COMBINE} -a 1 -o ${product}.bin)
    foreach(f ${TARGET_LOAD_${TARGET_COMBOIMAGE}})
      list(APPEND _deps ${f}.bin)
      list(APPEND _cmds -i ${f}.bin)
    endforeach()
    
    #string(REPLACE ";" " " _deps "${_deps}")
    #string(REPLACE ";" " " _cmds "${_cmds}")
    add_custom_command(OUTPUT ${product}.bin
      DEPENDS ${_deps}
      ${_cmds}
      VERBATIM
    )
    add_custom_target(${product}.all ALL DEPENDS ${product}.bin)
    return()
  endif()

  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "LPROBE" _index)
  if (${_index} GREATER -1 OR ${_index2} GREATER -1)
    #Lprobe scripts compile nothing. Provide a dummy
    #target and return
    add_custom_target(${product}.all ALL)
    if (NOT "${bootrom}" MATCHES ":")
      add_dependencies(${product}.all ${bootrom})
    endif()
    populate_dependencies(${product}.all)
    return()
  endif()

  set(EXTRA_FILES)

  set(${name} TRUE PARENT_SCOPE)

  expand_source_list(trg TARGET_FILES)

  foreach(f ${trg})
    GET_FILENAME_COMPONENT(ext ${f} EXT)
    if (ext STREQUAL ".lua")
      add_custom_command(
        OUTPUT ${product}.c
        DEPENDS ${f}
        COMMAND ${LUA} -e 'package.path=package.path.."\;${CMAKE_SOURCE_DIR}/lua2c/lib/?.lua"'
                ${CMAKE_SOURCE_DIR}/lua2c/lua2c.lua
                ${f}
                > ${product}.c
        COMMENT "Generating C code from ${f}"
      )
    list(REMOVE_ITEM trg ${f})
    list(APPEND trg ${product}.c)
    endif()
  endforeach()

  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "BANNER" _need_banner)
  if (${_need_banner} GREATER -1)
    list(APPEND trg ${CMAKE_SOURCE_DIR}/src/banner.c)
  endif()


  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "NOLIBS" _index)
  if (${_index} GREATER -1)
    add_executable(${product} ${trg})
  else()
    add_executable(${product} ${trg} $<TARGET_OBJECTS:rumboot-${TARGET_CONFIGURATION}>)
  endif()

  # Old cmake doesn't handle these well
  target_compile_options(${product} PUBLIC
    $<$<COMPILE_LANGUAGE:C>:${TARGET_CFLAGS} ${TARGET_CCFLAGS} ${TARGET_OPTIMIZE} -DRUMBOOT_PRODUCT_NAME=\"${product}\">
    $<$<COMPILE_LANGUAGE:CXX>:${TARGET_CFLAGS} ${TARGET_CXXFLAGS} ${TARGET_OPTIMIZE} -DRUMBOOT_PRODUCT_NAME=\"${product}\">
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp ${TARGET_CFLAGS} ${TARGET_ASFLAGS} ${TARGET_OPTIMIZE} -DRUMBOOT_PRODUCT_NAME=\"${product}\">
  )

  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "LUA" _index)
  if (${_index} GREATER -1)
    target_link_libraries(${product} lua)
  endif()

  if (NOT TARGET_LDS)
    set(TARGET_LDS ${RUMBOOT_PLATFORM_DEFAULT_LDS})
  else()
    set(TARGET_LDS ${CMAKE_SOURCE_DIR}/lds/${TARGET_LDS})
  endif()

  if (TARGET_LDS)
    if (EXISTS ${TARGET_LDS})
      set(ldf -Wl,-T${TARGET_LDS} -L${CMAKE_SOURCE_DIR}/lds)
    else()
      message(FATAL_ERROR "Target ${product} is missing required file: ${TARGET_LDS}. Did you commit it?")
    endif()
  else()
    set(ldf )
  endif()

  if (ldf)
    set_target_properties(${product} PROPERTIES LINK_DEPENDS ${TARGET_LDS})
  endif()

  target_link_libraries(${product} ${TARGET_LDFLAGS} ${ldf} -Wl,-Map,${product}.map)

  if (TARGET_APPEND)
    generate_product_name(tproduct ${TARGET_APPEND})
    if(NOT TARGET ${tproduct})
      locate_source_file(_append_list ${TARGET_APPEND})
      set(TARGET_APPEND ${_append_list})
    else()
      set(TARGET_APPEND ${PROJECT_BINARY_DIR}/${tproduct}.bin)
      add_dependencies(${product} ${tproduct}.all)
    endif()
  endif()

  if (NOT EXISTS ${CMAKE_BINARY_DIR}/.stamps/)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/.stamps/)
  endif()
  file(WRITE ${CMAKE_BINARY_DIR}/.stamps/${product} X)

  generate_stuff_for_target(${product})
  rumboot_platform_generate_stuff_for_taget(${product})

  foreach(dep ${TARGET_SUBPROJECT_DEPS})
    string(REPLACE ":" ";" dep ${dep})
    list(GET dep 0 _project)
    list(GET dep 1 _target)

    ExternalProject_Get_Property(${_project} BINARY_DIR)
    add_custom_command(
      TARGET ${product}.all
      COMMAND pwd && $(MAKE) ${_target}
      WORKING_DIRECTORY ${BINARY_DIR}
      COMMENT "(Re)building sub-project dependency ${_target}"
    )

    add_dependencies(${product}.all ${_project}-configure)

    endforeach()


  list (FIND TARGET_FEATURES "STUB" _index)


  list (FIND TARGET_FEATURES "STUB" _stub)
  if (NOT RUMBOOT_DISABLE_TESTING AND NOT ${_stub} GREATER -1 AND NOT (${RUMBOOT_PLATFORM} MATCHES "native" AND NOT CROSS_COMPILE))
    set(_plusargs "-A")
    expand_target_load(TARGET_LOAD TARGET_LOAD TARGET_LOAD_LIST)
    foreach(mem ${TARGET_LOAD_LIST})
    set(_load )
    foreach(f ${${mem}})
        if (f STREQUAL "SELF")
          list(APPEND _load ${PROJECT_BINARY_DIR}/${product}.bin)
        else()
          foreach(candidate ${PROJECT_BINARY_DIR}/${f}.bin ${PROJECT_BINARY_DIR}/${f} ${f})
            if (EXISTS ${candidate})
              list(APPEND _load ${candidate})
              break()
            endif()        
          endforeach()
        endif()
    endforeach()

    string(REPLACE ";" "," _load "${_load}")
    string(REPLACE "TARGET_LOAD_" "" mem ${mem})
    set(_plusargs "${_plusargs};+${mem}=${_load}")      
    endforeach()
  
    if (NOT ${RUMBOOT_PLATFORM} MATCHES "native")
      set(_suffix ".bin")
    else()
      set(_suffix "")
    endif()
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/runners)

    foreach(arg ${TARGET_IRUN_FLAGS})
      if (RUMBOOT_NEEDS_XRUN)
        set(_plusargs "${_plusargs};-A;${arg}")
      else()
        set(_plusargs "${_plusargs};${arg}")
      endif()
    endforeach()

    string(REPLACE ";" " " _plusargs "${_plusargs}")

    if (NOT "${TARGET_PREPCMD}" STREQUAL "")
      string(REPLACE ";" " " TARGET_PREPCMD "${TARGET_PREPCMD}")
    endif()

    if (RUMBOOT_NEEDS_XRUN)
      set(RUMBOOT_NEEDS_XRUN "YES") #Make comparison's in bash easier
    endif()
    configure_file(${CMAKE_SOURCE_DIR}/cmake/test-runner.sh.in ${PROJECT_BINARY_DIR}/runners/${product})
    
    if(CHMOD_PROG)
      execute_process(COMMAND ${CHMOD_PROG} +x ${PROJECT_BINARY_DIR}/runners/${product})
    endif()
    add_test(NAME ${product} COMMAND ${PROJECT_BINARY_DIR}/runners/${product})
    feature_check("EXTRACT_LABELS" TARGET_FEATURES _scan_source)
    if (_scan_source)
      extract_labels_from_source(${product} ${trg})
    endif()
    #SET_TESTS_PROPERTIES(${product} PROPERTIES TIMEOUT "45")  
    SET_TESTS_PROPERTIES(${product} PROPERTIES LABELS "full;${TARGET_TESTGROUP}")    
  endif()

  # Native platform is special - we always do unit-testing!
  if (${RUMBOOT_PLATFORM} MATCHES "native" AND NOT _index GREATER -1 AND NOT CROSS_COMPILE)
    add_test(${product} ${product})
    extract_labels_from_source(${product} ${trg})

    SET_TESTS_PROPERTIES(${product} PROPERTIES LABELS "full;${TARGET_TESTGROUP}")
    if (TARGET_TIMEOUT_CTEST)
      SET_TESTS_PROPERTIES(${product} PROPERTIES TIMEOUT "${TARGET_TIMEOUT_CTEST}")
    endif()
  endif()
endfunction()
