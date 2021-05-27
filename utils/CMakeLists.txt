cmake_minimum_required (VERSION 2.6)
project (rumboot-utils C)

if (NOT RUMBOOT_DISABLE_CCACHE)
  find_program(CCACHE_FOUND ccache)
else()
  set(CCACHE_FOUND No)
endif()

if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

set(missing_sources )
if (WIN32)
    set(missing_sources asprintf.c)
endif()
add_executable(romgen romgen.c ${missing_sources})
add_executable(shmtest shmtest.c ${missing_sources})
find_package(Git)

if (NOT WIN32)
    add_library(ini ini/ini.c)
    add_subdirectory(matrix_rm)
    add_subdirectory(bin_generator)
    set(RM_CORE_DIR npe_rm/rm_core)
    if(RUMBOOT_BUILD_NPE_RM)
      message("=== Compilation of NPE Refefence model enabled ===")
      if(NOT EXISTS ${PROJECT_SOURCE_DIR}/${RM_CORE_DIR}/CMakeLists.txt)
      # checkout it first time. Then
      #  I Want To git pull the npe_rm, then complie and run and then - commit (update the npe_rm)
      execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init npe_rm
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      )
      endif()
      set(COMPILE_FROM_ROMBOOT "YES")
      add_subdirectory(${RM_CORE_DIR})
    endif()
endif()
