SET(RUMBOOT_ARCH native)

file(GLOB PLATFORM_SOURCES
    ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/*.c
)

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(CMAKE_DUMP_FLAGS     "-EL")
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)
endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)
endmacro()

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  file(GLOB RUMBOOT_TARGETS ${CMAKE_SOURCE_DIR}/src/platform/${RUMBOOT_PLATFORM}/targets/*.c)
  foreach(target ${RUMBOOT_TARGETS})
    add_rumboot_target(${target})
  endforeach()
endmacro()
