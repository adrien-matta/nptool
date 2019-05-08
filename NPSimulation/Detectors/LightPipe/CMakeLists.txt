add_library(NPSLightPipe SHARED  LightPipe.cc)
target_link_libraries(NPSLightPipe NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPLightPipe)
