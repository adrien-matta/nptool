include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libPlastic.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Plastic ##
libPlastic.so: TPlasticData.o	TPlasticDataDict.o TPlasticPhysics.o	TPlasticPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TPlasticDataDict.cxx:	TPlasticData.h
			rootcint -f $@ -c $^

TPlasticPhysicsDict.cxx:	TPlasticPhysics.h
			rootcint -f $@ -c $^

# dependances
TPlasticData.o:	TPlasticData.cxx	TPlasticData.h
TPlasticPhysics.o:	TPlasticPhysics.cxx	TPlasticPhysics.h
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


