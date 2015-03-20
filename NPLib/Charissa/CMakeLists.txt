add_custom_command(OUTPUT TCharissaPhysicsDict.cxx TCharissaPhysicsDict_rdict.pcm TCharissaPhysics.rootmap COMMAND genreflex TCharissaPhysics.h -o TCharissaPhysicsDict.cxx -I../Core --rootmap=TCharissaPhysics.rootmap --rootmap-lib=libNPCharissa --quiet DEPENDS TCharissaPhysics.h)
add_custom_command(OUTPUT TCharissaDataDict.cxx TCharissaDataDict_rdict.pcm TCharissaData.rootmap COMMAND genreflex TCharissaData.h -o TCharissaDataDict.cxx -I../Core --rootmap=TCharissaData.rootmap --rootmap-lib=libNPCharissa --quiet DEPENDS TCharissaData.h)
add_library(NPCharissa SHARED TCharissaData.cxx TCharissaPhysics.cxx TCharissaDataDict.cxx TCharissaPhysicsDict.cxx TCharissaSpectra.cxx)
target_link_libraries(NPCharissa ${ROOT_LIBRARIES} NPCore) 
install(FILES TCharissaData.h TCharissaPhysics.h TCharissaSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TCharissaData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TCharissaPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
