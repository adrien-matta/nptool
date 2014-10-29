include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTigress.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Tigress ##
libTigress.so:	TTigressData.o	TTigressDataDict.o TTigressPhysics.o TTigressPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTigressDataDict.cxx:	TTigressData.h
			rootcint -f $@ -c $^

TTigressPhysicsDict.cxx:	TTigressPhysics.h
			rootcint -f $@ -c $^
			
# dependances
TTigressData.o: TTigressData.cxx TTigressData.h 
TTigressPhysics.o: TTigressPhysics.cxx TTigressPhysics.h
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


