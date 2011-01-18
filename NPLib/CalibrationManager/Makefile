include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libCalibrationManager.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libCalibrationManager.so:	CalibrationManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
# dependances
CalibrationManager.o:		CalibrationManager.cxx	CalibrationManager.h
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


