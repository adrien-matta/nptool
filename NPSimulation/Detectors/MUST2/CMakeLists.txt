add_library(NPSMUST2 SHARED  MUST2Array.cc MUST2Scorers.cc)
target_link_libraries(NPSMUST2 NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPMUST2)
