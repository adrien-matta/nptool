include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libRIPSData.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## RIPS ##
libRIPSData.so:	TRIPSData.o	TRIPSDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TRIPSDataDict.cxx:	TRIPSData.h
			rootcint -f $@ -c $^

#libRIPSPhysics.so:	TRIPSPhysics.o	TRIPSPhysicsDict.o
#			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

#TRIPSPhysicsDict.cxx:	TRIPSPhysics.h
#			rootcint -f $@ -c $^

# dependances
TRIPSData.o:	TRIPSData.cxx	TRIPSData.h
#TRIPSPhysics.o:	TRIPSPhysics.cxx	TRIPSPhysics.h
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


