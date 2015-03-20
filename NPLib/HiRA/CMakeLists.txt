add_custom_command(OUTPUT THiraDataDict.cxx THiraDataDict_rdict.pcm THiraData.rootmap COMMAND rootcint -f THiraDataDict.cxx -rmf THiraData.rootmap -rml libNPHiRA.dylib -I../Core -c THiraData.h DEPENDS THiraData.h)
add_library(NPHira SHARED THiraData.cxx THiraDataDict.cxx )
target_link_libraries(NPHira ${ROOT_LIBRARIES} NPCore) 
install(FILES THiraData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

install(FILES THiraData.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES THiraDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
install(FILES THiraDataDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
