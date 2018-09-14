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
  FEATURES LUA
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
      CONFIGURATION NATIVE
      NAME luac
      FILES ${CMAKE_SOURCE_DIR}/src/lib/lua-5.1.5/src/luac.c
  )

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
      FILES common/spl-stubs/ok.c
  )



endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
