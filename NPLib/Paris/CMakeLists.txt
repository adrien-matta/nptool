ROOT_GENERATE_DICTIONARY( TParisData.h "" TParisDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TParisPhysics.h "" TParisPhysicsDict.cxx "../Core")
add_library(NPParis SHARED TParisData.cxx TParisPhysics.cxx TParisDataDict.cxx TParisPhysicsDict.cxx )
target_link_libraries(NPParis ${ROOT_LIBRARIES} NPCore) 
install(FILES TParisData.h TParisPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

