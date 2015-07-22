add_custom_command(OUTPUT TSSSDPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TSSSDPhysics.h TSSSDPhysicsDict.cxx TSSSDPhysics.rootmap libNPSSSD.dylib DEPENDS TSSSDPhysics.h)
add_custom_command(OUTPUT TSSSDDataDict.cxx COMMAND ../scripts/build_dict.sh TSSSDData.h TSSSDDataDict.cxx TSSSDData.rootmap libNPSSSD.dylib DEPENDS TSSSDData.h)
add_library(NPSSSD SHARED TSSSDData.cxx TSSSDPhysics.cxx TSSSDDataDict.cxx TSSSDPhysicsDict.cxx )
target_link_libraries(NPSSSD ${ROOT_LIBRARIES} NPCore) 
install(FILES TSSSDData.h TSSSDPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

