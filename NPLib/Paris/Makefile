include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libParis.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Paris ##
libParis.so:	TParisData.o	TParisDataDict.o Paris.o TParisPhysics.o TParisPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TParisDataDict.cxx:	TParisData.h
			rootcint -f $@ -c $^

TParisPhysicsDict.cxx:	TParisPhysics.h
			rootcint -f $@ -c $^


# dependances
Paris.o:	Paris.cxx	Paris.h
TParisData.o:	TParisData.cxx	TParisData.h
TParisPhysics.o:	TParisPhysics.cxx	TParisPhysics.h

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


