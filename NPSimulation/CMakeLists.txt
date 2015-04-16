cmake_minimum_required (VERSION 2.8)
include("../NPLib/FindROOT.cmake")

project (NPSimulation)
set (NPSimulation_VERSION_MAJOR 2)
set (NPSimulation_VERSION_MINOR 0)

set(CMAKE_BUILD_TYPE Release)

# Finding Geant 4
find_package(Geant4 REQUIRED ui_all vis_all)
message(${Geant4_USE_FILE})
include(${Geant4_USE_FILE}) 

# Finding NPLib
find_package(NPLib)
include(${NPLib_USE_FILE})

set(DETLIST ${NPLib_DETECTOR_LIST})

if("${DETLIST}" MATCHES "")
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics -Wno-deprecated-register")
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
      string(COMPARE EQUAL "${child}" "lib" r1)
      string(COMPARE EQUAL "${child}" "scripts" r2)
      string(COMPARE EQUAL "${child}" "CMakeFiles" r3)
      string(COMPARE EQUAL "${child}" "Core" r4)
      string(COMPARE EQUAL "${child}" "bin" r5)
      string(COMPARE EQUAL "${child}" "macro" r6)
      string(COMPARE EQUAL "${child}" "icons" r7)
      IF(NOT r0 AND NOT r1 AND NOT r2 AND NOT r3 AND NOT r4 AND NOT r5 AND NOT r6 AND NOT r7)
        string(COMPARE EQUAL "${DETLIST}" "" r100)
        IF(r100)
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
set(SUB_DIRECTORY ${SUB_DIRECTORY} Core)
include_directories("Core/")

# Add each sub folder to the project
foreach(subdir ${SUB_DIRECTORY})
# add the subdirectory
  add_subdirectory(${subdir}) 
 endforeach()

add_executable(npsimulation Simulation.cc)
target_Link_libraries(npsimulation ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPSCore)
configure_file(macro/gui.mac.in macro/gui.mac @ONLY) 
configure_file(macro/aliases.mac.in macro/aliases.mac @ONLY) 

install(PROGRAMS npsimulation DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
