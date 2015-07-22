add_custom_command(OUTPUT TInitialConditionsDict.cxx COMMAND ../scripts/build_dict.sh TInitialConditions.h TInitialConditionsDict.cxx TInitialConditions.rootmap libNPInitialConditions.so)
add_library(NPInitialConditions SHARED TInitialConditions.cxx TInitialConditionsDict.cxx )
target_link_libraries(NPInitialConditions ${ROOT_LIBRARIES} NPCore)
install(FILES TInitialConditions.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
