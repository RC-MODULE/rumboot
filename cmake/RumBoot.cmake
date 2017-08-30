function(add_source SRC)
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/${SRC}" PARENT_SCOPE)
endfunction()

function(add_boot_source KEY SRC)
    if (${${KEY}})
        SET(PLATFORM_SOURCES "${PLATFORM_SOURCES};${CMAKE_SOURCE_DIR}/src/bootsrc/${SRC}" PARENT_SCOPE)
    endif()
endfunction()

macro(add_rumboot_test name_ binary baseclass)
    string(TOLOWER "${name_}" name)
    STRING(REGEX REPLACE "-" "_" name ${name})
    if (${RUMBOOT_PLATFORM} MATCHES "native")
      add_test(${name} ${binary} ${ARGN})
    else()
        file(APPEND ${RUMBOOT_MANIFEST}
        "${baseclass} test_rumboot_${name} ${binary}.bin")
        foreach(arg ${ARGN})
                file(APPEND ${RUMBOOT_MANIFEST} " ${arg}")
        endforeach()
        file(APPEND ${RUMBOOT_MANIFEST} "\n")
    endif()
endmacro()

macro(generate_stuff_for_target product)
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

  add_custom_target(
    objcopy-${product} ALL
    DEPENDS ${product}.bin ${product}.dmp
  )

endmacro()


macro(rumboot_add_configuration name)
  message(STATUS "Adding configuration ${name}")
  set(options DEFAULT)
  set(oneValueArgs SNAPSHOT LDS PREFIX NAME)
  set(multiValueArgs FILES IRUN_FLAGS CFLAGS)

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

function(add_rumboot_target)
  set(options )
  set(oneValueArgs SNAPSHOT LDS NAME PREFIX CONFIGURATION)
  set(multiValueArgs FILES IRUN_FLAGS CFLAGS TESTGROUP)

  cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (NOT TARGET_CONFIGURATION)
    set(TARGET_CONFIGURATION ${RUMBOOT_PLATFORM_DEFAULT_CONFIGURATION})
  endif()

  foreach(c ${oneValueArgs} ${multiValueArgs})
    config_load_param(${TARGET_CONFIGURATION} ${c})
  endforeach()

  if (NOT TARGET_NAME)
    list(GET TARGET_FILES 0 TARGET_NAME)
    GET_FILENAME_COMPONENT(TARGET_NAME ${TARGET_NAME} NAME_WE)
  endif()

  if (NOT TARGET_FILES)
    message(FATAL_ERROR "add_rumboot_target() requires TARGET_FILES to contain at least one file")
  endif()

  SET(product rumboot-${RUMBOOT_PLATFORM}-${CMAKE_BUILD_TYPE}-${TARGET_PREFIX}-${TARGET_NAME})
  set(name rumboot-${TARGET_SNAPSHOT}-${product})

  if (${name})
    message(STATUS "NOT Adding rumboot target: ${name} - already exists")
    return()
  endif()

  message(STATUS "Adding rumboot test: ${product}")
  set(${name} TRUE PARENT_SCOPE)

  foreach(f ${TARGET_FILES})
    if (NOT EXISTS ${f})
      LIST(APPEND trg ${RUMBOOT_PLATFORM_TARGET_DIR}/${f})
    else()
      LIST(APPEND trg ${f})
    endif()
  endforeach()

  add_executable(${product} ${trg} $<TARGET_OBJECTS:rumboot-${TARGET_CONFIGURATION}>)
  target_compile_definitions(${product} PUBLIC ${CFLAGS})

  if (NOT TARGET_LDS)
    set(TARGET_LDS ${RUMBOOT_PLATFORM_DEFAULT_LDS})
  endif()

  if (TARGET_LDS AND EXISTS ${CMAKE_SOURCE_DIR}/lds/${TARGET_LDS})
    set(ldf -T${CMAKE_SOURCE_DIR}/lds/${TARGET_LDS})
  else()
    set(ldf "")
  endif()

  target_link_libraries(${product} ${ldf} -Wl,-Map,${product}.map)
  install(TARGETS ${product} RUNTIME DESTINATION rumboot)

  generate_stuff_for_target(${product})
  install(FILES ${CMAKE_BINARY_DIR}/${product}.dmp DESTINATION rumboot)
  install(FILES ${CMAKE_BINARY_DIR}/${product}.bin DESTINATION rumboot)
  install(FILES ${CMAKE_BINARY_DIR}/${product}.map DESTINATION rumboot)

  rumboot_platform_generate_stuff_for_taget(${product})

  # Native platform is special - we do unit-testing!
  if (${RUMBOOT_PLATFORM} MATCHES "native")
    add_test(${product} ${product})
  endif()
endfunction()
