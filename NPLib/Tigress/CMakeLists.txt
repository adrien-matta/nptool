add_custom_command(OUTPUT TTigressPhysicsDict.cxx TTigressPhysicsDict_rdict.pcm TTigressPhysics.rootmap COMMAND ../scripts/build_dict.sh TTigressPhysics.h TTigressPhysicsDict.cxx TTigressPhysics.rootmap libNPTigress.dylib DEPENDS TTigressPhysics.h)
add_custom_command(OUTPUT TTigressDataDict.cxx TTigressDataDict_rdict.pcm TTigressData.rootmap COMMAND ../scripts/build_dict.sh TTigressData.h TTigressDataDict.cxx TTigressData.rootmap libNPTigress.dylib DEPENDS TTigressData.h)
add_library(NPTigress SHARED TTigressData.cxx TTigressPhysics.cxx TTigressDataDict.cxx TTigressPhysicsDict.cxx )
target_link_libraries(NPTigress ${ROOT_LIBRARIES} NPCore) 
install(FILES TTigressData.h TTigressPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

