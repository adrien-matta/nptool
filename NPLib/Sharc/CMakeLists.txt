add_custom_command(OUTPUT TSharcPhysicsDict.cxx TSharcPhysicsDict_rdict.pcm TSharcPhysics.rootmap COMMAND genreflex TSharcPhysics.h -o TSharcPhysicsDict.cxx -I../Core --rootmap=TSharcPhysics.rootmap --rootmap-lib=libNPSharc --quiet DEPENDS TSharcPhysics.h)
add_custom_command(OUTPUT TSharcDataDict.cxx TSharcDataDict_rdict.pcm TSharcData.rootmap COMMAND genreflex TSharcData.h -o TSharcDataDict.cxx -I../Core --rootmap=TSharcData.rootmap --rootmap-lib=libNPSharc --quiet DEPENDS TSharcData.h)
add_library(NPSharc SHARED TSharcData.cxx TSharcPhysics.cxx TSharcDataDict.cxx TSharcPhysicsDict.cxx )
target_link_libraries(NPSharc ${ROOT_LIBRARIES} NPCore) 
install(FILES TSharcData.h TSharcPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TSharcData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TSharcPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
