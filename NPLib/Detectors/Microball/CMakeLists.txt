add_custom_command(OUTPUT TMicroballPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMicroballPhysics.h TMicroballPhysicsDict.cxx TMicroballPhysics.rootmap libNPMicroball.dylib DEPENDS TMicroballPhysics.h)
add_custom_command(OUTPUT TMicroballDataDict.cxx COMMAND ../../scripts/build_dict.sh TMicroballData.h TMicroballDataDict.cxx TMicroballData.rootmap libNPMicroball.dylib DEPENDS TMicroballData.h)
add_library(NPMicroball SHARED TMicroballSpectra.cxx TMicroballData.cxx TMicroballPhysics.cxx TMicroballDataDict.cxx TMicroballPhysicsDict.cxx )
target_link_libraries(NPMicroball ${ROOT_LIBRARIES} NPCore) 
install(FILES TMicroballData.h TMicroballPhysics.h TMicroballSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

