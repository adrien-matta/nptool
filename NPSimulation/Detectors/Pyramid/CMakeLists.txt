add_library(NPSPyramid SHARED  Pyramid.cc)
target_link_libraries(NPSPyramid NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPPyramid)
