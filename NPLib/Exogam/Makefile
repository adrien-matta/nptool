include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libExogamData.so	libExogamPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libExogamData.so:	TExogamData.o	TExogamDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TExogamDataDict.cxx:	TExogamData.h
		rootcint -f $@ -c $^

libExogamPhysics.so:	TExogamPhysics.o	TExogamPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TExogamPhysicsDict.cxx: TExogamPhysics.h
		rootcint -f $@ -c $^

# dependances
TExogamData.o:		TExogamData.cxx		TExogamData.h
TExogamPhysics.o:	TExogamPhysics.cxx	TExogamPhysics.h
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


