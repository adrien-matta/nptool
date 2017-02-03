add_custom_command(OUTPUT THelios2PhysicsDict.cxx COMMAND ../../scripts/build_dict.sh THelios2Physics.h THelios2PhysicsDict.cxx THelios2Physics.rootmap libNPHelios2.dylib DEPENDS THelios2Physics.h)
add_custom_command(OUTPUT THelios2DataDict.cxx COMMAND ../../scripts/build_dict.sh THelios2Data.h THelios2DataDict.cxx THelios2Data.rootmap libNPHelios2.dylib DEPENDS THelios2Data.h)
add_library(NPHelios2 SHARED THelios2Spectra.cxx THelios2Data.cxx THelios2Physics.cxx THelios2DataDict.cxx THelios2PhysicsDict.cxx )
target_link_libraries(NPHelios2 ${ROOT_LIBRARIES} NPCore) 
install(FILES THelios2Data.h THelios2Physics.h THelios2Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

