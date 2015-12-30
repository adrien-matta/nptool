add_custom_command(OUTPUT TSpegPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TSpegPhysics.h TSpegPhysicsDict.cxx TSpegPhysics.rootmap libNPSPEG.dylib DEPENDS TSpegPhysics.h)
add_custom_command(OUTPUT TSpegPlasticDataDict.cxx COMMAND ../../scripts/build_dict.sh TSpegPlasticData.h TSpegPlasticDataDict.cxx TSpegPlasticData.rootmap libNPSPEG.dylib DEPENDS TSpegPlasticData.h)
add_custom_command(OUTPUT TSpegDCDataDict.cxx COMMAND ../../scripts/build_dict.sh TSpegDCData.h TSpegDCDataDict.cxx TSpegDCData.rootmap libNPSPEG.dylib DEPENDS TSpegDCData.h)
add_custom_command(OUTPUT TSpegCHIODataDict.cxx COMMAND ../../scripts/build_dict.sh TSpegCHIOData.h TSpegCHIODataDict.cxx TSpegCHIOData.rootmap libNPSPEG.dylib DEPENDS TSpegCHIOData.h)
add_library(NPSPEG SHARED TSpegCHIOData.cxx TSpegDCData.cxx TSpegPlasticData.cxx TSpegPhysics.cxx TSpegCHIODataDict.cxx TSpegDCDataDict.cxx TSpegPlasticDataDict.cxx TSpegPhysicsDict.cxx )
target_link_libraries(NPSPEG ${ROOT_LIBRARIES} NPCore) 
install(FILES TSpegCHIOData.h TSpegDCData.h TSpegPlasticData.h TSpegPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

