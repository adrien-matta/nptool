ROOT_GENERATE_DICTIONARY( TCharissaData.h "" TCharissaDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TCharissaPhysics.h "" TCharissaPhysicsDict.cxx "../Core")
add_library(NPCharissa SHARED TCharissaData.cxx TCharissaPhysics.cxx TCharissaDataDict.cxx TCharissaPhysicsDict.cxx TCharissaSpectra.cxx)
target_link_libraries(NPCharissa ${ROOT_LIBRARIES} NPCore) 
install(FILES TCharissaData.h TCharissaPhysics.h TCharissaSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

