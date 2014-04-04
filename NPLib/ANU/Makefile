include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libANU.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## ANU ##
libANU.so: TANUData.o TANUDataDict.o ANU.o TANUPhysics.o TANUPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TANUDataDict.cxx:	TANUData.h
			rootcint -f $@ -c $^

TANUPhysicsDict.cxx:	TANUPhysics.h
			rootcint -f $@ -c $^

# dependances
ANU.o: ANU.cxx ANU.h
TANUData.o: TANUData.cxx TANUData.h
TANUDataDict.o: TANUData.cxx TANUData.h
TANUPhysics.o: TANUPhysics.cxx TANUPhysics.h
TANUPhysicsDict.o: TANUPhysics.cxx TANUPhysics.h

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


