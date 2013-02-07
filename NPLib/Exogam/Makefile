include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libExogam.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## EXOGAM ##
libExogam.so:	TExogamData.o	TExogamDataDict.o TExogamPhysics.o	TExogamPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TExogamDataDict.cxx:	TExogamData.h
		rootcint -f $@ -c $^

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


