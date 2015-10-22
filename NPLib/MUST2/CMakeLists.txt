add_custom_command(OUTPUT TMUST2PhysicsDict.cxx COMMAND ../scripts/build_dict.sh TMUST2Physics.h TMUST2PhysicsDict.cxx TMUST2Physics.rootmap libNPMUST2.dylib DEPENDS TMUST2Physics.h)
add_custom_command(OUTPUT TMUST2DataDict.cxx COMMAND ../scripts/build_dict.sh TMUST2Data.h TMUST2DataDict.cxx TMUST2Data.rootmap libNPMUST2.dylib DEPENDS TMUST2Data.h)
add_library(NPMUST2 SHARED TMUST2Data.cxx TMUST2Physics.cxx TMUST2DataDict.cxx TMUST2PhysicsDict.cxx )
target_link_libraries(NPMUST2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TMUST2Data.h TMUST2Physics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

