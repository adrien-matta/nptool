include ../Makefile.arch

#------------------------------------------------------------------------------
#SHARELIB      =  libCharissaData.so	libCharissaPhysics.so   libCharissaSpectra.so
SHARELIB      =  libCharissa.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
#libCharissa.so: TCharissaData.o TCharissaDataDict.o TCharissaPhysics.o  TCharissaPhysicsDict.o TCharissaSpectra.o 
libCharissa.so: TCharissaData.o TCharissaDataDict.o TCharissaSpectra.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TCharissaDataDict.cxx:	TCharissaData.h
			rootcint -f $@ -c $^

TCharissaPhysicsDict.cxx: TCharissaPhysics.h
			rootcint -f $@ -c $^

# dependances
TCharissaData.o:		TCharissaData.cxx	TCharissaData.h
TCharissaPhysics.o:	TCharissaPhysics.cxx	TCharissaPhysics.h
TCharissaSpectra.o:	TCharissaSpectra.cxx	TCharissaSpectra.h
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


