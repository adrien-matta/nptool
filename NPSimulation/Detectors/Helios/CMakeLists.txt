add_library(NPSHelios SHARED  Helios.cc)
target_link_libraries(NPSHelios NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPHelios)
