add_library(NPSLassa SHARED  Lassa.cc)
target_link_libraries(NPSLassa NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPLassa)
