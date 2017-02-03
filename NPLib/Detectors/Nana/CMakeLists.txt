add_custom_command(OUTPUT TNanaPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TNanaPhysics.h TNanaPhysicsDict.cxx TNanaPhysics.rootmap libNPNana.dylib DEPENDS TNanaPhysics.h)
add_custom_command(OUTPUT TNanaDataDict.cxx COMMAND ../../scripts/build_dict.sh TNanaData.h TNanaDataDict.cxx TNanaData.rootmap libNPNana.dylib DEPENDS TNanaData.h)
add_library(NPNana SHARED TNanaData.cxx TNanaPhysics.cxx TNanaDataDict.cxx TNanaPhysicsDict.cxx )
target_link_libraries(NPNana ${ROOT_LIBRARIES} NPCore) 
install(FILES TNanaData.h TNanaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

