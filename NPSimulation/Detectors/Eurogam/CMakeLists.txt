add_library(NPSEurogam SHARED  Eurogam.cc)
target_link_libraries(NPSEurogam NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPEurogam)
