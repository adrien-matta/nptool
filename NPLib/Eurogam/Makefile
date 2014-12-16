include ../Makefile.arch
 

#------------------------------------------------------------------------------
SHARELIB      =  libEurogamData.so 

all:            $(SHARELIB)
#------------------------------------------------------------------------------

############### Detector ##############

## Eurogam ##
libEurogamData.so:	TEurogamData.o	TEurogamDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TEurogamDataDict.cxx:	TEurogamData.h
			rootcint -f $@ -c $^

# dependances
TEurogamData.o:	TEurogamData.cxx	TEurogamData.h
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


