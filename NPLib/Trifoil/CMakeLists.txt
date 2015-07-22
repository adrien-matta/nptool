add_custom_command(OUTPUT TTrifoilPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TTrifoilPhysics.h TTrifoilPhysicsDict.cxx TTrifoilPhysics.rootmap libNPTrifoil.dylib DEPENDS TTrifoilPhysics.h)
add_custom_command(OUTPUT TTrifoilDataDict.cxx COMMAND ../scripts/build_dict.sh TTrifoilData.h TTrifoilDataDict.cxx TTrifoilData.rootmap libNPTrifoil.dylib DEPENDS TTrifoilData.h)
add_library(NPTrifoil SHARED TTrifoilData.cxx TTrifoilPhysics.cxx TTrifoilDataDict.cxx TTrifoilPhysicsDict.cxx )
target_link_libraries(NPTrifoil ${ROOT_LIBRARIES} -lSpectrum NPCore) 
install(FILES TTrifoilData.h TTrifoilPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

