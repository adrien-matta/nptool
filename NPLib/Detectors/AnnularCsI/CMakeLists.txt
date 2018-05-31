add_custom_command(OUTPUT TAnnularCsIPhysicsDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularCsIPhysics.h TAnnularCsIPhysicsDict.cxx TAnnularCsIPhysics.rootmap libNPAnnularCsI.dylib DEPENDS TAnnularCsIPhysics.h)
add_custom_command(OUTPUT TAnnularCsIDataDict.cxx COMMAND ../../scripts/build_dict.sh TAnnularCsIData.h TAnnularCsIDataDict.cxx TAnnularCsIData.rootmap libNPAnnularCsI.dylib DEPENDS TAnnularCsIData.h)
add_library(NPAnnularCsI SHARED TAnnularCsISpectra.cxx TAnnularCsIData.cxx TAnnularCsIPhysics.cxx TAnnularCsIDataDict.cxx TAnnularCsIPhysicsDict.cxx AnnularCsI_Utils.cxx)
target_link_libraries(NPAnnularCsI ${ROOT_LIBRARIES} NPCore) 
install(FILES TAnnularCsIData.h TAnnularCsIPhysics.h TAnnularCsISpectra.h AnnularCsI_Utils.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

