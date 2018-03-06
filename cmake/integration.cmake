# RumBoot
# This invocation builds rumboot in native mode and runs the tests
# if native tests fail - no use to test anything at all

add_custom_target(rumboot)

add_external_component(rumboot-native-tests rumboot NO -DRUMBOOT_PLATFORM=native)

if(RUMBOOT_COVERAGE)
  configure_file(
      ${CMAKE_VERILOG_RULES_DIR}/gcovmerge.sh.in
      ${PROJECT_BINARY_DIR}/view_rumboot_coverage
  )
endif()
# This one builds our rumboot that we're gonna use for the SoC
# Disabled until the rumboot builds for target are actually supported
#add_external_component(rumboot rumboot -DRUMBOOT_PLATFORM=basis)
macro(rumboot_add_configuration name)
  message(STATUS "Adding configuration ${name}")
  set(options DEFAULT)
  set(oneValueArgs SNAPSHOT LDS PREFIX NAME BOOTROM)
  set(multiValueArgs FILES IRUN_FLAGS CFLAGS TESTGROUP FEATURES TIMEOUT)

  cmake_parse_arguments(CONFIGURATION_${name} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  LIST(APPEND RUMBOOT_CONFIGURATIONS ${name})
  if (CONFIGURATION_${name}_DEFAULT)
    set(RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION ${name})
    message(STATUS "Default configuration set to: ${RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION}")
  endif()
endmacro()

macro(config_load_param conf param)
  if (NOT TARGET_${param})
    set(TARGET_${param} ${CONFIGURATION_${conf}_${param}})
  endif()
endmacro()

macro(config_load_param_append conf param)
    set(TARGET_${param} ${TARGET_${param}} ${CONFIGURATION_${conf}_${param}})
endmacro()

function(add_rumboot_target)
  # snapshot lds prefix target
  set(optons "")
  set(oneValueArgs SNAPSHOT LDS NAME PREFIX CONFIGURATION BOOTROM)
  set(multiValueArgs FILES IRUN_FLAGS CFLAGS TESTGROUP LDFLAGS CHECKCMD FEATURES TIMEOUT)

  cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (NOT TARGET_CONFIGURATION)
    set(TARGET_CONFIGURATION ${RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION})
  endif()

  foreach(c ${oneValueArgs})
    config_load_param(${TARGET_CONFIGURATION} ${c})
  endforeach()

  foreach(c ${multiValueArgs})
    config_load_param_append(${TARGET_CONFIGURATION} ${c})
  endforeach()

  if (NOT TARGET_NAME)
    list(GET TARGET_FILES 0 TARGET_NAME)
    GET_FILENAME_COMPONENT(TARGET_NAME ${TARGET_NAME} NAME_WE)
  endif()

  if (NOT TARGET_SNAPSHOT)
    set(TARGET_SNAPSHOT ${RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT})
  endif()

  SET(product rumboot-${RUMBOOT_PLATFORM}-${buildtype}-${TARGET_PREFIX}-${TARGET_NAME})
  set(name rumboot-${TARGET_SNAPSHOT}-${product})

  if (${name})
    message(STATUS "NOT Adding rumboot test: ${name} - already exists")
    return()
  endif()

  list (FIND TARGET_FEATURES "STUB" _index)
  if (${_index} GREATER -1)
    return()
  endif()

  list (FIND TARGET_FEATURES "LPROBE" _index)
  if (${_index} GREATER -1)
    if (NOT EXISTS ${TARGET_FILES})
      set(trg ${RUMBOOT_PLATFORM_TARGET_DIR}/${TARGET_FILES})
    else()
      set(trg ${TARGET_FILES})
    endif()
    #lprobe scripts need extra love
    set(lprobe_flags "+LPROBE_SCRIPT=${trg}")
  else()
    set(lprobe_flags)
  endif()

  if (NOT TARGET_BOOTROM)
    set(bootrom_flags +BOOTROM=${rumboot_fulldir}/${product}.hex)
  else()
      set(bproduct rumboot-${RUMBOOT_PLATFORM}-${buildtype}-${TARGET_BOOTROM})
      set(bootrom_flags
          +BOOTROM=${rumboot_fulldir}/${bproduct}.hex
          +IM0BIN=${rumboot_fulldir}/${product}.bin
    )
  endif()

  if (NOT "${TARGET_CHECKCMD}" STREQUAL "")
    string(REPLACE ";" " " TARGET_CHECKCMD "${TARGET_CHECKCMD}")
  endif()

  if (TARGET_TIMEOUT)
    list(LENGTH TARGET_TIMEOUT len)
    if (NOT len EQUAL "2")
      message(FATAL_ERROR "Test ${product} has incorrect timeout ${TARGET_TIMEOUT}")
    endif()
    list(GET TARGET_TIMEOUT 0 timeout_value)
    list(GET TARGET_TIMEOUT 1 timeout_scale)
    set(timeout_flags
      +TIMEOUT=${timeout_value}
      +TIMEOUT_SCALE=${timeout_scale}
    )
  endif()

  list (FIND TARGET_TESTGROUP "${HDL_REGRESSION_LIST}" _index)
  if (${_index} GREATER -1 OR ${HDL_REGRESSION_LIST} STREQUAL "full")
    set(${name} TRUE PARENT_SCOPE)
    set(HDL_EXTRA_REBUILD_CMD
      "${HDL_EXTRA_REBUILD_CMD}
      make ${rumboot_dirname}-configure
      cd ${rumboot_dirname}
      make ${product}.all
      cd ..
      "
    )

    hdl_add_test(rumboot-${TARGET_SNAPSHOT}-${product} ${TARGET_SNAPSHOT}
        ${bootrom_flags}
        ${lprobe_flags}
        ${timeout_flags}
        +BOOTMAP=${rumboot_fulldir}/${product}.dmp
        ${TARGET_IRUN_FLAGS}
    )
  endif()

endfunction()

function(rumboot_load_build platform buildtype)
    message(STATUS "Loading rumboot verification targets...")
    set(buildtype ${buildtype}) # Make sure it propagates!
    set(rumboot_dirname rumboot-${platform}-${buildtype})
    set(rumboot_fulldir ${CMAKE_BINARY_DIR}/${rumboot_dirname})

    add_external_component(${rumboot_dirname} rumboot NO
        -DRUMBOOT_PLATFORM=${platform}
        -DCMAKE_BUILD_TYPE=${buildtype}
        -DRUMBOOT_TRACE=${RUMBOOT_TRACE}
        -DRUMBOOT_COVERAGE=${RUMBOOT_COVERAGE}
        -DRUMBOOT_PROFILE=${RUMBOOT_PROFILE}
    )

    set(RUMBOOT_UTILS_DIR ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils PARENT_SCOPE)

    add_dependencies(rumboot ${rumboot_dirname})
    #Suck in target information from rumboot, and play a little with SRC_DIR
    set(CMAKE_SOURCE_DIR_REAL ${CMAKE_SOURCE_DIR})
    set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/rumboot)

    set(RUMBOOT_PLATFORM ${platform})
    set(RUMBOOT_PLATFORM_TARGET_DIR ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/)
    include(${CMAKE_SOURCE_DIR}/cmake/platforms/${platform}.cmake)

    RUMBOOT_PLATFORM_ADD_COMPONENTS()
    set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR_REAL})
endfunction()
