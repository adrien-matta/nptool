ROOT_GENERATE_DICTIONARY( TTigressData.h "" TTigressDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TTigressPhysics.h "" TTigressPhysicsDict.cxx "../Core")
add_library(NPTigress SHARED TTigressData.cxx TTigressPhysics.cxx TTigressDataDict.cxx TTigressPhysicsDict.cxx )
target_link_libraries(NPTigress ${ROOT_LIBRARIES} NPCore) 
install(FILES TTigressData.h TTigressPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

