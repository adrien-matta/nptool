ROOT_GENERATE_DICTIONARY( THiraData.h "" THiraDataDict.cxx "")
add_library(NPHira SHARED THiraData.cxx THiraDataDict.cxx )
target_link_libraries(NPHira ${ROOT_LIBRARIES} NPCore) 
install(FILES THiraData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

