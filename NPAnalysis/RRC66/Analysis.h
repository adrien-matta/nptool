// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// NPL
#include "DetectorManager.h"
#include "NPOptionManager.h"
#include "NPReaction.h"
// STL C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>

// ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TMath.h>
// NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TMust2Physics.h"
#include "TSSSDPhysics.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


// ----------------------------------------------------------------------------------------------
void InitOutputBranch() ;
void InitInputBranch() ;
void ReInitValue() ;
void LoadCorrectionValueSSSD() ;
/////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------- DOUBLE, INT, BOOL AND MORE -------------------------------
namespace VARIABLE
	{
		//	Declare your Variable here:
		
		// Parameter 
		unsigned int MaximumMultiplicity = 1;
		double AlThicknessMUST2[4]={0.50*micrometer,0.65*micrometer,0.66*micrometer,0.57*micrometer};
		double AlThicknessSSSD[4]={0.73*micrometer,0.74*micrometer,0.80*micrometer,0.74*micrometer};
		double TargetThickness = 13.1*micrometer;
		double ZTarget = 0;
		// Array of Correction for the SSSD
      vector< vector< vector<double> > > CorrectionMust2;
      vector< vector< vector<double> > > CorrectionSSSD;
      vector< vector< vector<double> > > CorrectionValidation;
      double Validation = 0.4;
		
		// Inputs
		float   PPACF3[5][5];
		float   FPPosition[5][4];
      float   FPTime[5][6];
      float   FPdE[5][3];
      float   RF[2];
      float   Pla[4][5];
      int     GATCONF;
		
		// Outputs 
		Double_t XTarget = 0 ;
		Double_t YTarget = 0;
		Double_t Energy_Must2 = 0 ;
      Double_t Energy_SSSD = 0;
	   Double_t ELab = 0;
	   Double_t ThetaLab = 0;
	   Double_t ThetaCM  = 0;
      Double_t ExcitationEnergy = 0;	   
	
	}
	 
using namespace VARIABLE ;
// ----------------------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------GRAPH------------------------------------------------------
#include <TObject.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH2F.h>
#include <TGraph2D.h>

namespace GRAPH
	{
		//	Declare your Spectra here:
	}

using namespace GRAPH ;
// --------------------------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------CUT------------------------------------------------------
#include <TCutG.h>
namespace CUT
	{
		//	Declare your Cut here:

	}

using namespace CUT ;
// --------------------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------ENERGY LOSS----------------------------------------------
#include "NPEnergyLoss.h"
using namespace NPL ;
namespace ENERGYLOSS
	{
	
		//	Declare your Energy loss here	:
			EnergyLoss He3CD2 = EnergyLoss 	("He3_CD2.G4table","G4Table",100 );
													         
		   EnergyLoss He3Al = EnergyLoss 	("He3_Aluminium.G4table","G4Table",10);
	
	}
	
using namespace ENERGYLOSS ;
// ----------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


