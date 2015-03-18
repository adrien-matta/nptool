ROOT_GENERATE_DICTIONARY( TTrifoilData.h "" TTrifoilDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TTrifoilPhysics.h "" TTrifoilPhysicsDict.cxx "../Core")
add_library(NPTrifoil SHARED TTrifoilData.cxx TTrifoilPhysics.cxx TTrifoilDataDict.cxx TTrifoilPhysicsDict.cxx )
target_link_libraries(NPTrifoil ${ROOT_LIBRARIES} NPCore) 
install(FILES TTrifoilData.h TTrifoilPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

