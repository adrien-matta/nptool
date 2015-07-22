add_custom_command(OUTPUT TSiResPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TSiResPhysics.h TSiResPhysicsDict.cxx TSiResPhysics.rootmap libNPSiRes.dylib DEPENDS TSiResPhysics.h)
add_custom_command(OUTPUT TSiResDataDict.cxx COMMAND ../scripts/build_dict.sh TSiResData.h TSiResDataDict.cxx TSiResData.rootmap libNPSiRes.dylib DEPENDS TSiResData.h)
add_library(NPSiRes SHARED TSiResData.cxx TSiResPhysics.cxx TSiResDataDict.cxx TSiResPhysicsDict.cxx TSiResSpectra.cxx)
target_link_libraries(NPSiRes ${ROOT_LIBRARIES} NPCore) 
install(FILES TSiResData.h TSiResPhysics.h TSiResSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

