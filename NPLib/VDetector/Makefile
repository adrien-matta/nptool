include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libVDetector.so libDetectorManager.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## VDetector ##
libVDetector.so:	VDetector.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@	

## VDetector ##
libDetectorManager.so:	DetectorManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@	
			
# dependances
VDetector.o:		VDetector.cxx	VDetector.h
DetectorManager.o: DetectorManager.cxx DetectorManager.h
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


