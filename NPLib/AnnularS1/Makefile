include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libAnnularS1Data.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## AnnularS1 ##
libAnnularS1Data.so:	TS1Data.o	TS1DataDict.o TAnnularS1Physics.o	TAnnularS1PhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TS1DataDict.cxx:	TS1Data.h
			rootcint -f $@ -c $^

TAnnularS1PhysicsDict.cxx:	TAnnularS1Physics.h
			rootcint -f $@ -c $^

# dependances
TS1Data.o:	TS1Data.cxx	TS1Data.h
TAnnularS1Physics.o:	TAnnularS1Physics.cxx	TAnnularS1Physics.h

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


