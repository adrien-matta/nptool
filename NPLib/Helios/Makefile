include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libHelios.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libHelios.so: THeliosData.o THeliosDataDict.o Helios.o THeliosPhysics.o THeliosPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

THeliosDataDict.cxx:	THeliosData.h
			rootcint -f $@ -c $^

THeliosPhysicsDict.cxx:	THeliosPhysics.h
			rootcint -f $@ -c $^

# dependances
Helios.o: Helios.cxx Helios.h
THeliosData.o: THeliosData.cxx THeliosData.h
THeliosDataDict.o: THeliosData.cxx THeliosData.h
THeliosPhysics.o: THeliosPhysics.cxx THeliosPhysics.h
THeliosPhysicsDict.o: THeliosPhysics.cxx THeliosPhysics.h

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


