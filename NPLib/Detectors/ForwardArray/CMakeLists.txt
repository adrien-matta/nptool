add_custom_command(OUTPUT TForwardArrayPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TForwardArrayPhysics.h TForwardArrayPhysicsDict.cxx TForwardArrayPhysics.rootmap libNPForwardArray.dylib DEPENDS TForwardArrayPhysics.h)
add_custom_command(OUTPUT TForwardArrayDataDict.cxx COMMAND ../../scripts/build_dict.sh TForwardArrayData.h TForwardArrayDataDict.cxx TForwardArrayData.rootmap libNPForwardArray.dylib DEPENDS TForwardArrayData.h)
add_library(NPForwardArray SHARED TForwardArraySpectra.cxx TForwardArrayData.cxx TForwardArrayPhysics.cxx TForwardArrayDataDict.cxx TForwardArrayPhysicsDict.cxx )
target_link_libraries(NPForwardArray ${ROOT_LIBRARIES} NPCore) 
install(FILES TForwardArrayData.h TForwardArrayPhysics.h TForwardArraySpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

