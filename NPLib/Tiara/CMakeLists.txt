add_custom_command(OUTPUT TTiaraHyballPhysicsDict.cxx TTiaraHyballPhysicsDict_rdict.pcm TTiaraHyballPhysics.rootmap COMMAND genreflex TTiaraHyballPhysics.h -o TTiaraHyballPhysicsDict.cxx -I../Core --rootmap=TTiaraHyballPhysics.rootmap --rootmap-lib=libNPTiara --quiet DEPENDS TTiaraHyballPhysics.h)
add_custom_command(OUTPUT TTiaraBarrelPhysicsDict.cxx TTiaraBarrelPhysicsDict_rdict.pcm TTiaraBarrelPhysics.rootmap COMMAND genreflex TTiaraBarrelPhysics.h -o TTiaraBarrelPhysicsDict.cxx -I../Core --rootmap=TTiaraBarrelPhysics.rootmap --rootmap-lib=libNPTiara --quiet DEPENDS TTiaraBarrelPhysics.h)
add_custom_command(OUTPUT TTiaraHyballDataDict.cxx TTiaraHyballDataDict_rdict.pcm TTiaraHyballData.rootmap COMMAND genreflex TTiaraHyballData.h -o TTiaraHyballDataDict.cxx -I../Core --rootmap=TTiaraHyballData.rootmap --rootmap-lib=libNPTiara --quiet DEPENDS TTiaraHyballData.h)
add_custom_command(OUTPUT TTiaraBarrelDataDict.cxx TTiaraBarrelDataDict_rdict.pcm TTiaraBarrelData.rootmap COMMAND genreflex TTiaraBarrelData.h -o TTiaraBarrelDataDict.cxx -I../Core --rootmap=TTiaraBarrelData.rootmap --rootmap-lib=libNPTiara --quiet DEPENDS TTiaraBarrelData.h)

add_library(NPTiara SHARED TTiaraBarrelData.cxx TTiaraBarrelPhysics.cxx TTiaraBarrelDataDict.cxx TTiaraBarrelPhysicsDict.cxx TTiaraBarrelSpectra.cxx TTiaraHyballData.cxx TTiaraHyballPhysics.cxx TTiaraHyballDataDict.cxx TTiaraHyballPhysicsDict.cxx TTiaraHyballSpectra.cxx)


target_link_libraries(NPTiara ${ROOT_LIBRARIES} NPCore) 
install(FILES TTiaraBarrelData.h TTiaraBarrelPhysics.h TTiaraBarrelSpectra.h TTiaraHyballData.h TTiaraHyballPhysics.h TTiaraHyballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TTiaraBarrelData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
