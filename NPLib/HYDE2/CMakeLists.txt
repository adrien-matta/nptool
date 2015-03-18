ROOT_GENERATE_DICTIONARY( THyde2TrackerData.h "" THyde2TrackerDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( THyde2TrackerPhysics.h "" THyde2TrackerPhysicsDict.cxx "../Core")
add_library(NPHyde2Tracker SHARED Hyde2Tracker.cxx Hyde2TrackerModule.cxx Hyde2TrackerTrapezoid1.cxx Hyde2TrackerTrapezoid2.cxx Hyde2TrackerSquare1.cxx THyde2TrackerData.cxx THyde2TrackerPhysics.cxx THyde2TrackerDataDict.cxx THyde2TrackerPhysicsDict.cxx )
target_link_libraries(NPHyde2Tracker ${ROOT_LIBRARIES} NPCore) 
install(FILES THyde2TrackerData.h THyde2TrackerPhysics.h Hyde2Tracker.h Hyde2TrackerModule.h Hyde2TrackerSquare1.h Hyde2TrackerTrapezoid1.h Hyde2TrackerTrapezoid2.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

