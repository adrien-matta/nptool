add_custom_command(OUTPUT NPReactionDict.cxx COMMAND ../scripts/build_dict.sh NPReaction.h NPReactionDict.cxx NPReaction.rootmap libNPPhysics.so NPPhysicsLinkDef.h DEPENDS NPReaction.h) 
add_custom_command(OUTPUT NPEnergyLossDict.cxx COMMAND ../scripts/build_dict.sh NPEnergyLoss.h NPEnergyLossDict.cxx NPEnergyLoss.rootmap libNPPhysics.so NPPhysicsLinkDef.h DEPENDS NPEnergyLoss.h) 
add_custom_command(OUTPUT TInitialConditionsDict.cxx COMMAND ../scripts/build_dict.sh TInitialConditions.h TInitialConditionsDict.cxx TInitialConditions.rootmap libNPInitialConditions.so DEPENDS TInitialConditions.h)
add_custom_command(OUTPUT TInteractionCoordinatesDict.cxx COMMAND ../scripts/build_dict.sh TInteractionCoordinates.h TInteractionCoordinatesDict.cxx TInteractionCoordinates.rootmap libNPInteractionCoordinates.so DEPENDS TInteractionCoordinates.h)

add_library(NPPhysics SHARED NPBeam.cxx NPEnergyLoss.cxx NPFunction.cxx NPNucleus.cxx NPReaction.cxx NPReactionDict.cxx NPEnergyLossDict.cxx )
target_link_libraries(NPPhysics ${ROOT_LIBRARIES} NPCore) 

add_library(NPInitialConditions  SHARED  TInitialConditions.cxx TInitialConditionsDict.cxx )
target_link_libraries(NPInitialConditions  ${ROOT_LIBRARIES} NPCore) 

add_library(NPInteractionCoordinates SHARED TInteractionCoordinates.cxx TInteractionCoordinatesDict.cxx)
target_link_libraries(NPInteractionCoordinates ${ROOT_LIBRARIES} ) 

install(FILES NPBeam.h NPEnergyLoss.h NPFunction.h NPNucleus.h NPReaction.h TInitialConditions.h TInteractionCoordinates.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
