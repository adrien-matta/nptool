ROOT_GENERATE_DICTIONARY( TChateauCristalData.h "" TChateauCristalDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TChateauCristalPhysics.h "" TChateauCristalPhysicsDict.cxx "../Core")
add_library(NPChateauCristal SHARED TChateauCristalData.cxx TChateauCristalPhysics.cxx TChateauCristalDataDict.cxx TChateauCristalPhysicsDict.cxx )
target_link_libraries(NPChateauCristal ${ROOT_LIBRARIES} NPCore) 
install(FILES TChateauCristalData.h TChateauCristalPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

