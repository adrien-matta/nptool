add_custom_command(OUTPUT TMayaDataDict.cxx TMayaDataDict_rdict.pcm TMayaData.rootmap COMMAND ../scripts/build_dict.sh TMayaData.h TMayaDataDict.cxx TMayaData.rootmap libNPMaya.dylib DEPENDS TMayaData.h)

add_library(NPMaya SHARED TMayaData.cxx TMayaHisto.cxx TMayaDataDict.cxx TMayaHistoDict.cxx ConfigReader.cxx)
target_link_libraries(NPMaya ${ROOT_LIBRARIES} NPCore) 
install(FILES TMayaData.h TMayaHisto.h ConfigReader.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TMayaData.rootmap TMayaHisto.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMayaDataDict_rdict.pcm TMayaHistoDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TMayaDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
