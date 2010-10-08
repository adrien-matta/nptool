# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

ROOTCONFIG   := root-config

ARCH         := $(shell $(ROOTCONFIG) --arch)
PLATFORM     := $(shell $(ROOTCONFIG) --platform)
ALTCC        := $(shell $(ROOTCONFIG) --cc)
ALTCXX       := $(shell $(ROOTCONFIG) --cxx)
ALTF77       := $(shell $(ROOTCONFIG) --f77)
ALTLD        := $(shell $(ROOTCONFIG) --ld)

#CXX           =
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ifeq (debug,$(findstring debug,$(ROOTBUILD)))
OPT           = -g
OPT2          = -g
else
ifneq ($(findstring debug, $(strip $(shell $(ROOTCONFIG) --config))),)
OPT           = -g
OPT2          = -g
else
OPT           = -O
OPT2          = -O2
endif
endif

ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
HASTHREAD    := $(shell $(ROOTCONFIG) --has-thread)
ROOTDICTTYPE := $(shell $(ROOTCONFIG) --dicttype)
NOSTUBS      := $(shell $(ROOTCONFIG) --nostubs)
ROOTCINT     := rootcint

ifeq ($(ARCH),linux)
# Linux with egcs, gcc 2.9x, gcc 3.x
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxkcc)
# Linux with the KAI compiler
CXX           = KCC --one_instantiation_per_object
CXXFLAGS      = $(OPT) -fPIC +K0
LD            = KCC
LDFLAGS       = $(OPT) $(shell $(ROOTCONFIG) --cflags)
SOFLAGS       =
endif

ifeq ($(ARCH),linuxicc)
# Linux with Intel icc compiler
ICC_MAJOR    := $(shell icc -v 2>&1 | awk '{ if (NR==1) print $$2 }' | \
                cut -d'.' -f1)
ICC_MINOR    := $(shell icc -v 2>&1 | awk '{ if (NR==1) print $$2 }' | \
                cut -d'.' -f2)
CXX           = icc
CXXFLAGS      = $(OPT) -fPIC -wd1476
LD            = icpc
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxppcgcc)
# PPC Linux with gcc and glibc
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxia64gcc)
# Itanium Linux with gcc 2.9x
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxia64sgi)
# Itanium Linux with sgiCC
CXX           = sgiCC
CXXFLAGS      = $(OPT) -Wall -fPIC
LD            = gsgiCC
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxia64ecc)
# Itanium Linux with Intel icc (was ecc)
ICC_MAJOR    := $(shell icc -v 2>&1 | awk '{ if (NR==1) print $$2 }' | \
                cut -d'.' -f1)
ICC_MINOR    := $(shell icc -v 2>&1 | awk '{ if (NR==1) print $$2 }' | \
                cut -d'.' -f2)
CXX           = icc
CXXFLAGS      = $(OPT) -fPIC -wd1476 -ftz
LD            = icpc
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxx8664gcc)
# AMD Opteron and Intel EM64T (64 bit mode) Linux with gcc 3.x
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxppc64gcc)
# PPC64 Linux with gcc 3.x
CXX           = g++
CXXFLAGS      = $(OPT) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxx8664icc)
# AMD Opteron and Intel EM64T (64 bit mode) Linux with Intel icc compiler
CXX           = icc
CXXFLAGS      = $(OPT) -fPIC -wd1476 -wd1572
LD            = icpc
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxalphagcc)
# Alpha Linux with gcc
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxmips)
# GNU/Linux on mips (BE/LE, O32/N32/N64) with gcc
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxhppa)
# GNU/Linux on hppa with gcc
CXX           = g++
CXXFLAGS      = $(OPT2) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT2)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxarm)
# ARM Linux with egcs
CXX           = g++
CXXFLAGS      = $(OPT) -Wall -fPIC
LD            = g++
LDFLAGS       = $(OPT)
SOFLAGS       = -shared
endif

ifeq ($(ARCH),macosx)
# MacOS X with cc (GNU cc 2.95.2 and gcc 3.3)
MACOSX_MINOR := $(shell sw_vers | sed -n 's/ProductVersion://p' | cut -d . -f 2)
MACOSXTARGET := MACOSX_DEPLOYMENT_TARGET=10.$(MACOSX_MINOR)
CXX           = g++
CXXFLAGS      = $(OPT2) -pipe -Wall -W -Woverloaded-virtual
LD            = $(MACOSXTARGET) g++
LDFLAGS       = $(OPT2) -bind_at_load
# The SOFLAGS will be used to create the .dylib,
# the .so will be created separately
ifeq ($(subst $(MACOSX_MINOR),,1234),1234)
DllSuf        = so
else
DllSuf        = dylib
endif
UNDEFOPT      = dynamic_lookup
ifneq ($(subst $(MACOSX_MINOR),,12),12)
UNDEFOPT      = suppress
LD            = g++
endif
SOFLAGS       = -dynamiclib -single_module -undefined $(UNDEFOPT)
endif

ifeq ($(ARCH),macosxicc)
# MacOS X with Intel icc compiler
MACOSX_MINOR := $(shell sw_vers | sed -n 's/ProductVersion://p' | cut -d . -f 2)
MACOSXTARGET := MACOSX_DEPLOYMENT_TARGET=10.$(MACOSX_MINOR)
ifeq ($(MACOSX_MINOR),5)
MACOSX_MINOR := 4
endif
CXX           = icc
CXXFLAGS      = $(OPT) -fPIC -wd1476
LD            = $(MACOSXTARGET) icpc
LDFLAGS       = $(OPT)
# The SOFLAGS will be used to create the .dylib,
# the .so will be created separately
ifeq ($(subst $(MACOSX_MINOR),,1234),1234)
DllSuf        = so
else
DllSuf        = dylib
endif
SOFLAGS       = -dynamiclib -single_module -undefined dynamic_lookup
endif

ifeq ($(ARCH),macosx64)
# MacOS X >= 10.4 with gcc 64 bit mode (GNU gcc 4.*)
# Only specific option (-m64) comes from root-config
MACOSX_MINOR := $(shell sw_vers | sed -n 's/ProductVersion://p' | cut -d . -f 2)
MACOSXTARGET := MACOSX_DEPLOYMENT_TARGET=10.$(MACOSX_MINOR)
CXX           = g++
CXXFLAGS      = $(OPT2) -pipe -Wall -W -Woverloaded-virtual
LD            = $(MACOSXTARGET) g++ -m64
LDFLAGS       = $(OPT2) -bind_at_load
# The SOFLAGS will be used to create the .dylib,
# the .so will be created separately
ifeq ($(subst $(MACOSX_MINOR),,1234),1234)
DllSuf        = so
else
DllSuf        = dylib
endif
SOFLAGS       = -m64 -dynamiclib -single_module -undefined dynamic_lookup
endif

ifeq ($(ARCH),macosxxlc)
# MacOS X with IBM xlC compiler
MACOSX_MINOR := $(shell sw_vers | sed -n 's/ProductVersion://p' | cut -d . -f 2)
MACOSXTARGET := MACOSX_DEPLOYMENT_TARGET=10.$(MACOSX_MINOR)
CXX           = xlC
CXXFLAGS      = $(OPT)
LD            = $(MACOSXTARGET) xlC
LDFLAGS       = $(OPT) -Wl,-bind_at_load
# The SOFLAGS will be used to create the .dylib,
# the .so will be created separately
DllSuf        = dylib
UNDEFOPT      = dynamic_lookup
ifneq ($(subst $(MACOSX_MINOR),,12),12)
UNDEFOPT      = suppress
LD            = xlC
endif
SOFLAGS       = -qmkshrobj -single_module -undefined $(UNDEFOPT)
endif

CXXFLAGS     += $(ROOTCFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

INCLUDE		= -I$(CLHEP_BASE_DIR)/include
 
#------------------------------------------------------------------------------
SHARELIB      =	CalibManager Vdetec InputOutputRoot InitCond	InterCoord \
		Must2All	GaspardData	AnnularS1Data PlasticData DummyDetectorData SSSDData\
		Reaction	EnergyLoss	ParisData	ShieldData

all:         $(SHARELIB)
	rm -f ./include/*Dict.h
#------------------------------------------------------------------------------
############### Calibration ##############

## CalibrationManager ##
CalibManager:
		make -C ./CalibrationManager
		cp ./CalibrationManager/*.so ./lib ; cp ./CalibrationManager/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libCalibrationManager.so libCalibrationManager.dylib
endif

############### Detector ##############

## VDetector ##
Vdetec:
		cp ./VDetector/*.h ./include
		make -C ./VDetector
		cp ./VDetector/*.so ./lib
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libVDetector.so libVDetector.dylib
		cd lib; ln -sf libDetectorManager.so libDetectorManager.dylib
endif
		
## MUST2 ##
Must2All:
		make -C ./MUST2
		cp ./MUST2/*.so ./lib ; cp ./MUST2/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libMust2Data.so libMust2Data.dylib
		cd lib; ln -sf libMust2Physics.so libMust2Physics.dylib
endif

## SSSD ##
SSSDData:
		make -C ./SSSD
		cp ./SSSD/*.so ./lib ; cp ./SSSD/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libSSSDData.so libSSSDData.dylib
		cd lib; ln -sf libSSSDPhysics.so libSSSDPhysics.dylib
endif

## AnnularS1 ##
AnnularS1Data:
		make -C ./AnnularS1
		cp ./AnnularS1/*.so ./lib ; cp ./AnnularS1/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libAnnularS1Data.so libAnnularS1Data.dylib
endif

## Gaspard ##
GaspardData:
		make -C ./GASPARD
		cp ./GASPARD/*.so ./lib ; cp ./GASPARD/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libGaspardData.so libGaspardData.dylib
		cd lib; ln -sf libGaspardPhysics.so libGaspardPhysics.dylib
		cd lib; ln -sf libGaspardTrackerModules.so libGaspardTrackerModules.dylib
		cd lib; ln -sf libGaspardPhysicsNew.so libGaspardPhysicsNew.dylib
endif

## Plastic ##
PlasticData:
		make -C ./Plastic
		cp ./Plastic/*.so ./lib ; cp ./Plastic/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libPlasticData.so libPlasticData.dylib
		cd lib; ln -sf libPlasticPhysics.so libPlasticPhysics.dylib
endif

## DUMMY Detector ##
DummyDetectorData:
		make -C ./DummyDetector
		cp ./DummyDetector/*.so ./lib ; cp ./DummyDetector/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libDummyDetectorData.so libDummyDetectorData.dylib
endif

## Paris Detector ##
ParisData:
		make -C ./Paris
		cp ./Paris/*.so ./lib ; cp ./Paris/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libParisData.so libParisData.dylib
		cd lib; ln -sf libParisPhysics.so libParisPhysics.dylib
endif


## Paris Shield Detector ##
ShieldData:
		make -C ./Shield
		cp ./Shield/*.so ./lib ; cp ./Shield/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libShieldData.so libShieldData.dylib
		cd lib; ln -sf libShieldPhysics.so libShieldPhysics.dylib
endif

############# Simulation ##############

## InitialConditions ##
InitCond:
		make -C ./InitialConditions
		cp ./InitialConditions/*.so ./lib ; cp ./InitialConditions/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libInitialConditions.so libInititalConditions.dylib
endif

## InteractionCoordinates ##
InterCoord:	
		make -C ./InteractionCoordinates
		cp ./InteractionCoordinates/*.so ./lib ; cp ./InteractionCoordinates/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libInteractionCoordinates.so libInteractionCoordinates.dylib
endif

############# I/O Root File ############
InputOutputRoot:
	make -C ./IORoot
	cp ./IORoot/*.so ./lib ; cp ./IORoot/*.h ./include
ifeq ($(ARCH),macosx)
	cd lib; ln -sf libIORoot.so libIORoot.dylib
endif



############# Various Tools ############

## Reaction ##
Reaction:
		make libReaction.so -C ./Tools
		cp ./Tools/*.so ./lib ; cp ./Tools/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libReaction.so libReaction.dylib
endif

EnergyLoss:
		make libEnergyLoss.so -C ./Tools
		cp ./Tools/*.so ./lib ; cp ./Tools/*.h ./include
ifeq ($(ARCH),macosx)
		cd lib; ln -sf libEnergyLoss.so libEnergyLoss.dylib
endif
		
#######################################

############# Clean and More ##########
clean:
	make clean -C ./Tools
	make clean -C ./IORoot
	make clean -C ./VDetector
	make clean -C ./CalibrationManager
	make clean -C ./MUST2
	make clean -C ./SSSD
	make clean -C ./AnnularS1
	make clean -C ./GASPARD
	make clean -C ./InteractionCoordinates
	make clean -C ./InitialConditions
	make clean -C ./DummyDetector
	make clean -C ./Plastic
	make clean -C ./Paris
	make clean -C ./Shield
	
distclean:
	rm -f ./lib/*.so
ifeq ($(ARCH),macosx)
	rm -f ./lib/*.dylib
endif
	rm -f ./include/*.h 
	make distclean -C ./Tools
	make distclean -C ./IORoot
	make distclean -C ./VDetector
	make distclean -C ./CalibrationManager
	make distclean -C ./MUST2
	make distclean -C ./SSSD
	make distclean -C ./AnnularS1
	make distclean -C ./GASPARD
	make distclean -C ./InteractionCoordinates
	make distclean -C ./InitialConditions
	make distclean -C ./DummyDetector
	make distclean -C ./Plastic
	make distclean -C ./Paris
	make distclean -C ./Shield
.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<


