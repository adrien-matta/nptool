add_custom_command(OUTPUT TDUMMYDetectorDataDict.cxx COMMAND ../scripts/build_dict.sh TDUMMYDetectorData.h TDUMMYDetectorDataDict.cxx TDUMMYDetectorData.rootmap libNPDummyDetector.dylib DEPENDS TDUMMYDetectorData.h)
add_library(NPDummyDetector SHARED TDUMMYDetectorData.cxx TDUMMYDetectorDataDict.cxx )
target_link_libraries(NPDummyDetector ${ROOT_LIBRARIES} NPCore) 
install(FILES TDUMMYDetectorData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

