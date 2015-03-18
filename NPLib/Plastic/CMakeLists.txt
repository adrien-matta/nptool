ROOT_GENERATE_DICTIONARY( TPlasticData.h "" TPlasticDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TPlasticPhysics.h "" TPlasticPhysicsDict.cxx "../Core")
add_library(NPPlastic SHARED TPlasticData.cxx TPlasticPhysics.cxx TPlasticDataDict.cxx TPlasticPhysicsDict.cxx )
target_link_libraries(NPPlastic ${ROOT_LIBRARIES} NPCore) 
install(FILES TPlasticData.h TPlasticPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

