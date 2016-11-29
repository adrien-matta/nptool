add_library(NPSGASPARD SHARED  GaspardScorers.cc GaspardTracker.cc GaspardTrackerAnnular.cc GaspardTrackerDummyShape.cc GaspardTrackerModule.cc GaspardTrackerRectangle.cc GaspardTrackerSquare.cc GaspardTrackerTrapezoid.cc)
target_link_libraries(NPSGASPARD NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPGASPARD)
