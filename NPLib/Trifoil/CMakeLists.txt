add_custom_command(OUTPUT TTrifoilPhysicsDict.cxx TTrifoilPhysicsDict_rdict.pcm TTrifoilPhysics.rootmap COMMAND genreflex TTrifoilPhysics.h -o TTrifoilPhysicsDict.cxx -I../Core --rootmap=TTrifoilPhysics.rootmap --rootmap-lib=libNPTrifoil --quiet DEPENDS TTrifoilPhysics.h)
add_custom_command(OUTPUT TTrifoilDataDict.cxx TTrifoilDataDict_rdict.pcm TTrifoilData.rootmap COMMAND genreflex TTrifoilData.h -o TTrifoilDataDict.cxx -I../Core --rootmap=TTrifoilData.rootmap --rootmap-lib=libNPTrifoil --quiet DEPENDS TTrifoilData.h)
add_library(NPTrifoil SHARED TTrifoilData.cxx TTrifoilPhysics.cxx TTrifoilDataDict.cxx TTrifoilPhysicsDict.cxx )
target_link_libraries(NPTrifoil ${ROOT_LIBRARIES} NPCore) 
install(FILES TTrifoilData.h TTrifoilPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TTrifoilData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTrifoilPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
