include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libHyde.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Hyde ##
libHyde.so: THydeTrackerData.o THydeTrackerDataDict.o HydeTracker.o THydeTrackerPhysics.o THydeTrackerPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

THydeTrackerDataDict.cxx:	THydeTrackerData.h
			rootcint -f $@ -c $^

THydeTrackerPhysicsDict.cxx:	THydeTrackerPhysics.h
			rootcint -f $@ -c $^

# dependances
HydeTracker.o:	HydeTracker.cxx	HydeTracker.h
THydeTrackerData.o:	THydeTrackerData.cxx	THydeTrackerData.h
THydeTrackerPhysics.o:	THydeTrackerPhysics.cxx	THydeTrackerPhysics.h
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


