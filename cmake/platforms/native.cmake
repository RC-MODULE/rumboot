SET(RUMBOOT_ARCH native)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/arch/native/exception.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
)

set(RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP Yes)
set(RUMBOOT_HAS_V3_BOOTROM Yes)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(CMAKE_DUMP_FLAGS     "-EL")
    SET(CMAKE_C_FLAGS     "-std=gnu99 -g")
    if (NOT CROSS_COMPILE)
      SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32") #x86
      SET(CMAKE_EXE_LINKER_FLAGS     "-m32")
    elseif (${CROSS_COMPILE} STREQUAL "arm-rcm-linux-gnueabihf")
      SET(XILINX_FPGA_PROTO YES)
      SET(RUMBOOT_NEEDS_XRUN YES)
      SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -DDUT=NPE -DRUMBOOT_ENABLE_NATIVE_PCIE") #FixMe: This shouldn't be here
    else()
      message(FATAL_ERROR "Check flags before cross-compiling native for a weird arch")
    endif()
    SET(CMAKE_EXE_LINKER_FLAGS     "${CMAKE_EXE_LINKER_FLAGS} -g")
endmacro()

rumboot_add_configuration(
  NATIVE
  DEFAULT
  PREFIX "host"
  LDFLAGS -lm
  CFLAGS -DRUMBOOT_NATIVE -DCMAKE_BINARY_DIR=\"${CMAKE_BINARY_DIR}\"
  FEATURES COVERAGE
)

rumboot_add_configuration(
  NATIVE_SPL
  DEFAULT
  LDFLAGS -lm
  CFLAGS -DRUMBOOT_NATIVE -DCMAKE_BINARY_DIR=\"${CMAKE_BINARY_DIR}\"
  PREFIX "spl"
  PACKIMAGE_FLAGS -w V3
  FEATURES LUA PACKIMAGE STUB
)


function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

endmacro()

function(gen_chain_spl var name s1 s2)
  generate_product_name(SPL_CHAIN_EXEC ${name})
  message(${SPL_CHAIN_EXEC})
  add_custom_target(${SPL_CHAIN_EXEC} ALL
    COMMAND ${PYTHON_EXECUTABLE} ${RUMBOOT_PACKIMAGE} -f ${s1} -C
    COMMAND ${PYTHON_EXECUTABLE} ${RUMBOOT_COMBINE} -i ${s1} -i ${s2} -o ${SPL_CHAIN_EXEC}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Generating chained ${name}"
    DEPENDS ${s1} ${s2}
  )
  set(${var} ${SPL_CHAIN_EXEC} PARENT_SCOPE)
endfunction()

include(${CMAKE_SOURCE_DIR}/cmake/na.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)

  add_rumboot_target_dir(tests/ 
    CONFIGURATION NATIVE
    PREFIX test
  )

  add_rumboot_target(
    CONFIGURATION NATIVE
    FILES common/tools/print-heaps.c
  )

  if (XILINX_FPGA_PROTO)
    add_rumboot_target_dir(fpga/ 
      CONFIGURATION NATIVE
      PREFIX fpga
    )


    add_rumboot_target(
      CONFIGURATION NATIVE
      FILES scr1/targets/load_bin_example/hello_load_bin.c
      LOAD 
        myfile ${CMAKE_SOURCE_DIR}/src/platform/scr1/targets/load_bin_example/data
    )

    na_testsuite_init("NPE")
    na_testsuite_add_npe_tests("NATIVE")
    #na_testsuite_add_vpe_tests("NATIVE")
  endif()

  ############# BOOTROM UNIT TESTS #############
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        CONFIGURATION NATIVE
        FILES ${target}
    )
  endforeach()


  rumboot_bootrom_add_components(NATIVE NATIVE_SPL FALSE FALSE)
  
  rumboot_bootrom_add_common_units(
      CONFIGURATION NATIVE
  )

  add_rumboot_target(
      NAME fileboot
      CONFIGURATION NATIVE
      CFLAGS -DSOURCE=0 -DEXPECTED=1
      FILES common/bootrom/unit.c
      FEATURES STUB
  )

  add_rumboot_target(
      NAME bootsrc-check-io
      CONFIGURATION NATIVE
      CFLAGS -DSOURCE=0
      FILES common/bootrom/bootsource-test-io.c
      FEATURES STUB
  )

  gen_chain_spl(SPL_CHAIN_OK spl_chain_ok ${SPL_NEXT} ${SPL_OK})

  add_custom_target(pattern.bin ALL
    COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/wordpattern.py -f ${PROJECT_BINARY_DIR}/pattern.bin -s 8192
  )

  add_rumboot_test(host-bootsrc-check-io bootsrc-check-io-no-align --file pattern.bin)
  add_rumboot_test(host-bootsrc-check-io bootsrc-check-io-align-16  --align 16 --file pattern.bin)
  add_rumboot_test(host-bootsrc-check-io bootsrc-check-io-align-512 --align 512 --file pattern.bin)

  add_rumboot_test(host-fileboot unit-boot --file ${SPL_OK})

  add_rumboot_test(bootrom-loader bootrom-chain --file ${SPL_CHAIN_OK})
  add_rumboot_test(bootrom-loader bootrom-jump --file ${SPL_JUMP} --file2 ${SPL_OK_CHECK2})
  add_rumboot_test(bootrom-loader bootrom-jump-to-host --file ${SPL_JUMP_HOST} --hfile ${SPL_OK_CHECK2})
  add_rumboot_test(bootrom-loader bootrom-boot --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-boot-selftest --selftest --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-boot-a32 --align 32 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-boot-a64 --align 64 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-boot-a128 --align 128 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-bad-magic --file ${SPL_FAIL_BAD_MAGIC} --file2 ${SPL_OK_CHECK1})
  add_rumboot_test(bootrom-loader bootrom-bad-header-crc32 --file ${SPL_FAIL_BAD_HCRC32} --file2 ${SPL_OK_CHECK1})
  add_rumboot_test(bootrom-loader bootrom-bad-data-crc32 --file ${SPL_FAIL_BAD_DCRC32} --file2 ${SPL_OK_CHECK1})

  add_rumboot_test(bootrom-loader bootrom-bad-id --file ${SPL_FAIL_BAD_ID} --file2 ${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-bad-version --file ${SPL_FAIL_BAD_VERSION} --file2 ${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-bad-revision --file ${SPL_OK_BAD_REV} --file2 ${SPL_FAIL})
  add_rumboot_test(bootrom-loader bootrom-next --file ${SPL_NEXT} --file2 ${SPL_OK_CHECK2})
  add_rumboot_test(bootrom-loader bootrom-hostmode-ok --host --hfile ${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-hostmode-ok-reverse --host --hfile ${SPL_OK_REVERSE})

  add_rumboot_test(bootrom-loader bootrom-hostmode-compressed-ok --host --hfile ${COMPRESSED_OK})
  add_rumboot_test(bootrom-loader bootrom-hostmode-compressed-fail-oversized --host --hfile ${SPL_FAIL_COMPRESSED_OVERSIZED},${SPL_OK})

  add_rumboot_test(bootrom-loader bootrom-hostmode-bad-data --host --hfile ${SPL_FAIL_BAD_DCRC32},${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-hostmode-bad-magic --host --hfile ${SPL_FAIL_BAD_MAGIC},${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-hostmode-bad-id --host --hfile ${SPL_FAIL_BAD_ID},${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-fallback-to-hostmode --file ${SPL_FAIL_BAD_HCRC32} --file2 ${SPL_FAIL_BAD_DCRC32} --hfile ${SPL_OK})
  add_rumboot_test(bootrom-loader bootrom-hostmode-jump-to-file --host --file2 ${SPL_FAIL} --file2 ${SPL_OK_CHECK2} --hfile ${SPL_FAIL_BAD_ID},${SPL_JUMP})

endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
