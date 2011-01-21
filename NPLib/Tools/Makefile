include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libReaction.so libEnergyLoss.so libTagManager.so libOptionManager.so \
                libAsciiFile.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------

############# Various Tool ############

## Reaction ##
libReaction.so:	NPReaction.o NPNucleus.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

## EnergyLoss ##
libEnergyLoss.so: NPEnergyLoss.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
		
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
			
# dependances
NPReaction.o:NPReaction.cxx NPReaction.h
NPNucleus.o: NPNucleus.cxx NPNucleus.h
NPEnergyLoss.o:NPEnergyLoss.cxx NPEnergyLoss.h
NPTagManager.o:NPTagManager.cxx NPTagManager.h
NPOptionManager.o:NPOptionManager.cxx NPOptionManager.h
TAsciiFile.o:  TAsciiFile.cxx TAsciiFile.h
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


