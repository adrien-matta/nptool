ROOT_GENERATE_DICTIONARY( TSharcData.h "" TSharcDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSharcPhysics.h "" TSharcPhysicsDict.cxx "../Core")
add_library(NPSharc SHARED TSharcData.cxx TSharcPhysics.cxx TSharcDataDict.cxx TSharcPhysicsDict.cxx )
target_link_libraries(NPSharc ${ROOT_LIBRARIES} NPCore) 
install(FILES TSharcData.h TSharcPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

