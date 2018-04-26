add_library(NPSAnnularTelescope SHARED  AnnularTelescope.cc)
target_link_libraries(NPSAnnularTelescope NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPAnnularTelescope)
