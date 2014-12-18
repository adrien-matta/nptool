include ../Makefile.arch
 
#------------------------------------------------------------------------------
SHARELIB      =  libFatima.so
all:            $(SHARELIB)
#------------------------------------------------------------------------------
############### Detector ##############

## Fatima ##
libFatima.so: TFatimaData.o TFatimaDataDict.o TFatimaPhysics.o TFatimaPhysicsDict.o
			$(LD) $(SOFLAGS) $^ $(OutPutOpt) $@

TFatimaDataDict.cxx:	TFatimaData.h
			rootcint -f $@ -c $^

TFatimaPhysicsDict.cxx:	TFatimaPhysics.h
			rootcint -f $@ -c $^


# dependances
TFatimaData.o: TFatimaData.cxx TFatimaData.h
TFatimaPhysics.o: TFatimaPhysics.cxx TFatimaPhysics.h

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


