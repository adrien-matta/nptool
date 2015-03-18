ROOT_GENERATE_DICTIONARY( TCATSData.h "" TCATSDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TCATSPhysics.h "" TCATSPhysicsDict.cxx "../Core")
add_library(NPCATS SHARED TCATSData.cxx TCATSPhysics.cxx TCATSDataDict.cxx TCATSPhysicsDict.cxx TCATSSpectra.cxx)
target_link_libraries(NPCATS ${ROOT_LIBRARIES} NPCore) 
install(FILES TCATSData.h TCATSPhysics.h TCATSSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

