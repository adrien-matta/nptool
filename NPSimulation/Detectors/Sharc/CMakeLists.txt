add_library(NPSSharc SHARED  Sharc.cc)
target_link_libraries(NPSSharc NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPSharc)
