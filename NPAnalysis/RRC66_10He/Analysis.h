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
#include "TRIPSData.h"
#include "NPPhysicalConstants.h"
#include "NPGlobalSystemOfUnits.h"

// ----------------------------------------------------------------------------------------------
void TreatRIPS();
void InitOutputBranch() ;
void InitInputBranch() ;
void ReInitValue() ;
void LoadCorrectionValueSSSD() ;
void LoadCorrectionTimeValue(int i) ;  
/////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------- DOUBLE, INT, BOOL AND MORE -------------------------------
namespace VARIABLE
	{
		//	Declare your Variable here:
		TRandom Rand;
		// Parameter 
		int MaximumMultiplicity = 1;
		double AlThicknessMUST2[4]={0.50*micrometer,0.65*micrometer,0.66*micrometer,0.57*micrometer};
		double SiThicknessMUST2[4]={311.*micrometer,308.*micrometer,300.*micrometer,297.*micrometer};
		double AlThicknessSSSD[4]={0.70*micrometer,0.76*micrometer,0.78*micrometer,0.76*micrometer};
		double TargetThickness = 18.*micrometer;
		double ZTarget = 0.;
		double ZPPAC_A =  (-1569.1  -1560.9)/2.;
		double ZPPAC_B =  (-1269.1  -1260.9)/2.;
		double ZPPAC_TA = (-418.4   -407.4 )/2.;
		double ZPPAC_TB = (-380.0   -388.2 )/2.;
		
		double ZPPAC_A_X =  -1569.1;
		double ZPPAC_B_X =  -1269.1;
		double ZPPAC_TA_X = -418.4 ;
		double ZPPAC_TB_X = -380.0 ;
		
		double ZPPAC_A_Y =  -1560.9;
		double ZPPAC_B_Y =  -1260.9;
		double ZPPAC_TA_Y = -407.4 ;
		double ZPPAC_TB_Y = -388.2 ;
		
		double PPACT0 = 0 ;
		double PPACT1 = 0 ;
		int PPAC0 = 0 ;
		int PPAC1 = 0 ;


    // Corrected Kinematic	
    double Corrected_ThetaLab3;
    double Corrected_EnergyLab3;
    double Corrected_ThetaLab4;
    double Corrected_EnergyLab4;

	
		TVector3 BeamDirection=TVector3(-1000,-1000,-1000);
		double ThetaM2Surface=-1000;
		double ThetaNormalTarget=-1000;
		
		// Array of Correction for the SSSD
		int PixelMappingResolution = 4;
      vector< vector <vector <double>  > > CorrectionA;
      Double_t NormalisationA = 20.   ;// Normalise to 20um
		
		// Array of Correction for the MUST2 Time
      vector< vector< double > > CorrectionTimeMust2;
		int CorrectionTimeVersion=0;
		
		// Time calibration for the time of flight between F2 and F3
		double F2_F3_distance = 5.322;//m
		double F3_Target_distance = 1.5691;//m stated to be equal to PPAC distance (to be confirmed)

  //Original
  double a_BeamTof_old = 1.17227;
  double b_BeamTof_old = 13.6638;

  // Fit Li11 Method:
  //double b_BeamTof = 263.652  ;//  +/-   107.387
  //double a_BeamTof = -5.76768 ;//  +/-   3.8222
  
  // Fit Li11+B15 Method:
  //double b_BeamTof = 24.0225  ;//  +/-   2.86038
  //double a_BeamTof = 0.848848 ;//  +/-   0.0881467
  
  // Fit B15 + Li11 broh:
  /****************************************
  Minimizer is Linear
  Chi2                      =            0
  NDf                       =            0
  p0                        =      16.7007   +/-   5.43661
  p1                        =      1.07849   +/-   0.167561*/
  
  // Mean Method:
  //double a_BeamTof = 1.07849;
  //double b_BeamTof = 16.7007;
  
  // Max Method:
  //double a_BeamTof = 0.802397;
  //double b_BeamTof = 25.046684;
  
  
  // Multi Nuclei Method, second order:
  double a_BeamTof =     49.5567  ;//oth order +/-   0.591581
  double b_BeamTof =      1.27266 ;//1st order +/-   0.0845456
  double c_BeamTof =   -0.0386691 ;//2nd order +/-   0.014403
  
		// Inputs
		TRIPSData* RIPSData = new TRIPSData();
		TMust2Physics* T6 = new TMust2Physics();
      int     GATCONF;
		
		
		
		// Outputs 
		Double_t PlaF2_E = 0 ;
		Double_t PlaF2_T = 0;
		Double_t PlaF3_E = 0;
		Double_t PlaF3_T = 0;
		Double_t PlaF3_Tprec = 0 ;
		Double_t PlaF3_TL = 0;
		Double_t BeamEnergy=0;
		Double_t T_M2 = 0 ;
		Double_t T_M2_corr = 0 ;
		Double_t X_M2 = 0 ;
		Double_t Y_M2 = 0 ;
		Double_t Z_M2 = 0 ;
		Double_t ZD1_E = 0;
		Double_t ZD2_E = 0;
		Double_t ZD1_T = 0;
		Double_t ZD2_T = 0;
		Double_t XTarget = 0 ;
		Double_t XTarget_Prec = 0 ;
		Double_t YTarget = 0 ;
		Double_t Energy = 0 ;
		Int_t RunNumber = 0 ;
		
		Int_t DetectorNumber = 0 ;
		Double_t Si_E_M2 = 0 ;
		Double_t CsI_E_M2 = 0;
		Double_t CsI_E_M2_calculated= 0;
		Int_t Si_X_M2 = 0 ;
		Int_t Si_Y_M2 = 0 ;
      Double_t E_M2 = 0 ;
      Double_t E_SSSD = 0 ;
	   
	   Double_t T6_SiE = 0 ;
	   Double_t T6_CsIE = 0 ;
	   Int_t T6_CsIN = 0 ;
	   
	   Double_t ELab = 0 ;
	   Double_t ThetaLab = 0 ;
	   Double_t ThetaLabHeavy = 0 ;
	   Double_t ThetaCM  = 0 ;
	   Double_t ThetaCM2  = 0 ;
      Double_t ExcitationEnergy = 0 ;	
      
      // Corrected Value for particle ID
      Double_t E_Corr = 0 ;
      
      // Counter
      int RejectBeam=0;  
      int RejectTotal=0;  
      int RejectMultiplicity=0;  
      int MultiplicitySup1=0;  
      int Multiplicity0=0;
      int MultiplicityCsI=0;
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
			// For 3He
			EnergyLoss He3CD2 = EnergyLoss 	("He3_CD2.G4table","G4Table",10 );
		   EnergyLoss He3Al = EnergyLoss 	("He3_Aluminium.G4table","G4Table",1);
		   EnergyLoss He3Si = EnergyLoss 	("He3_Si.G4table","G4Table",10);
		   
		   // For 11Li
		   EnergyLoss Li11_BC400 = EnergyLoss ("Li11[0.0]_Plastic_BC400.G4table","G4Table",10);
		   EnergyLoss Li11_Mylar = EnergyLoss ("Li11[0.0]_Mylar.G4table","G4Table",10);
		   EnergyLoss Li11_CD2 = EnergyLoss ("Li11[0.0]_CD2.G4table","G4Table",10);
		   
		   //	Declare your Energy loss here	:
//			EnergyLoss He3CD2 = EnergyLoss 	("He3_CD2.srim","SRIM",10 );
//													         
//		   EnergyLoss He3Al = EnergyLoss 	("He3_Al.srim","SRIM",1);
//		   
//		   EnergyLoss He3Si = EnergyLoss 	("He3_Si.srim","SRIM",10);
	}
	
using namespace ENERGYLOSS ;
// ----------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


