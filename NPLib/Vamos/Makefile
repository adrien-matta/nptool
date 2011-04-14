include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libVamosPlasticData.so	libVamosFingerData.so \
		 libVamosCHIOData.so	libVamosDCData.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libVamosPlasticData.so: TVamosPlasticData.o   TVamosPlasticDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TVamosPlasticDataDict.cxx: TVamosPlasticData.h
	rootcint -f $@ -c $^

libVamosFingerData.so:  TVamosFingerData.o   TVamosFingerDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TVamosFingerDataDict.cxx:  TVamosFingerData.h
	rootcint -f $@ -c $^

libVamosCHIOData.so: TVamosCHIOData.o   TVamosCHIODataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TVamosCHIODataDict.cxx: TVamosCHIOData.h
	rootcint -f $@ -c $^

libVamosDCData.so:   TVamosDCData.o   TVamosDCDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

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


