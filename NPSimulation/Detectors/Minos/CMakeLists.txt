add_library(NPSMinos SHARED  Minos.cc)
target_link_libraries(NPSMinos NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPMinos)
