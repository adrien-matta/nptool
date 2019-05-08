add_custom_command(OUTPUT TLightPipePhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TLightPipePhysics.h TLightPipePhysicsDict.cxx TLightPipePhysics.rootmap libNPLightPipe.dylib DEPENDS TLightPipePhysics.h)
add_custom_command(OUTPUT TLightPipeDataDict.cxx COMMAND ../../scripts/build_dict.sh TLightPipeData.h TLightPipeDataDict.cxx TLightPipeData.rootmap libNPLightPipe.dylib DEPENDS TLightPipeData.h)
add_library(NPLightPipe SHARED TLightPipeSpectra.cxx TLightPipeData.cxx TLightPipePhysics.cxx TLightPipeDataDict.cxx TLightPipePhysicsDict.cxx )
target_link_libraries(NPLightPipe ${ROOT_LIBRARIES} NPCore) 
install(FILES TLightPipeData.h TLightPipePhysics.h TLightPipeSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

