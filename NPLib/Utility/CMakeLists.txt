add_executable(nptool-installer nptool-installer.cxx)
target_link_libraries(nptool-installer ${ROOT_LIBRARIES} NPCore -lThread -lGraf -lHist -lMatrix) 
add_executable(npanalysis npanalysis.cxx)
target_link_libraries(npanalysis ${ROOT_LIBRARIES} NPCore NPInitialConditions NPInteractionCoordinates -lThread -lGraf -lHist -lMatrix) 
add_executable(nponline nponline.cxx)
target_link_libraries(nponline ${ROOT_LIBRARIES} NPCore NPOnline) 
add_executable(nptool-cleaner nptool-cleaner.cxx)
target_link_libraries(nptool-cleaner ${ROOT_LIBRARIES} NPCore -lThread -lGraf -lHist -lMatrix) 
add_executable(nptool-wizard nptool-wizard.cxx)
target_link_libraries(nptool-wizard ${ROOT_LIBRARIES} NPCore -lThread -lGraf -lHist -lMatrix) 


install(PROGRAMS nptool-wizard nptool-cleaner nptool-installer npanalysis nponline DESTINATION ${CMAKE_BINARY_OUTPUT_DIRECTORY})
install(SCRIPT ../scripts/post_install.cmake ${DETLIST})
