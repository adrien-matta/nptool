add_custom_command(OUTPUT TRIPSDataDict.cxx TRIPSDataDict_rdict.pcm TRIPSData.rootmap COMMAND ../scripts/build_dict.sh TRIPSData.h TRIPSDataDict.cxx TRIPSData.rootmap libNPRiken.dylib DEPENDS TRIPSData.h)
add_library(NPRIPS SHARED TRIPSData.cxx TRIPSDataDict.cxx )
target_link_libraries(NPRIPS ${ROOT_LIBRARIES} NPCore) 
install(FILES TRIPSData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TRIPSData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TRIPSDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TRIPSDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
