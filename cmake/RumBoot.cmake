set(RUMBOOT_ONEVALUE_ARGS SNAPSHOT LDS PREFIX NAME BOOTROM CHECKPOINTS RESTORE TIMEOUT_CTEST VARIABLE CONFIGURATION APPEND COMBOIMAGE)
set(RUMBOOT_MULVALUE_ARGS FILES IRUN_FLAGS CFLAGS TESTGROUP LDFLAGS PREPCMD CHECKCMD FEATURES TIMEOUT LOAD DEPENDS PACKIMAGE_FLAGS)

find_program(PYTHON_EXECUTABLE
  NAMES python3 python3.4 python3.6 python3.1 python3.5 python3.2 python3.3
  HINTS /usr/local/bin
)

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

  #Fetch from current target dir
  fetch_sources(${RUMBOOT_PLATFORM_TARGET_DIR}/${dir})
  foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S} ${RUMBOOT_TARGETS_LUA})
    add_rumboot_target(
        ${ARGN}
        FILES ${target}
    )
  endforeach()

  #fetch from common directory
  fetch_sources(${RUMBOOT_PLATFORM_COMMON_DIR}/${dir})
  foreach(target ${RUMBOOT_TARGETS_C} ${RUMBOOT_TARGETS_S} ${RUMBOOT_TARGETS_LUA})
    add_rumboot_target(
        ${ARGN}
        FILES ${target}
    )
  endforeach()


endmacro()



if (CMAKE_VERILOG_RULES_DIR)
  get_filename_component(_dir ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
  include(${_dir}/integration.cmake)
  return()
endif()

##############################################################################


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
      install(TARGETS ${product} RUNTIME DESTINATION rumboot)
  endif()

  if (NOT RUMBOOT_PLATFORM STREQUAL "native")
      add_custom_command(
        OUTPUT ${product}.dmp
        COMMAND ${CROSS_COMPILE}-objdump -D ${CMAKE_DUMP_FLAGS} ${product} > ${product}.dmp
        COMMENT "Generating disassembly listing: ${product}.dmp"
        DEPENDS ${product}
      )

      add_custom_command(
        OUTPUT ${product}.bin
        COMMAND ${CROSS_COMPILE}-objcopy ${CMAKE_OBJCOPY_FLAGS} -O binary ${product} ${product}.bin
        COMMENT "Generating binary image ${product}.bin"
        DEPENDS ${product}
      )

      if (TARGET_APPEND)
        add_custom_command(
          APPEND
          OUTPUT ${product}.bin
          COMMAND cat ${TARGET_APPEND} >> ${product}.bin &&  truncate -s +1  ${product}.bin
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
        install(FILES ${CMAKE_BINARY_DIR}/${product}.dmp DESTINATION rumboot)
        install(FILES ${CMAKE_BINARY_DIR}/${product}.bin DESTINATION rumboot)
        install(FILES ${CMAKE_BINARY_DIR}/${product}.map DESTINATION rumboot)
      endif()

      add_custom_target(
        ${product}.all ALL
        DEPENDS ${product}.bin ${product}.dmp
      )
  else()
    add_custom_command(
      TARGET ${product}
      COMMAND ${packimage_cmd}
      COMMENT "Adding image header to ${product}"
    )
  endif()

  if (NOT ${TARGET_BOOTROM} STREQUAL "")
    add_dependencies(${product}.all ${bootrom})
  endif()

  populate_dependencies(${product}.all)
endmacro()


set(RUMBOOT_COVER_CFLAGS -DRUMBOOT_COVERAGE=1 -fprofile-arcs -ftest-coverage)
set(RUMBOOT_COVER_LFLAGS --coverage)


function(expand_target_load prefix)
  set(v 0)
  set(loadflags "")
  set(seen)
  set(plus)
  foreach(l ${TARGET_LOAD})
    if (v EQUAL 0)
      if (plus)
        list(APPEND seen ${plus})
        set("${prefix}_${plus}" ${${prefix}_${plus}} PARENT_SCOPE)
      endif()
      set(plus ${l})
      set(v 1)
      list (FIND seen "${plus}" _index)
      #If we've seen this plusarg before, skip
      if (NOT ${_index} GREATER -1)
        set("${prefix}_${plus}")
      endif()
    else()
      if (NOT ${_index} GREATER -1)
        set(trgs ${l})
        string(REPLACE "," ";" trgs "${trgs}")
        foreach(trg ${trgs})
          generate_product_name(tproduct ${trg})
          if(TARGET ${tproduct})
            list(APPEND "${prefix}_${plus}" ${tproduct})
          else()
            list(APPEND "${prefix}_${plus}" ${trg})
          endif()
        endforeach()
        set(v 0)
      endif()
    endif()
  endforeach()
endfunction()

macro(populate_dependencies target)
    #Filter out dependencies
    foreach(dep ${TARGET_DEPENDS})
      generate_product_name(dep ${dep})
      add_dependencies(${target} ${dep}.all)
    endforeach()
    #Process TARGET_LOAD dependencies
    set(v 0)
    set(loadflags "")
    foreach(l ${TARGET_LOAD})
      if (v EQUAL 0)
        set(plus ${l})
        set(v 1)
      else()
        set(trgs ${l})
        string(REPLACE "," ";" trgs "${trgs}")
        foreach(trg ${trgs})
          generate_product_name(tproduct ${trg})
          string(REGEX REPLACE "\\.hex$" "" tproduct ${tproduct})
          string(REGEX REPLACE "\\.hex\\/.+$" "" tproduct ${tproduct})
          if(TARGET ${tproduct})
            add_dependencies(${target} ${tproduct}.all)
          endif()
        endforeach()
        set(v 0)
      endif()
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


function(add_rumboot_test target name)
  generate_product_name(product ${target})
  add_test(${name} ${product} ${ARGN})
  SET_TESTS_PROPERTIES(${name} PROPERTIES TIMEOUT "15")
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
    message("Generating combo image from plusarg ${TARGET_COMBOIMAGE}")
    expand_target_load("TARGET_LOAD")
    set(_deps)
    set(_cmds COMMAND rm -f ${product}.bin)
    foreach(f ${TARGET_LOAD_${TARGET_COMBOIMAGE}})
      list(APPEND _deps ${f}.bin)
      list(APPEND _cmds COMMAND cat ${f}.bin >> ${product}.bin)
      if (RUMBOOT_COMBOIMAGE_PAD)
        list(APPEND _cmds COMMAND truncate ${product}.bin -s %${RUMBOOT_COMBOIMAGE_PAD})
      endif()
    endforeach()
    
    #string(REPLACE ";" " " _deps "${_deps}")
    #string(REPLACE ";" " " _cmds "${_cmds}")
    add_custom_command(OUTPUT ${product}.bin
      DEPENDS ${_deps}
      ${_cmds}
      VERBATIM
    )
    add_custom_target(${product}.all ALL DEPENDS ${product}.bin)

    message(${product}.all)
    return()
  endif()

  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "LPROBE" _index)
  if (${_index} GREATER -1 OR ${_index2} GREATER -1)
    #Lprobe scripts compile nothing. Provide a dummy
    #target and return
    add_custom_target(${product}.all ALL DEPENDS ${bootrom})
    populate_dependencies(${product}.all)
    return()
  endif()

  set(EXTRA_FILES)

  set(${name} TRUE PARENT_SCOPE)

  foreach(f ${TARGET_FILES})
    locate_source_file(trg ${f})
  endforeach()

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



  add_executable(${product} ${trg} $<TARGET_OBJECTS:rumboot-${TARGET_CONFIGURATION}>)
  target_compile_options(${product} PUBLIC ${TARGET_CFLAGS} -DRUMBOOT_PRODUCT_NAME=\"${product}\")

  list (FIND CONFIGURATION_${TARGET_CONFIGURATION}_FEATURES "LUA" _index)
  if (${_index} GREATER -1)
    target_link_libraries(${product} lua)
  endif()

  if (NOT TARGET_LDS)
    set(TARGET_LDS ${RUMBOOT_PLATFORM_DEFAULT_LDS})
  else()
    set(TARGET_LDS ${CMAKE_SOURCE_DIR}/lds/${TARGET_LDS})
  endif()

  if (TARGET_LDS AND EXISTS ${TARGET_LDS})
    set(ldf -T${TARGET_LDS} -L${CMAKE_SOURCE_DIR}/lds)
  else()
    set(ldf )
  endif()

  if (ldf)
    set_target_properties(${product} PROPERTIES LINK_DEPENDS ${TARGET_LDS})
  endif()

  target_link_libraries(${product} ${CONFIGURATION_${TARGET_CONFIGURATION}_LDFLAGS} ${ldf} -Wl,-Map,${product}.map)

  if (TARGET_APPEND)
    locate_source_file(_append_list ${TARGET_APPEND})
    set(TARGET_APPEND ${_append_list})
  endif()

  generate_stuff_for_target(${product})

  rumboot_platform_generate_stuff_for_taget(${product})



  list (FIND TARGET_FEATURES "STUB" _index)

  #For standalone builds we enable testing via rumboot-xrun
  if (NOT RUMBOOT_TESTING_PORT)
    set(RUMBOOT_TESTING_PORT /dev/ttyUSB0)
  endif()

  if (NOT RUMBOOT_TESTING_RESETSEQ)
    set(RUMBOOT_TESTING_RESETSEQ pl2303)
  endif()

  if (NOT RUMBOOT_TESTING_RESETPORT)
    set(RUMBOOT_TESTING_RESETPORT -1)
  endif()


  list (FIND TARGET_FEATURES "STUB" _stub)
  if (NOT RUMBOOT_DISABLE_TESTING AND NOT ${_stub} GREATER -1 AND NOT ${RUMBOOT_PLATFORM} MATCHES "native")
    add_test(NAME ${product} COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/rumboot-packimage.py/rumboot_xrun.py -r ${RUMBOOT_TESTING_RESETSEQ} -f ${product}.bin -P ${RUMBOOT_TESTING_RESETPORT} -p ${RUMBOOT_TESTING_PORT})
    SET_TESTS_PROPERTIES(${product} PROPERTIES TIMEOUT "45")  
    SET_TESTS_PROPERTIES(${product} PROPERTIES LABELS "full;${TARGET_TESTGROUP}")    
  endif()

  # Native platform is special - we always do unit-testing!
  if (${RUMBOOT_PLATFORM} MATCHES "native" AND NOT _index GREATER -1 )
    add_test(${product} ${product})

    SET_TESTS_PROPERTIES(${product} PROPERTIES LABELS "full;${TARGET_TESTGROUP}")
    if (TARGET_TIMEOUT_CTEST)
      SET_TESTS_PROPERTIES(${product} PROPERTIES TIMEOUT "${TARGET_TIMEOUT_CTEST}")
    endif()
  endif()
endfunction()
