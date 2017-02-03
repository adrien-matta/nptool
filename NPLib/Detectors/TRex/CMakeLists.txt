add_custom_command(OUTPUT TTRexPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TTRexPhysics.h TTRexPhysicsDict.cxx TTRexPhysics.rootmap libNPTRex.dylib DEPENDS TTRexPhysics.h)
add_custom_command(OUTPUT TTRexDataDict.cxx COMMAND ../../scripts/build_dict.sh TTRexData.h TTRexDataDict.cxx TTRexData.rootmap libNPTRex.dylib DEPENDS TTRexData.h)
add_library(NPTRex SHARED TTRexSpectra.cxx TTRexData.cxx TTRexPhysics.cxx TTRexDataDict.cxx TTRexPhysicsDict.cxx )
target_link_libraries(NPTRex ${ROOT_LIBRARIES} NPCore) 
install(FILES TTRexData.h TTRexPhysics.h TTRexSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

