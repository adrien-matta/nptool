add_library(NPSTigress SHARED  Tigress.cc)
target_link_libraries(NPSTigress NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPTigress)
