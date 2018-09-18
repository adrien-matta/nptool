<<<<<<< HEAD
add_library(NPSScorers SHARED NPSHitsMap.hh CalorimeterScorers.cc  SiliconScorers.cc PhotoDiodeScorers.cc ObsoleteGeneralScorers.cc DriftElectronScorers.cc TPCScorers.cc MDMScorer.cc NeutronDetectorScorers.cc )
=======
add_library(NPSScorers SHARED NPSHitsMap.hh CalorimeterScorers.cc InteractionScorers.cc DSSDScorers.cc SiliconScorers.cc PhotoDiodeScorers.cc ObsoleteGeneralScorers.cc DriftElectronScorers.cc TPCScorers.cc MDMScorer.cc )
>>>>>>> b55c970220ed010e24e3a5ac9f01b96f9c840b16
target_link_libraries(NPSScorers ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPInitialConditions -lNPInteractionCoordinates)
