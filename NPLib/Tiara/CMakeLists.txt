add_custom_command(OUTPUT TTiaraHyballPhysicsDict.cxx TTiaraHyballPhysicsDict_rdict.pcm TTiaraHyballPhysics.rootmap COMMAND rootcint -f TTiaraHyballPhysicsDict.cxx -rmf TTiaraHyballPhysics.rootmap -rml libNPTiara.dylib -I../Core -c TTiaraHyballPhysics.h DEPENDS TTiaraHyballPhysics.h)
add_custom_command(OUTPUT TTiaraBarrelPhysicsDict.cxx TTiaraBarrelPhysicsDict_rdict.pcm TTiaraBarrelPhysics.rootmap COMMAND rootcint -f TTiaraBarrelPhysicsDict.cxx -rmf TTiaraBarrelPhysics.rootmap -rml libNPTiara.dylib -I../Core -c TTiaraBarrelPhysics.h DEPENDS TTiaraBarrelPhysics.h)
add_custom_command(OUTPUT TTiaraHyballDataDict.cxx TTiaraHyballDataDict_rdict.pcm TTiaraHyballData.rootmap COMMAND rootcint -f TTiaraHyballDataDict.cxx -rmf TTiaraHyballData.rootmap -rml libNPTiara.dylib -I../Core -c TTiaraHyballData.h DEPENDS TTiaraHyballData.h)
add_custom_command(OUTPUT TTiaraBarrelDataDict.cxx TTiaraBarrelDataDict_rdict.pcm TTiaraBarrelData.rootmap COMMAND rootcint -f TTiaraBarrelDataDict.cxx -rmf TTiaraBarrelData.rootmap -rml libNPTiara.dylib -I../Core -c TTiaraBarrelData.h DEPENDS TTiaraBarrelData.h)

add_library(NPTiara SHARED TTiaraBarrelData.cxx TTiaraBarrelPhysics.cxx TTiaraBarrelDataDict.cxx TTiaraBarrelPhysicsDict.cxx TTiaraBarrelSpectra.cxx TTiaraHyballData.cxx TTiaraHyballPhysics.cxx TTiaraHyballDataDict.cxx TTiaraHyballPhysicsDict.cxx TTiaraHyballSpectra.cxx)


target_link_libraries(NPTiara ${ROOT_LIBRARIES} NPCore) 
install(FILES TTiaraBarrelData.h TTiaraBarrelPhysics.h TTiaraBarrelSpectra.h TTiaraHyballData.h TTiaraHyballPhysics.h TTiaraHyballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TTiaraBarrelData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelPhysicsDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballPhysicsDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraBarrelPhysicsDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TTiaraHyballPhysicsDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
