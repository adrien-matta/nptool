add_custom_command(OUTPUT NPRansacDict.cxx COMMAND ../scripts/build_dict.sh NPRansac.h NPRansacDict.cxx NPRansac.rootmap libNPTrackReconstruction.so NPTrackReconstructionLinkDef.h DEPENDS NPRansac.h)

add_library(NPTrackReconstruction SHARED NPRansac.cxx NPTrack.cxx NPRansacDict.cxx)
target_link_libraries(NPTrackReconstruction ${ROOT_LIBRARIES} NPCore)

install(FILES NPRansac.h NPTrack.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
