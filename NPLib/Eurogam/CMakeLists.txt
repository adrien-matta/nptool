add_custom_command(OUTPUT TEurogamDataDict.cxx TEurogamDataDict_rdict.pcm TEurogamData.rootmap COMMAND genreflex TEurogamData.h -o TEurogamDataDict.cxx -I../Core --rootmap=TEurogamData.rootmap --rootmap-lib=libNPEurogam --quiet DEPENDS TEurogamData.h)
add_library(NPEurogam SHARED TEurogamData.cxx TEurogamDataDict.cxx )
target_link_libraries(NPEurogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TEurogamData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TEurogamData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TEurogamDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
