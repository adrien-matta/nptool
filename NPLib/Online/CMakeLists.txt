add_custom_command(OUTPUT NPOnlineDict.cxx COMMAND ../scripts/build_dict.sh NPOnlineGUI.h NPOnlineDict.cxx NPOnline.rootmap libNPCore.dylib NPOnlineLinkDef.h DEPENDS NPOnlineGUI.h)
add_library(NPOnline SHARED NPSpectraClient.cxx NPOnlineGUI.cxx NPOnlineDict.cxx )
target_link_libraries(NPOnline ${ROOT_LIBRARIES} Thread NPCore) 
install(FILES NPOnlineGUI.h NPSpectraClient.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
