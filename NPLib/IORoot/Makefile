include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libIORoot.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libIORoot.so:	RootInput.o RootOutput.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

# dependances
RootInput.o:	RootInput.cxx	RootInput.h
RootOutput.o:	RootOutput.cxx	RootOutput.h
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


