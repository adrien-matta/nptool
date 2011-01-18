include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libW1Data.so libW1Physics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## W1 ##
libW1Data.so:	TW1Data.o	TW1DataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TW1DataDict.cxx:	TW1Data.h
			rootcint -f $@ -c $^

libW1Physics.so:	TW1Physics.o	TW1PhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TW1PhysicsDict.cxx:	TW1Physics.h
			rootcint -f $@ -c $^
			
# dependances
TW1Data.o:		TW1Data.cxx	TW1Data.h
TW1Physics.o:	TW1Physics.cxx	TW1Physics.h
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


