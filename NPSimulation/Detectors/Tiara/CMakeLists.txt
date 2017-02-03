add_library(NPSTiara SHARED  Tiara.cc)
target_link_libraries(NPSTiara NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPTiara)
