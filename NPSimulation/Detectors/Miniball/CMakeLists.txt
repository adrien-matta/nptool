# Check for gdml support
if(Geant4_gdml_FOUND)  
add_library(NPSMiniball SHARED  Miniball.cc)
target_link_libraries(NPSMiniball NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPMiniball)
endif()
