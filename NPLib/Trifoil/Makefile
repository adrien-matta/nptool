include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTrifoilData.so libTrifoilPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Trifoil ##
libTrifoilData.so:	TTrifoilData.o	TTrifoilDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTrifoilDataDict.cxx:	TTrifoilData.h
			rootcint -f $@ -c $^

libTrifoilPhysics.so:	TTrifoilPhysics.o	TTrifoilPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTrifoilPhysicsDict.cxx:	TTrifoilPhysics.h
			rootcint -f $@ -c $^
			
# dependances
TTrifoilData.o:		TTrifoilData.cxx	TTrifoilData.h
TTrifoilPhysics.o:	TTrifoilPhysics.cxx	TTrifoilPhysics.h
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


