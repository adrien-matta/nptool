include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libChio_anData.so libChio_digData.so libChio_anPhysics.so libChio_digPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Chio_an & Chio_dig ##
libChio_anData.so:	TChio_anData.o	TChio_anDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChio_anDataDict.cxx:	TChio_anData.h
			rootcint -f $@ -c $^

libChio_anPhysics.so:      TChio_anPhysics.o TChio_anPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChio_anPhysicsDict.cxx:   TChio_anPhysics.h
			rootcint -f $@ -c $^

libChio_digData.so:	TChio_digData.o	TChio_digDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChio_digDataDict.cxx:	TChio_digData.h
			rootcint -f $@ -c $^

libChio_digPhysics.so:      TChio_digPhysics.o TChio_digPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChio_digPhysicsDict.cxx:   TChio_digPhysics.h
			rootcint -f $@ -c $^

# dependences
TChio_anData.o:	TChio_anData.cxx	TChio_anData.h
TChio_digData.o: TChio_digData.cxx	TChio_digData.h
TChio_anPhysics.o: TChio_anPhysics.cxx	TChio_anPhysics.h
TChio_digPhysics.o: TChio_digPhysics.cxx	TChio_digPhysics.h
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


