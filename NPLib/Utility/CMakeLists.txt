add_executable(nptool-libmap nptool-libmap.cxx)
target_link_libraries(nptool-libmap ${ROOT_LIBRARIES} NPCore) 
add_executable(npanalysis npanalysis.cxx)
target_link_libraries(npanalysis ${ROOT_LIBRARIES} NPCore) 
install(PROGRAMS nptool-libmap npanalysis DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})

