# include same architecture file than for NPLib
# so that consistency is ensured
include $(NPLIB)/Makefile.arch

# additional libraries
LIBRARY  = `$(NPLIB)/liblist`
LIBRARY += -L$(CLHEP_LIB_DIR) -l$(CLHEP_LIB)

PROGRAMS	= NPAnalysis

all:	$(PROGRAMS)

NPAnalysis:	Analysis.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(LIBRARY) $(OutPutOpt) $@
	@echo "$@ done"
	

# rule for creating .o from .cxx
.SUFFIXES: .$(SrcSuf)
.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

# some cleaning
clean:
	rm -rf *.o

distclean:
	make clean; rm $(PROGRAMS)

# dependences
Analysis.o:	Analysis.cxx	Analysis.h
