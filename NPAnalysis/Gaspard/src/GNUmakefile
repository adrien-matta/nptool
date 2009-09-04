###Make file made by Adrien MATTA/ Institut de Physique Nucleaire d'Orsay IPNO###
#			Made to compile the ROOT Analyser for MUST2 experiment

CPP=g++
EXEC=Analysis

# local includes
NPAINCLUDES = $(NPANA)/include

# ROOT includes
CXXFLAGS += `root-config --cflags`

# CLHEP includes
CXXFLAGS += -I$(CLHEP_INCLUDE_DIR)
CXXFLAGS += -I$(NPAINCLUDES)
CXXFLAGS += -I$(NPLIB)/include


LDFLAGS  = `root-config --libs` -lMathMore
#LDFLAGS += -L$(NPLIB) -lMust2Data -lReaction -lEnergyLossCorrection 
LDFLAGS += -L$(NPLIB)/lib -lMust2Data -lGaspardData -lInteractionCoordinates \
			-lInitialConditions -lReaction
LDFLAGS += -L$(CLHEP_LIB_DIR) -l$(CLHEP_LIB)

SRC= $(wildcard *.cc)
INC= $(wildcart $(NPAINCLUDES)/*.hh)
OBJ=$(SRC:.cc=.o)

#all:$(EXEC)
#	@$(CPP) -o $@ -c $< $(CXXFLAGS)

Analysis:$(OBJ) $(INC)
	@$(CPP) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	mv Analysis  ../Analysis

%.o: %.cc
	@echo "CXXFLAGS vaut $(CXXFLAGS)"
	@echo "LDFLAGS vaut $(LDFLAGS)"
	@$(CPP) $(CXXFLAGS) -o $@ -c $<
	
.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
