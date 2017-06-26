 # This Makefile shows nicely how to compile and link applications
 CLASSNAME := GUser
 GUSERDIR := ./
 G2RCORE := $(GANIL2ROOT)/Core/
 G2RINC := $(GANIL2ROOT)/include/
 G2RLIB := $(GANIL2ROOT)/lib/
 LIBDIR := $(GRUDIR)/lib/
 VIGRUDIR := $(GRUDIR)/vigru/
 GTDIR := $(GRUDIR)/GTape/
 GGRUDIR := $(GRUDIR)/GGRU/
 GSOAPDIR := $(GRUDIR)/gsoap/
 MFM_DIR := $(GRUDIR)/MFMlib/
 GRUFLAGS := -I$(GRUDIR)/include/ -I$(LIBDIR) -I$(VIGRUDIR) -I$(GTDIR) -I$(GGRUDIR) -I$(GSOAPDIR) -I$(MFM_DIR) -I$(G2RINC) -I$(NPTOOL)/NPLib/include/
 INCLIST := General.h GAcq.h GDevice.h
 # Extensions des fichiers
 ObjSuf = o
 SrcSuf = cxx
 ExeSuf =
 DllSuf = so
 OutPutOpt = -o
 # Compilateur et options
 CXX = g++
 CXXFLAGS = -O -Wall -fPIC
 LD = g++
 LDFLAGS = -O
 SOFLAGS = -shared
 # Options de ROOT
 ROOTCFLAGS := $(shell root-config --cflags) -I$(ROOTSYS)/xmlparser/inc -I$(ROOTSYS)/io/xmlparser/inc
 ROOTLIBS := $(shell root-config --libs) -lHtml -lSpectrum
 ROOTGLIBS := $(shell root-config --glibs)
 CXXFLAGS += $(ROOTCFLAGS) $(GRUFLAGS)
 LIBS = $(ROOTLIBS) $(SYSLIBS)
 GLIBS = $(ROOTGLIBS) $(SYSLIBS)
 #------------------------------------------------------------------------------
 SHARELIB = $(CLASSNAME).so
 all: $(SHARELIB)
 #------------------------------------------------------------------------------

 $(CLASSNAME).so:$(CLASSNAME)Dict.o  $(CLASSNAME).o
		 $(LD) $(SOFLAGS) $^ $(OutPutOpt) $@
 $(CLASSNAME)Dict.cxx: 	$(CLASSNAME).h
			@echo "Generating dictionary $@..."
			rootcint -f $@ -c -p $(GRUFLAGS) $^ $(CLASSNAME)LinkDef.h

clean:
	@rm -f core *~ *.o *Dict*

distclean:
	make clean; rm -f *.so

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

# dependances
$(CLASSNAME).o: $(CLASSNAME).cxx $(CLASSNAME).h
