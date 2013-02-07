include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libVDetector.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## VDetector ##
libVDetector.so: VDetector.o DetectorManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@	
			
# dependances
VDetector.o:		VDetector.cxx	VDetector.h
DetectorManager.o: DetectorManager.cxx DetectorManager.h	../DetectorList.inc
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


