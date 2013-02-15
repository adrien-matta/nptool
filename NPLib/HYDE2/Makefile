include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libHyde2Data.so	\
		 libHyde2TrackerModules.so 	libHyde2Physics.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Hyde2 ##
libHyde2Data.so:	THyde2TrackerData.o THyde2TrackerDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

THyde2TrackerDataDict.cxx:	THyde2TrackerData.h
			rootcint -f $@ -c $^

libHyde2Physics.so:	Hyde2Tracker.o THyde2TrackerPhysics.o \
				THyde2TrackerPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

THyde2TrackerPhysicsDict.cxx:	THyde2TrackerPhysics.h
			rootcint -f $@ -c $^

libHyde2TrackerModules.so:       Hyde2TrackerModule.o Hyde2TrackerSquare1.o \
	                         Hyde2TrackerTrapezoid1.o Hyde2TrackerTrapezoid2.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@



# dependances
Hyde2Tracker.o:	Hyde2Tracker.cxx	Hyde2Tracker.h
THyde2TrackerData.o:	THyde2TrackerData.cxx	THyde2TrackerData.h
THyde2TrackerPhysics.o:	THyde2TrackerPhysics.cxx	THyde2TrackerPhysics.h
Hyde2TrackerModule.o:		Hyde2TrackerModule.cxx	Hyde2TrackerModule.h
Hyde2TrackerSquare1.o:		Hyde2TrackerSquare1.cxx	Hyde2TrackerSquare1.h
Hyde2TrackerTrapezoid1.o:	Hyde2TrackerTrapezoid1.cxx	Hyde2TrackerTrapezoid1.h
Hyde2TrackerTrapezoid2.o:	Hyde2TrackerTrapezoid2.cxx	Hyde2TrackerTrapezoid2.h
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


