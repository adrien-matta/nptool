cmake_minimum_required (VERSION 2.8) 
project (NPAnalysis)
set(CMAKE_BUILD_TYPE Release)  

#Finding NPTool
find_package(NPLib)
include(${NPLib_USE_FILE})

#include the nptool standard CMake preamble 
include("${NPLIB}/ressources/CMake/NPTool_CMake_Preamble.cmake")

add_library(NPAnalysis SHARED Analysis.cxx)
target_link_libraries(NPAnalysis ${ROOT_LIBRARIES} -L${NPLIB}/lib -lNPCore -lNPPhysics -lNPInteractionCoordinates -lNPInitialConditions)
