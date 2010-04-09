// You can use this file to declare your spectra, file, energy loss , ... and whatever you want.
// This way you can remove all unnecessary declaration in the main programm.
// In order to help debugging and organizing we use Name Space.

/////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------- VARIOUS INCLUDE ---------------------------------------

// NPA
#include "DetectorManager.h"

// STL C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <time.h>

// ROOT
#include <TROOT.h>
#include <TCutG.h>
#include <TChain.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TVector3.h>
#include <TRandom.h>

// NPL
#include "TPlasticData.h"
#include "NPReaction.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "TInitialConditions.h"
#include "TTriggerData.h"
#include "TTacData.h"
#include "TMust2Physics.h"
#include "TCATSPhysics.h"
#include "TSSSDPhysics.h"
#include "TPlasticPhysics.h"

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
	 //open the ROOT file for cuts
	  TFile *FCuts = new TFile("cut_protons.root");
	  TCutG *cut_protons = (TCutG*) FCuts->Get("protons");
        }

using namespace CUT ;
// --------------------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------ENERGY LOSS----------------------------------------------
#include "NPEnergyLoss.h"
using namespace NPL ;
namespace ENERGYLOSS
	{
	  // Beam Energy Loss
	  /*
	  EnergyLoss Fe60TargetCD2    = EnergyLoss      ("Fe60[0.0]_CD2.G4table",
							 "G4Table"              ,
							 1000                   );
	  */
	  EnergyLoss Fe60TargetCD2    = EnergyLoss 	("60Fe_CD2.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 60		  );

	  EnergyLoss Fe60TargetCarbone= EnergyLoss 	("60Fe_carbone.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 60		  );

	  /*
	  EnergyLoss S34TargetCD2    = EnergyLoss      ("S34[0.0]_CD2.G4table",
							 "G4Table"              ,
							 1000                   );
	  */
	  EnergyLoss Si34TargetCD2    = EnergyLoss 	("34Si_CD2.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 34		  );

	  EnergyLoss Si34TargetCarbone= EnergyLoss 	("34Si_carbone.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 34		  );

	  EnergyLoss S36TargetCD2    = EnergyLoss 	("36S_CD2.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 36		  );

	  EnergyLoss S36TargetCarbone= EnergyLoss 	("36S_carbone.txt",
							 "LISE"            ,
							 1000		   ,
							 1		   ,
							 36		  );

	  //	proton Energy Loss
	  EnergyLoss protonTargetWind = EnergyLoss 	("proton_Mylar.txt",
							 "LISE"            ,
							  1000		   ,
							  1,
							  1		  );
		
	  EnergyLoss protonTargetC   = EnergyLoss       ("proton_carbone.txt" 	,
							 "LISE"                         ,
							 1000		 		,
							 1				,
							 1				);

	  EnergyLoss protonTargetCD2 = EnergyLoss       ("proton_cd2.txt" 	,
							 "LISE"                         ,
							 1000		 		,
							 1				,
							 1				);

	  
	  EnergyLoss protonStripAl   = EnergyLoss 	("proton_Al.txt" 		,
							 "LISE"                         ,
							 1000				,
							 1				,
							 1				);
	  
	  
	}
	
using namespace ENERGYLOSS ;
// ----------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////////////
// -----------------------------------Random Engine----------------------------------------------
#include "TRandom3.h"
namespace RANDOMENGINE
	{
	
		TRandom3 RandomEngine = TRandom3();
	
	}
	
using namespace RANDOMENGINE ;
// ----------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////

