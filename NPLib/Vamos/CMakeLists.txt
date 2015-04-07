add_custom_command(OUTPUT TVamosPlasticDataDict.cxx TVamosPlasticDataDict_rdict.pcm TVamosPlasticData.rootmap COMMAND ../scripts/build_dict.sh TVamosPlasticData.h TVamosPlasticDataDict.cxx TVamosPlasticData.rootmap libNPVamos.dylib DEPENDS TVamosPlasticData.h)
add_custom_command(OUTPUT TVamosFingerDataDict.cxx TVamosFingerDataDict_rdict.pcm TVamosFingerData.rootmap COMMAND ../scripts/build_dict.sh TVamosFingerData.h TVamosFingerDataDict.cxx TVamosFingerData.rootmap libNPVamos.dylib DEPENDS TVamosFingerData.h)
add_custom_command(OUTPUT TVamosDCDataDict.cxx TVamosDCDataDict_rdict.pcm TVamosDCData.rootmap COMMAND ../scripts/build_dict.sh TVamosDCData.h TVamosDCDataDict.cxx TVamosDCData.rootmap libNPVamos.dylib DEPENDS TVamosDCData.h)
add_custom_command(OUTPUT TVamosCHIODataDict.cxx TVamosCHIODataDict_rdict.pcm TVamosCHIOData.rootmap COMMAND ../scripts/build_dict.sh TVamosCHIOData.h TVamosCHIODataDict.cxx TVamosCHIOData.rootmap libNPVamos.dylib DEPENDS TVamosCHIOData.h)
add_library(NPVamos SHARED TVamosCHIOData.cxx TVamosDCData.cxx TVamosPlasticData.cxx TVamosFingerData.cxx TVamosCHIODataDict.cxx TVamosDCDataDict.cxx TVamosPlasticDataDict.cxx TVamosFingerDataDict.cxx )
target_link_libraries(NPVamos ${ROOT_LIBRARIES} NPCore) 
install(FILES TVamosCHIOData.h TVamosDCData.h TVamosPlasticData.h TVamosFingerData.h DESTINATION ${CMAKE_INCLUDE_OUTPUT_DIRECTORY})

