ROOT_GENERATE_DICTIONARY( TChio_anData.h "" TChio_anDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TChio_anPhysics.h "" TChio_anPhysicsDict.cxx "../Core")
ROOT_GENERATE_DICTIONARY( TChio_digData.h "" TChio_digDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TChio_digPhysics.h "" TChio_digPhysicsDict.cxx "../Core")

add_library(NPChio SHARED TChio_anData.cxx TChio_anPhysics.cxx TChio_anDataDict.cxx TChio_anPhysicsDict.cxx TChio_digData.cxx TChio_digPhysics.cxx TChio_digDataDict.cxx TChio_digPhysicsDict.cxx )
target_link_libraries(NPChio ${ROOT_LIBRARIES} NPCore) 
install(FILES TChio_anData.h TChio_anPhysics.h TChio_digData.h TChio_digPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

