add_custom_command(OUTPUT TMayaDataDict.cxx TMayaDataDict_rdict.pcm TMayaData.rootmap COMMAND rootcint -f TMayaDataDict.cxx -rmf TMayaData.rootmap -rml libNPMaya.dylib -I../Core -c TMayaData.h DEPENDS TMayaData.h)
add_custom_command(OUTPUT TMayaHistoDict.cxx TMayaHistoDict_rdict.pcm TMayaHisto.rootmap COMMAND rootcint -f TMayaHistoDict.cxx -rmf TMayaHisto.rootmap -rml libNPMaya.dylib -I../Core -c TMayaHisto.h DEPENDS TMayaHisto.h)

add_library(NPMaya SHARED TMayaData.cxx TMayaHisto.cxx TMayaDataDict.cxx TMayaHistoDict.cxx ConfigReader.cxx)
target_link_libraries(NPMaya ${ROOT_LIBRARIES} NPCore) 
install(FILES TMayaData.h TMayaHisto.h ConfigReader.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TMayaData.rootmap TMayaHisto.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMayaDataDict_rdict.pcm TMayaHistoDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMayaDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
