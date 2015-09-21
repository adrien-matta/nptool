# This file find your active root installation and use it for configuration
# Simplified version of the macro from Fair Root

message("Looking for Root:")
set(NPTOOL_ROOT_SEARCHPATH 
    $ENV{ROOTSYS}/bin 
    /usr/bin
    $ENV{PATH}
   )

find_program(NPTOOL_ROOT_CONFIG NAMES root-config PATHS ${NPTOOL_ROOT_SEARCHPATH} NO_DEFAULT_PATH)
if(NPTOOL_ROOT_CONFIG MATCHES "NOTFOUND")
  message(FATAL_ERROR "ROOT is not found, check your installation")
endif()

exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--version" OUTPUT_VARIABLE NPTOOL_ROOT_VERSION)
# now parse the parts of the user given version string into variables
string(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+" "\\1" req_root_major_vers "${ROOT_MIN_VERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" req_root_minor_vers "${ROOT_MIN_VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+)" "\\1" req_root_patch_vers "${ROOT_MIN_VERSION}")

# and now the version string given by qmake
string(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" found_root_major_vers "${ROOTVERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" found_root_minor_vers "${ROOTVERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" found_root_patch_vers "${ROOTVERSION}")


if(found_root_major_vers LESS 5)
  message( FATAL_ERROR "Invalid ROOT version \"${NPTOOL_ROOT_VERSION}\", at least major version 5 is required")
else()
  message(" Root ${NPTOOL_ROOT_VERSION} found")
endif()
 
## Collect the different information ##

# Lib
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--glibs" OUTPUT_VARIABLE ROOT_LIBRARIES)

# Remove unnecessary dependencies from the list return by root
# This actually improve slightly performances
string(REPLACE "-lGui" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lCore" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lRIO" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lNet" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lHist" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lGraf " " " ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lGraf3d" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lGpad" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lTree" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lRint" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lPostscript" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lMatrix" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lPhysics" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
#string(REPLACE "-lMathCore" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lThread" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")
string(REPLACE "-lpthread" "" ROOT_LIBRARIES "${ROOT_LIBRARIES}")

# Lib directories
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--libdir" OUTPUT_VARIABLE ROOT_LIBRARY_DIR)
link_directories( ${ROOT_LIBRARY_DIR})  

# Include directories
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--incdir" OUTPUT_VARIABLE ROOT_INCLUDE_DIR)
include_directories( ${ROOT_INCLUDE_DIR})  

# Get the compilator flag from root to assure consistancy
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--cflags" OUTPUT_VARIABLE NPTOOL_ROOT_CFLAGS )
if(CMAKE_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NPTOOL_ROOT_CFLAGS}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS_RELEASE} ${NPTOOL_ROOT_CFLAGS}")
endif()
