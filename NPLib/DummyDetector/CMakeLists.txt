ROOT_GENERATE_DICTIONARY( TDUMMYDetectorData.h "" TDUMMYDetectorDataDict.cxx "")
add_library(NPDUMMYDetector SHARED TDUMMYDetectorData.cxx TDUMMYDetectorDataDict.cxx )
target_link_libraries(NPDUMMYDetector ${ROOT_LIBRARIES} NPCore) 
install(FILES TDUMMYDetectorData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

