add_custom_command(OUTPUT TAGATAPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TAGATAPhysics.h TAGATAPhysicsDict.cxx TAGATAPhysics.rootmap libNPAGATA.dylib DEPENDS TAGATAPhysics.h)
add_custom_command(OUTPUT TAGATADataDict.cxx COMMAND ../../scripts/build_dict.sh TAGATAData.h TAGATADataDict.cxx TAGATAData.rootmap libNPAGATA.dylib DEPENDS TAGATAData.h)
add_library(NPAGATA SHARED TAGATASpectra.cxx TAGATAData.cxx TAGATAPhysics.cxx TAGATADataDict.cxx TAGATAPhysicsDict.cxx )
target_link_libraries(NPAGATA ${ROOT_LIBRARIES} NPCore) 
install(FILES TAGATAData.h TAGATAPhysics.h TAGATASpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

