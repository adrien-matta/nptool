add_custom_command(OUTPUT TAnnularTelescopePhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularTelescopePhysics.h TAnnularTelescopePhysicsDict.cxx TAnnularTelescopePhysics.rootmap libNPAnnularTelescope.dylib DEPENDS TAnnularTelescopePhysics.h)
add_custom_command(OUTPUT TAnnularTelescopeDataDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularTelescopeData.h TAnnularTelescopeDataDict.cxx TAnnularTelescopeData.rootmap libNPAnnularTelescope.dylib DEPENDS TAnnularTelescopeData.h)
add_library(NPAnnularTelescope SHARED TAnnularTelescopeSpectra.cxx TAnnularTelescopeData.cxx TAnnularTelescopePhysics.cxx TAnnularTelescopeDataDict.cxx TAnnularTelescopePhysicsDict.cxx AnnularTelescope_Utils.cxx)
target_link_libraries(NPAnnularTelescope ${ROOT_LIBRARIES} NPCore) 
install(FILES TAnnularTelescopeData.h TAnnularTelescopePhysics.h TAnnularTelescopeSpectra.h AnnularTelescope_Utils.h AnnularTelescopeHits.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

