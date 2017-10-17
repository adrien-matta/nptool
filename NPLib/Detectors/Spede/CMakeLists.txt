add_custom_command(OUTPUT TSpedePhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TSpedePhysics.h TSpedePhysicsDict.cxx TSpedePhysics.rootmap libNPSpede.dylib DEPENDS TSpedePhysics.h)
add_custom_command(OUTPUT TSpedeDataDict.cxx COMMAND ../../scripts/build_dict.sh TSpedeData.h TSpedeDataDict.cxx TSpedeData.rootmap libNPSpede.dylib DEPENDS TSpedeData.h)
add_library(NPSpede SHARED TSpedeSpectra.cxx TSpedeData.cxx TSpedePhysics.cxx TSpedeDataDict.cxx TSpedePhysicsDict.cxx )
target_link_libraries(NPSpede ${ROOT_LIBRARIES} NPCore) 
install(FILES TSpedeData.h TSpedePhysics.h TSpedeSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

