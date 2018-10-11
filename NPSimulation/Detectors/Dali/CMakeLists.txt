add_library(NPSDali SHARED  Dali.cc)
target_link_libraries(NPSDali NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPDali)
