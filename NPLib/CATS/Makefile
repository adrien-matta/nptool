include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libCATS.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## CATS ## 
libCATS.so: TCATSData.o TCATSDataDict.o TCATSPhysics.o TCATSPhysicsDict.o TCATSSpectra.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TCATSDataDict.cxx:	TCATSData.h
			rootcint -f $@ -c $^

TCATSPhysicsDict.cxx: TCATSPhysics.h
			rootcint -f $@ -c $^

# dependances
TCATSData.o: TCATSData.cxx TCATSData.h
TCATSDataDict.o: TCATSData.cxx TCATSData.h
TCATSPhysicsDict.o: TCATSPhysics.cxx TCATSPhysics.h
TCATSPhysics.o:	TCATSPhysics.cxx TCATSPhysics.h
TCATSSpectra.o:	TCATSSpectra.cxx TCATSSpectra.h
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


