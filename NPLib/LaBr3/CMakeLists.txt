ROOT_GENERATE_DICTIONARY( TLaBr3Data.h "" TLaBr3DataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TLaBr3Physics.h "" TLaBr3PhysicsDict.cxx "../Core")
add_library(NPLaBr3 SHARED TLaBr3Data.cxx TLaBr3Physics.cxx TLaBr3DataDict.cxx TLaBr3PhysicsDict.cxx TLaBr3Spectra.cxx)
target_link_libraries(NPLaBr3 ${ROOT_LIBRARIES} NPCore) 
install(FILES TLaBr3Data.h TLaBr3Physics.h TLaBr3Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

