cmake_minimum_required (VERSION 2.6)
project (KnoRBA-Common)

include ("cmake/setup-systemwide-include-dir.cmake")

set (KCOMMON_INCLUDE_DIR "${KSYSTEM_INCLUDE_DIR}/org/knorba/common")

file (MAKE_DIRECTORY ${KCOMMON_INCLUDE_DIR})

configure_file (src/mem.h "${KSYSTEM_INCLUDE_DIR}/mem.h" COPYONLY)

configure_file (src/Streamer.h "${KCOMMON_INCLUDE_DIR}/Streamer.h" COPYONLY)
configure_file (src/Logger.h   "${KCOMMON_INCLUDE_DIR}/Logger.h"   COPYONLY)

add_library (org-knorba-common STATIC
  src/Streamer.cpp
  src/Logger.cpp)

file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable (bin/TestLogger
  test/TestLogger.cpp)

target_link_libraries (bin/TestLogger
  org-knorba-common)
