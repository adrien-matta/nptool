add_custom_command(OUTPUT TCsIPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TCsIPhysics.h TCsIPhysicsDict.cxx TCsIPhysics.rootmap libNPCsI.dylib DEPENDS TCsIPhysics.h)
add_custom_command(OUTPUT TCsIDataDict.cxx COMMAND ../../scripts/build_dict.sh TCsIData.h TCsIDataDict.cxx TCsIData.rootmap libNPCsI.dylib DEPENDS TCsIData.h)
add_library(NPCsI SHARED TCsIData.cxx TCsIPhysics.cxx TCsIDataDict.cxx TCsIPhysicsDict.cxx )
target_link_libraries(NPCsI ${ROOT_LIBRARIES} NPCore) 
install(FILES TCsIData.h TCsIPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

