add_custom_command(OUTPUT TLaBr3PhysicsDict.cxx COMMAND ../scripts/build_dict.sh TLaBr3Physics.h TLaBr3PhysicsDict.cxx TLaBr3Physics.rootmap libNPLaBr3.dylib DEPENDS TLaBr3Physics.h)
add_custom_command(OUTPUT TLaBr3DataDict.cxx COMMAND ../scripts/build_dict.sh TLaBr3Data.h TLaBr3DataDict.cxx TLaBr3Data.rootmap libNPLaBr3.dylib DEPENDS TLaBr3Data.h)
add_library(NPLaBr3 SHARED TLaBr3Data.cxx TLaBr3Physics.cxx TLaBr3DataDict.cxx TLaBr3PhysicsDict.cxx TLaBr3Spectra.cxx)
target_link_libraries(NPLaBr3 ${ROOT_LIBRARIES} NPCore) 
install(FILES TLaBr3Data.h TLaBr3Physics.h TLaBr3Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

