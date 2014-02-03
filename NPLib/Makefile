# include Makefile options for different platforms
include $(NPTOOL)/NPLib/Makefile.arch

#include /usr/local/root-5.32.01/etc/Makefile.arch
#------------------------------------------------------------------------------
SHARELIB	= $(shell cat .core_libs)
SHARELIB += $(shell cat .detector_libs)
FILLINC		= FillIncludeDir
FILLLIB		= FillLibraryDir
LIBLIST		= liblistfile

#Silence the process
all: .SILENT
.SILENT: $(SHARELIB) $(LIBLIST)

#------------------------------------------------------------------------------
############### fillinclib ##############
FillIncludeDir:
		./scripts/fillincdir.sh

FillLibraryDir: $(SHARELIB) $(FILLINC)
ifeq ($(findstring macosx, $(ARCH)), macosx)
		./scripts/filllibmacdir.sh
endif

############### liblist ##############
liblistfile:  $(FILLLIB) $(SHARELIB) $(FILLIB)
		./scripts/buildliblist.sh

########### actual target ##############
# include makefile created by the configure script
# where the wanted detector are defined
include $(NPTOOL)/NPLib/Makefile.detector

############# Clean and More ##########
clean:
	rm -f ./*/*.o

distclean:
	@echo " + Deleting libraries in lib directory"
	rm -f ./lib/*
	@echo " + Deleting header files in include directory"
	rm -f ./include/* 
	@echo " + Deleting liblist file"
	rm -f liblist
	@echo " + Emptying Makefile.detector"
	rm -f Makefile.detector
	touch Makefile.detector
	@echo " + Deleting NPToolLogon"
	rm -f ./scripts/NPToolLogon_C*
	@echo " + Deleting intermediate compilation files"
	rm -f ./*/*Dict*
	rm -f ./*/*.o
	@echo " + Deleting shared libraries files in detector directory"
	rm -f ./*/*.so
