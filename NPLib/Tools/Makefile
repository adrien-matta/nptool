include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libTools.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------

############# Various Tool ############

## TAGManager ##
libTools.so: NPTagManager.o NPTagManagerDict.o NPOptionManager.o TAsciiFile.o TAsciiFileDict.o CalibrationManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
NPTagManagerDict.cxx: NPTagManager.h
			rootcint -f $@ -c $^

TAsciiFileDict.cxx: TAsciiFile.h
			rootcint -f $@ -c $^
            
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


