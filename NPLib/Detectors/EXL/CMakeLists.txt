add_custom_command(OUTPUT TExlPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TExlPhysics.h TExlPhysicsDict.cxx TExlPhysics.rootmap libNPEXL.dylib DEPENDS TExlPhysics.h)
add_custom_command(OUTPUT TExlDataDict.cxx COMMAND ../../scripts/build_dict.sh TExlData.h TExlDataDict.cxx TExlData.rootmap libNPEXL.dylib DEPENDS TExlData.h)
add_library(NPEXL SHARED TExlData.cxx TExlPhysics.cxx TExlDataDict.cxx TExlPhysicsDict.cxx )
target_link_libraries(NPEXL ${ROOT_LIBRARIES} NPCore) 
install(FILES TExlData.h TExlPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

