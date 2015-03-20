add_custom_command(OUTPUT TSiLiPhysicsDict.cxx TSiLiPhysicsDict_rdict.pcm TSiLiPhysics.rootmap COMMAND genreflex TSiLiPhysics.h -o TSiLiPhysicsDict.cxx -I../Core --rootmap=TSiLiPhysics.rootmap --rootmap-lib=libNPSiLi --quiet DEPENDS TSiLiPhysics.h)
add_custom_command(OUTPUT TSiLiDataDict.cxx TSiLiDataDict_rdict.pcm TSiLiData.rootmap COMMAND genreflex TSiLiData.h -o TSiLiDataDict.cxx -I../Core --rootmap=TSiLiData.rootmap --rootmap-lib=libNPSiLi --quiet DEPENDS TSiLiData.h)
add_library(NPSiLi SHARED TSiLiData.cxx TSiLiPhysics.cxx TSiLiDataDict.cxx TSiLiPhysicsDict.cxx TSiLiSpectra.cxx)
target_link_libraries(NPSiLi ${ROOT_LIBRARIES} NPCore) 
install(FILES TSiLiData.h TSiLiPhysics.h TSiLiSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TSiLiData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TSiLiPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
