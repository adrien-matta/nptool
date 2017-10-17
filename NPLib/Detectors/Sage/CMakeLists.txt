add_custom_command(OUTPUT TSagePhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TSagePhysics.h TSagePhysicsDict.cxx TSagePhysics.rootmap libNPSage.dylib DEPENDS TSagePhysics.h)
add_custom_command(OUTPUT TSageDataDict.cxx COMMAND ../../scripts/build_dict.sh TSageData.h TSageDataDict.cxx TSageData.rootmap libNPSage.dylib DEPENDS TSageData.h)
add_library(NPSage SHARED TSageSpectra.cxx TSageData.cxx TSagePhysics.cxx TSageDataDict.cxx TSagePhysicsDict.cxx )
target_link_libraries(NPSage ${ROOT_LIBRARIES} NPCore) 
install(FILES TSageData.h TSagePhysics.h TSageSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

