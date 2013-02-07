include ../Makefile.arch

#------------------------------------------------------------------------------
#SHARELIB      =  libMust2Data.so	libMust2Physics.so   libMust2Spectra.so
SHARELIB      =  libMust2.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libMust2.so: TMust2Data.o TMust2DataDict.o TMust2Physics.o TMust2Spectra.o TMust2SpectraDict.o TMust2PhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TMust2DataDict.cxx:	TMust2Data.h
			rootcint -f $@ -c $^

TMust2PhysicsDict.cxx: TMust2Physics.h
			rootcint -f $@ -c $^

TMust2SpectraDict.cxx: TMust2Spectra.h
			rootcint -f $@ -c $^

# dependances
TMust2Data.o:		TMust2Data.cxx	TMust2Data.h
TMust2Physics.o:	TMust2Physics.cxx	TMust2Physics.h
TMust2Spectra.o:	TMust2Spectra.cxx	TMust2Spectra.h
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


