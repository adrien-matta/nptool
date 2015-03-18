COPYFILES	= CopyIncSrcFiles
DELFILES	= DelIncSrcFiles
NPSIM		= NPSimulation

all:	$(DELFILES) $(COPYFILES) $(NPSIM)
DelIncSrcFiles: $(NPTOOL)/NPLib/liblist
	./scripts/deletefiles.sh

CopyIncSrcFiles: DelIncSrcFiles
	./scripts/copyfiles.sh

NPSimulation: DelIncSrcFiles CopyIncSrcFiles
	make -f Makefile.geant4

clean:
	./scripts/deletefiles.sh

distclean:
	./scripts/deletefiles.sh
	@echo " + Deleting bin and tmp directories"
	rm -rf bin
	rm -rf tmp
