add_custom_command(OUTPUT TNeutronWallPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TNeutronWallPhysics.h TNeutronWallPhysicsDict.cxx TNeutronWallPhysics.rootmap libNPNeutronWall.dylib DEPENDS TNeutronWallPhysics.h)
add_custom_command(OUTPUT TNeutronWallDataDict.cxx COMMAND ../../scripts/build_dict.sh TNeutronWallData.h TNeutronWallDataDict.cxx TNeutronWallData.rootmap libNPNeutronWall.dylib DEPENDS TNeutronWallData.h)
add_library(NPNeutronWall SHARED TNeutronWallSpectra.cxx TNeutronWallData.cxx TNeutronWallPhysics.cxx TNeutronWallDataDict.cxx TNeutronWallPhysicsDict.cxx )
target_link_libraries(NPNeutronWall ${ROOT_LIBRARIES} NPCore) 
install(FILES TNeutronWallData.h TNeutronWallPhysics.h TNeutronWallSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

