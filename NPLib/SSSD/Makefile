include ../base_Makefile
 
#------------------------------------------------------------------------------
SHARELIB      =  libSSSDData.so libSSSDPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SSSD ##
libSSSDData.so:	TSSSDData.o	TSSSDDataDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSSSDDataDict.cxx:	TSSSDData.h
			rootcint -f $@ -c $^

libSSSDPhysics.so:	TSSSDPhysics.o	TSSSDPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSSSDPhysicsDict.cxx:	TSSSDPhysics.h
			rootcint -f $@ -c $^
			
# dependances
TSSSDData.o:		TSSSDData.cxx	TSSSDData.h
TSSSDPhysics.o:	TSSSDPhysics.cxx	TSSSDPhysics.h
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


