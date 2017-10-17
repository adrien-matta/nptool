add_custom_command(OUTPUT TJurogamPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TJurogamPhysics.h TJurogamPhysicsDict.cxx TJurogamPhysics.rootmap libNPJurogam.dylib DEPENDS TJurogamPhysics.h)
add_custom_command(OUTPUT TJurogamDataDict.cxx COMMAND ../../scripts/build_dict.sh TJurogamData.h TJurogamDataDict.cxx TJurogamData.rootmap libNPJurogam.dylib DEPENDS TJurogamData.h)
add_library(NPJurogam SHARED TJurogamSpectra.cxx TJurogamData.cxx TJurogamPhysics.cxx TJurogamDataDict.cxx TJurogamPhysicsDict.cxx )
target_link_libraries(NPJurogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TJurogamData.h TJurogamPhysics.h TJurogamSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

