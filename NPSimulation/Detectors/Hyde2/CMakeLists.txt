add_library(NPSHyde2 SHARED  Hyde2Scorers.cc Hyde2Tracker.cc Hyde2TrackerModule.cc Hyde2TrackerSquare1.cc Hyde2TrackerTrapezoid1.cc Hyde2TrackerTrapezoid2.cc)
target_link_libraries(NPSHyde2 NPSCore ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} NPHYDE2)
