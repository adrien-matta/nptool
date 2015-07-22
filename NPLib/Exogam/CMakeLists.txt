add_custom_command(OUTPUT TExogamPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TExogamPhysics.h TExogamPhysicsDict.cxx TExogamPhysics.rootmap libNPExogam.dylib DEPENDS TExogamPhysics.h)
add_custom_command(OUTPUT TExogamDataDict.cxx COMMAND ../scripts/build_dict.sh TExogamData.h TExogamDataDict.cxx TExogamData.rootmap libNPExogam.dylib DEPENDS TExogamData.h)
add_library(NPExogam SHARED TExogamData.cxx TExogamPhysics.cxx TExogamDataDict.cxx TExogamPhysicsDict.cxx TExogamSpectra.cxx)
target_link_libraries(NPExogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TExogamData.h TExogamPhysics.h TExogamSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

