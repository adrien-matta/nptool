if(DEFINED ENV{ROOTSYS})
add_custom_command(OUTPUT TMayaDataDict.cxx COMMAND ../scripts/build_dict.sh TMayaData.h TMayaDataDict.cxx TMayaData.rootmap libNPMaya.dylib DEPENDS TMayaData.h)
add_custom_command(OUTPUT TMayaHistoDict.cxx COMMAND ../scripts/build_dict.sh TMayaHisto.h TMayaHistoDict.cxx TMayaHisto.rootmap libNPMaya.dylib DEPENDS TMayaHisto.h)


add_library(NPMaya SHARED TMayaData.cxx TMayaHisto.cxx TMayaDataDict.cxx TMayaHistoDict.cxx ConfigReader.cxx)
target_link_libraries(NPMaya ${ROOT_LIBRARIES} -lGraf NPCore) 
install(FILES TMayaData.h TMayaHisto.h ConfigReader.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

endif()
