cmake_minimum_required (VERSION 2.6)
project (KnoRBA-Common)

include ("cmake/setup-systemwide-include-dir.cmake")

set (KCOMMON_INCLUDE_DIR "${KSYSTEM_INCLUDE_DIR}/org/knorba/common")

file (MAKE_DIRECTORY ${KCOMMON_INCLUDE_DIR})

configure_file (src/common.h   "${KCOMMON_INCLUDE_DIR}/common.h"      COPYONLY)
configure_file (src/Streamer.h "${KCOMMON_INCLUDE_DIR}/Streamer.h"    COPYONLY)
configure_file (src/Logger.h   "${KCOMMON_INCLUDE_DIR}/Logger.h"      COPYONLY)
configure_file (src/Timer.h    "${KCOMMON_INCLUDE_DIR}/Timer.h"       COPYONLY)
configure_file (src/ObjectDumpBuilder.h 
                               "${KCOMMON_INCLUDE_DIR}/ObjectDumpBuilder.h" 
                                                                      COPYONLY)
configure_file (src/ManagedPointer.h
                               "${KCOMMON_INCLUDE_DIR}/ManagedPointer.h" 
                                                                      COPYONLY)

add_library (org-knorba-common STATIC
  src/Streamer.cpp
  src/Logger.cpp
  src/Timer.cpp
 # src/ObjectDumpBuilder.cpp
 # src/ManagedPointer.cpp
  )

add_executable (TestLogger
  test/TestLogger.cpp)
target_link_libraries (TestLogger
  org-knorba-common)

#add_executable (TestObjectDumpBuilder
#  test/TestObjectDumpBuidler.cpp)
#target_link_libraries(TestObjectDumpBuilder
#  org-knorba-common)

set_target_properties(org-knorba-common TestLogger
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KSYSTEM_LIB_DIR}
  RUNTIME_OUTPUT_DIRECTORY ${KSYSTEM_BINARY_DIR})
