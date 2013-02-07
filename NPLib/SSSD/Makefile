include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libSSSD.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## SSSD ##
libSSSD.so: TSSSDData.o	TSSSDDataDict.o TSSSDPhysics.o	TSSSDPhysicsDict.o
		$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TSSSDDataDict.cxx:	TSSSDData.h
			rootcint -f $@ -c $^

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


