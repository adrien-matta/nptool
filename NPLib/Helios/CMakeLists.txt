add_custom_command(OUTPUT THeliosPhysicsDict.cxx THeliosPhysicsDict_rdict.pcm THeliosPhysics.rootmap COMMAND genreflex THeliosPhysics.h -o THeliosPhysicsDict.cxx -I../Core --rootmap=THeliosPhysics.rootmap --rootmap-lib=libNPHelios --quiet DEPENDS THeliosPhysics.h)
add_custom_command(OUTPUT THeliosDataDict.cxx THeliosDataDict_rdict.pcm THeliosData.rootmap COMMAND genreflex THeliosData.h -o THeliosDataDict.cxx -I../Core --rootmap=THeliosData.rootmap --rootmap-lib=libNPHelios --quiet DEPENDS THeliosData.h)
add_library(NPHelios SHARED Helios.cxx THeliosData.cxx THeliosPhysics.cxx THeliosDataDict.cxx THeliosPhysicsDict.cxx )
target_link_libraries(NPHelios ${ROOT_LIBRARIES} NPCore) 
install(FILES THeliosData.h THeliosPhysics.h Helios.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES THeliosData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES THeliosPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
