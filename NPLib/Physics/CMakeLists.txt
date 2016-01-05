add_custom_command(OUTPUT NPReactionDict.cxx COMMAND ../scripts/build_dict.sh NPReaction.h NPReactionDict.cxx NPReaction.rootmap libNPPhysics.so NPPhysicsLinkDef.h) 
add_custom_command(OUTPUT NPEnergyLossDict.cxx COMMAND ../scripts/build_dict.sh NPEnergyLoss.h NPEnergyLossDict.cxx NPEnergyLoss.rootmap libNPPhysics.so NPPhysicsLinkDef.h) 
add_custom_command(OUTPUT TInitialConditionsDict.cxx COMMAND ../scripts/build_dict.sh TInitialConditions.h TInitialConditionsDict.cxx TInitialConditions.rootmap libNPPhysics.so)
add_custom_command(OUTPUT TInteractionCoordinatesDict.cxx COMMAND ../scripts/build_dict.sh TInteractionCoordinates.h TInteractionCoordinatesDict.cxx TInteractionCoordinates.rootmap libNPPhysics.so)

add_library(NPPhysics SHARED NPBeam.cxx NPEnergyLoss.cxx NPFunction.cxx NPNucleus.cxx NPReaction.cxx TInitialConditions.cxx TInteractionCoordinates.cxx NPReactionDict.cxx NPEnergyLossDict.cxx TInitialConditionsDict.cxx TInteractionCoordinatesDict.cxx)
target_link_libraries(NPPhysics ${ROOT_LIBRARIES} MathMore NPCore) 
install(FILES NPBeam.h NPEnergyLoss.h NPFunction.h NPNucleus.h NPReaction.h TInitialConditions.h TInteractionCoordinates.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
