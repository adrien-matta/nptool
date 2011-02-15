// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// NPA
#include "DetectorManager.h"
#include "NPOptionManager.h"
#include "Paris.h"
#include "Shield.h"

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

// NPL
//#include "TMust2Data.h"
//#include "TMust2Physics.h"
#include "TParisData.h"
#include "TShieldData.h"
#include "TInitialConditions.h"
#include "NPReaction.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "TInteractionCoordinates.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


// ----------------------------------------------------------------------------------------------
double ThetaCalculation (TVector3 A , TVector3 B) ;
/////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------- DOUBLE, INT, BOOL AND MORE -------------------------------
namespace VARIABLE
	{
		//	Declare your Variable here:
		
			double X1,Y1,Z1				;
			int N1,N2 = 0				;
			bool check= false			;
	
		//	A Usefull Simple Random Generator
			TRandom Rand;
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
	
			TH1F *myHist1D = new TH1F("Hist1D","Histogramm 1D ; x ; count", 1000 , -5 , 5 )		       	;

			TH1F *Ein = new TH1F("Eincident","Incident energy", 1000 , 0 , 50 )	       		;
			TH1F *PhPeakEff = new TH1F("PhPeak","Full absorption efficiency", 1000 , 0 , 50 )	       		;
			TH1F *PhPeakEffNoAB = new TH1F("PhPeakNoAB","Full absorption efficiency - No addback PARIS & SHIELD", 1000 , 0 , 50 )	       		;

			TH1F *PhPeakEffParis = new TH1F("PhPeakParis","Full absorption efficiency", 1000 , 0 , 50 )	       	;
			TH1F *PhPeakEffParisLaBr = new TH1F("PhPeakParisLaBr","Full absorption efficiency in Inner layer", 1000 , 0 , 50 )	       	;
			TH1F *PhPeakEffParisCsI = new TH1F("PhPeakParisCsI","Full absorption efficiency in Outer layer", 1000 , 0 , 50 )	       	;

			TH1F *PhPeakEffShield = new TH1F("PhPeakShield","Full absorption efficiency", 1000 , 0 , 50 ) 	      	;
	
			TH2F *myHist2D = new TH2F("Hist2D","Histogramm 2D ; x ; y ", 128 , 1 , 128 , 128 , 1 , 128 )	;

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
   // Declare your Energy loss here
//   EnergyLoss LightTargetCD2 = EnergyLoss("proton_cd2.txt", 100, 1, 1); // LISE++
   EnergyLoss LightTarget = EnergyLoss("proton_CD2.G4table", "G4Table", 1000);          // G4
  //EnergyLoss BeamTarget  = EnergyLoss("Sn132[0.0]_CD2.G4table", "G4Table", 1000);      // G4
}

using namespace ENERGYLOSS ;
// ----------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


