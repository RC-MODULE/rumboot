SET(RUMBOOT_ARCH native)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(CMAKE_DUMP_FLAGS     "-EL")
    SET(CMAKE_C_FLAGS     "-std=gnu99 -m32")
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
  FEATURES LUA PACKIMAGE STUB
)


function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)

endmacro()

function(gen_chain_spl var name s1 s2)
  generate_product_name(SPL_CHAIN_EXEC ${name})
  add_custom_target(${SPL_CHAIN_EXEC}
    COMMAND cat ${s1} > ${SPL_CHAIN_EXEC}
    COMMAND cat ${s2}   >> ${SPL_CHAIN_EXEC}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Generating chained ${name}"
    DEPENDS ${s1} ${s2}
  )
  set(var ${SPL_CHAIN_EXEC} PARENT_SCOPE)
endfunction()


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        CONFIGURATION NATIVE
        FILES ${target}
    )
  endforeach()


  rumboot_add_bootrom_components(NATIVE_SPL NATIVE)

  add_rumboot_target(
      NAME fileboot
      CONFIGURATION NATIVE
      CFLAGS -DSOURCE=0 -DEXPECTED=1
      FILES common/bootrom/unit.c
      FEATURES STUB
  )



   gen_chain_spl(spl_chain_ok SPL_CHAIN_OK SPL_NEXT SPL_OK)

  #TODO: Add selftest stub code and tests
  #TODO: Test chain-booting appended images from one source
  add_rumboot_test(host-bootrom bootrom-chain --file ${SPL_CHAIN_OK})
  add_rumboot_test(host-fileboot unit-boot --file ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-jump --file ${SPL_JUMP} --file2 ${SPL_OK_CHECK2})
  add_rumboot_test(host-bootrom bootrom-jump-to-host --file ${SPL_JUMP_HOST} --hfile ${SPL_OK_CHECK2})
  add_rumboot_test(host-bootrom bootrom-boot --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-selftest --selftest --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a32 --align 32 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a64 --align 64 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a128 --align 128 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-bad-magic --file ${SPL_FAIL_BAD_MAGIC} --file2 ${SPL_OK_CHECK1})
  add_rumboot_test(host-bootrom bootrom-bad-header-crc32 --file ${SPL_FAIL_BAD_HCRC32} --file2 ${SPL_OK_CHECK1})
  add_rumboot_test(host-bootrom bootrom-bad-data-crc32 --file ${SPL_FAIL_BAD_DCRC32} --file2 ${SPL_OK_CHECK1})

  add_rumboot_test(host-bootrom bootrom-bad-id --file ${SPL_FAIL_BAD_ID} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-version --file ${SPL_FAIL_BAD_VERSION} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-revision --file ${SPL_OK_BAD_REV} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-next --file ${SPL_NEXT} --file2 ${SPL_OK_CHECK2})
  add_rumboot_test(host-bootrom bootrom-hostmode-ok --host --hfile ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-hostmode-bad-data --host --hfile ${SPL_FAIL_BAD_DCRC32},${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-hostmode-bad-magic --host --hfile ${SPL_FAIL_BAD_MAGIC},${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-hostmode-bad-id --host --hfile ${SPL_FAIL_BAD_ID},${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-fallback-to-hostmode --file ${SPL_FAIL_BAD_HCRC32} --file2 ${SPL_FAIL_BAD_DCRC32} --hfile ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-hostmode-jump-to-file --host --file2 ${SPL_FAIL} --file2 ${SPL_OK_CHECK2} --hfile ${SPL_FAIL_BAD_ID},${SPL_JUMP})

endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
