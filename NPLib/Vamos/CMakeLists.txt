ROOT_GENERATE_DICTIONARY( TVamosCHIOData.h "" TVamosCHIODataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TVamosDCData.h "" TVamosDCDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TVamosPlasticData.h "" TVamosPlasticDataDict.cxx "")
ROOT_GENERATE_DICTIONARY( TVamosFingerData.h "" TVamosFingerDataDict.cxx "")
add_library(NPVamos SHARED TVamosCHIOData.cxx TVamosDCData.cxx TVamosPlasticData.cxx TVamosFingerData.cxx TVamosCHIODataDict.cxx TVamosDCDataDict.cxx TVamosPlasticDataDict.cxx TVamosFingerDataDict.cxx )
target_link_libraries(NPVamos ${ROOT_LIBRARIES} NPCore) 
install(FILES TVamosCHIOData.h TVamosDCData.h TVamosPlasticData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

