# include Makefile options for different platforms
include $(NPTOOL)/NPLib/Makefile.arch

#include /usr/local/root-5.32.01/etc/Makefile.arch
#------------------------------------------------------------------------------
SHARELIB	= SharedLib
FILLINC		= FillIncludeDir
FILLLIB		= FillLibraryDir
LIBLIST		= liblistfile

all:	$(SHARELIB) $(FILLLIB) $(LIBLIST)
#------------------------------------------------------------------------------
############### fillinclib ##############
FillIncludeDir:
		./scripts/fillincdir.sh

FillLibraryDir:
		./scripts/filllibdir.sh
#ifeq ($(ARCH),macosx)
ifeq ($(findstring macosx, $(ARCH)), macosx)
		./scripts/filllibmacdir.sh
endif

############### liblist ##############
liblistfile:
		./scripts/buildliblist.sh

############### sharedlib ##############
SharedLib:
		./scripts/makefile.sh

############# Clean and More ##########
clean:
	$(NPTOOL)/NPLib/scripts/makefile.sh clean detector
	
cleancore:
	$(NPTOOL)/NPLib/scripts/makefile.sh clean core
	
distclean:
	@echo " + Deleting libraries in lib directory....."
	rm -f ./lib/*
	@echo " + Deleting header files in include directory....."
	rm -f ./include/* 
	@echo " + Deleting liblist file....."
	rm -f liblist
	rm -f ./scripts/NPToolLogon_C*
	./scripts/makefile.sh distclean detector
	./scripts/makefile.sh distclean core

# include makefile created by the configure script
# where the wanted detector are defined
include $(NPTOOL)/NPLib/Makefile.detector

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<


