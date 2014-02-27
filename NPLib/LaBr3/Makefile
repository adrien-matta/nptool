include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libLaBr3.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## LaBr3 ##
libLaBr3.so: TLaBr3Data.o	TLaBr3DataDict.o TLaBr3Physics.o	TLaBr3PhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TLaBr3DataDict.cxx:	TLaBr3Data.h
			rootcint -f $@ -c $^

TLaBr3PhysicsDict.cxx:	TLaBr3Physics.h
			rootcint -f $@ -c $^

# dependances
TLaBr3Data.o:	TLaBr3Data.cxx	TLaBr3Data.h
TLaBr3Physics.o:	TLaBr3Physics.cxx	TLaBr3Physics.h
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


