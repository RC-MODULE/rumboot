CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

INCLUDE(cmake/RumBoot.cmake)
find_package(Doxygen)

SET(RUMBOOT_PLATFORM "native" CACHE STRING "RumBoot platform to compile for")

if (NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Production" CACHE string "BuildType")
endif()

INCLUDE(cmake/platforms/${RUMBOOT_PLATFORM}.cmake)

if (CROSS_COMPILE)
  SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/cross-compile.cmake")
endif()

PROJECT(rumboot C ASM)
SET(PROJECT_VERSION   0.3)
enable_testing()

SET(RUMBOOT_MANIFEST "${CMAKE_BINARY_DIR}/rumboot-manifest-${CMAKE_BUILD_TYPE}.txt")
file(WRITE ${RUMBOOT_MANIFEST} "")
install(FILES ${RUMBOOT_MANIFEST} DESTINATION rumboot)

if (CROSS_COMPILE)
  CROSS_COMPILE_DETECT_SYSROOT()
endif()

RUMBOOT_PLATFORM_SET_COMPILER_FLAGS()

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/include/
)

execute_process(COMMAND
  git rev-parse --short HEAD
  OUTPUT_VARIABLE GIT_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND
  git rev-parse --abbrev-ref HEAD
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_VERSION=\\\"${GIT_BRANCH}-${GIT_REVISION}\\\"")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_BUILD_TYPE=\\\"${CMAKE_BUILD_TYPE}\\\"")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_PLATFORM=\\\"${RUMBOOT_PLATFORM}\\\"")

message("\n\n ==== RumBoot Configuration summary ====")

message("Platform:         ${RUMBOOT_PLATFORM}")
message("Architecture:     ${RUMBOOT_ARCH}")
if (CROSS_COMPILE)
  message("Target triplet:   ${CROSS_COMPILE}")
endif()

message("BuildType:        ${CMAKE_BUILD_TYPE}")

if (DOXYGEN_FOUND)
  message("Doxygen Docs:     enabled")
else()
  message("Doxygen Docs:     disabled")
endif()

if (${RUMBOOT_DEBUG})
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DROMBOOT_DEBUG")
  SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -DROMBOOT_DEBUG")
  message("Debugging:        Enabled")
else()
  message("Debugging:        Disabled")
endif()


RUMBOOT_PLATFORM_PRINT_SUMMARY()

message("C Flags:          ${CMAKE_C_FLAGS}")
message("ASM Flags:        ${CMAKE_ASM_FLAGS}")

message(" ==== ==== ==== ==== ==== ==== ==== ====\n\n")

#Sanity checking. Production builds need these
if (${CMAKE_BUILD_TYPE} MATCHES "Production")
  if (${RUMBOOT_DEBUG})
    message( FATAL_ERROR "Fatal error: You want to ship DEBUG build to production. Srsly?")
  endif()
endif()


ADD_SOURCE("src/lib/crc32.c")
ADD_SOURCE("src/lib/logo.c")
ADD_SOURCE("src/lib/stdio.c")
ADD_SOURCE("src/lib/printf.c")
ADD_SOURCE("src/lib/testsuite.c")
ADD_LIBRARY(rumboot OBJECT ${PLATFORM_SOURCES})

if (${RUMBOOT_PLATFORM_HAS_LDS})
  string(REPLACE " " ";" CMAKE_C_FLAGS_SPLIT ${CMAKE_C_FLAGS})
  get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  foreach(dir ${dirs})
    SET(CMAKE_C_FLAGS_SPLIT "${CMAKE_C_FLAGS_SPLIT};-I${dir}")
  endforeach()
  add_custom_target(
      lds
      COMMAND ${CMAKE_C_COMPILER} -P -E ${CMAKE_C_FLAGS_SPLIT} -o lds/linker.lds  ${CMAKE_SOURCE_DIR}/lds/${RUMBOOT_PLATFORM}.lds.c
      COMMENT "Generating Platform LD Script"
    )
    add_dependencies(rumboot lds)
endif()


RUMBOOT_PLATFORM_ADD_COMPONENTS()


if (DOXYGEN_FOUND)
  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )

  add_custom_target(docs ALL COMMAND doxygen > doxygen.log
  )

endif()


SET(CPACK_GENERATOR "TGZ")
SET(CPACK_PACKAGE_FILE_NAME "rumboot-${CMAKE_BUILD_TYPE}")
SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
INCLUDE(CPack)
