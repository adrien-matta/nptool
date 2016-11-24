add_custom_command(OUTPUT NPOnlineDict.cxx COMMAND ../scripts/build_dict.sh NPOnline.h NPOnlineDict.cxx NPOnline.rootmap libNPCore.dylib NPOnlineLinkDef.h DEPENDS NPOnline.h)
add_library(NPOnline SHARED NPOnline.cxx NPOnlineDict.cxx )
target_link_libraries(NPOnline ${ROOT_LIBRARIES} Net Thread NPCore) 
install(FILES NPOnline.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
