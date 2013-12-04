COPYFILES	= CopyIncSrcFiles
DELFILES	= DelIncSrcFiles
NPSIM		= NPSimulation

all:	$(DELFILES) $(COPYFILES) $(NPSIM)


CopyIncSrcFiles:
	./scripts/copyfiles.sh

DelIncSrcFiles:
	./scripts/deletefiles.sh

NPSimulation:
	make -j 10 -f Makefile.geant4

clean:
	./scripts/deletefiles.sh

distclean:
	./scripts/deletefiles.sh
	@echo " + Deleting bin and tmp directories"
	rm -rf bin
	rm -rf tmp
