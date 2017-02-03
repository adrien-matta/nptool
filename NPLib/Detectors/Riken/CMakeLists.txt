add_custom_command(OUTPUT TRIPSDataDict.cxx COMMAND ../../scripts/build_dict.sh TRIPSData.h TRIPSDataDict.cxx TRIPSData.rootmap libNPRiken.dylib DEPENDS TRIPSData.h)
add_library(NPRiken SHARED TRIPSData.cxx TRIPSDataDict.cxx )
target_link_libraries(NPRiken ${ROOT_LIBRARIES} NPCore) 
install(FILES TRIPSData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

