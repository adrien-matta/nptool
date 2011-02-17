include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libCATSData.so	libCATSPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## CATS ##
libCATSData.so:	TCATSData.o	TCATSDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TCATSDataDict.cxx:	TCATSData.h
			rootcint -f $@ -c $^

libCATSPhysics.so:	TCATSPhysics.o	TCATSPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TCATSPhysicsDict.cxx: TCATSPhysics.h
			rootcint -f $@ -c $^

# dependances
TCATSData.o:		TCATSData.cxx	TCATSData.h
TCATSDataDict.o:		TCATSDataDict.cxx	TCATSDataDict.h
TCATSPhysics.o:	TCATSPhysics.cxx	TCATSPhysics.h
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


