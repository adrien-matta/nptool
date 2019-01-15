add_custom_command(OUTPUT TVamosPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TVamosPhysics.h TVamosPhysicsDict.cxx TVamosPhysics.rootmap libNPVamos.dylib DEPENDS TVamosPhysics.h)
add_custom_command(OUTPUT TVamosDataDict.cxx COMMAND ../../scripts/build_dict.sh TVamosData.h TVamosDataDict.cxx TVamosData.rootmap libNPVamos.dylib DEPENDS TVamosData.h)
add_library(NPVamos SHARED TVamosData.cxx TVamosPhysics.cxx TVamosDataDict.cxx TVamosPhysicsDict.cxx )
target_link_libraries(NPVamos ${ROOT_LIBRARIES} NPCore) 
install(FILES TVamosData.h TVamosPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

