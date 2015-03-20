add_custom_command(OUTPUT TPlasticPhysicsDict.cxx TPlasticPhysicsDict_rdict.pcm TPlasticPhysics.rootmap COMMAND genreflex TPlasticPhysics.h -o TPlasticPhysicsDict.cxx -I../Core --rootmap=TPlasticPhysics.rootmap --rootmap-lib=libNPPlastic --quiet DEPENDS TPlasticPhysics.h)
add_custom_command(OUTPUT TPlasticDataDict.cxx TPlasticDataDict_rdict.pcm TPlasticData.rootmap COMMAND genreflex TPlasticData.h -o TPlasticDataDict.cxx -I../Core --rootmap=TPlasticData.rootmap --rootmap-lib=libNPPlastic --quiet DEPENDS TPlasticData.h)
add_library(NPPlastic SHARED TPlasticData.cxx TPlasticPhysics.cxx TPlasticDataDict.cxx TPlasticPhysicsDict.cxx )
target_link_libraries(NPPlastic ${ROOT_LIBRARIES} NPCore) 
install(FILES TPlasticData.h TPlasticPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TPlasticData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TPlasticPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
