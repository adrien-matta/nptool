add_custom_command(OUTPUT TS2PhysicsDict.cxx COMMAND ../scripts/build_dict.sh TS2Physics.h TS2PhysicsDict.cxx TS2Physics.rootmap libNPAnnularS2.dylib DEPENDS TS2Physics.h)
add_custom_command(OUTPUT TS2DataDict.cxx COMMAND ../scripts/build_dict.sh TS2Data.h TS2DataDict.cxx TS2Data.rootmap libNPAnnularS2.dylib DEPENDS TS2Data.h)
add_library(NPAnnularS2 SHARED TS2Data.cxx TS2Physics.cxx TS2DataDict.cxx TS2PhysicsDict.cxx )
target_link_libraries(NPAnnularS2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TS2Data.h TS2Physics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

