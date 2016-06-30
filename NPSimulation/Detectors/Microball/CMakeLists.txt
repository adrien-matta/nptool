add_library(NPSMicroball SHARED  Microball.cc)
target_link_libraries(NPSMicroball NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPMicroball)
