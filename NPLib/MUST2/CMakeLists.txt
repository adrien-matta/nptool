ROOT_GENERATE_DICTIONARY( TMust2Data.h "" TMust2DataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TMust2Physics.h "" TMust2PhysicsDict.cxx "../Core")
add_library(NPMust2 SHARED TMust2Data.cxx TMust2Physics.cxx TMust2DataDict.cxx TMust2PhysicsDict.cxx TMust2Spectra.cxx)
target_link_libraries(NPMust2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TMust2Data.h TMust2Physics.h TMust2Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

