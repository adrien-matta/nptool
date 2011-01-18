include ../base_Makefile

#------------------------------------------------------------------------------
SHARELIB      =  libMust2Data.so	libMust2Physics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libMust2Data.so:	TMust2Data.o	TMust2DataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TMust2DataDict.cxx:	TMust2Data.h
			rootcint -f $@ -c $^

libMust2Physics.so:	TMust2Physics.o	TMust2PhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TMust2PhysicsDict.cxx: TMust2Physics.h
			rootcint -f $@ -c $^

# dependances
TMust2Data.o:		TMust2Data.cxx	TMust2Data.h
TMust2DataDict.o:		TMust2DataDict.cxx	TMust2DataDict.h
TMust2Physics.o:	TMust2Physics.cxx	TMust2Physics.h
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


