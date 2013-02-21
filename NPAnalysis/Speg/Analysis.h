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
#include <TSystem.h>
#include <TBranch.h>
#include <TH1F.h>
#include <TTree.h>
#include <TGraph.h>
#include <TSpectrum.h>
#include <TObject.h>

// NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPReaction.h"
#include "TInitialConditions.h"
#include "TSpegPhysics.h"

// Use CLHEP System of unit and Physical Constant
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"

/*------------------Function-----------------------------*/

void InitOutputBranch();
void ReInitValue();
double GetSPEG_Energy(double E4ex, double ThetaSPEG);
void GetPhysicsResults(double T3, double ThetaSPEG, double PhiSPEG, double E4ex);

/*--------------------------Variables--------------------*/

double mass1 = -1000;
double mass2 = -1000;
double mass3 = -1000;
double mass4 = -1000;
double Thetafoc = -1000;
double Phifoc = -1000;
double M_over_Q = -1000;
double Z = -1000;
double ThetaSPEG = -1000;
double PhiSPEG = -1000;
double Time_Plastic_Right = -1000;
double Khi2 = -1000;
double Energy_3 = -1000;
double ExcitationEnergy_4 = -1000;
double Energy_4 = -1000;
double Theta_4 = -1000;
double Phi_4 = -1000;
double Pi = 3.141592654;
double T1 = -1000;
double P1 = -1000;
double T3 = -1000;
double P3 = -1000;
double T4 = -1000;
double P4 = -1000;
double A = -1000;
double B = -1000;
double C = -1000;
double Q = -1000;
double delta = -1000;

TVector3 P1Vector;
TVector3 P3Vector;
TVector3 P4Vector;

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


