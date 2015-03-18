ROOT_GENERATE_DICTIONARY( TFatimaData.h "" TFatimaDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TFatimaPhysics.h "" TFatimaPhysicsDict.cxx "../Core")
add_library(NPFatima SHARED TFatimaData.cxx TFatimaPhysics.cxx TFatimaDataDict.cxx TFatimaPhysicsDict.cxx )
target_link_libraries(NPFatima ${ROOT_LIBRARIES} NPCore) 
install(FILES TFatimaData.h TFatimaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

