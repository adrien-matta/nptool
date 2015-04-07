add_custom_command(OUTPUT TMust2PhysicsDict.cxx TMust2PhysicsDict_rdict.pcm TMust2Physics.rootmap COMMAND ../scripts/build_dict.sh TMust2Physics.h TMust2PhysicsDict.cxx TMust2Physics.rootmap libNPMUST2.dylib DEPENDS TMust2Physics.h)
add_custom_command(OUTPUT TMust2DataDict.cxx TMust2DataDict_rdict.pcm TMust2Data.rootmap COMMAND ../scripts/build_dict.sh TMust2Data.h TMust2DataDict.cxx TMust2Data.rootmap libNPMUST2.dylib DEPENDS TMust2Data.h)
add_library(NPMUST2 SHARED TMust2Data.cxx TMust2Physics.cxx TMust2DataDict.cxx TMust2PhysicsDict.cxx TMust2Spectra.cxx)
target_link_libraries(NPMUST2 ${ROOT_LIBRARIES} NPCore) 

install(FILES TMust2Data.h TMust2Physics.h TMust2Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
