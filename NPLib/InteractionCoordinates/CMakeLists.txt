add_custom_command(OUTPUT TInteractionCoordinatesDict.cxx TInteractionCoordinatesDict_rdict.pcm TInteractionCoordinates.rootmap COMMAND rootcint -f TInteractionCoordinatesDict.cxx -rml TInteractionCoordinates.rootmap -rmf libNPInteractionCoordinates.dylib -I../Core -c TInteractionCoordinates.h)
add_library(NPInteractionCoordinates SHARED TInteractionCoordinates.cxx TInteractionCoordinatesDict.cxx )
target_link_libraries(NPInteractionCoordinates ${ROOT_LIBRARIES} NPCore)
install(FILES TInteractionCoordinates.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})
install(FILES TInteractionCoordinatesDict_rdict.pcm TInteractionCoordinates.rootmap DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

