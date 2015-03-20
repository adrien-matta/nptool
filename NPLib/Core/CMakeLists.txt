add_custom_command(OUTPUT TAsciiFileDict.cxx TAsciiFile.rootmap TAsciiFileDict_rdict.pcm COMMAND rootcint -f TAsciiFileDict.cxx -rmf TAsciiFile.rootmap -rml libNPCore.dylib -c TAsciiFile.h)
add_custom_command(OUTPUT NPVDetectorDict.cxx NPVDetectorDict_rdict.pcm NPVDetector.rootmap COMMAND rootcint -f NPVDetectorDict.cxx -rmf NPVDetector.rootmap -rml libNPCore.dylib -c NPVDetector.h NPCoreLinkdef.h)
  
add_library(NPCore SHARED NPVAnalysis.cxx NPAnalysisFactory.cxx NPCalibrationManager.cxx NPOptionManager.cxx RootOutput.cxx RootInput.cxx TAsciiFile.cxx TAsciiFileDict.cxx NPDetectorManager.cxx NPVDetector.cxx NPVDetectorDict.cxx NPVSpectra.cxx NPDetectorFactory.cxx)
target_link_libraries(NPCore ${ROOT_LIBRARIES}) 
install(FILES NPVAnalysis.h NPAnalysisFactory.h  NPCalibrationManager.h NPOptionManager.h RootInput.h RootOutput.h TAsciiFile.h NPDetectorManager.h NPVDetector.h NPGlobalSystemOfUnits.h NPPhysicalConstants.h NPSystemOfUnits.h NPVSpectra.h NPDetectorFactory.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
install(FILES NPVDetectorDict_rdict.pcm TAsciiFileDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES NPVDetector.rootmap TAsciiFile.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
