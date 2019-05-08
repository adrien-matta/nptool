add_custom_command(OUTPUT Tbeam_dumpPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh Tbeam_dumpPhysics.h Tbeam_dumpPhysicsDict.cxx Tbeam_dumpPhysics.rootmap libNPbeam_dump.dylib DEPENDS Tbeam_dumpPhysics.h)
add_custom_command(OUTPUT Tbeam_dumpDataDict.cxx COMMAND ../../scripts/build_dict.sh Tbeam_dumpData.h Tbeam_dumpDataDict.cxx Tbeam_dumpData.rootmap libNPbeam_dump.dylib DEPENDS Tbeam_dumpData.h)
add_library(NPbeam_dump SHARED Tbeam_dumpSpectra.cxx Tbeam_dumpData.cxx Tbeam_dumpPhysics.cxx Tbeam_dumpDataDict.cxx Tbeam_dumpPhysicsDict.cxx )
target_link_libraries(NPbeam_dump ${ROOT_LIBRARIES} NPCore) 
install(FILES Tbeam_dumpData.h Tbeam_dumpPhysics.h Tbeam_dumpSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

