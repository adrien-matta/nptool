include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libChateauCristal.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## ChateauCristal ##
libChateauCristal.so: TChateauCristalData.o TChateauCristalDataDict.o TChateauCristalPhysics.o TChateauCristalPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChateauCristalDataDict.cxx:	TChateauCristalData.h
			rootcint -f $@ -c $^

TChateauCristalPhysicsDict.cxx: TChateauCristalPhysics.h
			rootcint -f $@ -c $^

# dependances
TChateauCristalData.o: TChateauCristalData.cxx TChateauCristalData.h
TChateauCristalDataDict.o: TChateauCristalData.cxx TChateauCristalData.h
TChateauCristalPhysicsDict.o: TChateauCristalPhysics.cxx TChateauCristalPhysics.h
TChateauCristalPhysics.o: TChateauCristalPhysics.cxx TChateauCristalPhysics.h
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


