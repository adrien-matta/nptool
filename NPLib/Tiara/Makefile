include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTiara.so
#libTiaraPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Sharc ##
libTiara.so:	TTiaraBarrelData.o	TTiaraBarrelDataDict.o TTiaraHyballData.o	TTiaraHyballDataDict.o TTiaraHyballPhysics.o TTiaraHyballPhysicsDict.o TTiaraHyballSpectra.o

		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTiaraBarrelDataDict.cxx:	TTiaraBarrelData.h
			rootcint -f $@ -c $^

TTiaraHyballDataDict.cxx:	TTiaraHyballData.h
			rootcint -f $@ -c $^

TTiaraHyballPhysicsDict.cxx:	TTiaraHyballPhysics.h
			rootcint -f $@ -c $^

# dependances
TTiaraHyballSpectra.o: TTiaraHyballSpectra.cxx	TTiaraHyballSpectra.h
TTiaraBarrelData.o: TTiaraBarrelData.cxx	TTiaraBarrelData.h
TTiaraHyballData.o: TTiaraHyballData.cxx	TTiaraHyballData.h
TTiaraHyballPhysics.o: TTiaraHyballPhysics.cxx	TTiaraHyballPhysics.h

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
