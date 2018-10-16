add_library(NPSbeam_dump SHARED  beam_dump.cc)
target_link_libraries(NPSbeam_dump NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${NPLib_LIBRARIES} -lNPbeam_dump)
