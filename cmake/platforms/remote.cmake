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
    SET(CMAKE_C_FLAGS     "-std=gnu99 -g -DRUMBOOT_NATIVE -DRUMBOOT_REMOTE")
    if (NOT CROSS_COMPILE)
      SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32") #x86
      SET(CMAKE_EXE_LINKER_FLAGS     "-m32")
    elseif (${CROSS_COMPILE} STREQUAL "arm-rcm-linux-gnueabihf")
      SET(XILINX_FPGA_PROTO YES)
      SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -DDUT=NPE -DRUMBOOT_ENABLE_NATIVE_PCIE -DRUMBOOT_PLATFORM_REMOTE=1") #FixMe: This shouldn't be here
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
  CFLAGS -DCMAKE_BINARY_DIR=\"${CMAKE_BINARY_DIR}\"
  FEATURES COVERAGE
)

rumboot_add_configuration(
  NATIVE_SPL
  DEFAULT
  LDFLAGS -lm
  CFLAGS -DCMAKE_BINARY_DIR=\"${CMAKE_BINARY_DIR}\"
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

  add_rumboot_target(
    CONFIGURATION NATIVE
    FILES hello.c
  )

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

endmacro()

macro(rumboot_platform_setup_configuration)

endmacro()
