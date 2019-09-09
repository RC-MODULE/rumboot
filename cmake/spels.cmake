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

    set(_name memory-test-${TARGET_NAME})
    add_rumboot_target(
        CONFIGURATION ${TARGET_CONFIGURATION}
        FILES common/spels/iram.c
        PREFIX "spels"
        NAME ${_name}
        TESTGROUP spels
        CFLAGS ${_cflags}
    )

    list(APPEND SPELS_TEST_CHAIN spels-${_name})
    string(REPLACE SPELS_TEST_CHAIN ";" "," ${SPELS_TEST_CHAIN})
    set(SPELS_TEST_CHAIN ${SPELS_TEST_CHAIN} PARENT_SCOPE)

endfunction()

function(rumboot_spels_arch_tests)
    set(options )

    set(oneValueArgs   CONFIGURATION )
    set(multiValueArgs MEMTEST)

    cmake_parse_arguments(TARGET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_rumboot_target(
        CONFIGURATION ${TARGET_CONFIGURATION}
        FILES common/spels/mathtest.c
        PREFIX "spels"
        TESTGROUP spels
        NAME math-test
    )
    list(APPEND SPELS_TEST_CHAIN spels-math-test)
    string(REPLACE SPELS_TEST_CHAIN ";" "," ${SPELS_TEST_CHAIN})
    set(SPELS_TEST_CHAIN ${SPELS_TEST_CHAIN} PARENT_SCOPE)

endfunction()