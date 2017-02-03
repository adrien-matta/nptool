add_library(NPSProcess SHARED  FastDriftElectron.cc NPIonIonInelasticPhysic.cc PhysicsList.cc G4DriftElectron.cc G4IonizationWithDE.cc G4DriftElectronPhysics.cc G4DEAbsorption.cc G4DEAmplification.cc G4DETransport.cc )
target_link_libraries(NPSProcess ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPInitialConditions -lNPInteractionCoordinates)
