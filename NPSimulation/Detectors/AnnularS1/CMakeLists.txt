add_library(NPSAnnularS1 SHARED  AnnularS1.cc)
target_link_libraries(NPSAnnularS1 NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPAnnularS1)
