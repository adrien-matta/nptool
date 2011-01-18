include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      = libInitialConditions.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## InitialConditions ##
libInitialConditions.so:	TInitialConditions.o TInitialConditionsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TInitialConditionsDict.cxx:	TInitialConditions.h
			rootcint -f $@ -c $^

# dependances
TInitialConditions.o:	TInitialConditions.cxx	TInitialConditions.h
TInitialConditionsDict.o: TInitialConditionsDict.cxx TInitialConditionsDict.h
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


