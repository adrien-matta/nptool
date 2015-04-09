# - Find ROOT instalation
# This module tries to find the ROOT installation on your system.
# It tries to find the root-config script which gives you all the needed information.
# If the system variable LOCAL_ROOTSYS is set this is straight forward.
# If not the module uses the pathes given in ROOT_CONFIG_SEARCHPATH.
# If you need an other path you should add this path to this varaible.  
# The root-config script is then used to detect basically everything else.
# This module defines a number of key variables and macros.

# F.Uhlig@gsi.de (fairroot.gsi.de)


MESSAGE(STATUS "Looking for Root...")

SET(ROOT_CONFIG_SEARCHPATH
  $ENV{ROOTSYS}/bin
   /usr/bin
)

SET(ROOT_DEFINITIONS "")

SET(ROOT_INSTALLED_VERSION_TOO_OLD FALSE)

SET(ROOT_CONFIG_EXECUTABLE ROOT_CONFIG_EXECUTABLE-NOTFOUND)

FIND_PROGRAM(ROOT_CONFIG_EXECUTABLE NAMES root-config PATHS
   ${ROOT_CONFIG_SEARCHPATH}
   NO_DEFAULT_PATH)
    
IF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
  MESSAGE( FATAL_ERROR "ROOT not installed in the searchpath and LOCAL_ROOTSYS is not set. Please
 set LOCAL_ROOTSYS or add the path to your ROOT installation in the Macro FindROOT.cmake in the
 subdirectory cmake/modules.")
ELSE (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
  STRING(REGEX REPLACE "(^.*)/bin/root-config" "\\1" test ${ROOT_CONFIG_EXECUTABLE}) 
  SET( ENV{LOCAL_ROOTSYS} ${test})
  set( LOCAL_ROOTSYS ${test})
ENDIF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")  

 
IF (ROOT_CONFIG_EXECUTABLE)
   
  SET(ROOT_FOUND FALSE)

  EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE ROOTVERSION)

  MESSAGE(STATUS "Looking for Root... - found $ENV{LOCAL_ROOTSYS}/bin/root")
  MESSAGE(STATUS "Looking for Root... - version ${ROOTVERSION} ")   

  # we need at least version 5.00/00
  IF (NOT ROOT_MIN_VERSION)
    SET(ROOT_MIN_VERSION "5.00/00")
  ENDIF (NOT ROOT_MIN_VERSION)
   
  # now parse the parts of the user given version string into variables
  STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+" "\\1" req_root_major_vers "${ROOT_MIN_VERSION}")
  STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" req_root_minor_vers "${ROOT_MIN_VERSION}")
  STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+)" "\\1" req_root_patch_vers "${ROOT_MIN_VERSION}")
   
  # and now the version string given by qmake
  STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" found_root_major_vers "${ROOTVERSION}")
  STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" found_root_minor_vers "${ROOTVERSION}")
  STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" found_root_patch_vers "${ROOTVERSION}")

  IF (found_root_major_vers LESS 5)
    MESSAGE( FATAL_ERROR "Invalid ROOT version \"${ROOTERSION}\", at least major version 4 is required, e.g. \"5.00/00\"")
  ENDIF (found_root_major_vers LESS 5)

  # compute an overall version number which can be compared at once
  MATH(EXPR req_vers "${req_root_major_vers}*10000 + ${req_root_minor_vers}*100 + ${req_root_patch_vers}")
  MATH(EXPR found_vers "${found_root_major_vers}*10000 + ${found_root_minor_vers}*100 + ${found_root_patch_vers}")
   
  IF (found_vers LESS req_vers)
    SET(ROOT_FOUND FALSE)
    SET(ROOT_INSTALLED_VERSION_TOO_OLD TRUE)
  ELSE (found_vers LESS req_vers)
    SET(ROOT_FOUND TRUE)
  ENDIF (found_vers LESS req_vers)

ENDIF (ROOT_CONFIG_EXECUTABLE)


IF (ROOT_FOUND)

  # ask root-config for the library dir
  # Set ROOT_LIBRARY_DIR

  EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
    ARGS "--libdir"
    OUTPUT_VARIABLE ROOT_LIBRARY_DIR_TMP )

  IF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
    SET(ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR_TMP} )
  ELSE(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
    MESSAGE("Warning: ROOT_CONFIG_EXECUTABLE reported ${ROOT_LIBRARY_DIR_TMP} as library path,")
    MESSAGE("Warning: but ${ROOT_LIBRARY_DIR_TMP} does NOT exist, ROOT must NOT be installed correctly.")
  ENDIF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
    
  # ask root-config for the binary dir
  EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE}
    ARGS "--bindir"
    OUTPUT_VARIABLE root_bins )
  SET(ROOT_BINARY_DIR ${root_bins})

  # ask root-config for the include dir
  EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
    ARGS "--incdir" 
    OUTPUT_VARIABLE root_headers )
  SET(ROOT_INCLUDE_DIR ${root_headers})
      # CACHE INTERNAL "")

  # ask root-config for the library varaibles
  EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
#    ARGS "--noldflags --noauxlibs --libs" 
    ARGS "--glibs" 
    OUTPUT_VARIABLE root_flags )

#  STRING(REGEX MATCHALL "([^ ])+"  root_libs_all ${root_flags})
#  STRING(REGEX MATCHALL "-L([^ ])+"  root_library ${root_flags})
#  REMOVE_FROM_LIST(root_flags "${root_libs_all}" "${root_library}")

  SET(ROOT_LIBRARIES ${root_flags})

  # Make variables changeble to the advanced user
  MARK_AS_ADVANCED( ROOT_LIBRARY_DIR ROOT_INCLUDE_DIR ROOT_DEFINITIONS)

  # Set ROOT_INCLUDES
  SET( ROOT_INCLUDES ${ROOT_INCLUDE_DIR})

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${ROOT_LIBRARY_DIR})

  #######################################
  #
  #       Check the executables of ROOT 
  #          ( rootcint ) 
  #
  #######################################

  FIND_PROGRAM(ROOT_CINT_EXECUTABLE
    NAMES rootcint
    PATHS ${ROOT_BINARY_DIR}
    NO_DEFAULT_PATH
    )

ENDIF (ROOT_FOUND)
