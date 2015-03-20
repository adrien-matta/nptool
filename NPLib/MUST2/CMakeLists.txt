add_custom_command(OUTPUT TMust2PhysicsDict.cxx TMust2PhysicsDict_rdict.pcm TMust2Physics.rootmap COMMAND genreflex TMust2Physics.h -o TMust2PhysicsDict.cxx -I../Core --rootmap=TMust2Physics.rootmap --rootmap-lib=libNPMUST2 --quiet DEPENDS TMust2Physics.h)
add_custom_command(OUTPUT TMust2DataDict.cxx TMust2DataDict_rdict.pcm TMust2Data.rootmap COMMAND genreflex TMust2Data.h -o TMust2DataDict.cxx -I../Core --rootmap=TMust2Data.rootmap --rootmap-lib=libNPMUST2 --quiet DEPENDS TMust2Data.h)
add_library(NPMust2 SHARED TMust2Data.cxx TMust2Physics.cxx TMust2DataDict.cxx TMust2PhysicsDict.cxx TMust2Spectra.cxx)
target_link_libraries(NPMust2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TMust2Data.h TMust2Physics.h TMust2Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TMust2Data.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMust2Physics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
