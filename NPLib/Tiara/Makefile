include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTiara.so
#libTiaraPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Sharc ##
libTiara.so:	TTiaraBarrelData.o	TTiaraBarrelDataDict.o TTiaraHyballData.o	TTiaraHyballDataDict.o

		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTiaraBarrelDataDict.cxx:	TTiaraBarrelData.h
			rootcint -f $@ -c $^

TTiaraHyballDataDict.cxx:	TTiaraHyballData.h
			rootcint -f $@ -c $^

# dependances
TTiaraBarrelData.o:		TTiaraBarrelData.cxx	TTiaraBarrelData.h
TTiaraHyballData.o:		TTiaraHyballData.cxx	TTiaraHyballData.h

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


