add_custom_command(OUTPUT TLassaPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TLassaPhysics.h TLassaPhysicsDict.cxx TLassaPhysics.rootmap libNPLassa.dylib DEPENDS TLassaPhysics.h)
add_custom_command(OUTPUT TLassaDataDict.cxx COMMAND ../../scripts/build_dict.sh TLassaData.h TLassaDataDict.cxx TLassaData.rootmap libNPLassa.dylib DEPENDS TLassaData.h)
add_library(NPLassa SHARED TLassaData.cxx TLassaPhysics.cxx TLassaDataDict.cxx TLassaPhysicsDict.cxx )
target_link_libraries(NPLassa ${ROOT_LIBRARIES} NPCore) 
install(FILES TLassaData.h TLassaPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

