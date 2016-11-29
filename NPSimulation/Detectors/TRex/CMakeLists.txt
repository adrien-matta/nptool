# Check for gdml support
if(Geant4_gdml_FOUND)  
add_library(NPSTRex SHARED  TRex.cc)
target_link_libraries(NPSTRex NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPTRex)
endif()
