add_custom_command(OUTPUT TFatimaPhysicsDict.cxx TFatimaPhysicsDict_rdict.pcm TFatimaPhysics.rootmap COMMAND genreflex TFatimaPhysics.h -o TFatimaPhysicsDict.cxx -I../Core --rootmap=TFatimaPhysics.rootmap --rootmap-lib=libNPFatima --quiet DEPENDS TFatimaPhysics.h)
add_custom_command(OUTPUT TFatimaDataDict.cxx TFatimaDataDict_rdict.pcm TFatimaData.rootmap COMMAND genreflex TFatimaData.h -o TFatimaDataDict.cxx -I../Core --rootmap=TFatimaData.rootmap --rootmap-lib=libNPFatima --quiet DEPENDS TFatimaData.h)
add_library(NPFatima SHARED TFatimaData.cxx TFatimaPhysics.cxx TFatimaDataDict.cxx TFatimaPhysicsDict.cxx )
target_link_libraries(NPFatima ${ROOT_LIBRARIES} NPCore) 
install(FILES TFatimaData.h TFatimaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TFatimaData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TFatimaPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
