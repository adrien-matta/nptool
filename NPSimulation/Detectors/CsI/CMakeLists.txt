add_library(NPSCsI SHARED  CsI.cc)
target_link_libraries(NPSCsI NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPCsI)
