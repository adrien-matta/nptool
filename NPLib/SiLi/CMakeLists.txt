ROOT_GENERATE_DICTIONARY( TSiLiData.h "" TSiLiDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TSiLiPhysics.h "" TSiLiPhysicsDict.cxx "../Core")
add_library(NPSiLi SHARED TSiLiData.cxx TSiLiPhysics.cxx TSiLiDataDict.cxx TSiLiPhysicsDict.cxx TSiLiSpectra.cxx)
target_link_libraries(NPSiLi ${ROOT_LIBRARIES} NPCore) 
install(FILES TSiLiData.h TSiLiPhysics.h TSiLiSpectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

