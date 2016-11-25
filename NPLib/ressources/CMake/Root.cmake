# This file find your active root installation and use it for configuration
# Simplified version of the macro from Fair Root

message("Looking for Root:")
set(NPTOOL_ROOT_SEARCHPATH 
    $ENV{ROOTSYS}/bin 
    $ENV{PATH}
    /usr/bin  # Debian install
    /opt/local/bin # Macports install
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

# List of Root dependencies
#set(ROOT_LIBRARIES dl Gui Core RIO Net Hist Gpad Tree Physics MathCore Thread )
set(ROOT_LIBRARIES dl Gui Core Net Gpad Hist Physics Tree )

# Lib directories
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--libdir" OUTPUT_VARIABLE ROOT_LIBRARY_DIR)
link_directories( ${ROOT_LIBRARY_DIR})  

# Include directories
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--incdir" OUTPUT_VARIABLE ROOT_INCLUDE_DIR)
include_directories(SYSTEM ${ROOT_INCLUDE_DIR})  

# Get the compilator flag from root to assure consistancy
exec_program(${NPTOOL_ROOT_CONFIG} ARGS "--cflags" OUTPUT_VARIABLE NPTOOL_ROOT_CFLAGS )
if(CMAKE_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NPTOOL_ROOT_CFLAGS}")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS_RELEASE} ${NPTOOL_ROOT_CFLAGS}")
endif()
