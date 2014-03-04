include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libSiLi.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SiLi ##
libSiLi.so: TSiLiData.o	TSiLiDataDict.o TSiLiPhysics.o	TSiLiPhysicsDict.o TSiLiSpectra.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSiLiDataDict.cxx:	TSiLiData.h
			rootcint -f $@ -c $^

TSiLiPhysicsDict.cxx:	TSiLiPhysics.h
			rootcint -f $@ -c $^

# dependances
TSiLiData.o:	TSiLiData.cxx	TSiLiData.h
TSiLiPhysics.o:	TSiLiPhysics.cxx	TSiLiPhysics.h
TSiLiSpectra.o:	TSiLiSpectra.cxx	TSiLiSpectra.h
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


