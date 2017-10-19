## NEED FORTRAN ##
enable_language(Fortran)
set (CMAKE_Fortran_FLAGS "-finit-local-zero -falign-commons -fno-automatic")
##

add_custom_command(OUTPUT TMDMPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMDMPhysics.h TMDMPhysicsDict.cxx TMDMPhysics.rootmap libNPMDM.dylib DEPENDS TMDMPhysics.h)
add_custom_command(OUTPUT TMDMDataDict.cxx COMMAND ../../scripts/build_dict.sh TMDMData.h TMDMDataDict.cxx TMDMData.rootmap libNPMDM.dylib DEPENDS TMDMData.h)
add_library(NPMDM SHARED TMDMSpectra.cxx TMDMData.cxx TMDMPhysics.cxx TMDMDataDict.cxx TMDMPhysicsDict.cxx MDMTrace.cpp RAYTKIN1.F)
target_link_libraries(NPMDM ${ROOT_LIBRARIES} NPCore NPPhysics Minuit2) 
install(FILES TMDMData.h TMDMPhysics.h TMDMSpectra.h MDMTrace.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

