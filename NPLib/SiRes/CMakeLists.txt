ROOT_GENERATE_DICTIONARY( TSiResData.h "" TSiResDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSiResPhysics.h "" TSiResPhysicsDict.cxx "../Core")
add_library(NPSiRes SHARED TSiResData.cxx TSiResPhysics.cxx TSiResDataDict.cxx TSiResPhysicsDict.cxx TSiResSpectra.cxx)
target_link_libraries(NPSiRes ${ROOT_LIBRARIES} NPCore) 
install(FILES TSiResData.h TSiResPhysics.h TSiResSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

