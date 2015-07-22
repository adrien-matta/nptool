add_custom_command(OUTPUT THyde2TrackerPhysicsDict.cxx COMMAND ../scripts/build_dict.sh THyde2TrackerPhysics.h THyde2TrackerPhysicsDict.cxx THyde2TrackerPhysics.rootmap libNPHYDE2.dylib DEPENDS THyde2TrackerPhysics.h)
add_custom_command(OUTPUT THyde2TrackerDataDict.cxx COMMAND ../scripts/build_dict.sh THyde2TrackerData.h THyde2TrackerDataDict.cxx THyde2TrackerData.rootmap libNPHYDE2.dylib DEPENDS THyde2TrackerData.h)
add_library(NPHYDE2 SHARED Hyde2Tracker.cxx Hyde2TrackerModule.cxx Hyde2TrackerTrapezoid1.cxx Hyde2TrackerTrapezoid2.cxx Hyde2TrackerSquare1.cxx THyde2TrackerData.cxx THyde2TrackerPhysics.cxx THyde2TrackerDataDict.cxx THyde2TrackerPhysicsDict.cxx )
target_link_libraries(NPHYDE2 ${ROOT_LIBRARIES} NPCore) 
install(FILES THyde2TrackerData.h THyde2TrackerPhysics.h Hyde2Tracker.h Hyde2TrackerModule.h Hyde2TrackerSquare1.h Hyde2TrackerTrapezoid1.h Hyde2TrackerTrapezoid2.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

