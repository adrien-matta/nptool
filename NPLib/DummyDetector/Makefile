include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libDUMMYDetectorData.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## DUMMYDetector ##
libDUMMYDetectorData.so:	TDUMMYDetectorData.o	TDUMMYDetectorDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TDUMMYDetectorDataDict.cxx:	TDUMMYDetectorData.h
			rootcint -f $@ -c $^

# dependances
TDUMMYDetectorData.o:	TDUMMYDetectorData.cxx	TDUMMYDetectorData.h
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


