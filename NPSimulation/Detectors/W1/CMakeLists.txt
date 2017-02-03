add_library(NPSW1 SHARED  W1.cc W1Scorers.cc)
target_link_libraries(NPSW1 NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPW1)
