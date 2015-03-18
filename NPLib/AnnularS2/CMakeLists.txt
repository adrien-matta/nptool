ROOT_GENERATE_DICTIONARY( TS2Data.h "" TS2DataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TS2Physics.h "" TS2PhysicsDict.cxx "../Core")
add_library(NPS2 SHARED TS2Data.cxx TS2Physics.cxx TS2DataDict.cxx TS2PhysicsDict.cxx )
target_link_libraries(NPS2 ${ROOT_LIBRARIES} NPCore) 
install(FILES TS2Data.h TS2Physics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

