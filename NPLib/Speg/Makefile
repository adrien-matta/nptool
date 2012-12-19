include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libSpegDCData.so libSpegCHIOData.so \
                 libSpegPlasticData.so \
                 libSpegPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SpegDC ##
libSpegDCData.so:   TSpegDCData.o   TSpegDCDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSpegDCDataDict.cxx:   TSpegDCData.h
	rootcint -f $@ -c $^

## SpegCHIO ##
libSpegCHIOData.so: TSpegCHIOData.o   TSpegCHIODataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSpegCHIODataDict.cxx: TSpegCHIOData.h
	rootcint -f $@ -c $^

## SpegPlastic ##
libSpegPlasticData.so: TSpegPlasticData.o   TSpegPlasticDataDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSpegPlasticDataDict.cxx: TSpegPlasticData.h
	rootcint -f $@ -c $^

libSpegPhysics.so:	TSpegPhysics.o	TSpegPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

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


