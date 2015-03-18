ROOT_GENERATE_DICTIONARY( TExlData.h "" TExlDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TExlPhysics.h "" TExlPhysicsDict.cxx "../Core")
add_library(NPExl SHARED TExlData.cxx TExlPhysics.cxx TExlDataDict.cxx TExlPhysicsDict.cxx )
target_link_libraries(NPExl ${ROOT_LIBRARIES} NPCore) 
install(FILES TExlData.h TExlPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

