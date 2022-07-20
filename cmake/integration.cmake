# RumBoot
# This invocation builds rumboot in native mode and runs the tests
# if native tests fail - no use to test anything at all${RUMBOOT_ONEVALUE_ARGS}

add_custom_target(rumboot ALL)
add_custom_target(tarball)

#add_external_component(rumboot-native-tests rumboot NO -DRUMBOOT_PLATFORM=native)

if(RUMBOOT_COVERAGE)
  configure_file(
      ${CMAKE_VERILOG_RULES_DIR}/gcovmerge.sh.in
      ${PROJECT_BINARY_DIR}/view_rumboot_coverage
  )
endif()


function(add_rumboot_target)
  # snapshot lds prefix target
  set(optons "")
  set(oneValueArgs ${RUMBOOT_ONEVALUE_ARGS})
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

  set(maplist)

  if (NOT TARGET_BOOTROM)
    set(bootrom_flags +BOOTROM=${rumboot_fulldir}/${product}.hex)
    set(maplist ${rumboot_fulldir}/${product}.dmp)
  else()
      set(_extrom)
      expand_external_dependency(TARGET_BOOTROM _extrom)
      if (_extrom)
        set(bootrom_flags
            +BOOTROM="${_extrom}.hex"
        )
        set(maplist
            ${_extrom}.map
        )
      else()
        generate_product_name(bproduct ${TARGET_BOOTROM})
        set(_hex "${rumboot_fulldir}/${bproduct}.hex")
        set(bootrom_flags
            +BOOTROM="${_hex}"
        )
        set(maplist
        ${rumboot_fulldir}/${bproduct}.dmp
        )
      endif()
  endif()

  set(v 0)
  set(loadflags "")
  foreach(l ${TARGET_LOAD})
    if (v EQUAL 0)
      set(plus ${l})
      set(v 1)
    else()
      set(trgs ${l})
      string(REPLACE "," ";" trgs "${trgs}")
      set(trglist)
      foreach(trg ${trgs})
          if (trglist)
            set(trglist "${trglist},")
          endif()
          if (maplist)
            set(maplist "${maplist},")
          endif()
          generate_product_name(tproduct ${trg})
          string(REGEX MATCH "\\/.+$" HPATH ${tproduct})
          string(REGEX REPLACE "\\.hex\\/.+$" "" tproduct ${tproduct})
          string(REGEX REPLACE "\\.hex$" "" tproduct ${tproduct})
          if (${trg} STREQUAL "SELF")
            set(trglist "${trglist}${rumboot_fulldir}/${product}.bin")
            set(maplist "${maplist}${rumboot_fulldir}/${product}.dmp")
          elseif(${trg} MATCHES "\\.hex$")
            set(trglist "${trglist}${rumboot_fulldir}/${tproduct}.hex${HPATH}")
            set(maplist "${maplist}${rumboot_fulldir}/${tproduct}.dmp")
          elseif(TARGET ${tproduct})
            set(trglist "${trglist}${rumboot_fulldir}/${tproduct}.bin")
            set(maplist "${maplist}${rumboot_fulldir}/${tproduct}.dmp")
          else()
            expand_external_dependency(trg _exttrg)
            if (_exttrg)
              set(trglist "${trglist}${_exttrg}.bin")
              set(maplist "${trglist}${_exttrg}.map")
            else()
              set(trglist "${trglist}${trg}")
            endif()
          endif()
      endforeach()
      set(loadflags "${loadflags} +${plus}=${trglist}")
      set(v 0)
    endif()
  endforeach()

  if (NOT "${TARGET_CHECKCMD}" STREQUAL "")
    string(REPLACE ";" " " TARGET_CHECKCMD "${TARGET_CHECKCMD}")
  endif()

  if (NOT "${TARGET_PREPCMD}" STREQUAL "")
    string(REPLACE ";" " " TARGET_PREPCMD "${TARGET_PREPCMD}")
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
      ${CMAKE_COMMAND} --build . -t ${rumboot_dirname}-configure
      [ -f \"${rumboot_dirname}/.stamps/${product}\" ] || ${CMAKE_COMMAND} --build . -t ${rumboot_dirname}-configure
      cd ${rumboot_dirname}
      ${CMAKE_COMMAND} --build . -t ${product}.all $THREADS
      cd ..
      "
    )

    hdl_add_test(rumboot-${TARGET_SNAPSHOT}-${product} ${TARGET_SNAPSHOT}
        ${bootrom_flags}
        ${lprobe_flags}
        ${timeout_flags}
        +BOOTMAP=${maplist}
        ${loadflags}
        ${TARGET_IRUN_FLAGS}
    )

    if (TARGET_TESTGROUP)
      string(REPLACE " " ";" TARGET_TESTGROUP "${TARGET_TESTGROUP}")
    endif()
    SET_TESTS_PROPERTIES(rumboot-${TARGET_SNAPSHOT}-${product} PROPERTIES LABELS "full;${TARGET_TESTGROUP}")
    feature_check("EXTRACT_LABELS" TARGET_FEATURES _scan_source)
    if (_scan_source)
      expand_source_list(TARGET_FILES TARGET_FILES)
      extract_labels_from_source(rumboot-${TARGET_SNAPSHOT}-${product} ${TARGET_FILES})
    endif()
    if (TARGET_TIMEOUT_CTEST)
      hdl_test_set_timeout(rumboot-${TARGET_SNAPSHOT}-${product} "${TARGET_TIMEOUT_CTEST}")
    endif()

    if (HDL_TEST_TIMEOUT)
      get_test_property(rumboot-${TARGET_SNAPSHOT}-${product} TIMEOUT timeout)
      if("${timeout}" STREQUAL "NOTFOUND" OR ${timeout} GREATER ${HDL_TEST_TIMEOUT})
        SET_TESTS_PROPERTIES(rumboot-${TARGET_SNAPSHOT}-${product} PROPERTIES TIMEOUT "${HDL_TEST_TIMEOUT}")
      endif()
    endif()

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
        -DRUMBOOT_SOC_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DRUMBOOT_DISABLE_TESTING=Yes
        ${RUMBOOT_EXTRA_CMAKE_ARGS}
        ${ARGN}
      )
    #FixMe: Figure out why native testing fails on yacc
    #add_dependencies(${rumboot_dirname} rumboot-native-tests)

    if (RUMBOOT_PACKAGED)
      message(STATUS "Downloading CI package: ${RUMBOOT_PACKAGE_URL}")
      if (NOT RUMBOOT_PACKAGE_URL)
        message(FATAL_ERROR "Please set RUMBOOT_PACKAGE_URL to the CI location")
      endif()
      if (NOT RUMBOOT_BUILD_TYPE STREQUAL "Production")
        message(FATAL_ERROR "Please specify -DRUMBOOT_BUILD_TYPE=Production to test prepackaged rumboot")
      endif()
      file(DOWNLOAD
        ${RUMBOOT_PACKAGE_URL}
        ${CMAKE_BINARY_DIR}/rumboot-from-ci.tgz
        SHOW_PROGRESS
        STATUS _DL_STATUS
      )
      list(GET _DL_STATUS 0 _status)
      if (NOT _status STREQUAL "0")
        message(FATAL_ERROR "Failed to download rumboot CI package: ${RUMBOOT_PACKAGE_URL}")
      endif()

      add_custom_command(
          TARGET rumboot
          DEPENDS ${rumboot_fulldir}
          COMMAND tar vxpf ${CMAKE_BINARY_DIR}/rumboot-from-ci.tgz --strip-components=1
          WORKING_DIRECTORY ${rumboot_fulldir}
        )
    endif()

    set(RUMBOOT_SOC_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    set(RUMBOOT_UTILS_DIR  ${CMAKE_BINARY_DIR}/${rumboot_dirname}/utils PARENT_SCOPE)
    set(RUMBOOT_BINARY_DIR ${CMAKE_BINARY_DIR}/${rumboot_dirname} PARENT_SCOPE)

    if (${platform} STREQUAL ${PROJECT_NAME})
      add_custom_command(
          TARGET tarball
          COMMAND cd ${CMAKE_BINARY_DIR}/${rumboot_dirname} && ${CMAKE_COMMAND} --build . -t package
        )
    endif()
    
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
