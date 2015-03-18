ROOT_GENERATE_DICTIONARY( TGaspardTrackerData.h "" TGaspardTrackerDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TGaspardTrackerPhysics.h "" TGaspardTrackerPhysicsDict.cxx "../Core")
add_library(NPGaspardTracker SHARED GaspardTracker.cxx GaspardTrackerAnnular.cxx GaspardTrackerDummyShape.cxx GaspardTrackerModule.cxx GaspardTrackerTrapezoid.cxx GaspardTrackerSquare.cxx GaspardTrackerRectangle.cxx TGaspardTrackerData.cxx TGaspardTrackerPhysics.cxx TGaspardTrackerDataDict.cxx TGaspardTrackerPhysicsDict.cxx )
target_link_libraries(NPGaspardTracker ${ROOT_LIBRARIES} NPCore) 
install(FILES TGaspardTrackerData.h TGaspardTrackerPhysics.h GaspardTracker.h GaspardTrackerAnnular.h GaspardTrackerDummyShape.h GaspardTrackerModule.h GaspardTrackerRectangle.h GaspardTrackerSquare.h GaspardTrackerTrapezoid.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

