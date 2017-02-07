add_library(NPSSage SHARED  Sage.cc SageHVField.cc)
target_link_libraries(NPSSage NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPSage)
