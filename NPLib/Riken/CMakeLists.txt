add_custom_command(OUTPUT TRIPSDataDict.cxx TRIPSDataDict_rdict.pcm TRIPSData.rootmap COMMAND rootcint -f TRIPSDataDict.cxx -rmf TRIPSData.rootmap -rml libNPRiken.dylib -I../Core -c TRIPSData.h DEPENDS TRIPSData.h)
add_library(NPRIPS SHARED TRIPSData.cxx TRIPSDataDict.cxx )
target_link_libraries(NPRIPS ${ROOT_LIBRARIES} NPCore) 
install(FILES TRIPSData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TRIPSData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TRIPSDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TRIPSDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
