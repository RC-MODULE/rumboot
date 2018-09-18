macro(rumboot_add_bootrom_components spl_conf romconf)
  if (NOT RUMBOOT_PLATFORM STREQUAL "native")
    set(ROM_EXTRAFEATURES NOCODE)
  endif()

  add_rumboot_target(
      NAME bootrom
      CONFIGURATION ${romconf}
      FILES common/bootrom/bootrom.c
      FEATURES STUB ${ROM_EXTRAFEATURES}
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0
      VARIABLE SPL_OK
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok-check2
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0 -DSPL_COUNT_CHECK=2
      VARIABLE SPL_OK_CHECK2
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME ok-check1
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=0 -DSPL_COUNT_CHECK=1
      VARIABLE SPL_OK_CHECK1
  )

  add_rumboot_target(
      CONFIGURATION ${spl_conf}
      NAME fail
      FILES common/bootrom/spl.c
      CFLAGS -DTERMINATE_SIMULATION -DEXITCODE=1
      VARIABLE SPL_FAIL
  )

  add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-magic
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s magic 0xbadc0de
     VARIABLE SPL_FAIL_BAD_MAGIC
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-version
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s version 1 -c
     VARIABLE SPL_FAIL_BAD_VERSION
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-header-crc
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s header_crc32 0xb00bc0de
     VARIABLE SPL_FAIL_BAD_HCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME fail-bad-data-crc
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=1 -DTERMINATE_SIMULATION
     FEATURES STUB PACKIMAGE
     PACKIMAGE_FLAGS -s data_crc32 0xb00bc0de -c
     VARIABLE SPL_FAIL_BAD_DCRC32
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME next
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=0
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_NEXT
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME jump
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=2
     FEATURES STUB PACKIMAGE
     VARIABLE SPL_JUMP
   )

   add_rumboot_target(
     FILES common/bootrom/spl.c
     NAME jump_host
     CONFIGURATION ${spl_conf}
     CFLAGS -DEXITCODE=-1
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

macro(rumboot_unit_test_bootsource)
  set(options DEFAULT)
  set(oneValueArgs   ID TAG MEMTAG TAGOFFSET CONFIGURATION)
  set(multiValueArgs IRUN_FLAGS)

  cmake_parse_arguments(BOOTSOURCE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(_commas)

  foreach(offset RANGE ${BOOTSOURCE_TAGOFFSET})
    set(_commas "${_commas},")
  endforeach()

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-ok"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom
          IRUN_FLAGS ${IRUN_FLAGS}
          CFLAGS -DSOURCE=${id} -DEXPECTED=true
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-ok
  )

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-next-ok"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom
          IRUN_FLAGS ${IRUN_FLAGS}
          CFLAGS -DSOURCE=${id} -DEXPECTED=true
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-ok-check2
  )

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-bad-magic"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom
          IRUN_FLAGS ${IRUN_FLAGS}
          CFLAGS -DSOURCE=${id} -DEXPECTED=false
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-magic
  )

  add_rumboot_target(
          NAME "unit-${BOOTSOURCE_TAG}-bad-version"
          CONFIGURATION ${BOOTSOURCE_CONFIGURATION}
          PREFIX "bootrom"
          FILES common/bootrom/unit.c
          TESTGROUP bootrom
          IRUN_FLAGS ${IRUN_FLAGS}
          CFLAGS -DSOURCE=${id} -DEXPECTED=false
          LOAD ${BOOTSOURCE_MEMTAG} ${_commas}spl-fail-bad-version
  )
endmacro()
