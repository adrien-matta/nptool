  
add_library(NPPhysics SHARED NPBeam.cxx NPEnergyLoss.cxx NPFunction.cxx NPNucleus.cxx NPReaction.cxx NPReactionDict.cxx)
target_link_libraries(NPPhysics ${ROOT_LIBRARIES} MathMore NPCore) 
install(FILES NPBeam.h NPEnergyLoss.h NPFunction.h NPNucleus.h NPReaction.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
install(FILES NPReactionDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES NPReaction.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
