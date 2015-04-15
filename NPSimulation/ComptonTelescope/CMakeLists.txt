add_library(NPSComptonTelescope SHARED  ComptonTelescope.cc ComptonTelescopeScorers.cc)
target_link_libraries(NPSComptonTelescope NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPComptonTelescope)
