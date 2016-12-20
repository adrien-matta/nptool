add_library(NPSEventGenerator SHARED EventGeneratorParticleDecay.cc EventGeneratorBeam.cc EventGeneratorTwoBodyReaction.cc EventGeneratorpBUU.cc EventGeneratorGammaDecay.cc EventGeneratorIsotropic.cc VEventGenerator.cc)
target_link_libraries(NPSEventGenerator ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPInitialConditions -lNPInteractionCoordinates)
