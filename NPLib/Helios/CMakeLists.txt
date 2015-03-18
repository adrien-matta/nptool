ROOT_GENERATE_DICTIONARY( THeliosData.h "" THeliosDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( THeliosPhysics.h "" THeliosPhysicsDict.cxx "../Core")
add_library(NPHelios SHARED Helios.cxx THeliosData.cxx THeliosPhysics.cxx THeliosDataDict.cxx THeliosPhysicsDict.cxx )
target_link_libraries(NPHelios ${ROOT_LIBRARIES} NPCore) 
install(FILES THeliosData.h THeliosPhysics.h Helios.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

