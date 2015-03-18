ROOT_GENERATE_DICTIONARY( TMayaData.h "" TMayaDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TMayaHisto.h "" TMayaHistoDict.cxx "")
ROOT_GENERATE_DICTIONARY( ConfigReader.h "" ConfigReaderDict.cxx "")
add_library(NPMaya SHARED TMayaData.cxx TMayaHisto.cxx TMayaDataDict.cxx TMayaHistoDict.cxx ConfigReader.cxx)
target_link_libraries(NPMaya ${ROOT_LIBRARIES} NPCore) 
install(FILES TMayaData.h TMayaHisto.h ConfigReader.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

