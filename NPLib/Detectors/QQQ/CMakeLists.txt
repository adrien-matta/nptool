add_custom_command(OUTPUT TQQQPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TQQQPhysics.h TQQQPhysicsDict.cxx TQQQPhysics.rootmap libNPQQQ.dylib DEPENDS TQQQPhysics.h)
add_custom_command(OUTPUT TQQQDataDict.cxx COMMAND ../../scripts/build_dict.sh TQQQData.h TQQQDataDict.cxx TQQQData.rootmap libNPQQQ.dylib DEPENDS TQQQData.h)
add_library(NPQQQ SHARED TQQQData.cxx TQQQPhysics.cxx TQQQDataDict.cxx TQQQPhysicsDict.cxx )
target_link_libraries(NPQQQ ${ROOT_LIBRARIES} NPCore) 
install(FILES TQQQData.h TQQQPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

