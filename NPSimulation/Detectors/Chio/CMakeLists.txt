add_library(NPSChio SHARED  Chio.cc)
target_link_libraries(NPSChio NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPChio)
