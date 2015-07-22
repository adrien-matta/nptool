add_custom_command(OUTPUT TChio_digPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TChio_digPhysics.h TChio_digPhysicsDict.cxx TChio_digPhysics.rootmap libNPChio.dylib DEPENDS TChio_digPhysics.h)
add_custom_command(OUTPUT TChio_anPhysicsDict.cxx COMMAND ../scripts/build_dict.sh TChio_anPhysics.h TChio_anPhysicsDict.cxx TChio_anPhysics.rootmap libNPChio.dylib DEPENDS TChio_anPhysics.h)
add_custom_command(OUTPUT TChio_digDataDict.cxx COMMAND ../scripts/build_dict.sh TChio_digData.h TChio_digDataDict.cxx TChio_digData.rootmap libNPChio.dylib DEPENDS TChio_digData.h)
add_custom_command(OUTPUT TChio_anDataDict.cxx COMMAND ../scripts/build_dict.sh TChio_anData.h TChio_anDataDict.cxx TChio_anData.rootmap libNPChio.dylib DEPENDS TChio_anData.h)

add_library(NPChio SHARED TChio_anData.cxx TChio_anPhysics.cxx TChio_anDataDict.cxx TChio_anPhysicsDict.cxx TChio_digData.cxx TChio_digPhysics.cxx TChio_digDataDict.cxx TChio_digPhysicsDict.cxx )
target_link_libraries(NPChio ${ROOT_LIBRARIES} NPCore) 
install(FILES TChio_anData.h TChio_anPhysics.h TChio_digData.h TChio_digPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

