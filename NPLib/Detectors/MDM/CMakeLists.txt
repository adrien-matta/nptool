include(CheckLanguage)

add_custom_command(OUTPUT TMDMPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMDMPhysics.h TMDMPhysicsDict.cxx TMDMPhysics.rootmap libNPMDM.dylib DEPENDS TMDMPhysics.h)
add_custom_command(OUTPUT TMDMPhysicsMinimizerDict.cxx COMMAND ../../scripts/build_dict.sh TMDMPhysicsMinimizer.h TMDMPhysicsMinimizerDict.cxx TMDMPhysicsMinimizer.rootmap libNPMDM.dylib DEPENDS TMDMPhysicsMinimizer.h)
add_custom_command(OUTPUT TMDMDataDict.cxx COMMAND ../../scripts/build_dict.sh TMDMData.h TMDMDataDict.cxx TMDMData.rootmap libNPMDM.dylib DEPENDS TMDMData.h)

## Check for FORTRAN compiler, if so
## compile RAYTRACE code
if("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
  check_language(Fortran)
  if(CMAKE_Fortran_COMPILER)
    message(STATUS "Compiling libMDM with RAYTRACE support included.")
    add_definitions(-DUSE_RAYTRACE)
    enable_language(Fortran)
    set (CMAKE_Fortran_FLAGS "-finit-local-zero -falign-commons -fno-automatic")
    add_library(NPMDM SHARED TMDMSpectra.cxx TMDMData.cxx TMDMPhysics.cxx TMDMPhysicsMinimizer TMDMDataDict.cxx TMDMPhysicsDict.cxx TMDMPhysicsMinimizerDict.cxx MDMTrace.cpp RAYTKIN1.F)
  else()
## No fortran support, compile "fake" c-version
    message(STATUS "No Fortran support, disabling RAYTRACE in libMDM")
    add_library(NPMDM SHARED TMDMSpectra.cxx TMDMData.cxx TMDMPhysics.cxx TMDMPhysicsMinimizer TMDMDataDict.cxx TMDMPhysicsDict.cxx TMDMPhysicsMinimizerDict.cxx MDMTrace.cpp)
  endif()
else()
    message(STATUS "Fortran support only included with Unix Makefile generator, disabling RAYTRACE in libMDM.")
    add_library(NPMDM SHARED TMDMSpectra.cxx TMDMData.cxx TMDMPhysics.cxx TMDMPhysicsMinimizer TMDMDataDict.cxx TMDMPhysicsDict.cxx TMDMPhysicsMinimizerDict.cxx MDMTrace.cpp)
endif()      

target_link_libraries(NPMDM ${ROOT_LIBRARIES} NPCore NPPhysics Minuit2) 
install(FILES TMDMData.h TMDMPhysics.h TMDMPhysicsMinimizer.h TMDMSpectra.h MDMTrace.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

