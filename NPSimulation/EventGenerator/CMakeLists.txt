add_library(NPSEventGenerator OBJECT EventGeneratorBeam.cc EventGeneratorMultipleParticle.cc EventGeneratorIsotropic.cc EventGeneratorCosmic.cc VEventGenerator.cc) 
#target_link_libraries(NPSEventGenerator ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPInitialConditions -lNPInteractionCoordinates )
