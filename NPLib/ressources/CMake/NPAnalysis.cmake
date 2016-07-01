cmake_minimum_required (VERSION 2.8) 
#Finding NPTool
set(NPTOOL "$ENV{NPTOOL}")
set(NPLIB "${NPTOOL}/NPLib")
set(NPTOOL_INCLUDE_DIR "${NPLIB}/include")
set(NPTOOL_LIB_DIR "${NPLIB}/lib")
  
include("${NPLIB}/ressources/CMake/Root.cmake")  

include(CheckCXXCompilerFlag)
project (NPAnalysis)
set(CMAKE_BUILD_TYPE Release)  

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# This suppress the Up-to-Date message of file installed for cmake 3.1 and above
set(CMAKE_INSTALL_MESSAGE LAZY) 



# Add root to the link and include directories
include_directories( ${ROOT_INCLUDE_DIR})
link_directories( ${ROOT_LIBRARY_DIR})
include_directories( ${NPTOOL_INCLUDE_DIR})
link_directories( ${NPTOOL_LIB_DIR})

# Check for user disabling of c++11 support
string(COMPARE EQUAL "${CPP11}" "no" nocpp11)
if(nocpp11)
 message(" -> C++11 support disable")
endif()


# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics -undefined dynamic_lookup")
  if( UNIX AND NOT APPLE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  endif()
  if( APPLE AND CLANG_VERSION_MAJOR VERSION_LESS 5 AND NOT nocpp11)
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
  if(COMPILER_SUPPORTS_CXX11 AND NOT nocpp11)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  endif()

  if(COMPILER_SUPPORTS_CXX0X AND NOT nocpp11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()


add_library(NPAnalysis SHARED Analysis.cxx)
target_link_libraries(NPAnalysis ${ROOT_LIBRARIES} -L${NPLIB}/lib -lNPCore -lNPPhysics -lNPInteractionCoordinates -lNPInitialConditions)
