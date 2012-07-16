include $(NPTOOL)/NPLib/Makefile.arch
#include /usr/local/root-5.32.01/etc/Makefile.arch
#------------------------------------------------------------------------------
SHARELIB	= SharedLib
FILLINC		= FillIncludeDir
FILLLIB		= FillLibraryDir
LIBLIST		= liblistfile

all:	$(FILLINC) $(SHARELIB) $(FILLLIB) $(LIBLIST)
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
	$(NPTOOL)/NPLib/scripts/makefile.sh clean
	
distclean:
	rm -f ./lib/*
	rm -f ./include/* 
	rm -f liblist
	rm -f ./scripts/NPToolLogon_C*
	./scripts/makefile.sh distclean

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<


