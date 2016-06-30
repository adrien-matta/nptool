add_library(NPSNeutronWall SHARED  NeutronWall.cc)
target_link_libraries(NPSNeutronWall NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPNeutronWall)
