include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libSiRes.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SiRes ##
libSiRes.so: TSiResData.o	TSiResDataDict.o TSiResPhysics.o	TSiResPhysicsDict.o TSiResSpectra.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSiResDataDict.cxx:	TSiResData.h
			rootcint -f $@ -c $^

TSiResPhysicsDict.cxx:	TSiResPhysics.h
			rootcint -f $@ -c $^

# dependances
TSiResData.o:	TSiResData.cxx	TSiResData.h
TSiResPhysics.o:	TSiResPhysics.cxx	TSiResPhysics.h
TSiResSpectra.o:	TSiResSpectra.cxx	TSiResSpectra.h
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


