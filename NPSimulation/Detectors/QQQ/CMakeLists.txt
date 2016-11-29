add_library(NPSQQQ SHARED  QQQ.cc)
target_link_libraries(NPSQQQ NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPQQQ)
