ROOT_GENERATE_DICTIONARY( TNanaData.h "" TNanaDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TNanaPhysics.h "" TNanaPhysicsDict.cxx "../Core")
add_library(NPNana SHARED TNanaData.cxx TNanaPhysics.cxx TNanaDataDict.cxx TNanaPhysicsDict.cxx )
target_link_libraries(NPNana ${ROOT_LIBRARIES} NPCore) 
install(FILES TNanaData.h TNanaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

