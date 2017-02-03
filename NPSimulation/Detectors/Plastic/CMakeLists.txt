add_library(NPSPlastic SHARED  Plastic.cc)
target_link_libraries(NPSPlastic NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPPlastic)
