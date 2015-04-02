add_library(NPInitialConditions SHARED TInitialConditions.cxx TInitialConditionsDict.cxx )
target_link_libraries(NPInitialConditions ${ROOT_LIBRARIES} NPCore)
install(FILES TInitialConditions.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
install(FILES TInitialConditionsDict_rdict.pcm TInitialConditions.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
