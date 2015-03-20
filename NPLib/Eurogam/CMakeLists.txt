add_custom_command(OUTPUT TEurogamDataDict.cxx TEurogamDataDict_rdict.pcm TEurogamData.rootmap COMMAND rootcint -f TEurogamDataDict.cxx -rmf TEurogamData.rootmap -rml libNPEurogam.dylib -I../Core -c TEurogamData.h DEPENDS TEurogamData.h)
add_library(NPEurogam SHARED TEurogamData.cxx TEurogamDataDict.cxx )
target_link_libraries(NPEurogam ${ROOT_LIBRARIES} NPCore) 
install(FILES TEurogamData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES TEurogamData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TEurogamDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES TEurogamDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
