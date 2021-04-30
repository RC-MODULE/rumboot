SET(RUMBOOT_ARCH nmc)
SET(RUMBOOT_PLATFORM nmc)

set(RUMBOOT_PLATFORM_DEFAULT_LDS nmc/generic.lds)

if (RUMBOOT_DEFAULT_SNAPSHOT)
  set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT ${RUMBOOT_DEFAULT_SNAPSHOT})
else()
  set(RUMBOOT_PLATFORM_DEFAULT_SNAPSHOT top)
endif()

if(NOT RUMBOOT_SOC)
  message("NMC: Standalone build!")
  set(SOC_OBJCOPY_FLAGS --remove-section=.header)
  set(SOC_BOOTROM )
  set(SOC_PACKIMAGE_FLAGS )
  set(SOC_FEATURES )
else()
  message("NMC: SOC build for ${RUMBOOT_SOC}!")
  set(SOC_OBJCOPY_FLAGS )
  set(SOC_BOOTROM ${RUMBOOT_SOC}:bootrom-stub)
  set(SOC_EMI_INITIALIZER ${RUMBOOT_SOC}:stub-emi_initializer_notlb_unload)
  set(SOC_PACKIMAGE_FLAGS -CiR 0x80020000 -F SYNC True)
  set(SOC_PACKIMAGE_FLAGS_EMI -CiR 0x0 -F SYNC True)
  set(SOC_FEATURES PACKIMAGE)
endif()

#These are configurations for our binaries
rumboot_add_configuration(
  IRAM
  DEFAULT
  PREFIX iram
  LDS nmc/generic.lds
  FILES ${CMAKE_SOURCE_DIR}/src/platform/nmc/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  LDFLAGS "-Wl,\"-estart\""
  IRUN_FLAGS ${BOOTROM_IFLAGS} +RUMBOOT_RUNTIME_ADDR=5A000 
  LOAD 
    IM1_IMAGE SELF
    IM0BIN SELF
  CFLAGS -fnmc-compatible-if-packed -DRUMBOOT_NOENTRY
  OBJCOPY_FLAGS ${SOC_OBJCOPY_FLAGS}
  FEATURES ${SOC_FEATURES}
  #We need a relocatable image 0x80020000
  PACKIMAGE_FLAGS ${SOC_PACKIMAGE_FLAGS}
  #External bootrom-stub dependency
  BOOTROM ${SOC_BOOTROM}
  )

rumboot_add_configuration(
  CORE
  PREFIX core
  FILES ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
  LDS nmc/generic.lds
  LDFLAGS "-Wl,\"-ecorestart\""
  CFLAGS -fnmc-compatible-if-packed -mmas -DRUMBOOT_NOENTRY 
  IRUN_FLAGS ${BOOTROM_IFLAGS} +RUMBOOT_RUNTIME_ADDR=5A000
  LOAD 
    IM1_IMAGE SELF
    IM0BIN SELF
  FEATURES NOLIBS ${SOC_FEATURES}
  OBJCOPY_FLAGS ${SOC_OBJCOPY_FLAGS}
  PACKIMAGE_FLAGS ${SOC_PACKIMAGE_FLAGS} -s target_cpu 1
  #External bootrom-stub dependency
  BOOTROM ${SOC_BOOTROM}
  )

if(RUMBOOT_SOC)
  rumboot_add_configuration(
      SRAM
      DEFAULT
      PREFIX sram
      LDS nmc/emi.lds
      FILES ${CMAKE_SOURCE_DIR}/src/platform/nmc/startup.S ${CMAKE_SOURCE_DIR}/src/lib/bootheader.c
      LDFLAGS "-Wl,\"-estart\""
      IRUN_FLAGS ${BOOTROM_IFLAGS} +RUMBOOT_RUNTIME_ADDR=5A000 
      LOAD 
        IM0BIN ${SOC_EMI_INITIALIZER},SELF
      CFLAGS -fnmc-compatible-if-packed -DRUMBOOT_ENTRY=start  
      OBJCOPY_FLAGS ${SOC_OBJCOPY_FLAGS}
      FEATURES ${SOC_FEATURES}
      PACKIMAGE_FLAGS ${SOC_PACKIMAGE_FLAGS_EMI}
      #External bootrom-stub dependency
      BOOTROM ${SOC_BOOTROM}
      )
endif()
  

macro(dap_integration_test sourcefile)
  GET_FILENAME_COMPONENT(__NAME ${sourcefile} NAME_WE)
  add_rumboot_target(
      CONFIGURATION IRAM
      PREFIX dap
      FILES ${sourcefile}
      NAME ${__NAME}
      IRUN_FLAGS +DS5_JTAG
    )
  if (NMC_DBG_INTEGRATION_TESTS AND CMAKE_VERILOG_RULES_LOADED)
    hdl_test_provide_file(rumboot-${RUMBOOT_DEFAULT_SNAPSHOT}-rumboot-${RUMBOOT_PLATFORM}-${RUMBOOT_BUILD_TYPE}-dap-${__NAME} ${PROJECT_BINARY_DIR}/units/ca5-validation-tests/${__NAME}.bsi JTAGbsi)
    hdl_test_provide_file(rumboot-${RUMBOOT_DEFAULT_SNAPSHOT}-rumboot-${RUMBOOT_PLATFORM}-${RUMBOOT_BUILD_TYPE}-dap-${__NAME} ${PROJECT_BINARY_DIR}/units/ca5-validation-tests/SWIM${__NAME}.hex SWIM.hex)
    hdl_test_provide_file(rumboot-${RUMBOOT_DEFAULT_SNAPSHOT}-rumboot-${RUMBOOT_PLATFORM}-${RUMBOOT_BUILD_TYPE}-dap-${__NAME} ${PROJECT_BINARY_DIR}/units/ca5-validation-tests/SWJIM${__NAME}.hex SWJIMCtl.hex)
  endif()
  endmacro()

include(${CMAKE_SOURCE_DIR}/cmake/bootrom.cmake)

macro(RUMBOOT_PLATFORM_ADD_COMPONENTS)
  add_rumboot_target_dir(iram/
    CONFIGURATION IRAM
  )

  if (RUMBOOT_SOC)
    add_rumboot_target_dir(iram/
      CONFIGURATION SRAM
    )
  endif()

  #Clang doesn't support legacy stuff
  if (NOT RUMBOOT_NMC_USE_CLANG)
    add_rumboot_target_dir(coretests/
      CONFIGURATION CORE
    )
    if (RUMBOOT_SOC)
      add_rumboot_target_dir(coretests/emi/
        CONFIGURATION CORE
        PREFIX coresram
        LOAD IM0BIN ${SOC_EMI_INITIALIZER},SELF
      )
    endif()

  endif()

  add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS -DUSE_SWINT=30
    FILES common/irq/irq-atomics.c
    PREFIX "iram-irqlo"
)

add_rumboot_target(
    CONFIGURATION IRAM
    CFLAGS -DUSE_SWINT=30
    FILES common/irq/irq-defhandler.c
    PREFIX "iram-irqlo"
)

add_rumboot_target(
  CONFIGURATION IRAM
  CFLAGS -DUSE_SWINT=41
  FILES common/irq/irq-atomics.c
  PREFIX "iram-irqhi"
)

add_rumboot_target(
  CONFIGURATION IRAM
  CFLAGS -DUSE_SWINT=41
  FILES common/irq/irq-defhandler.c
  PREFIX "iram-irqhi"
)

add_rumboot_target(
  CONFIGURATION IRAM
  FILES common/bootrom/timer.c
)

  dap_integration_test(dap/nmc_dbg_brp.S)
  dap_integration_test(dap/nmc_dbg_dap_integration.S)
  dap_integration_test(dap/nmc_dbg_drar_dsar.S)
  dap_integration_test(dap/nmc_dbg_dscr.S)
  dap_integration_test(dap/nmc_dbg_modes.S)
  dap_integration_test(dap/nmc_dbg_sftrst.S)

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
    if (EXISTS /usr/local/bin/nmc-gcc)
        set(CROSS_COMPILE "/usr/local/bin/nmc")
    else()
      SET(CROSS_COMPILE nmc)
    endif()
    if(RUMBOOT_NMC_USE_CLANG)
      SET(CMAKE_C_COMPILER       /opt/llvm-nmc/usr/local/bin/clang)
      SET(CMAKE_CXX_COMPILER     /opt/llvm-nmc/usr/local/bin/clang++)
    endif()
endif()

set(CMAKE_C_COMPILER_WORKS 1)
