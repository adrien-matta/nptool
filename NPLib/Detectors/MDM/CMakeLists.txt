add_custom_command(OUTPUT TMDMPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMDMPhysics.h TMDMPhysicsDict.cxx TMDMPhysics.rootmap libNPMDM.dylib DEPENDS TMDMPhysics.h)
add_custom_command(OUTPUT TMDMDataDict.cxx COMMAND ../../scripts/build_dict.sh TMDMData.h TMDMDataDict.cxx TMDMData.rootmap libNPMDM.dylib DEPENDS TMDMData.h)
add_library(NPMDM SHARED TMDMSpectra.cxx TMDMData.cxx TMDMPhysics.cxx TMDMDataDict.cxx TMDMPhysicsDict.cxx )
target_link_libraries(NPMDM ${ROOT_LIBRARIES} NPCore) 
install(FILES TMDMData.h TMDMPhysics.h TMDMSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

