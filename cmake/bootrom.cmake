macro(rumboot_bootrom_add_components spl_conf romconf)
  add_rumboot_target(
      PREFIX "bootrom"
      NAME "loader"
      CONFIGURATION ${romconf}
      FILES common/bootrom/bootrom.c
      FEATURES STUB ${ROM_EXTRAFEATURES} CPACK
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0
      PACKIMAGE_FLAGS ${ARGN} -c
      VARIABLE SPL_OK
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok-check2
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0 -DSPL_COUNT_CHECK=2
      PACKIMAGE_FLAGS ${ARGN} -c
      VARIABLE SPL_OK_CHECK2
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok-check1
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0 -DSPL_COUNT_CHECK=1
      PACKIMAGE_FLAGS ${ARGN} -c
      VARIABLE SPL_OK_CHECK1
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME fail
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=1
      PACKIMAGE_FLAGS ${ARGN} -c
      VARIABLE SPL_FAIL
  )

  add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-magic
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s magic 0xbadc0de ${ARGN}
     VARIABLE SPL_FAIL_BAD_MAGIC
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-version
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s version 1 -c ${ARGN}
     VARIABLE SPL_FAIL_BAD_VERSION
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-header-crc
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s header_crc32 0xb00bc0de ${ARGN}
     VARIABLE SPL_FAIL_BAD_HCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-data-crc
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s data_crc32 0xb00bc0de -c ${ARGN}
     VARIABLE SPL_FAIL_BAD_DCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME next
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=0
     PACKIMAGE_FLAGS ${ARGN} -c
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_NEXT
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME jump
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=2
     PACKIMAGE_FLAGS ${ARGN} -c
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_JUMP
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME jump_host
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=-ETOHOST
     PACKIMAGE_FLAGS ${ARGN} -c
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_JUMP_HOST
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-id
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s chip_id 1 -c
     VARIABLE SPL_FAIL_BAD_ID
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME ok-bad-revision
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=0 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s chip_rev 99 -c
     VARIABLE SPL_OK_BAD_REV
   )
endmacro()

macro(rumboot_bootrom_unit_test)
  set(options DEFAULT)
  set(oneValueArgs   ID TAG MEMTAG TAGOFFSET CONFIGURATION FULL)
  set(multiValueArgs IRUN_FLAGS)

  cmake_parse_arguments(BOOTSOURCE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(_commas)


  foreach(offset RANGE 0 ${BOOTSOURCE_TAGOFFSET})
    if (NOT ${offset} EQUAL "0")
      set(_commas "${_commas}!,")
    endif()
  endforeach()

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-io-check"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/bootsource-test-io.c
          TESTGROUP bootrom
          PREPCMD ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/wordpattern.py -e big -f pattern.bin -s 16384
          IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
          CFLAGS -DSOURCE=${BOOTSOURCE_ID}
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}pattern.bin
  )

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-ok"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom bootrom-unit
          IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
          CFLAGS -DSOURCE=${BOOTSOURCE_ID} -DEXPECTED=0
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-ok
  )

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-bad-data"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom bootrom-unit
          IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
          CFLAGS -DSOURCE=${BOOTSOURCE_ID} -DEXPECTED=-EBADDATACRC
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-data-crc
  )

  if (BOOTSOURCE_FULL)
    add_rumboot_target(
            NAME "unit-${BOOTSOURCE_TAG}-bad-magic"
            CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
            PREFIX "bootrom"
            FILES common/bootrom/unit.c
            TESTGROUP bootrom bootrom-unit
            IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
            CFLAGS -DSOURCE=${BOOTSOURCE_ID} -DEXPECTED=-EBADMAGIC
            LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-magic
          )

          add_rumboot_target(
            NAME "unit-${BOOTSOURCE_TAG}-bad-version"
            CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
            PREFIX "bootrom"
            FILES common/bootrom/unit.c
            TESTGROUP bootrom bootrom-unit
            IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
            CFLAGS -DSOURCE=${BOOTSOURCE_ID} -DEXPECTED=-EBADVERSION
            LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-version
          )

          add_rumboot_target(
            NAME "unit-${BOOTSOURCE_TAG}-bad-header"
            CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
            PREFIX "bootrom"
            FILES common/bootrom/unit.c
            TESTGROUP bootrom bootrom-unit
            IRUN_FLAGS ${BOOTSOURCE_IRUN_FLAGS}
            CFLAGS -DSOURCE=${BOOTSOURCE_ID} -DEXPECTED=-EBADHDRCRC
            LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-header-crc
          )
  endif()


endmacro()

macro(rumboot_bootrom_integration_test romconf)
  add_rumboot_target(
          PREFIX "bootrom-integration-no-selftest"
          CONFIGURATION ${romconf}
          BOOTROM bootrom-loader
          TESTGROUP bootrom bootrom-integration
          FEATURES NOCODE ${CONFIGURATION_${romconf}_FEATURES}
          ${ARGN}
  )

  string(REPLACE "IRUN_FLAGS" "IRUN_FLAGS;+BOOT_SELFTEST=1" _tmp "${ARGN}")
  add_rumboot_target(
          PREFIX "bootrom-integration-selftest"
          CONFIGURATION ${romconf}
          BOOTROM bootrom-loader
          TESTGROUP bootrom bootrom-integration
          FEATURES NOCODE ${CONFIGURATION_${romconf}_FEATURES}
          ${_tmp}
  )


endmacro()

# TODO: Take this one apart
macro(add_bootrom_stuff romconf)
  add_rumboot_target(
          NAME "spi-cs0-boot"
          CONFIGURATION ${romconf}
          PREFIX "bootrom"
          BOOTROM bootrom-loader
          TESTGROUP bootrom
          FEATURES NOCODE
          LOAD
            SPI0_CONF spl-ok,spl-fail

  )

  add_rumboot_target(
          NAME "spi-cs1-boot"
          CONFIGURATION ROM
          PREFIX "bootrom"
          BOOTROM bootrom-loader
          TESTGROUP bootrom
          LOAD
            SPI0_CONF spl-fail-bad-magic,spl-ok
          FEATURES NOCODE
  )

  add_rumboot_target(
          NAME "i2c-eeprom-a0-boot"
          CONFIGURATION ROM
          PREFIX "bootrom"
          BOOTROM bootrom-loader
          TESTGROUP bootrom
          LOAD
            SPI0_CONF spl-fail-bad-magic,spl-fail-bad-magic
            I2C0_CONF spl-ok,spl-fail,spl-fail,spl-fail
            I2C1_CONF spl-fail,spl-fail,spl-fail,spl-fail
          FEATURES NOCODE
  )


  rumboot_unit_test(0 spi0_cs0 SPI0_CONF 0)
  rumboot_unit_test(1 spi0_cs1 SPI0_CONF 1)
  rumboot_unit_test(2 i2c0_50  I2C0_CONF 0)
  rumboot_unit_test(3 i2c0_51  I2C0_CONF 1)
  rumboot_unit_test(4 i2c0_52  I2C0_CONF 2)
  rumboot_unit_test(5 i2c0_53  I2C0_CONF 3)

  add_rumboot_target(
          NAME "host-shim"
          CONFIGURATION ROM
          PREFIX "bootrom"
          BOOTROM bootrom-loader
          TESTGROUP bootrom bootrom-unit-tests
          IRUN_FLAGS +BOOT_HOST
          LOAD
            SPI0_CONF spl-fail,spl-fail
            HOSTMOCK spl-ok
          FEATURES NOCODE
  )


endmacro()
