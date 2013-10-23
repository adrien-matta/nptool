include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTiara.so
#libTiaraPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Sharc ##
libTiara.so:	TTiaraBarrelData.o	TTiaraBarrelDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTiaraBarrelDataDict.cxx:	TTiaraBarrelData.h
			rootcint -f $@ -c $^
# dependances
TTiaraBarrelData.o:		TTiaraBarrelData.cxx	TTiaraBarrelData.h
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


