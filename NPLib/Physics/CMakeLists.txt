add_custom_command(OUTPUT NPReactionDict.cxx COMMAND ../scripts/build_dict.sh NPReaction.h NPReactionDict.cxx NPReaction.rootmap libNPPhysics.so NPPhysicsLinkDef.h) 
add_library(NPPhysics SHARED NPBeam.cxx NPEnergyLoss.cxx NPFunction.cxx NPNucleus.cxx NPReaction.cxx NPReactionDict.cxx)
target_link_libraries(NPPhysics ${ROOT_LIBRARIES} MathMore NPCore) 
install(FILES NPBeam.h NPEnergyLoss.h NPFunction.h NPNucleus.h NPReaction.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
