include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      = libNPPhysics.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------

############# Various Tool ############

## Reaction ##
libNPPhysics.so: NPReaction.o NPNucleus.o NPBeam.o NPEnergyLoss.o NPFunction.o NPReactionDict.o
	$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

NPReactionDict.cxx: NPReaction.h
	rootcint -f $@ -c $^ linkdef.h		

# dependances
NPReaction.o:NPReaction.cxx NPReaction.h
NPReactionDict.o:NPReaction.cxx NPReaction.h linkdef.h
NPNucleus.o: NPNucleus.cxx NPNucleus.h
NPEnergyLoss.o:NPEnergyLoss.cxx NPEnergyLoss.h
NPBeam.o: NPBeam.cxx NPBeam.h
NPFunction.o: NPFunction.cxx NPFunction.h
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


