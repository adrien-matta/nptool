add_library(NPModularLeaf SHARED TModularLeafPhysics.cxx)
target_link_libraries(NPModularLeaf ${ROOT_LIBRARIES} NPCore) 
install(FILES  TModularLeafPhysics.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

