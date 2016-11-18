add_custom_command(OUTPUT TFPDTamuPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TFPDTamuPhysics.h TFPDTamuPhysicsDict.cxx TFPDTamuPhysics.rootmap libNPFPDTamu.dylib DEPENDS TFPDTamuPhysics.h)
add_custom_command(OUTPUT TFPDTamuDataDict.cxx COMMAND ../../scripts/build_dict.sh TFPDTamuData.h TFPDTamuDataDict.cxx TFPDTamuData.rootmap libNPFPDTamu.dylib DEPENDS TFPDTamuData.h)
add_library(NPFPDTamu SHARED TFPDTamuSpectra.cxx TFPDTamuData.cxx TFPDTamuPhysics.cxx TFPDTamuDataDict.cxx TFPDTamuPhysicsDict.cxx )
target_link_libraries(NPFPDTamu ${ROOT_LIBRARIES} NPCore) 
install(FILES TFPDTamuData.h TFPDTamuPhysics.h TFPDTamuSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

