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

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        CONFIGURATION NATIVE
        FILES ${target}
    )
  endforeach()

  add_rumboot_target(
      NAME fileboot
      CONFIGURATION NATIVE
      CFLAGS -DSOURCE=0 -DEXPECTED=1
      FILES common/bootrom/unit.c
      FEATURES STUB
  )

  add_rumboot_target(
      NAME bootrom
      CONFIGURATION NATIVE
      FILES common/bootrom/bootrom.c
      FEATURES STUB
  )

  add_rumboot_target(
      CONFIGURATION NATIVE_SPL
      NAME ok
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0
      VARIABLE SPL_OK
  )

  add_rumboot_target(
      CONFIGURATION NATIVE_SPL
      NAME fail
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=1 -DSIGNAL=SIGUSR2
      VARIABLE SPL_FAIL
  )

  add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-magic
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION -DSIGNAL=SIGUSR2
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s magic 0xbadc0de
     VARIABLE SPL_FAIL_BAD_MAGIC
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-version
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION -DSIGNAL=SIGUSR2
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s version 1 -c
     VARIABLE SPL_FAIL_BAD_VERSION
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-header-crc
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION -DSIGNAL=SIGUSR2
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s header_crc32 0xb00bc0de
     VARIABLE SPL_FAIL_BAD_HCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-data-crc
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION -DSIGNAL=SIGUSR2
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s data_crc32 0xb00bc0de -c
     VARIABLE SPL_FAIL_BAD_DCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME next
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=0
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_NEXT
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME jump
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=2
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_JUMP
   )


   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-id
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s chip_id 1 -c
     VARIABLE SPL_FAIL_BAD_ID
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME ok-bad-revision
     CONFIGURATION NATIVE_SPL
     CFLAGS -DEXITCODE=0 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s chip_rev 99 -c
     VARIABLE SPL_OK_BAD_REV
   )

  #TODO: Test all header combination in host mocking mode
  #TODO: Add selftest stub code and tests
  #TODO: Test chain-booting appended images from one source
  add_rumboot_test(host-fileboot unit-boot --file ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-jump --file ${SPL_JUMP} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-boot --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a32 --align 32 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a64 --align 64 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-boot-a128 --align 128 --file ${SPL_OK} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-bad-magic --file ${SPL_FAIL_BAD_MAGIC} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-header-crc32 --file ${SPL_FAIL_BAD_HCRC32} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-data-crc32 --file ${SPL_FAIL_BAD_DCRC32} --file2 ${SPL_OK})

  add_rumboot_test(host-bootrom bootrom-bad-id --file ${SPL_FAIL_BAD_ID} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-version --file ${SPL_FAIL_BAD_VERSION} --file2 ${SPL_OK})
  add_rumboot_test(host-bootrom bootrom-bad-revision --file ${SPL_OK_BAD_REV} --file2 ${SPL_FAIL})
  add_rumboot_test(host-bootrom bootrom-bad-version --file ${SPL_NEXT} --file2 ${SPL_OK})
endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
