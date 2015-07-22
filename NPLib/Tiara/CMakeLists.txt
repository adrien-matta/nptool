add_custom_command(OUTPUT TTiaraHyballPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TTiaraHyballPhysics.h TTiaraHyballPhysicsDict.cxx TTiaraHyballPhysics.rootmap libNPTiara.dylib DEPENDS TTiaraHyballPhysics.h)
add_custom_command(OUTPUT TTiaraBarrelPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TTiaraBarrelPhysics.h TTiaraBarrelPhysicsDict.cxx TTiaraBarrelPhysics.rootmap libNPTiara.dylib DEPENDS TTiaraBarrelPhysics.h)
add_custom_command(OUTPUT TTiaraHyballDataDict.cxx COMMAND ../scripts/build_dict.sh TTiaraHyballData.h TTiaraHyballDataDict.cxx TTiaraHyballData.rootmap libNPTiara.dylib DEPENDS TTiaraHyballData.h)
add_custom_command(OUTPUT TTiaraBarrelDataDict.cxx COMMAND ../scripts/build_dict.sh TTiaraBarrelData.h TTiaraBarrelDataDict.cxx TTiaraBarrelData.rootmap libNPTiara.dylib DEPENDS TTiaraBarrelData.h)

add_library(NPTiara SHARED TTiaraBarrelData.cxx TTiaraBarrelPhysics.cxx TTiaraBarrelDataDict.cxx TTiaraBarrelPhysicsDict.cxx TTiaraBarrelSpectra.cxx TTiaraHyballData.cxx TTiaraHyballPhysics.cxx TTiaraHyballDataDict.cxx TTiaraHyballPhysicsDict.cxx TTiaraHyballSpectra.cxx)


target_link_libraries(NPTiara ${ROOT_LIBRARIES} NPCore) 
install(FILES TTiaraBarrelData.h TTiaraBarrelPhysics.h TTiaraBarrelSpectra.h TTiaraHyballData.h TTiaraHyballPhysics.h TTiaraHyballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

