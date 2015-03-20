add_custom_command(OUTPUT TDUMMYDetectorDataDict.cxx TDUMMYDetectorDataDict_rdict.pcm TDUMMYDetectorData.rootmap COMMAND genreflex TDUMMYDetectorData.h -o TDUMMYDetectorDataDict.cxx -I../Core --rootmap=TDUMMYDetectorData.rootmap --rootmap-lib=libNPDummyDetector --quiet DEPENDS TDUMMYDetectorData.h)
add_library(NPDUMMYDetector SHARED TDUMMYDetectorData.cxx TDUMMYDetectorDataDict.cxx )
target_link_libraries(NPDUMMYDetector ${ROOT_LIBRARIES} NPCore) 
install(FILES TDUMMYDetectorData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TDUMMYDetectorData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
