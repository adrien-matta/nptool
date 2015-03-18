ROOT_GENERATE_DICTIONARY( TComptonTelescopeData.h "" TComptonTelescopeDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TComptonTelescopePhysics.h "" TComptonTelescopePhysicsDict.cxx "../Core")
add_library(NPComptonTelescope SHARED TComptonTelescopeData.cxx TComptonTelescopePhysics.cxx TComptonTelescopeDataDict.cxx TComptonTelescopePhysicsDict.cxx TComptonTelescopeSpectra.cxx)
target_link_libraries(NPComptonTelescope ${ROOT_LIBRARIES} NPCore) 
install(FILES TComptonTelescopeData.h TComptonTelescopePhysics.h TComptonTelescopeSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

