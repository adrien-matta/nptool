include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libChio.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Chio_an & Chio_dig ##
libChio.so: TChio_anData.o TChio_anDataDict.o TChio_anPhysics.o TChio_anPhysicsDict.o TChio_digData.o	TChio_digDataDict.o TChio_digPhysics.o TChio_digPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TChio_anDataDict.cxx:	TChio_anData.h
			rootcint -f $@ -c $^

TChio_anPhysicsDict.cxx:   TChio_anPhysics.h
			rootcint -f $@ -c $^

TChio_digDataDict.cxx:	TChio_digData.h
			rootcint -f $@ -c $^

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


