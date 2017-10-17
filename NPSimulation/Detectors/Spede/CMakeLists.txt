find_package(cadmesh)
include_directories(${CADMESH_INCLUDE_DIRS})

add_library(NPSSpede SHARED  Spede.cc SpedeHVField.cc)
target_link_libraries(NPSSpede NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} cadmesh -lNPSpede)
