include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libMayaData.so  libConfigReader.so   libMayaHisto.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Maya ##
libMayaData.so:	TMayaData.o	TMayaDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TMayaDataDict.cxx:	TMayaData.h
			rootcint -f $@ -c $^

libMayaHisto.so:	TMayaHisto.o	TMayaHistoDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TMayaHistoDict.cxx:	TMayaHisto.h
			rootcint -f $@ -c $^

libConfigReader.so:	ConfigReader.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

ConfigReaderDict.cxx:	TConfigReader.h
			rootcint -f $@ -c $^

# dependances
TMayaData.o:		TMayaData.cxx	TMayaData.h
TMayaHisto.o:		TMayaHisto.cxx	TMayaHisto.h
ConfigReader.o:	ConfigReader.cxx	ConfigReader.h
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


