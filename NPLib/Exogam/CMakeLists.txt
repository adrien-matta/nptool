add_custom_command(OUTPUT TExogamPhysicsDict.cxx TExogamPhysicsDict_rdict.pcm TExogamPhysics.rootmap COMMAND genreflex TExogamPhysics.h -o TExogamPhysicsDict.cxx -I../Core --rootmap=TExogamPhysics.rootmap --rootmap-lib=libNPExogam --quiet DEPENDS TExogamPhysics.h)
add_custom_command(OUTPUT TExogamDataDict.cxx TExogamDataDict_rdict.pcm TExogamData.rootmap COMMAND genreflex TExogamData.h -o TExogamDataDict.cxx -I../Core --rootmap=TExogamData.rootmap --rootmap-lib=libNPExogam --quiet DEPENDS TExogamData.h)
add_library(NPExogam SHARED TExogamData.cxx TExogamPhysics.cxx TExogamDataDict.cxx TExogamPhysicsDict.cxx TExogamSpectra.cxx)
target_link_libraries(NPExogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TExogamData.h TExogamPhysics.h TExogamSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TExogamData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TExogamPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
