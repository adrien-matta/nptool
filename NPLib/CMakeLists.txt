cmake_minimum_required (VERSION 3.1)
include("FindROOT.cmake")
 
project (NPLib)
set (NPLIB_VERSION_MAJOR 2)
set (NPLIB_VERSION_MINOR 0)

set(CMAKE_BUILD_TYPE Release)

set(DETLIST ${ETLIST})

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
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
# Add each folder in the include path
#foreach(subdir ${SUB_DIRECTORY})
#  # Allow inclusion of project headers
#  include_directories(${subdir})
#endforeach() 
include_directories("Core/")

# Add each sub folder to the project
foreach(subdir ${SUB_DIRECTORY})
# add the subdirectory
  add_subdirectory(${subdir}) 
endforeach()


add_custom_target(LIBLIST ALL nptool-libmap ${DETLIST})
add_dependencies(LIBLIST nptool-libmap)

# Link executable against the local lib
#link_directories( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
