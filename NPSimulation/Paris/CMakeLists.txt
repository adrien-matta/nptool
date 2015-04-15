add_library(NPSParis SHARED  Paris.cc)
target_link_libraries(NPSParis NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPParis)
