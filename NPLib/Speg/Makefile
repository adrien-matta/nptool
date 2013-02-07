include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libSpeg.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SpegDC ##
libSpeg.so:   TSpegDCData.o   TSpegDCDataDict.o TSpegCHIOData.o   TSpegCHIODataDict.o TSpegPlasticData.o   TSpegPlasticDataDict.o TSpegPhysics.o	TSpegPhysicsDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSpegDCDataDict.cxx:   TSpegDCData.h
	rootcint -f $@ -c $^

TSpegCHIODataDict.cxx: TSpegCHIOData.h
	rootcint -f $@ -c $^

## SpegPlastic ##
TSpegPlasticDataDict.cxx: TSpegPlasticData.h
	rootcint -f $@ -c $^

TSpegPhysicsDict.cxx: TSpegPhysics.h
			rootcint -f $@ -c $^
# dependances
TSpegDCData.o:   	TSpegDCData.cxx  	TSpegDCData.h
TSpegCHIOData.o: 	TSpegCHIOData.cxx   	TSpegCHIOData.h
TSpegPlasticData.o: 	TSpegPlasticData.cxx   TSpegPlasticData.h
TSpegPhysics.o:	TSpegPhysics.cxx	TSpegPhysics.h
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


