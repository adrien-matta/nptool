include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libChateauCristalData.so	libChateauCristalPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## ChateauCristal ##
libChateauCristalData.so:	TChateauCristalData.o	TChateauCristalDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChateauCristalDataDict.cxx:	TChateauCristalData.h
			rootcint -f $@ -c $^

libChateauCristalPhysics.so:	TChateauCristalPhysics.o	TChateauCristalPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChateauCristalPhysicsDict.cxx: TChateauCristalPhysics.h
			rootcint -f $@ -c $^

# dependances
TChateauCristalData.o:		TChateauCristalData.cxx	TChateauCristalData.h
TChateauCristalDataDict.o:		TChateauCristalDataDict.cxx	TChateauCristalDataDict.h
TChateauCristalPhysics.o:	TChateauCristalPhysics.cxx	TChateauCristalPhysics.h
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


