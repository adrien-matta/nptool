add_custom_command(OUTPUT TFatimaPhysicsDict.cxx TFatimaPhysicsDict_rdict.pcm TFatimaPhysics.rootmap COMMAND ../scripts/build_dict.sh TFatimaPhysics.h TFatimaPhysicsDict.cxx TFatimaPhysics.rootmap libNPFatima.dylib DEPENDS TFatimaPhysics.h)
add_custom_command(OUTPUT TFatimaDataDict.cxx TFatimaDataDict_rdict.pcm TFatimaData.rootmap COMMAND ../scripts/build_dict.sh TFatimaData.h TFatimaDataDict.cxx TFatimaData.rootmap libNPFatima.dylib DEPENDS TFatimaData.h)
add_library(NPFatima SHARED TFatimaData.cxx TFatimaPhysics.cxx TFatimaDataDict.cxx TFatimaPhysicsDict.cxx )
target_link_libraries(NPFatima ${ROOT_LIBRARIES} NPCore) 
install(FILES TFatimaData.h TFatimaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

