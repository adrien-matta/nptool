include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libNPCore.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------

############# Various Tool ############

## TAGManager ##
libNPCore.so: RootInput.o RootOutput.o NPOptionManager.o TAsciiFile.o TAsciiFileDict.o CalibrationManager.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
TAsciiFileDict.cxx: TAsciiFile.h
			rootcint -f $@ -c $^
            
# dependances
RootInput.o: RootInput.cxx RootInput.h
RootOutput.o: RootOutput.cxx RootOutput.h
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


