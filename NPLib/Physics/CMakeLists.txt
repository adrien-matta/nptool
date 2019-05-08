add_custom_command(OUTPUT NPNucleusDict.cxx COMMAND ../scripts/build_dict.sh NPNucleus.h NPNucleusDict.cxx NPNucleus.rootmap libNPPhysics.so NPNucleusLinkDef.h DEPENDS NPNucleus.h NPNucleusLinkDef.h) 

add_custom_command(OUTPUT NPReactionDict.cxx COMMAND ../scripts/build_dict.sh NPReaction.h NPReactionDict.cxx NPReaction.rootmap libNPPhysics.so NPReactionLinkDef.h DEPENDS NPReaction.h NPReactionLinkDef.h) 

add_custom_command(OUTPUT NPQFSDict.cxx COMMAND ../scripts/build_dict.sh NPQFS.h NPQFSDict.cxx NPQFS.rootmap libNPPhysics.so NPQFSLinkDef.h DEPENDS NPQFS.h NPQFSLinkDef.h) 

add_custom_command(OUTPUT NPDecayDict.cxx COMMAND ../scripts/build_dict.sh NPDecay.h NPDecayDict.cxx NPDecay.rootmap libNPPhysics.so NPDecayLinkDef.h DEPENDS NPDecay.h NPDecayLinkDef.h) 

add_custom_command(OUTPUT NPEnergyLossDict.cxx COMMAND ../scripts/build_dict.sh NPEnergyLoss.h NPEnergyLossDict.cxx NPEnergyLoss.rootmap libNPPhysics.so NPEnergyLossLinkDef.h DEPENDS NPEnergyLoss.h NPEnergyLossLinkDef.h) 

add_custom_command(OUTPUT TInitialConditionsDict.cxx COMMAND ../scripts/build_dict.sh TInitialConditions.h TInitialConditionsDict.cxx TInitialConditions.rootmap libNPInitialConditions.so DEPENDS TInitialConditions.h)

add_custom_command(OUTPUT TInteractionCoordinatesDict.cxx COMMAND ../scripts/build_dict.sh TInteractionCoordinates.h TInteractionCoordinatesDict.cxx TInteractionCoordinates.rootmap libNPInteractionCoordinates.so DEPENDS TInteractionCoordinates.h)

add_custom_command(OUTPUT TReactionConditionsDict.cxx COMMAND ../scripts/build_dict.sh TReactionConditions.h TReactionConditionsDict.cxx TReactionConditions.rootmap libNPReactionConditions.so DEPENDS TReactionConditions.h)

add_library(NPPhysics SHARED NPDecay.cxx NPBeam.cxx NPEnergyLoss.cxx NPFunction.cxx NPNucleus.cxx NPReaction.cxx NPQFS.cxx NPNucleusDict.cxx NPReactionDict.cxx NPQFSDict.cxx NPEnergyLossDict.cxx )
target_link_libraries(NPPhysics ${ROOT_LIBRARIES} Physics NPCore) 

add_library(NPInitialConditions  SHARED  TInitialConditions.cxx TInitialConditionsDict.cxx )
target_link_libraries(NPInitialConditions  ${ROOT_LIBRARIES} ) 

add_library(NPInteractionCoordinates SHARED TInteractionCoordinates.cxx TInteractionCoordinatesDict.cxx)
target_link_libraries(NPInteractionCoordinates ${ROOT_LIBRARIES} ) 

add_library(NPReactionConditions SHARED TReactionConditions.cxx TReactionConditionsDict.cxx)
target_link_libraries(NPReactionConditions ${ROOT_LIBRARIES} ) 


install(FILES NPDecay.h NPBeam.h NPEnergyLoss.h NPFunction.h NPNucleus.h NPReaction.h  NPQFS.h TInitialConditions.h TInteractionCoordinates.h TReactionConditions.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
