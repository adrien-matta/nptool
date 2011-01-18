include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libGaspardData.so	\
		 libGaspardTrackerModules.so 	libGaspardPhysics.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Gaspard ##
libGaspardData.so:	TGaspardTrackerData.o TGaspardTrackerDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TGaspardTrackerDataDict.cxx:	TGaspardTrackerData.h
			rootcint -f $@ -c $^

libGaspardPhysics.so:	GaspardTracker.o TGaspardTrackerPhysics.o \
				TGaspardTrackerPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TGaspardTrackerPhysicsDict.cxx:	TGaspardTrackerPhysics.h
			rootcint -f $@ -c $^

libGaspardTrackerModules.so:	GaspardTrackerModule.o	GaspardTrackerDummyShape.o \
				GaspardTrackerTrapezoid.o	GaspardTrackerAnnular.o \
				GaspardTrackerSquare.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@



# dependances
GaspardTracker.o:	GaspardTracker.cxx	GaspardTracker.h
TGaspardTrackerData.o:	TGaspardTrackerData.cxx	TGaspardTrackerData.h
TGaspardTrackerPhysics.o:	TGaspardTrackerPhysics.cxx	TGaspardTrackerPhysics.h
GaspardTrackerModule.o:		GaspardTrackerModule.cxx	GaspardTrackerModule.h
GaspardTrackerDummyShape.o:	GaspardTrackerDummyShape.cxx	GaspardTrackerDummyShape.h
GaspardTrackerTrapezoid.o:	GaspardTrackerTrapezoid.cxx	GaspardTrackerTrapezoid.h
GaspardTrackerAnnular.o:	GaspardTrackerAnnular.cxx	GaspardTrackerAnnular.h
GaspardTrackerSquare.o:		GaspardTrackerSquare.cxx	GaspardTrackerSquare.h
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


