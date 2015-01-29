include ../Makefile.arch

#------------------------------------------------------------------------------
#SHARELIB      =  libHiraData.so	libHiraPhysics.so   libHiraSpectra.so
SHARELIB      =  libHira.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## HiRA ##
#libHira.so: THiraData.o THiraDataDict.o THiraPhysics.o  THiraPhysicsDict.o THiraSpectra.o 
#		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

libHira.so: THiraData.o THiraDataDict.o  
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

THiraDataDict.cxx:	THiraData.h
			rootcint -f $@ -c $^

#THiraPhysicsDict.cxx: THiraPhysics.h
#			rootcint -f $@ -c $^

# dependances
THiraData.o:		THiraData.cxx	THiraData.h
#THiraPhysics.o:	THiraPhysics.cxx	THiraPhysics.h
#THiraSpectra.o:	THiraSpectra.cxx	THiraSpectra.h
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


