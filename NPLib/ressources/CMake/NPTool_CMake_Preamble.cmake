# Set usefull variable
set(NPTOOL "$ENV{NPTOOL}")
set(NPLIB "${NPTOOL}/NPLib")
set(NPTOOL_INCLUDE_DIR "${NPLIB}/include")
set(NPTOOL_LIB_DIR "${NPLIB}/lib")

# Look for Root   
include("${NPLIB}/ressources/CMake/Root.cmake")  

# Setting the policy to match Cmake version
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# This suppress the Up-to-Date message of file installed for cmake 3.1 and above
set(CMAKE_INSTALL_MESSAGE LAZY) 

# Set the Search path for library
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set NPLib/lib as one of the link directory
link_directories(${NPTOOL_LIB_DIR})

# Check for user disabling of c++11 support
string(COMPARE EQUAL "${CPP11}" "no" nocpp11)
if(nocpp11)
 message(" -> C++11 support disable")
endif()


# If the compiler is Clang, silence the unrecognised flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang.*")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments -fcolor-diagnostics")
  if( UNIX AND NOT APPLE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  endif()
  if( APPLE AND CLANG_VERSION_MAJOR VERSION_LESS 5 AND NOT nocpp11)
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()

# If compiler is GCC active the color diagnostic
if(${CMAKE_CXX_COMPILER_ID} MATCHES ".*GNU.*")
set(CMAKE_CXX_FLAGS "-march=native ${CMAKE_CXX_FLAGS} ")
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

# Adjust the compiler language flag
set(NOCPPFLAGS true)
if(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*11")
  set(NOCPPFLAGS false)
elseif(${CMAKE_CXX_FLAGS} MATCHES ".*std=.*0x")
  set(NOCPPFLAGS false) 
endif()

include(CheckCXXCompilerFlag)

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


