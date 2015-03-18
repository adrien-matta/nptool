ROOT_GENERATE_DICTIONARY( TSSSDData.h "" TSSSDDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSSSDPhysics.h "" TSSSDPhysicsDict.cxx "../Core")
add_library(NPSSSD SHARED TSSSDData.cxx TSSSDPhysics.cxx TSSSDDataDict.cxx TSSSDPhysicsDict.cxx )
target_link_libraries(NPSSSD ${ROOT_LIBRARIES} NPCore) 
install(FILES TSSSDData.h TSSSDPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

