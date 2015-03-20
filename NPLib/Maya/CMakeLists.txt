add_custom_command(OUTPUT TMayaDataDict.cxx TMayaDataDict_rdict.pcm TMayaData.rootmap COMMAND genreflex TMayaData.h -o TMayaDataDict.cxx -I../Core --rootmap=TMayaData.rootmap --rootmap-lib=libNPMaya --quiet DEPENDS TMayaData.h)
add_custom_command(OUTPUT TMayaHistoDict.cxx TMayaHistoDict_rdict.pcm TMayaHisto.rootmap COMMAND genreflex TMayaHisto.h -o TMayaHistoDict.cxx -I../Core --rootmap=TMayaHisto.rootmap --rootmap-lib=libNPMaya --quiet DEPENDS TMayaHisto.h)

add_library(NPMaya SHARED TMayaData.cxx TMayaHisto.cxx TMayaDataDict.cxx TMayaHistoDict.cxx ConfigReader.cxx)
target_link_libraries(NPMaya ${ROOT_LIBRARIES} NPCore) 
install(FILES TMayaData.h TMayaHisto.h ConfigReader.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TMayaData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMayaDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
