# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile based on examples module by Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := Simulation
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

# G4 work directory is the path where
# Result of compilation is put in (./$G4WORKDIR/bin/$G4SYSTEM/)
# to execute your programm : ./$G4WORKDIR/bin/$G4SYSTEM/Simulation xx.reaction xx.detector
G4WORKDIR = ./

CPPFLAGS += $(shell root-config --cflags) 
CPPFLAGS += -I$(NPLIB)/include
EXTRALIBS = $(shell root-config --glibs) -lMathMore
EXTRALIBS += `$(NPLIB)/liblist`
.PHONY: all
all: lib bin

cleanVRML:
	rm -r *.wrl

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

