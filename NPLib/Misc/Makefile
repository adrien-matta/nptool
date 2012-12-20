include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libTacData.so   libTacPhysics.so	libTriggerData.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------

############### Detector ##############
libTacData.so:	TTacData.o	TTacDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTacDataDict.cxx:	TTacData.h
			rootcint -f $@ -c $^

libTacPhysics.so:	TTacPhysics.o	TTacPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTacPhysicsDict.cxx:	TTacPhysics.h
			rootcint -f $@ -c $^

libTriggerData.so:	TTriggerData.o	TTriggerDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTriggerDataDict.cxx: TTriggerData.h
			rootcint -f $@ -c $^

# dependances
TTacData.o:		   TTacData.cxx	   TTacData.h
TTacPhysics.o:		TTacPhysics.cxx	TTacPhysics.h
TTriggerData.o:	TTriggerData.cxx	TTriggerData.h
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


