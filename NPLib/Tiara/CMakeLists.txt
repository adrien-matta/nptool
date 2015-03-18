ROOT_GENERATE_DICTIONARY( TTiaraBarrelData.h "" TTiaraBarrelDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TTiaraBarrelPhysics.h "" TTiaraBarrelPhysicsDict.cxx "../Core")
ROOT_GENERATE_DICTIONARY( TTiaraHyballData.h "" TTiaraHyballDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TTiaraHyballPhysics.h "" TTiaraHyballPhysicsDict.cxx "../Core")

add_library(NPTiara SHARED TTiaraBarrelData.cxx TTiaraBarrelPhysics.cxx TTiaraBarrelDataDict.cxx TTiaraBarrelPhysicsDict.cxx TTiaraBarrelSpectra.cxx TTiaraHyballData.cxx TTiaraHyballPhysics.cxx TTiaraHyballDataDict.cxx TTiaraHyballPhysicsDict.cxx TTiaraHyballSpectra.cxx)


target_link_libraries(NPTiara ${ROOT_LIBRARIES} NPCore) 
install(FILES TTiaraBarrelData.h TTiaraBarrelPhysics.h TTiaraBarrelSpectra.h TTiaraHyballData.h TTiaraHyballPhysics.h TTiaraHyballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

