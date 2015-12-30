add_custom_command(OUTPUT TTacPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TTacPhysics.h TTacPhysicsDict.cxx TTacPhysics.rootmap libNPGANIL.dylib DEPENDS TTacPhysics.h)
add_custom_command(OUTPUT TTriggerDataDict.cxx COMMAND ../../scripts/build_dict.sh TTriggerData.h TTriggerDataDict.cxx TTriggerData.rootmap libNPGANIL.dylib DEPENDS TTriggerData.h)
add_custom_command(OUTPUT TTacDataDict.cxx COMMAND ../../scripts/build_dict.sh TTacData.h TTacDataDict.cxx TTacData.rootmap libNPGANIL.dylib DEPENDS TTacData.h)
add_library(NPGANIL SHARED TTacPhysics.cxx TTacData.cxx TTriggerData.cxx TTacDataDict.cxx TTriggerDataDict.cxx TTacPhysicsDict.cxx)
target_link_libraries(NPGANIL ${ROOT_LIBRARIES} NPCore) 
install(FILES TTacData.h TTriggerData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

