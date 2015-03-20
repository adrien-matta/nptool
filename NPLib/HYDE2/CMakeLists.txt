add_custom_command(OUTPUT THyde2TrackerPhysicsDict.cxx THyde2TrackerPhysicsDict_rdict.pcm THyde2TrackerPhysics.rootmap COMMAND genreflex THyde2TrackerPhysics.h -o THyde2TrackerPhysicsDict.cxx -I../Core --rootmap=THyde2TrackerPhysics.rootmap --rootmap-lib=libNPHYDE2 --quiet DEPENDS THyde2TrackerPhysics.h)
add_custom_command(OUTPUT THyde2TrackerDataDict.cxx THyde2TrackerDataDict_rdict.pcm THyde2TrackerData.rootmap COMMAND genreflex THyde2TrackerData.h -o THyde2TrackerDataDict.cxx -I../Core --rootmap=THyde2TrackerData.rootmap --rootmap-lib=libNPHYDE2 --quiet DEPENDS THyde2TrackerData.h)
add_library(NPHyde2Tracker SHARED Hyde2Tracker.cxx Hyde2TrackerModule.cxx Hyde2TrackerTrapezoid1.cxx Hyde2TrackerTrapezoid2.cxx Hyde2TrackerSquare1.cxx THyde2TrackerData.cxx THyde2TrackerPhysics.cxx THyde2TrackerDataDict.cxx THyde2TrackerPhysicsDict.cxx )
target_link_libraries(NPHyde2Tracker ${ROOT_LIBRARIES} NPCore) 
install(FILES THyde2TrackerData.h THyde2TrackerPhysics.h Hyde2Tracker.h Hyde2TrackerModule.h Hyde2TrackerSquare1.h Hyde2TrackerTrapezoid1.h Hyde2TrackerTrapezoid2.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES THyde2TrackerData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES THyde2TrackerPhysics.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
