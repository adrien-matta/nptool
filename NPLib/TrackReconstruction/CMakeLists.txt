add_custom_command(OUTPUT NPRansacDict.cxx COMMAND ../scripts/build_dict.sh NPRansac.h NPRansacDict.cxx NPRansac.rootmap libNPTrackReconstruction.so NPTrackReconstructionLinkDef.h DEPENDS NPRansac.h)

add_custom_command(OUTPUT NPClusterDict.cxx COMMAND ../scripts/build_dict.sh NPCluster.h NPClusterDict.cxx NPCluster.rootmap libNPTrackReconstruction.so NPTrackReconstructionLinkDef.h DEPENDS NPCluster.h)

add_library(NPTrackReconstruction SHARED NPRansac.cxx NPCluster.cxx NPTrack.cxx NPRansacDict.cxx NPClusterDict.cxx)
target_link_libraries(NPTrackReconstruction ${ROOT_LIBRARIES} NPCore)

install(FILES NPRansac.h NPCluster.h NPTrack.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
