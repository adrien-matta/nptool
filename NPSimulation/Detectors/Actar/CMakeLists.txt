add_library(NPSActar SHARED  Actar.cc)
target_link_libraries(NPSActar NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPActar)
