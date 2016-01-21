# Check for gdml support
if(Geant4_gdml_FOUND)  
add_library(NPSAGATA SHARED  AGATA.cc)
target_link_libraries(NPSAGATA NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPAGATA)
endif()
