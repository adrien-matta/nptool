add_library(NPSMDM SHARED  MDM.cc)
target_link_libraries(NPSMDM NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPMDM)
