add_custom_command(OUTPUT TDaliPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TDaliPhysics.h TDaliPhysicsDict.cxx TDaliPhysics.rootmap libNPDali.dylib DEPENDS TDaliPhysics.h)
add_custom_command(OUTPUT TDaliDataDict.cxx COMMAND ../../scripts/build_dict.sh TDaliData.h TDaliDataDict.cxx TDaliData.rootmap libNPDali.dylib DEPENDS TDaliData.h)
add_library(NPDali SHARED TDaliSpectra.cxx TDaliData.cxx TDaliPhysics.cxx TDaliDataDict.cxx TDaliPhysicsDict.cxx )
target_link_libraries(NPDali ${ROOT_LIBRARIES} NPCore) 
install(FILES TDaliData.h TDaliPhysics.h TDaliSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

