include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libNana.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Nana ##
libNana.so: TNanaData.o TNanaDataDict.o TNanaPhysics.o TNanaPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TNanaDataDict.cxx:	TNanaData.h
			rootcint -f $@ -c $^

TNanaPhysicsDict.cxx:	TNanaPhysics.h
			rootcint -f $@ -c $^


# dependances
TNanaData.o: TNanaData.cxx TNanaData.h
TNanaPhysics.o: TNanaPhysics.cxx TNanaPhysics.h

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


