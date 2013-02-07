include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libVamos.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Vamos ##
libVamos.so: TVamosPlasticData.o   TVamosPlasticDataDict.o TVamosFingerData.o TVamosFingerDataDict.o TVamosCHIOData.o TVamosCHIODataDict.o TVamosDCData.o TVamosDCDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TVamosPlasticDataDict.cxx: TVamosPlasticData.h
	rootcint -f $@ -c $^

TVamosFingerDataDict.cxx:  TVamosFingerData.h
	rootcint -f $@ -c $^

TVamosCHIODataDict.cxx: TVamosCHIOData.h
	rootcint -f $@ -c $^

TVamosDCDataDict.cxx:   TVamosDCData.h
	rootcint -f $@ -c $^


# dependances
TVamosPlasticData.o: 	TVamosPlasticData.cxx   TVamosPlasticData.h
TVamosFingerData.o:  	TVamosFingerData.cxx 	TVamosFingerData.h
TVamosCHIOData.o: 	TVamosCHIOData.cxx   	TVamosCHIOData.h
TVamosDCData.o:   	TVamosDCData.cxx  	TVamosDCData.h
#######################################

############# Clean and More ##########
clean:
	@rm -f core *~ *.o *Dict*

distclean:
	  make clean; rm -f *.so

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<


