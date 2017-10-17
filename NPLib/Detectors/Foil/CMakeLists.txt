add_custom_command(OUTPUT TFoilPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TFoilPhysics.h TFoilPhysicsDict.cxx TFoilPhysics.rootmap libNPFoil.dylib DEPENDS TFoilPhysics.h)
add_custom_command(OUTPUT TFoilDataDict.cxx COMMAND ../../scripts/build_dict.sh TFoilData.h TFoilDataDict.cxx TFoilData.rootmap libNPFoil.dylib DEPENDS TFoilData.h)
add_library(NPFoil SHARED TFoilData.cxx TFoilPhysics.cxx TFoilDataDict.cxx TFoilPhysicsDict.cxx )
target_link_libraries(NPFoil ${ROOT_LIBRARIES} NPCore) 
install(FILES TFoilData.h TFoilPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

