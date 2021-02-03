SET(RUMBOOT_ARCH nmc)
SET(RUMBOOT_PLATFORM nmc)

set(RUMBOOT_PLATFORM_DEFAULT_LDS nmc3/generic.lds)
set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT default)

#These are configurations for our binaries
rumboot_add_configuration(
  IRAM
  DEFAULT
  PREFIX iram
  SNAPSHOT top
  LDS nmc3/generic.lds
  FILES ${CMAKE_SOURCE_DIR}/src/platform/nmc/startup.S 
  LDFLAGS "-Wl,\"-estart\""
  IRUN_FLAGS ${BOOTROM_IFLAGS} +RUMBOOT_RUNTIME_ADDR=5A000
  LOAD IM1_IMAGE SELF
  CFLAGS -DRUMBOOT_MAIN_NORETURN
)

rumboot_add_configuration(
  CORE
  PREFIX core
  SNAPSHOT top
  LDS nmc3/generic.lds
  LDFLAGS "-Wl,\"-ecorestart\""
  CFLAGS -mmas -save-temps
  IRUN_FLAGS ${BOOTROM_IFLAGS} +RUMBOOT_RUNTIME_ADDR=5A000
  LOAD IM1_IMAGE SELF
  FEATURES NOLIBS
)


include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target_dir(iram/
    CONFIGURATION IRAM
  )

  #Clang doesn't support legacy stuff
  if (NOT RUMBOOT_NMC_USE_CLANG)
    add_rumboot_target_dir(coretests/
      CONFIGURATION CORE
    )
  endif()


endmacro()

if (CMAKE_VERILOG_RULES_LOADED)
    return()
endif()

file(GLOB PLATFORM_SOURCES
  ${CMAKE_SOURCE_DIR}/src/platform/nmc/glue.c
  ${CMAKE_SOURCE_DIR}/src/platform/nmc/bootglue.c
  ${CMAKE_SOURCE_DIR}/src/arch/nmc/exception.c
  ${CMAKE_SOURCE_DIR}/src/lib/drivers/irq-nmc.c
  )

macro(RUMBOOT_PLATFORM_SET_COMPILER_FLAGS)
    SET(RUMBOOT_COMMON_FLAGS "-std=gnu99")
    SET(CMAKE_C_FLAGS " ${RUMBOOT_COMMON_FLAGS} -Wall -fdata-sections -ffunction-sections -DRUMBOOT_PLATFORM_NUM_HEAPS=8")
    if (NOT RUMBOOT_NMC_USE_CLANG)
      set(CMAKE_C_FLAGS "-mnmc4-fixed ${CMAKE_C_FLAGS}")
    endif()
    
    SET(CMAKE_ASM_FLAGS "")
    SET(CMAKE_OBJCOPY_FLAGS )
    SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -Wl,--gc-sections")
    SET(CMAKE_DUMP_FLAGS     --byte-addr -S)
endmacro()

function(RUMBOOT_PLATFORM_PRINT_SUMMARY)

endfunction()

macro(rumboot_platform_generate_stuff_for_taget product)


endmacro()


if (NOT CROSS_COMPILE)
    set(CMAKE_C_COMPILER_WORKS 1)
    if(NOT RUMBOOT_NMC_USE_CLANG)
      SET(CROSS_COMPILE nmc)
    else()
      SET(CMAKE_C_COMPILER       /opt/llvm-nmc/usr/local/bin/clang)
      SET(CMAKE_CXX_COMPILER     /opt/llvm-nmc/usr/local/bin/clang++)
    endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
