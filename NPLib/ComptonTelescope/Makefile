include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libComptonTelescopeData.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## MUST2 ##
libComptonTelescopeData.so:	TComptonTelescopeData.o	TComptonTelescopeDataDict.o \
                              TComptonTelescopeProcessData.o   TComptonTelescopeProcessDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TComptonTelescopeDataDict.cxx:	TComptonTelescopeData.h
			rootcint -f $@ -c $^

TComptonTelescopeProcessDataDict.cxx:	TComptonTelescopeProcessData.h
			rootcint -f $@ -c $^

# dependances
TComptonTelescopeData.o:		TComptonTelescopeData.cxx	TComptonTelescopeData.h
TComptonTelescopeProcessData.o:		TComptonTelescopeProcessData.cxx	TComptonTelescopeProcessData.h
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


