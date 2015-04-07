add_custom_command(OUTPUT TSharcPhysicsDict.cxx TSharcPhysicsDict_rdict.pcm TSharcPhysics.rootmap COMMAND ../scripts/build_dict.sh TSharcPhysics.h TSharcPhysicsDict.cxx TSharcPhysics.rootmap libNPSharc.dylib DEPENDS TSharcPhysics.h)
add_custom_command(OUTPUT TSharcDataDict.cxx TSharcDataDict_rdict.pcm TSharcData.rootmap COMMAND ../scripts/build_dict.sh TSharcData.h TSharcDataDict.cxx TSharcData.rootmap libNPSharc.dylib DEPENDS TSharcData.h)
add_library(NPSharc SHARED TSharcData.cxx TSharcPhysics.cxx TSharcDataDict.cxx TSharcPhysicsDict.cxx )
target_link_libraries(NPSharc ${ROOT_LIBRARIES} NPCore) 
install(FILES TSharcData.h TSharcPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

