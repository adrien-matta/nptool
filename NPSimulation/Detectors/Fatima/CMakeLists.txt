add_library(NPSFatima SHARED  Fatima.cc)
target_link_libraries(NPSFatima NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPFatima)
