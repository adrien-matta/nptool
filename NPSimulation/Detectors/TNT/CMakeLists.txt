add_library(NPSTNT SHARED  TNT.cc)
target_link_libraries(NPSTNT NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPTNT)
