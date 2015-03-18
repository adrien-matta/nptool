ROOT_GENERATE_DICTIONARY( TSpegCHIOData.h "" TSpegCHIODataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSpegDCData.h "" TSpegDCDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSpegPlasticData.h "" TSpegPlasticDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSpegPhysics.h "" TSpegPhysicsDict.cxx "../Core")
add_library(NPSpeg SHARED TSpegCHIOData.cxx TSpegDCData.cxx TSpegPlasticData.cxx TSpegPhysics.cxx TSpegCHIODataDict.cxx TSpegDCDataDict.cxx TSpegPlasticDataDict.cxx TSpegPhysicsDict.cxx )
target_link_libraries(NPSpeg ${ROOT_LIBRARIES} NPCore) 
install(FILES TSpegCHIOData.h TSpegDCData.h TSpegPlasticData.h TSpegPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

