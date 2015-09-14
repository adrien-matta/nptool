cmake_minimum_required (VERSION 2.8)
include(CheckCXXCompilerFlag)
project(NPLib CXX)
# Major change in the Core/Physics (affecting the detector/analysis/simulation)
set(NPLIB_VERSION_MAJOR 2)
# Minor change in the Core/Physics (not affecting any other part)
set(NPLIB_VERSION_MINOR 1)
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

# This suppress the Up-to-Date message of file installed for cmake 3.1 and above
set(CMAKE_INSTALL_MESSAGE LAZY) 
message("${CMAKE_CXX_FLAGS}")

# look for Root
include("Root.cmake")

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
  if( UNIX AND NOT APPLE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()


# Adjust the compiler language flag
set(NOCPPFLAGS true)
if(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*11")
  set(NOCPPFLAGS false)
elseif(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*0x")
  set(NOCPPFLAGS false) 
endif()

if(NOCPPFLAGS)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  if(COMPILER_SUPPORTS_CXX11)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  endif()

  if(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
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
      string(COMPARE EQUAL "${child}" "Calibration" r11)
      string(COMPARE EQUAL "${child}" "Online" r111)
      IF(NOT r0 AND NOT r1 AND NOT r2 AND NOT r3 AND NOT r4 AND NOT r5 AND NOT r6 AND NOT r7 AND NOT r8 AND NOT r9 AND NOT r10 AND NOT r11 AND NOT r111)
        string(COMPARE EQUAL "${DETLIST}" "" r12)
        IF(r12)
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
set(SUB_DIRECTORY ${SUB_DIRECTORY} Core Physics Calibration InitialConditions InteractionCoordinates Online Utility)
include_directories("Core/")
include_directories("Physics/")
include_directories("Online/")

# Add each sub folder to the project
set(TARGET_LIST "")
foreach(subdir ${SUB_DIRECTORY})
# add the subdirectory
  add_subdirectory(${subdir})
  string(COMPARE EQUAL "${subdir}" "Utility" r0)
  string(COMPARE EQUAL "${subdir}" "Maya" r1)

  if(NOT r0 AND NOT r1)
    LIST(APPEND TARGET_LIST NP${subdir})
  endif()
  if(r1 AND DEFINED ENV{ROOTSYS})
	LIST(APPEND TARGET_LIST NP${subdir})
  endif()

endforeach()
export(TARGETS ${TARGET_LIST} FILE NPLibTargets.cmake)
export(PACKAGE NPLib)
file(RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}")
set(CONF_INCLUDE_DIRS "./" "./")
configure_file(NPLibConfig.cmake.in "${PROJECT_BINARY_DIR}/NPLibConfig.cmake" @ONLY)

