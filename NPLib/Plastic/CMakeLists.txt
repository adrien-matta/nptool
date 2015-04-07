add_custom_command(OUTPUT TPlasticPhysicsDict.cxx TPlasticPhysicsDict_rdict.pcm TPlasticPhysics.rootmap COMMAND ../scripts/build_dict.sh TPlasticPhysics.h TPlasticPhysicsDict.cxx TPlasticPhysics.rootmap libNPPlastic.dylib DEPENDS TPlasticPhysics.h)
add_custom_command(OUTPUT TPlasticDataDict.cxx TPlasticDataDict_rdict.pcm TPlasticData.rootmap COMMAND ../scripts/build_dict.sh TPlasticData.h TPlasticDataDict.cxx TPlasticData.rootmap libNPPlastic.dylib DEPENDS TPlasticData.h)
add_library(NPPlastic SHARED TPlasticData.cxx TPlasticPhysics.cxx TPlasticDataDict.cxx TPlasticPhysicsDict.cxx )
target_link_libraries(NPPlastic ${ROOT_LIBRARIES} NPCore) 
install(FILES TPlasticData.h TPlasticPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

