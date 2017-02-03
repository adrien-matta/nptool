# Check for gdml support
if(Geant4_gdml_FOUND)  
add_library(NPSSpice SHARED  Spice.cc)
target_link_libraries(NPSSpice NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} )
endif()
