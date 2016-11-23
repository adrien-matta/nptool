add_library(NPSNana SHARED  Nana.cc)
target_link_libraries(NPSNana NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPNana)
