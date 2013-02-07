include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libGaspard.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Gaspard ##
libGaspard.so:	TGaspardTrackerData.o TGaspardTrackerDataDict.o GaspardTracker.o TGaspardTrackerPhysics.o TGaspardTrackerPhysicsDict.o GaspardTrackerModule.o GaspardTrackerDummyShape.o GaspardTrackerTrapezoid.o	GaspardTrackerAnnular.o GaspardTrackerSquare.o

			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TGaspardTrackerDataDict.cxx:	TGaspardTrackerData.h
			rootcint -f $@ -c $^


TGaspardTrackerPhysicsDict.cxx:	TGaspardTrackerPhysics.h
			rootcint -f $@ -c $^


# dependances
GaspardTracker.o: GaspardTracker.cxx	GaspardTracker.h
TGaspardTrackerData.o:	TGaspardTrackerData.cxx TGaspardTrackerData.h
TGaspardTrackerPhysics.o: TGaspardTrackerPhysics.cxx	TGaspardTrackerPhysics.h
GaspardTrackerModule.o: GaspardTrackerModule.cxx GaspardTrackerModule.h
GaspardTrackerDummyShape.o: GaspardTrackerDummyShape.cxx GaspardTrackerDummyShape.h
GaspardTrackerTrapezoid.o: GaspardTrackerTrapezoid.cxx GaspardTrackerTrapezoid.h
GaspardTrackerAnnular.o: GaspardTrackerAnnular.cxx GaspardTrackerAnnular.h
GaspardTrackerSquare.o: GaspardTrackerSquare.cxx GaspardTrackerSquare.h
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


