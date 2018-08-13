# RumBoot
# This invocation builds rumboot in native mode and runs the tests
# if native tests fail - no use to test anything at all${RUMBOOT_ONEVALUE_ARGS}

add_custom_target(rumboot)

add_external_component(rumboot-native-tests rumboot NO -DRUMBOOT_PLATFORM=native)

if(RUMBOOT_COVERAGE)
  configure_file(
      ${CMAKE_VERILOG_RULES_DIR}/gcovmerge.sh.in
      ${PROJECT_BINARY_DIR}/view_rumboot_coverage
  )
endif()


function(add_rumboot_target)
  # snapshot lds prefix target
  set(optons "")
  set(oneValueArgs ${RUMBOOT_ONEVALUE_ARGS} CONFIGURATION)
  set(multiValueArgs ${RUMBOOT_MULVALUE_ARGS})

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

  add_custom_target(${product})

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
      generate_product_name(bproduct ${TARGET_BOOTROM})
      set(bootrom_flags
          +BOOTROM=${rumboot_fulldir}/${bproduct}.hex
    )
  endif()

  set(v 0)
  set(loadflags "")
  foreach(l ${TARGET_LOAD})
    if (v EQUAL 0)
      set(plus ${l})
      set(v 1)
    else()
      set(trg ${l})
      generate_product_name(tproduct ${trg})
      if (${trg} STREQUAL "SELF")
        set(loadflags "${loadflags} +${plus}=${rumboot_fulldir}/${product}.bin")
      elseif(TARGET ${tproduct})
        set(loadflags "${loadflags} +${plus}=${rumboot_fulldir}/${tproduct}.bin")
      else()
        set(loadflags "${loadflags} +${plus}=${trg}")
      endif()
      set(v 0)
    endif()
  endforeach()

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
        ${loadflags}
        ${TARGET_IRUN_FLAGS}
    )
    SET_TESTS_PROPERTIES(rumboot-${TARGET_SNAPSHOT}-${product} PROPERTIES LABELS "${TARGET_TESTGROUP}")

    #FixMe: Hack. ARM ModelManager fucks up simulation with a segfault if
    #No JTAGbsi file is found. That happens even if it's not really running
    #So we always provide a dummy here to be safe.
    hdl_test_provide_file(rumboot-${TARGET_SNAPSHOT}-${product} ${CMAKE_SOURCE_DIR}/scripts/empty.txt JTAGbsi)


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

    set(RUMBOOT_UTILS_DIR  ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils PARENT_SCOPE)
    set(RUMBOOT_BINARY_DIR ${CMAKE_BINARY_DIR}/${rumboot_dirname} PARENT_SCOPE)

    add_dependencies(rumboot ${rumboot_dirname})
    #Suck in target information from rumboot, and play a little with SRC_DIR
    set(CMAKE_SOURCE_DIR_REAL ${CMAKE_SOURCE_DIR})
    set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/rumboot)

    set(RUMBOOT_PLATFORM ${platform})
    set(RUMBOOT_PLATFORM_TARGET_DIR ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/)
    set(RUMBOOT_PLATFORM_COMMON_DIR ${CMAKE_SOURCE_DIR}/src/platform/)

    include(${CMAKE_SOURCE_DIR}/cmake/platforms/${platform}.cmake)

    RUMBOOT_PLATFORM_ADD_COMPONENTS()
    set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR_REAL})
endfunction()
