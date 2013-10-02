# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := Simulation
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

#G4 work directory is the path where
#Result of compilation is put
# to execute your programm : ./$G4WORKDIR/bin/$G4SYSTEM/Simulation xx.reaction xx.detector
G4WORKDIR = ./

CPPFLAGS += $(shell root-config --cflags) 
CPPFLAGS += -I$(NPLIB)/include
EXTRALIBS = $(shell root-config --glibs) 
EXTRALIBS += -L$(NPLIB)/lib -lMust2Data -lAnnularS1Data -lGaspardData \
		-lInitialConditions -lInteractionCoordinates \
		-lReaction -lIORoot
.PHONY: all
all: lib bin

cleanVRML:
	rm -r *.wrl

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

