add_custom_command(OUTPUT TW1PhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TW1Physics.h TW1PhysicsDict.cxx TW1Physics.rootmap libNPW1.dylib DEPENDS TW1Physics.h)
add_custom_command(OUTPUT TW1DataDict.cxx COMMAND ../../scripts/build_dict.sh TW1Data.h TW1DataDict.cxx TW1Data.rootmap libNPW1.dylib DEPENDS TW1Data.h)
add_library(NPW1 SHARED TW1Spectra.cxx TW1Data.cxx TW1Physics.cxx TW1DataDict.cxx TW1PhysicsDict.cxx )
target_link_libraries(NPW1 ${ROOT_LIBRARIES} NPCore) 
install(FILES TW1Data.h TW1Physics.h TW1Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

