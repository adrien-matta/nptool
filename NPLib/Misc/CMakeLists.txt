ROOT_GENERATE_DICTIONARY( TTacData.h "" TTacDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TTriggerData.h "" TTriggerDataDict.cxx "")
add_library(NPMisc SHARED TTacData.cxx TTriggerData.cxx TTacDataDict.cxx TTriggerDataDict.cxx)
target_link_libraries(NPMisc ${ROOT_LIBRARIES} NPCore) 
install(FILES TTacData.h TTriggerData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

