add_library(NPSForwardArray SHARED  ForwardArray.cc)
target_link_libraries(NPSForwardArray NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPForwardArray)
