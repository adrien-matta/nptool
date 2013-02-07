include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTrifoil.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Trifoil ##
libTrifoil.so:	TTrifoilData.o	TTrifoilDataDict.o TTrifoilPhysics.o TTrifoilPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTrifoilDataDict.cxx:	TTrifoilData.h
			rootcint -f $@ -c $^

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


