include ../base_Makefile

#------------------------------------------------------------------------------
SHARELIB      =  libShieldData.so 	libShieldPhysics.so

all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Shield ##
libShieldData.so:	TShieldData.o	TShieldDataDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TShieldDataDict.cxx:	TShieldData.h
			rootcint -f $@ -c $^

libShieldPhysics.so:	Shield.o TShieldPhysics.o TShieldPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TShieldPhysicsDict.cxx:	TShieldPhysics.h
			rootcint -f $@ -c $^



# dependances
Shield.o:	Shield.cxx	Shield.h
TShieldData.o:	TShieldData.cxx	TShieldData.h
TShieldPhysics.o:	TShieldPhysics.cxx	TShieldPhysics.h

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


