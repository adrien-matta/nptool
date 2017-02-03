add_custom_command(OUTPUT TSharcPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TSharcPhysics.h TSharcPhysicsDict.cxx TSharcPhysics.rootmap libNPSharc.dylib DEPENDS TSharcPhysics.h)
add_custom_command(OUTPUT TSharcDataDict.cxx COMMAND ../../scripts/build_dict.sh TSharcData.h TSharcDataDict.cxx TSharcData.rootmap libNPSharc.dylib DEPENDS TSharcData.h)
add_library(NPSharc SHARED TSharcSpectra.cxx TSharcData.cxx TSharcPhysics.cxx TSharcDataDict.cxx TSharcPhysicsDict.cxx )
target_link_libraries(NPSharc ${ROOT_LIBRARIES} NPCore) 
install(FILES TSharcData.h TSharcPhysics.h TSharcSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

