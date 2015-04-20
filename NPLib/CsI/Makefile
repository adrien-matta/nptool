include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libCsI.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## CsI ##
libCsI.so: TCsIData.o	TCsIDataDict.o TCsIPhysics.o	TCsIPhysicsDict.o 
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TCsIDataDict.cxx:	TCsIData.h
			rootcint -f $@ -c $^

TCsIPhysicsDict.cxx:	TCsIPhysics.h
			rootcint -f $@ -c $^

# dependances
TCsIData.o:	TCsIData.cxx	TCsIData.h
TCsIPhysics.o:	TCsIPhysics.cxx	TCsIPhysics.h
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


