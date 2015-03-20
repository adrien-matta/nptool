add_custom_command(OUTPUT TTigressPhysicsDict.cxx TTigressPhysicsDict_rdict.pcm TTigressPhysics.rootmap COMMAND genreflex TTigressPhysics.h -o TTigressPhysicsDict.cxx -I../Core --rootmap=TTigressPhysics.rootmap --rootmap-lib=libNPTigress --quiet DEPENDS TTigressPhysics.h)
add_custom_command(OUTPUT TTigressDataDict.cxx TTigressDataDict_rdict.pcm TTigressData.rootmap COMMAND genreflex TTigressData.h -o TTigressDataDict.cxx -I../Core --rootmap=TTigressData.rootmap --rootmap-lib=libNPTigress --quiet DEPENDS TTigressData.h)
add_library(NPTigress SHARED TTigressData.cxx TTigressPhysics.cxx TTigressDataDict.cxx TTigressPhysicsDict.cxx )
target_link_libraries(NPTigress ${ROOT_LIBRARIES} NPCore) 
install(FILES TTigressData.h TTigressPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TTigressData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTigressPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
