enable_language(Fortran)
set (CMAKE_Fortran_FLAGS "-finit-local-zero -falign-commons -fno-automatic")
add_library(NPSMDM SHARED  MDM.cc MDMTrace.cpp Rayin.cpp RAYTKIN1.F)
#add_custom_command(TARGET NPSMDM PRE_BUILD COMMAND gfortran ARGS -c -finit-local-zero -falign-commons -fno-automatic RAYTKIN1.F)
target_link_libraries(NPSMDM NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPMDM)

# # FORTRAN SETUP
# set (CMAKE_Fortran_COMPILER "gfortran")
# set (CMAKE_Fortran_FLAGS "-finit-local-zero -falign-commons -fno-automatic")
