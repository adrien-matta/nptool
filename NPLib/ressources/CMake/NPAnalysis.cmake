cmake_minimum_required (VERSION 2.8) 
project (NPAnalysis)
set(CMAKE_BUILD_TYPE Release)  
# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

#Finding NPTool
find_package(NPLib)
include(${NPLib_USE_FILE})

#include the nptool standard CMake preamble 
include("${NPLIB}/ressources/CMake/NPTool_CMake_Preamble.cmake")

# allow link to not care for undefined reference
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics -undefined dynamic_lookup")
endif()

add_library(NPAnalysis SHARED Analysis.cxx)
target_link_libraries(NPAnalysis ${ROOT_LIBRARIES} -L${NPLIB}/lib -lNPCore -lNPPhysics -lNPInteractionCoordinates -lNPInitialConditions)
