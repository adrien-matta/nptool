add_custom_command(OUTPUT TTNTPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TTNTPhysics.h TTNTPhysicsDict.cxx TTNTPhysics.rootmap libNPTNT.dylib DEPENDS TTNTPhysics.h)
add_custom_command(OUTPUT TTNTDataDict.cxx COMMAND ../../scripts/build_dict.sh TTNTData.h TTNTDataDict.cxx TTNTData.rootmap libNPTNT.dylib DEPENDS TTNTData.h)
add_library(NPTNT SHARED TTNTSpectra.cxx TTNTData.cxx TTNTPhysics.cxx TTNTDataDict.cxx TTNTPhysicsDict.cxx )
target_link_libraries(NPTNT ${ROOT_LIBRARIES} NPCore) 
install(FILES TTNTData.h TTNTPhysics.h TTNTSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

