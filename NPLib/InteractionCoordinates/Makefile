include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libInteractionCoordinates.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## InteractionCoordinates ##
libInteractionCoordinates.so:	TInteractionCoordinates.o TInteractionCoordinatesDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TInteractionCoordinatesDict.cxx:	TInteractionCoordinates.h
			rootcint -f $@ -c $^


# dependances
TInteractionCoordinates.o:	TInteractionCoordinates.cxx	TInteractionCoordinates.h
TInteractionCoordinatesDict.o: TInteractionCoordinatesDict.cxx TInteractionCoordinatesDict.h
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


