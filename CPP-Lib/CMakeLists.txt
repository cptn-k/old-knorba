cmake_minimum_required (VERSION 2.6)
project (KnoRBA-CPP-Lib)

include ("../Common/cmake/setup-systemwide-include-dir.cmake")

add_subdirectory (../C-Lib lib/KnoRBA-C-Lib)

configure_file (include/knorba.hpp "${KNORBA_INCLUDE_DIR}/knorba.hpp" COPYONLY)
