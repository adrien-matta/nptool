cmake_minimum_required (VERSION 2.8)
include("FindROOT.cmake")

project(NPLib)
# Major change in the Core/Physics (affecting the detector/analysis/simulation)
set(NPLIB_VERSION_MAJOR 2)
# Minor change in the Core/Physics (not affecting any other part)
set(NPLIB_VERSION_MINOR 0)
# Change any of the detector in NPA
set(NPLIB_VERSION_DETA 35)

configure_file(Core/NPLibVersion.h.in Core/NPLibVersion.h @ONLY)

set(CMAKE_BUILD_TYPE Release)

set(DETLIST ${ETLIST})

string(COMPARE EQUAL "${DETLIST}" "" rdet)
if(rdet)
    message("Building all detectors")
  else()
    message("Building the following detectors ${DETLIST}") 
endif()

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# Add root to the link and include directories
include_directories( ${ROOT_INCLUDE_DIR})
link_directories( ${ROOT_LIBRARY_DIR})

# Get the compilator flag from root to assure consistancy
EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE}
     ARGS "--cflags"
     OUTPUT_VARIABLE root_cflags )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${root_cflags}")

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
endif()

set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/include )

# A macro that return the list of directory to compile
MACRO(subdirlist result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      string(COMPARE EQUAL "${child}" ".tmp" r0)
      string(COMPARE EQUAL "${child}" "scripts" r1)
      string(COMPARE EQUAL "${child}" "include" r2)
      string(COMPARE EQUAL "${child}" "lib" r3)
      string(COMPARE EQUAL "${child}" "CMakeFiles" r4)
      string(COMPARE EQUAL "${child}" "Core" r5)
      string(COMPARE EQUAL "${child}" "Physics" r6)
      string(COMPARE EQUAL "${child}" "InitialConditions" r7)
      string(COMPARE EQUAL "${child}" "InteractionCoordinates" r8)
      string(COMPARE EQUAL "${child}" "Utility" r9)
      string(COMPARE EQUAL "${child}" "bin" r10)
      IF(NOT r0 AND NOT r1 AND NOT r2 AND NOT r3 AND NOT r4 AND NOT r5 AND NOT r6 AND NOT r7 AND NOT r8 AND NOT r9 AND NOT r10)
        string(COMPARE EQUAL "${DETLIST}" "" r11)
        IF(r11)
          LIST(APPEND dirlist ${child})
        ELSEIF(${DETLIST} MATCHES ".*${child}.*" )
          LIST(APPEND dirlist ${child}) 
        ENDIF()
      ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Call the Macro
subdirlist(SUB_DIRECTORY ${CMAKE_BINARY_DIR})
set(SUB_DIRECTORY ${SUB_DIRECTORY} Core Physics InitialConditions InteractionCoordinates Utility)
include_directories("Core/")
include_directories("Online/")

# Add each sub folder to the project
set(TARGET_LIST "")
foreach(subdir ${SUB_DIRECTORY})
# add the subdirectory
  add_subdirectory(${subdir})
  string(COMPARE EQUAL "${subdir}" "Utility" r0)
  if(NOT r0)
    LIST(APPEND TARGET_LIST NP${subdir})
  endif()
endforeach()

export(TARGETS ${TARGET_LIST} FILE NPLibTargets.cmake)
export(PACKAGE NPLib)
file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}")
set(CONF_INCLUDE_DIRS "./" "./")
configure_file(NPLibConfig.cmake.in "${PROJECT_BINARY_DIR}/NPLibConfig.cmake" @ONLY)

install(SCRIPT scripts/post_install.cmake ${DETLIST})
