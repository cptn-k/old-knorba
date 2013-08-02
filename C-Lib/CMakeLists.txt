cmake_minimum_required (VERSION 2.6)
project (KnoRBA-C-Lib)

include ("../Common/cmake/setup-systemwide-include-dir.cmake")

configure_file (include/knorba.h "${KNORBA_INCLUDE_DIR}/knorba.h" COPYONLY)
