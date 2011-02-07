include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libTagManager.so libOptionManager.so libAsciiFile.so libCalibrationManager.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------

############# Various Tool ############

## TAGManager ##
libTagManager.so: NPTagManager.o NPTagManagerDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
NPTagManagerDict.cxx: NPTagManager.h
			rootcint -f $@ -c $^
			
## OptionManager ##
libOptionManager.so: NPOptionManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
## AsciiFile ##
libAsciiFile.so: TAsciiFile.o TAsciiFileDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TAsciiFileDict.cxx: TAsciiFile.h
			rootcint -f $@ -c $^

## Calibration Manager ##
libCalibrationManager.so:  CalibrationManager.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
            
# dependances
NPTagManager.o:NPTagManager.cxx NPTagManager.h
NPOptionManager.o:NPOptionManager.cxx NPOptionManager.h
TAsciiFile.o:  TAsciiFile.cxx TAsciiFile.h
CalibrationManager.o:   CalibrationManager.cxx  CalibrationManager.h
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


