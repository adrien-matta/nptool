include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libExlData.so libExlPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Exl ##
libExlData.so:	TExlData.o	TExlDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TExlDataDict.cxx:	TExlData.h
			rootcint -f $@ -c $^

libExlPhysics.so:	TExlPhysics.o	TExlPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TExlPhysicsDict.cxx:	TExlPhysics.h
			rootcint -f $@ -c $^

# dependances
TExlData.o:	TExlData.cxx	TExlData.h
TExlPhysics.o:	TExlPhysics.cxx	TExlPhysics.h
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


