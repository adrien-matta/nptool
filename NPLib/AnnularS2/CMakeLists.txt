add_custom_command(OUTPUT TS2PhysicsDict.cxx TS2PhysicsDict_rdict.pcm TS2Physics.rootmap COMMAND genreflex TS2Physics.h -o TS2PhysicsDict.cxx -I../Core --rootmap=TS2Physics.rootmap --rootmap-lib=libNPAnnularS2 --quiet DEPENDS TS2Physics.h)
add_custom_command(OUTPUT TS2DataDict.cxx TS2DataDict_rdict.pcm TS2Data.rootmap COMMAND genreflex TS2Data.h -o TS2DataDict.cxx -I../Core --rootmap=TS2Data.rootmap --rootmap-lib=libNPAnnularS2 --quiet DEPENDS TS2Data.h)
add_library(NPS2 SHARED TS2Data.cxx TS2Physics.cxx TS2DataDict.cxx TS2PhysicsDict.cxx )
target_link_libraries(NPS2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TS2Data.h TS2Physics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TS2Data.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TS2Physics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
