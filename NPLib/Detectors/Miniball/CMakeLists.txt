add_custom_command(OUTPUT TMiniballPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMiniballPhysics.h TMiniballPhysicsDict.cxx TMiniballPhysics.rootmap libNPMiniball.dylib DEPENDS TMiniballPhysics.h)
add_custom_command(OUTPUT TMiniballDataDict.cxx COMMAND ../../scripts/build_dict.sh TMiniballData.h TMiniballDataDict.cxx TMiniballData.rootmap libNPMiniball.dylib DEPENDS TMiniballData.h)
add_library(NPMiniball SHARED TMiniballSpectra.cxx TMiniballData.cxx TMiniballPhysics.cxx TMiniballDataDict.cxx TMiniballPhysicsDict.cxx )
target_link_libraries(NPMiniball ${ROOT_LIBRARIES} NPCore) 
install(FILES TMiniballData.h TMiniballPhysics.h TMiniballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

