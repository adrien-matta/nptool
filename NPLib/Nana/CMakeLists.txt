add_custom_command(OUTPUT TNanaPhysicsDict.cxx TNanaPhysicsDict_rdict.pcm TNanaPhysics.rootmap COMMAND genreflex TNanaPhysics.h -o TNanaPhysicsDict.cxx -I../Core --rootmap=TNanaPhysics.rootmap --rootmap-lib=libNPNana --quiet DEPENDS TNanaPhysics.h)
add_custom_command(OUTPUT TNanaDataDict.cxx TNanaDataDict_rdict.pcm TNanaData.rootmap COMMAND genreflex TNanaData.h -o TNanaDataDict.cxx -I../Core --rootmap=TNanaData.rootmap --rootmap-lib=libNPNana --quiet DEPENDS TNanaData.h)
add_library(NPNana SHARED TNanaData.cxx TNanaPhysics.cxx TNanaDataDict.cxx TNanaPhysicsDict.cxx )
target_link_libraries(NPNana ${ROOT_LIBRARIES} NPCore) 
install(FILES TNanaData.h TNanaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TNanaData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TNanaPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
