// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// NPA
#include "NPDetectorManager.h"
#include "NPOptionManager.h"

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
#include "RootInput.h"
#include "RootOutput.h"
#include "NPReaction.h"
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"
#include "THiraData.h"
#include "THiraPhysics.h"


// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"

Double_t TargetThickness;
Double_t E_ThinSi = 0;
Double_t E_ThickSi = 0;
Double_t E_CsI = 0;
Double_t ELab = 0;
Double_t ThetaLab = 0;
Double_t PhiLab = 0;
Double_t ThetaLab_simu = 0;
Double_t ThetaCM = 0;
Double_t ExcitationEnergy = 0;
Double_t X = 0;
Double_t Y = 0;
Double_t Z = 0;
Double_t TelescopeNumber = 0;

Double_t BeamEnergy = 0;

void ReInitOuputValue();

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
	
			TH1F *myHist1D = new TH1F("Hist1D","Histogramm 1D ; x ; count", 1000 , -5 , 5 )					;
	
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
	
		//	Declare your Energy loss here	:
	/*		EnergyLoss ProtonTarget = EnergyLoss 	(	"CD2.txt" 	,
														100 		,
														1,
														1			);
	*/
	}
	
using namespace ENERGYLOSS ;
// ----------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


