add_library(NPSTigress SHARED  Tigress.cc TigressScorers.cc)
target_link_libraries(NPSTigress NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPTigress)
