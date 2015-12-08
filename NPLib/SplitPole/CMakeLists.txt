add_custom_command(OUTPUT TSplitPolePhysicsDict.cxx COMMAND ../scripts/build_dict.sh TSplitPolePhysics.h TSplitPolePhysicsDict.cxx TSplitPolePhysics.rootmap libNPSplitPole.dylib DEPENDS TSplitPolePhysics.h)
add_custom_command(OUTPUT TSplitPoleDataDict.cxx COMMAND ../scripts/build_dict.sh TSplitPoleData.h TSplitPoleDataDict.cxx TSplitPoleData.rootmap libNPSplitPole.dylib DEPENDS TSplitPoleData.h)
add_custom_command(OUTPUT TSplitPoleNMRDict.cxx COMMAND ../scripts/build_dict.sh TSplitPoleNMR.h TSplitPoleNMRDict.cxx TSplitPoleNMR.rootmap libNPSplitPole.dylib DEPENDS TSplitPoleNMR.h)
add_library(NPSplitPole SHARED TSplitPoleSpectra.cxx TSplitPoleData.cxx TSplitPolePhysics.cxx TSplitPoleNMR.cxx TSplitPoleDataDict.cxx TSplitPolePhysicsDict.cxx TSplitPoleNMRDict.cxx)
target_link_libraries(NPSplitPole ${ROOT_LIBRARIES} NPCore)
install(FILES TSplitPoleData.h TSplitPolePhysics.h TSplitPoleSpectra.h TSplitPoleNMR.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

