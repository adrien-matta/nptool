ROOT_GENERATE_DICTIONARY( TExogamData.h "" TExogamDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TExogamPhysics.h "" TExogamPhysicsDict.cxx "../Core")
add_library(NPExogam SHARED TExogamData.cxx TExogamPhysics.cxx TExogamDataDict.cxx TExogamPhysicsDict.cxx TExogamSpectra.cxx)
target_link_libraries(NPExogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TExogamData.h TExogamPhysics.h TExogamSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

