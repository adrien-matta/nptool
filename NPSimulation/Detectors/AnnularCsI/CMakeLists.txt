add_library(NPSAnnularCsI SHARED  AnnularCsI.cc)
target_link_libraries(NPSAnnularCsI NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPAnnularCsI)
