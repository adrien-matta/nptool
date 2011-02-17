// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// NPA
#include "DetectorManager.h"
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
#include "TPlasticData.h"
#include "TMust2Data.h"
#include "TMust2Physics.h"
#include "TSSSDPhysics.h"
#include "TPlasticPhysics.h"
#include "GaspardTracker.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


// ----------------------------------------------------------------------------------------------
void InitOutputBranch() ;
/////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------- DOUBLE, INT, BOOL AND MORE -------------------------------
namespace VARIABLE
	{
		//	Declare your Variable here:
		
		Float_t Energy_Must2 = 0 ;
      Float_t Energy_SSSD = 0;
	
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


