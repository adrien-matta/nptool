add_library(NPSDUMMYDetector SHARED  DummyDetector.cc)
target_link_libraries(NPSDUMMYDetector NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPDUMMYDetector)
