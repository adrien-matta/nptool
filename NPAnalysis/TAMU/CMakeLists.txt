cmake_minimum_required (VERSION 2.8) 
#Finding NPTool
set(NPTOOL "$ENV{NPTOOL}")
set(NPLIB "${NPTOOL}/NPLib")
set(NPTOOL_INCLUDE_DIR "${NPLIB}/include")
set(NPTOOL_LIB_DIR "${NPLIB}/lib")
  
include("${NPLIB}/Root.cmake")  

project (NPAnalysis)
set(CMAKE_BUILD_TYPE Release)  
# Add root to the link and include directories
include_directories( ${ROOT_INCLUDE_DIR})
link_directories( ${ROOT_LIBRARY_DIR})
include_directories( ${NPTOOL_INCLUDE_DIR})
link_directories( ${NPTOOL_LIB_DIR})

# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -undefined dynamic_lookup")
endif()

add_library(NPAnalysis SHARED Analysis.cxx)
target_link_libraries(NPAnalysis ${ROOT_LIBRARIES} -L${NPLIB}/lib -lNPCore -lNPPhysics -lNPInitialConditions -lNPInteractionCoordinates)
