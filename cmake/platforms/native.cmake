SET(RUMBOOT_ARCH native)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
    ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-dummy.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(CMAKE_DUMP_FLAGS     "-EL")
    SET(CMAKE_C_FLAGS     "-std=gnu99")
endmacro()

rumboot_add_configuration(
  NATIVE
  DEFAULT
  LDFLAGS -lm
  FEATURES LUA
)

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)
endmacro()

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(
        SNAPSHOT "null"
        PREFIX "test"
        FILES ${target}
    )
  endforeach()

  add_rumboot_target(
      SNAPSHOT "null"
      PREFIX "host"
      NAME luac
      FILES ${CMAKE_SOURCE_DIR}/src/lib/lua-5.1.5/src/luac.c
  )

endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
