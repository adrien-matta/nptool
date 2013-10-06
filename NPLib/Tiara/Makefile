include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libTiara.so
#libTiaraPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Sharc ##
libTiara.so:	TTiaraData.o	TTiaraDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TTiaraDataDict.cxx:	TTiaraData.h
			rootcint -f $@ -c $^
# dependances
TTiaraData.o:		TTiaraData.cxx	TTiaraData.h
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


