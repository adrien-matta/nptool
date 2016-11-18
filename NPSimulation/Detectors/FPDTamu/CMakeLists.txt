add_library(NPSFPDTamu SHARED  FPDTamu.cc)
target_link_libraries(NPSFPDTamu NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPFPDTamu)
