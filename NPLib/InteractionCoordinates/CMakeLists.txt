ROOT_GENERATE_DICTIONARY( TInteractionCoordinates.h "" TInteractionCoordinatesDict.cxx "../Core")
add_library(NPInteractionCoordinates SHARED TInteractionCoordinates.cxx TInteractionCoordinatesDict.cxx )
target_link_libraries(NPInteractionCoordinates ${ROOT_LIBRARIES} NPCore)
install(FILES TInteractionCoordinates.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
install(FILES TInteractionCoordinatesDict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

