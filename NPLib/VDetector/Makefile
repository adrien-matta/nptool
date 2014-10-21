include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libVDetector.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## VDetector ##
libVDetector.so: VDetector.o DetectorManager.o VSpectra.o VDetectorDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@	
	
VDetectorDict.cxx:	VDetector.h
			rootcint -f $@ -c $^ linkdef.h
		
# dependances
VDetector.o: VDetector.cxx	VDetector.h
VSpectra.o: VSpectra.cxx VSpectra.h
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


