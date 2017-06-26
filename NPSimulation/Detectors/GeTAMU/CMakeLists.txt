add_library(NPSGeTAMU SHARED  GeTAMU.cc GeTAMUScorers.cc)
target_link_libraries(NPSGeTAMU NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPGeTAMU)
