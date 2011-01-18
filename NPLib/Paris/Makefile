include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libParisData.so	libParisPhysics.so 
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Paris ##
libParisData.so:	TParisData.o	TParisDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TParisDataDict.cxx:	TParisData.h
			rootcint -f $@ -c $^

libParisPhysics.so:	Paris.o TParisPhysics.o TParisPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

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


