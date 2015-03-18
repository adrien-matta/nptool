ROOT_GENERATE_DICTIONARY( TW1Data.h "" TW1DataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TW1Physics.h "" TW1PhysicsDict.cxx "../Core")
add_library(NPW1 SHARED TW1Data.cxx TW1Physics.cxx TW1DataDict.cxx TW1PhysicsDict.cxx )
target_link_libraries(NPW1 ${ROOT_LIBRARIES} NPCore) 
install(FILES TW1Data.h TW1Physics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

