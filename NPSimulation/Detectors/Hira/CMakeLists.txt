add_library(NPSHira SHARED  Hira.cc)
target_link_libraries(NPSHira NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPHira)
