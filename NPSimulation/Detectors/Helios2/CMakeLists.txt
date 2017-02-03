add_library(NPSHelios2 SHARED  Helios2.cc)
target_link_libraries(NPSHelios2 NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPHelios2)
