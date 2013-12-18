include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libS2.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## S2 detector ##
libS2.so:	TS2Data.o	TS2DataDict.o  TS2Physics.o   TS2PhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TS2DataDict.cxx:	TS2Data.h
			rootcint -f $@ -c $^

TS2PhysicsDict.cxx:	TS2Physics.h
			rootcint -f $@ -c $^

# dependances
TS2Data.o:	   TS2Data.cxx	   TS2Data.h
TS2Physics.o:	TS2Physics.cxx	TS2Physics.h
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


