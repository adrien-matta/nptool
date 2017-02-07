add_custom_command(OUTPUT TAnnularS4PhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularS4Physics.h TAnnularS4PhysicsDict.cxx TAnnularS4Physics.rootmap libNPAnnularS4.dylib DEPENDS TAnnularS4Physics.h)
add_custom_command(OUTPUT TAnnularS4DataDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularS4Data.h TAnnularS4DataDict.cxx TAnnularS4Data.rootmap libNPAnnularS4.dylib DEPENDS TAnnularS4Data.h)
add_library(NPAnnularS4 SHARED TAnnularS4Spectra.cxx TAnnularS4Data.cxx TAnnularS4Physics.cxx TAnnularS4DataDict.cxx TAnnularS4PhysicsDict.cxx )
target_link_libraries(NPAnnularS4 ${ROOT_LIBRARIES} NPCore) 
install(FILES TAnnularS4Data.h TAnnularS4Physics.h TAnnularS4Spectra.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

