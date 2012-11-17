include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libSharcData.so 
#libSharcPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Sharc ##
libSharcData.so:	TSharcData.o	TSharcDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSharcDataDict.cxx:	TSharcData.h
			rootcint -f $@ -c $^

#libSharcPhysics.so:	TSharcPhysics.o	TSharcPhysicsDict.o
#		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

#TSharcPhysicsDict.cxx:	TSharcPhysics.h
#			rootcint -f $@ -c $^
			
# dependances
TSharcData.o:		TSharcData.cxx	TSharcData.h
#TSharcPhysics.o:	TSharcPhysics.cxx	TSharcPhysics.h
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


