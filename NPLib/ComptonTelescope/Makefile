include ../Makefile.arch

#------------------------------------------------------------------------------
SHARELIB      =  libComptonTelescope.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## ComptonTelescope ##
libComptonTelescope.so:	TComptonTelescopeData.o	         TComptonTelescopeDataDict.o \
                        TComptonTelescopePhysics.o       TComptonTelescopePhysicsDict.o \
                        TComptonTelescopeProcessData.o   TComptonTelescopeProcessDataDict.o \
                        TComptonTelescopeSpectra.o 
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TComptonTelescopeDataDict.cxx:	TComptonTelescopeData.h
			rootcint -f $@ -c $^

TComptonTelescopePhysicsDict.cxx:	TComptonTelescopePhysics.h
			rootcint -f $@ -c $^

TComptonTelescopeProcessDataDict.cxx:	TComptonTelescopeProcessData.h
			rootcint -f $@ -c $^

# dependances
TComptonTelescopeData.o:		   TComptonTelescopeData.cxx	      TComptonTelescopeData.h
TComptonTelescopePhysics.o:      TComptonTelescopePhysics.cxx	   TComptonTelescopePhysics.h
TComptonTelescopeSpectra.o:      TComptonTelescopeSpectra.cxx	   TComptonTelescopeSpectra.h
TComptonTelescopeProcessData.o:  TComptonTelescopeProcessData.cxx TComptonTelescopeProcessData.h
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


