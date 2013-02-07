include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libTacTrigger.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------

############### Detector ##############
libTacTrigger.so: TTacData.o	TTacDataDict.o TTacPhysics.o	TTacPhysicsDict.o TTriggerData.o	TTriggerDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTacDataDict.cxx:	TTacData.h
			rootcint -f $@ -c $^

TTacPhysicsDict.cxx:	TTacPhysics.h
			rootcint -f $@ -c $^

TTriggerDataDict.cxx: TTriggerData.h
			rootcint -f $@ -c $^

# dependances
TTacData.o: TTacData.cxx TTacData.h
TTacPhysics.o: TTacPhysics.cxx	TTacPhysics.h
TTriggerData.o:	 TTriggerData.cxx TTriggerData.h
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


