cmake_minimum_required (VERSION 2.8)
include(CheckCXXCompilerFlag)

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})


project (NPSimulation)
set (NPSIM_VERSION_MAJOR 2)
set (NPSIM_VERSION_MINOR 1)
set (NPSIM_VERSION_DETS 29)

set(CMAKE_BUILD_TYPE Release)

# Finding NPLib
find_package(NPLib)
include(${NPLib_USE_FILE})

#include the nptool standard CMake preamble 
include("../NPLib/ressources/CMake/NPTool_CMake_Preamble.cmake")

# Finding Geant 4
find_package(Geant4 REQUIRED ui_all vis_all)

# Rely on name of the file to set the version  
set(G4Version "${Geant4_VERSION}")
string(FIND "${G4Version}" "." pos)
string(SUBSTRING "${G4Version}" "0" "${pos}" NPS_GEANT4_VERSION_MAJOR)
string(REPLACE "${NPS_GEANT4_VERSION_MAJOR}." "" G4Version "${G4Version}")
string(FIND "${G4Version}" "." pos)
string(SUBSTRING "${G4Version}" "0" "${pos}" NPS_GEANT4_VERSION_MINOR)
string(REPLACE "${NPS_GEANT4_VERSION_MINOR}." "" G4Version "${G4Version}")
set(NPS_GEANT4_VERSION_PATCH "${G4Version}")

message ("Geant4 found, Version : ${NPS_GEANT4_VERSION_MAJOR}.${NPS_GEANT4_VERSION_MINOR}.${NPS_GEANT4_VERSION_PATCH}")

include(${Geant4_USE_FILE}) 
add_definitions(${Geant4_DEFINITIONS})    

configure_file(Core/NPSimulationVersion.hh.in Core/NPSimulationVersion.hh @ONLY) 

 # Detector List
set(DETLIST ${NPLib_DETECTOR_LIST})

string(COMPARE EQUAL "${DETLIST}" "" rdet)
if(rdet)
    message("Building all detectors")
  else()
    message("Building the following detectors ${DETLIST}") 
endif()

# Add root to the link and include directories

if(Geant4_gdml_FOUND)  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNPS_GDML")     
endif()


# set the Geant4 version info
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNPS_GEANT4_VERSION_MAJOR=${NPS_GEANT4_VERSION_MAJOR}") 

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics -Wno-deprecated-register -Wno-shadow")
endif()

set(CMAKE_BINARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/include )

# A macro that return the list of directory to compile
MACRO(subdirlist result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/Detectors/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      string(COMPARE EQUAL "${child}" "CMakeFiles" r0)
      string(COMPARE EQUAL "${child}" ".DS_Store" r1) 
      set(det "")
      string(REPLACE "Detectors/" "" det ${child})

      IF(NOT r0 AND NOT r1)
        string(COMPARE EQUAL "${DETLIST}" "" r2)
        IF(r2)
          LIST(APPEND dirlist ${child})
        ELSEIF(${DETLIST} MATCHES ".*${det}.*" )
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
configure_file(ressources/macro/gui.mac.in ressources/macro/gui.mac @ONLY) 
configure_file(ressources/macro/aliases.mac.in ressources/macro/aliases.mac @ONLY) 

# Generate vis.mac file if does not exist
if(EXISTS ressources/macro/vis.mac)
else()
  configure_file(ressources/macro/vis.mac.in ressources/macro/vis.mac @ONLY)
endif()

# Generate verbose.mac file if does not exist
if(EXISTS ressources/macro/verbose.mac)
else()
  configure_file(ressources/macro/verbose.mac.in ressources/macro/verbose.mac @ONLY)
endif()

# Generate PhysicsListOption file if does not exist
if(EXISTS PhysicsListOption.txt)
else()
  configure_file(ressources/input/PhysicsListOption.txt.in PhysicsListOption.txt @ONLY)
endif()


install(PROGRAMS npsimulation DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
