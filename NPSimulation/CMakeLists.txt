cmake_minimum_required (VERSION 2.8)
include(CheckCXXCompilerFlag)

project (NPSimulation)
set (NPSIM_VERSION_MAJOR 2)
set (NPSIM_VERSION_MINOR 0)
set (NPSIM_VERSION_DETS 18)

set(CMAKE_BUILD_TYPE Release)

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# This suppress the Up-to-Date message of file installed for cmake 3.1 and above
set(CMAKE_INSTALL_MESSAGE LAZY) 

# Finding Geant 4
find_package(Geant4 REQUIRED ui_all vis_all)

# Rely on name of the file to set the version  
string(REGEX MATCH "Geant4-*.*.*/" G4Version "${Geant4_USE_FILE}")
string(REPLACE "Geant4-" "" G4Version "${G4Version}")
string(REPLACE "/" "" G4Version "${G4Version}")
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

# Finding NPLib
find_package(NPLib)
include(${NPLib_USE_FILE})

configure_file(Core/NPSimulationVersion.hh.in Core/NPSimulationVersion.hh @ONLY) 

# look for Root
include("../NPLib/ressources/CMake/Root.cmake")

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
  if( UNIX AND NOT APPLE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
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
      string(COMPARE EQUAL "${child}" ".tmp" r0)
      IF(NOT r0 )
        string(COMPARE EQUAL "${DETLIST}" "" r1)
        IF(r1)
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
configure_file(ressources/macro/gui.mac.in ressources/macro/gui.mac @ONLY) 
configure_file(ressources/macro/aliases.mac.in ressources/macro/aliases.mac @ONLY) 

install(PROGRAMS npsimulation DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
