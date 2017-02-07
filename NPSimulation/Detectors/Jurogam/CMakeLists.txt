add_library(NPSJurogam SHARED  Jurogam.cc PhaseI.cc PhaseIBGO.cc Clover.cc CloverBGO.cc Materials.cc)
target_link_libraries(NPSJurogam NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPJurogam)
