add_custom_command(OUTPUT TModularLeafPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TModularLeafPhysics.h TModularLeafPhysicsDict.cxx TModularLeafPhysics.rootmap libNPModularLeaf.dylib DEPENDS TModularLeafPhysics.h)
add_library(NPModularLeaf SHARED TModularLeafPhysics.cxx TModularLeafPhysicsDict.cxx)
target_link_libraries(NPModularLeaf ${ROOT_LIBRARIES} NPCore) 
install(FILES  TModularLeafPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

