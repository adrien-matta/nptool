add_custom_command(OUTPUT TSiLiPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TSiLiPhysics.h TSiLiPhysicsDict.cxx TSiLiPhysics.rootmap libNPSiLi.dylib DEPENDS TSiLiPhysics.h)
add_custom_command(OUTPUT TSiLiDataDict.cxx COMMAND ../../scripts/build_dict.sh TSiLiData.h TSiLiDataDict.cxx TSiLiData.rootmap libNPSiLi.dylib DEPENDS TSiLiData.h)
add_library(NPSiLi SHARED TSiLiData.cxx TSiLiPhysics.cxx TSiLiDataDict.cxx TSiLiPhysicsDict.cxx TSiLiSpectra.cxx)
target_link_libraries(NPSiLi ${ROOT_LIBRARIES} NPCore) 
install(FILES TSiLiData.h TSiLiPhysics.h TSiLiSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

