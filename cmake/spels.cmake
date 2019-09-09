function(rumboot_spels_memory_test)
    set(options )

    set(oneValueArgs   CONFIGURATION START END NAME)
    set(multiValueArgs MEMTEST)

    cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_cflags )

    if (TARGET_START)
        set(_cflags ${_cflags} -DSTART=${TARGET_START})
    endif()

    if (TARGET_END)
        set(_cflags ${_cflags} -DEND=${TARGET_END})
    endif()
    message("${_cflags}")

    add_rumboot_target(
        CONFIGURATION ${TARGET_CONFIGURATION}
        FILES common/spels/iram.c
        PREFIX "spels"
        NAME memory-test-${TARGET_NAME}
        CFLAGS ${_cflags}
    )

endfunction()

function(rumboot_spels_arch_tests)
    set(options )

    set(oneValueArgs   CONFIGURATION )
    set(multiValueArgs MEMTEST)

    cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_rumboot_target(
        CONFIGURATION ${TARGET_CONFIGURATION}
        FILES basis/targets/mathtest.c
        PREFIX "spels"
        NAME math-test
    )

endfunction()