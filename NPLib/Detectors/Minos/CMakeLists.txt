add_custom_command(OUTPUT TMinosPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TMinosPhysics.h TMinosPhysicsDict.cxx TMinosPhysics.rootmap libNPMinos.dylib DEPENDS TMinosPhysics.h)
add_custom_command(OUTPUT TMinosDataDict.cxx COMMAND ../../scripts/build_dict.sh TMinosData.h TMinosDataDict.cxx TMinosData.rootmap libNPMinos.dylib DEPENDS TMinosData.h)
add_library(NPMinos SHARED TMinosSpectra.cxx TMinosData.cxx TMinosPhysics.cxx TMinosDataDict.cxx TMinosPhysicsDict.cxx )
target_link_libraries(NPMinos ${ROOT_LIBRARIES} NPCore) 
install(FILES TMinosData.h TMinosPhysics.h TMinosSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

