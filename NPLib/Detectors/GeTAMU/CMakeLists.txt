add_custom_command(OUTPUT TGeTAMUPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TGeTAMUPhysics.h TGeTAMUPhysicsDict.cxx TGeTAMUPhysics.rootmap libNPGeTAMU.dylib DEPENDS TGeTAMUPhysics.h)
add_custom_command(OUTPUT TGeTAMUDataDict.cxx COMMAND ../../scripts/build_dict.sh TGeTAMUData.h TGeTAMUDataDict.cxx TGeTAMUData.rootmap libNPGeTAMU.dylib DEPENDS TGeTAMUData.h)
add_library(NPGeTAMU SHARED TGeTAMUData.cxx TGeTAMUPhysics.cxx TGeTAMUDataDict.cxx TGeTAMUPhysicsDict.cxx )
target_link_libraries(NPGeTAMU ${ROOT_LIBRARIES} NPCore) 
install(FILES TGeTAMUData.h TGeTAMUPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

