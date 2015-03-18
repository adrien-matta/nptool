ROOT_GENERATE_DICTIONARY( TEurogamData.h "" TEurogamDataDict.cxx "")
add_library(NPEurogam SHARED TEurogamData.cxx TEurogamDataDict.cxx )
target_link_libraries(NPEurogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TEurogamData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

