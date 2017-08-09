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

function(add_rumboot_target snapshot lds prefix target)
    GET_FILENAME_COMPONENT(f ${target} NAME_WE)
    SET(product rumboot-${RUMBOOT_PLATFORM}-${CMAKE_BUILD_TYPE}-${prefix}-${f})

    if (TARGET ${product})
      return()
    endif()

    foreach(f ${target} ${ARGN})
      if (NOT EXISTS ${f})
        LIST(APPEND trg ${RUMBOOT_PLATFORM_TARGET_DIR}/${f})
      else()
        LIST(APPEND trg ${f})
      endif()
    endforeach()

    add_executable(${product} ${trg} $<TARGET_OBJECTS:rumboot>)

    if (EXISTS ${CMAKE_SOURCE_DIR}/lds/${lds})
      set(ldf -T${CMAKE_SOURCE_DIR}/lds/${lds})
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
