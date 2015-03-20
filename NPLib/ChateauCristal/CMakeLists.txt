add_custom_command(OUTPUT TChateauCristalPhysicsDict.cxx TChateauCristalPhysicsDict_rdict.pcm TChateauCristalPhysics.rootmap COMMAND genreflex TChateauCristalPhysics.h -o TChateauCristalPhysicsDict.cxx -I../Core --rootmap=TChateauCristalPhysics.rootmap --rootmap-lib=libNPChateauCristal --quiet DEPENDS TChateauCristalPhysics.h)
add_custom_command(OUTPUT TChateauCristalDataDict.cxx TChateauCristalDataDict_rdict.pcm TChateauCristalData.rootmap COMMAND genreflex TChateauCristalData.h -o TChateauCristalDataDict.cxx -I../Core --rootmap=TChateauCristalData.rootmap --rootmap-lib=libNPChateauCristal --quiet DEPENDS TChateauCristalData.h)
add_library(NPChateauCristal SHARED TChateauCristalData.cxx TChateauCristalPhysics.cxx TChateauCristalDataDict.cxx TChateauCristalPhysicsDict.cxx )
target_link_libraries(NPChateauCristal ${ROOT_LIBRARIES} NPCore) 
install(FILES TChateauCristalData.h TChateauCristalPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TChateauCristalData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TChateauCristalPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
