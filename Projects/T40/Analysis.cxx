/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : march 2015                                               *
 * Last update    : December 2016 [m.moukaddam@surrey.ac.uk]                 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class describing the property of an Analysis object                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include<cassert>
#include<iostream>

using namespace std;

#include "Analysis.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "NPFunction.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "TMath.h"
#include "TGraph.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){

  TH  = (TTiaraHyballPhysics*) m_DetectorManager -> GetDetector("HyballWedge");
  TB  = (TTiaraBarrelPhysics*) m_DetectorManager -> GetDetector("Tiara");
  TF  = (TFPDTamuPhysics*) m_DetectorManager -> GetDetector("FPDTamu");
  TG  = (TGeTAMUPhysics*) m_DetectorManager -> GetDetector("GeTAMU");


  // get reaction information
  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = myReaction->GetBeamEnergy();
  cout << "Original Beam energy (entrance of target): " << OriginalBeamEnergy << endl ;

  // target thickness
  TargetThickness = 0*m_DetectorManager->GetTargetThickness();
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();

  // energy losses
/*Original Files
  string light=NPL::ChangeNameToG4Standard(myReaction->GetNucleus3().GetName());
  string beam=NPL::ChangeNameToG4Standard(myReaction->GetNucleus1().GetName());
  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".SRIM","SRIM",10 );
  //LightSi = NPL::EnergyLoss(light+"_Si.G4table","SRIM",10);
  LightSi = NPL::EnergyLoss("He4_Si.SRIM","SRIM",100);
  
  BeamTarget = NPL::EnergyLoss(beam+"_"+TargetMaterial+".SRIM","SRIM",10);
  FinalBeamEnergy = BeamTarget.Slow(OriginalBeamEnergy, TargetThickness*0.5, 0);
  myReaction->SetBeamEnergy(FinalBeamEnergy);
  cout << "Final Beam energy (middle of target): " << FinalBeamEnergy << endl;
*/

//Copied from Momo's Slack 170222.
  string light=NPL::ChangeNameToG4Standard(myReaction->GetNucleus3().GetName());
  string beam=NPL::ChangeNameToG4Standard(myReaction->GetNucleus1().GetName());
  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".SRIM","SRIM",10 );
//by Shuya 170505
//Note when you analyze the triple alpha calibration run, use He4_Al and He4_Si
  LightAl = NPL::EnergyLoss(light+"_Al.SRIM","SRIM",10);
  //LightAl = NPL::EnergyLoss("He4_Al.SRIM","SRIM",10);
  LightSi = NPL::EnergyLoss(light+"_Si.SRIM","SRIM",10);
  //LightSi = NPL::EnergyLoss("He4_Si.SRIM","SRIM",10);
  BeamTarget = NPL::EnergyLoss(beam+"_"+TargetMaterial+".SRIM","SRIM",10);
  FinalBeamEnergy = BeamTarget.Slow(OriginalBeamEnergy, TargetThickness*0.5, 0);
  myReaction->SetBeamEnergy(FinalBeamEnergy);
  cout << "Final Beam energy (middle of target): " << FinalBeamEnergy << endl;

  Initial = new TInitialConditions();
  Rand = new TRandom3();
  ThetaNormalTarget = 0 ;
  ThetaTHSurface = 0;
  ThetaTBSurface = 0;
  Si_E_TH = 0 ;
  Si_E_InnerTB = 0;
  Si_E_OuterTB = 0;
  Si_E_TB = 0 ;
  Energy = 0;


  //Original_ELab=0;
  //Original_ThetaLab=0;
  XTarget =0;
  YTarget =0;
  BeamDirection = TVector3(0,0,1);
  InitOutputBranch();
  InitInputBranch();

  //Ge
  GammaSinglesE=0;

  //FPD
  Delta_E = 0; // Energy ionisation chamber
  Micro2_E = 0; // Energy from micromega total
	Micro1_E_row1_2 = 0; // Energy from micromega rows 1 & 2 ("delta E in stopping mode")
	Micro2_E_row1_2 = 0; // Energy from micromega rows 1-2  ("E in stopping mode")
  Micro1_E_row1 = 0 ;// Energy from micromega row 1
  Micro1_E_col4 = 0 ;// energy from micromega col 4
  Plast_E = 0; // Energy Plastic
	for(int i=0; i< kNumAw; ++i) {
		Aw_X[i] = -1000;
		Aw_Z[i] = -1000;
	}
	Aw_Theta1_2 = -1000;
	Aw_ThetaFit = -1000;
	Aw_ThetaFit_R2 = -1000;
//by Shuya 170516
  Micro1_E_col1 = 0. ;// energy from micromega col 1
  Micro1_E_col2 = 0. ;// energy from micromega col 2
  Micro1_E_col3 = 0. ;// energy from micromega col 3
  Micro1_E_col5 = 0. ;// energy from micromega col 5
  Micro1_E_col6 = 0. ;// energy from micromega col 6
  Micro1_E_col7 = 0. ;// energy from micromega col 7
  Micro2_E_col1 = 0. ;// energy from micromega2 col 1
  Micro2_E_col2 = 0. ;// energy from micromega2 col 2
  Micro2_E_col3 = 0. ;// energy from micromega2 col 3
  Micro2_E_col4 = 0. ;// energy from micromega2 col 3
  Micro2_E_col5 = 0. ;// energy from micromega2 col 5
  Micro2_E_col6 = 0. ;// energy from micromega2 col 6
  Micro2_E_col7 = 0. ;// energy from micromega2 col 7

  //TAC
  TacSiGeOR     = -1000;
  TacSiMicro    = -1000;
 	TacSiMicro_E  = -1000;
	TacSiMicro_dE = -1000;
	TacSiPlastLeft  = -1000;
	TacSiPlastRight = -1000;

  //by Shuya 170524
	//RunNumber = 0;
	RunNumber = -1;

  //by Shuya 170524
	entry = 0;
	EntryNumber = 0;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){

  // Reinitiate calculated variable
  ReInitValue();

  ////////////////////////////////////////// LOOP on TiaraHyball + SSSD Hit //////////////////////////////////////////
  for(unsigned int countTiaraHyball = 0 ; countTiaraHyball < TH->Strip_E.size() ; countTiaraHyball++){
   /////////////////////////////
   // Part 1 : Impact Angle
    ThetaTHSurface = 0;
    ThetaNormalTarget = 0;
    if(XTarget>-1000 && YTarget>-1000){
      TVector3 BeamImpact(XTarget,YTarget,0);
      TVector3 HitDirection = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball) - BeamImpact ;

      ThetaLab = HitDirection.Angle( BeamDirection );
      ThetaTHSurface = HitDirection.Angle(TVector3(0,0,-1)); // vector Normal on Hyball
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    }
    else{
      BeamDirection = TVector3(-1000,-1000,-1000);
      ThetaTHSurface    = -1000  ;
      ThetaNormalTarget = -1000  ;
    }

    /////////////////////////////
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Si_E_TH = TH->Strip_E[countTiaraHyball];
    Energy = Si_E_TH; // calibration for hyball is in MeV
    // Correct for energy loss using the thickness of the target and the dead layer
    ELab = LightSi.EvaluateInitialEnergy( Energy ,0.61*micrometer , ThetaTHSurface); // 0.1 um of Aluminum
    ELab = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

   /////////////////////////////
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

    /////////////////////////////
    // Part 5 : Implementing randomised position impact matrix for the Hyball
    TVector3 HyballRandomImpactPosition = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball);
    TiaraIMX = HyballRandomImpactPosition.X();
    TiaraIMY = HyballRandomImpactPosition.Y();
    TiaraIMZ = HyballRandomImpactPosition.Z();
    LightParticleDetected = true ;
  } // end loop TiaraHyball

  /////////////////////////// LOOP on TiaraBarrel /////////////////////////////
  for(unsigned int countTiaraBarrel = 0 ; countTiaraBarrel < TB->Strip_E.size() ; countTiaraBarrel++){
    /////////////////////////////
    // Part 1 : Impact Angle
    ThetaTBSurface = 0;
    ThetaNormalTarget = 0;
    if(XTarget>-1000 && YTarget>-1000){
      TVector3 BeamImpact(XTarget,YTarget,0);
      TVector3 HitDirection = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel) - BeamImpact ;
      //Angle of emission wrt to beam
      ThetaLab = HitDirection.Angle( BeamDirection );
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
      //angle of impact on detector
      TVector3 NormalOnBarrel(0,1,0); // Normal on detector 3 
      int det = TB->Detector_N[countTiaraBarrel];
      NormalOnBarrel.RotateZ((3-det)*45*deg);
      ThetaTBSurface = HitDirection.Angle(NormalOnBarrel);
    }
    else{
      BeamDirection = TVector3(-1000,-1000,-1000);
      ThetaTBSurface    = -1000  ;
      ThetaNormalTarget = -1000  ;
    }

    /////////////////////////////
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Si_E_InnerTB = TB->Strip_E[countTiaraBarrel];
    Energy = Si_E_InnerTB*keV;// calibration for barrel is in keV
    
    //treat the back detector (in progress)
    if(false && TB->Outer_Strip_E.size()>0)
	    if(TB->Outer_Strip_E[countTiaraBarrel]>0){
	      Si_E_OuterTB = TB->Outer_Strip_E[countTiaraBarrel];
	      Energy = Si_E_InnerTB*keV + Si_E_OuterTB*keV;
	    }

    // Evaluate energy using the thickness, Target and Si dead layer Correction
    ELab = LightSi.EvaluateInitialEnergy( Energy ,0.3*micrometer, ThetaTBSurface);
    //ELab = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

    /////////////////////////////
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );

    //////////////////////////////
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

    /////////////////////////////
    // Part 5 : Implementing randomised position impact matrix for both the entire Barrel (all 8 strips) and each strip making up the octagonal Barrel individually
    TVector3 BarrelRandomImpactPosition = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel);
    TiaraIMX = BarrelRandomImpactPosition.X();
    TiaraIMY = BarrelRandomImpactPosition.Y();
    TiaraIMZ = BarrelRandomImpactPosition.Z();
    LightParticleDetected = true ;
  } // end loop TiaraBarrel

/////////////////////////// GENERAL treatment on Ge TAMU /////////////////////////////

  /////////////////////////////
  // Part 1 : Get the recoil beta vetor
 TLorentzVector Recoil_LV;
 TVector3 RecoilBeta;
 if (LightParticleDetected) Recoil_LV =  myReaction -> GetEnergyImpulsionLab_4();
 else Recoil_LV =  myReaction -> GetEnergyImpulsionLab_1();
  RecoilBeta = Recoil_LV.Vect();
  RecoilBeta *= (1/Recoil_LV.E());// divide by the total energy (T+M) to get the velocity (beta) vector

  /////////////////////////////
  // Part 2 : Calculate Doppler-Corrected energies for singles, and addback spectra
  TG->DCSingles(RecoilBeta);
  TG->AddBack(RecoilBeta);

  /////////////////////////// LOOP on Ge TAMU /////////////////////////////
  //for(unsigned int countGe = 0 ; countGe < TG->Singles_E.size() ; countGe++){ // multiplicity treated for now is zero
  //
  //Event by event treatment goes here
  //
  //}


 ////////////////////////////////////////// LOOP on FPD  //////////////////////////////////////////
	// Micromega energy
	// Sums across various rows & columns
	if(TF->MicroRowNumber.size())
	{
		Micro1_E_row1 = TF->GetMicroGroupEnergy(1,1,1,1,7); // energy sum from the row 1
		Micro1_E_col4 = TF->GetMicroGroupEnergy(1,1,4,4,4); // energy sum from the col 4
		//by Shuya 170516. Since Micro2_E is dE detector, we always expect the particles penetrate through the whole rows. That is why you should use GetMicroRowGeomEnergy() instead of GetMicroGroupEnergy().
		//Micro2_E      = TF->GetMicroGroupEnergy(2,1,4,1,7); // energy sum from all the pads
		Micro2_E      = TF->GetMicroRowGeomEnergy(2,1,4,0); // energy sum from all the pads
		Micro1_E_row1_2 = TF->GetMicroGroupEnergy(1,1,2,1,7); // energy sum from row 1-2
		Micro2_E_row1_2 = TF->GetMicroGroupEnergy(2,1,2,1,7); // energy sum from row 3-6

		//by Shuya 170516. For Micro1's energy sum, you need to choose which of GetMicroGroupEnergy() and GetMicroRowGeomEnergy(). If you're using the particles penetrate the Micro1, better to use GetMicroRowGeomEnergy().
		Micro1_E_col1 = TF->GetMicroGroupEnergy(1,1,4,1,1); // energy sum from the col 1
		Micro1_E_col2 = TF->GetMicroGroupEnergy(1,1,4,2,2); // energy sum from the col 2
		Micro1_E_col3 = TF->GetMicroGroupEnergy(1,1,4,3,3); // energy sum from the col 3
		Micro1_E_col5 = TF->GetMicroGroupEnergy(1,1,4,5,5); // energy sum from the col 5
		Micro1_E_col6 = TF->GetMicroGroupEnergy(1,1,4,6,6); // energy sum from the col 6
		Micro1_E_col7 = TF->GetMicroGroupEnergy(1,1,4,7,7); // energy sum from the col 7

		Micro2_E_col1      = TF->GetMicroRowGeomEnergy(2,1,4,1); // energy sum from the col1.
		Micro2_E_col2      = TF->GetMicroRowGeomEnergy(2,1,4,2); // energy sum from the col2.
		Micro2_E_col3      = TF->GetMicroRowGeomEnergy(2,1,4,3); // energy sum from the col3.
		Micro2_E_col4      = TF->GetMicroRowGeomEnergy(2,1,4,4); // energy sum from the col4.
		Micro2_E_col5      = TF->GetMicroRowGeomEnergy(2,1,4,5); // energy sum from the col5.
		Micro2_E_col6      = TF->GetMicroRowGeomEnergy(2,1,4,6); // energy sum from the col6.
		Micro2_E_col7      = TF->GetMicroRowGeomEnergy(2,1,4,7); // energy sum from the col7.
	}
	else
	{
		Micro1_E_row1 = -1000;
		Micro1_E_col4 = -1000;
		Micro1_E_row1_2 = -1000;
		Micro2_E_row1_2 = -1000;
		Micro2_E      = -1000;
		//by Shuya 170516
		Micro1_E_col1 = -1000;
		Micro1_E_col2 = -1000;
		Micro1_E_col3 = -1000;
		Micro1_E_col5 = -1000;
		Micro1_E_col6 = -1000;
		Micro1_E_col7 = -1000;
		Micro2_E_col1 = -1000;
		Micro2_E_col2 = -1000;
		Micro2_E_col3 = -1000;
		Micro2_E_col4 = -1000;
		Micro2_E_col5 = -1000;
		Micro2_E_col6 = -1000;
		Micro2_E_col7 = -1000;
	}

	// Delta E ion chamber
	Delta_E      = TF->DeltaEnergy.empty() ? -1000 : TF->DeltaEnergy[0];

	// Energy in plastic
	Plast_E      = TF->PlastCharge.empty() ? -1000 : TF->PlastCharge[0];

	// Avalanche wire
	for(size_t iw=0; iw< TF->AWireDetNumber.size(); ++iw) {
		int detNumber = TF->AWireDetNumber[iw];

		if(detNumber >=0 && detNumber< 4) {
			Aw_X[detNumber] = TF->AWirePositionX[iw];
			Aw_Z[detNumber] = TF->AWirePositionZ[iw];
		}
		else {
			cerr << "WARNING:: Wire number not bewtween 0 and 4!\n";
		}
	}

	// (calculate angles)
	if(Aw_X[0] != -1000 && Aw_X[1] != -1000) {
		Aw_Theta1_2 = TMath::ATan((Aw_X[1] - Aw_X[0]) / (Aw_Z[1] - Aw_Z[0]));
		Aw_Theta1_2 *= (180/TMath::Pi());
	}

	int numValid = 0;
	for(int i=0; i< kNumAw; ++i) {
		if(Aw_X[i] != -1000) { ++numValid; }
		if(numValid == 2) {  // at least 2 points to calculate an angle
			Aw_ThetaFit = TF->AWireAngle*(180/TMath::Pi());
			Aw_ThetaFit_R2 = TF->AWireFitR2;
			break;
		}
	}

  ////////////////////////////////////////// TAC  //////////////////////////////////////////
  // The Physics classes for FPDTamu are made to hold the time information from every channel
  // i.e. there can not be channels providing time more than channels providing energies
  // In Tiara@TAMU campaign the times are provided as OR from the Micro and the Germanium
  // i.e. one time channel per detector. Typically this information is given to the first channel
  // of the detector, e.g. for micro: FpdTAMU MICRO_R1_C1_T and for HPGe: GeTamu CLOVER01_CRYSTAL01_T

	if(TF->MicroTimeOR.size()){
		TacSiMicro = TF->MicroTimeOR[0];

		for(UInt_t ti = 0; ti< TF->MicroTimeOR.size(); ++ti) {
			switch(TF->MicroTimeRowNumber[ti]) {
			case 0:
				TacSiMicro_dE = TF->MicroTimeOR[ti];
				break;
			case 5:
				TacSiMicro_E  = TF->MicroTimeOR[ti];
				break;
			default:
				break;
			}
		}
	}

  // For the plastic there's two ways to calculate the times, both ar OR.
  // The two available time channels i.e. Plast Right and Plast left are used in this case
  if(TF->PlastRightTime.size()==1)
      TacSiPlastRight = TF->PlastRightTime[0];
  else
    TacSiPlastRight = -999;

  if(TF->PlastLeftTime.size()==1)
      TacSiPlastLeft = TF->PlastLeftTime[0];
  else
    TacSiPlastLeft = -999;


	if(TG->GeTime.size()==1)
    TacSiGeOR = TG->GeTime[0];
  else
    TacSiGeOR = -999;

  //by Shuya 170524
 	//RunNumber = RootInput::getInstance()->GetChain()->GetFileNumber() + 1;
	if(currentfilename != RootInput::getInstance()->GetChain()->GetCurrentFile()->GetName())
	{
		RunNumber++;
		currentfilename = RootInput::getInstance()->GetChain()->GetCurrentFile()->GetName();
	}

  //by Shuya 170524
 	EntryNumber = RootInput::getInstance()->GetChain()->GetEntryNumber(entry);
	entry++;

}


////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}


void Analysis::ReInitValue(){
  //Silicon
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  LightParticleDetected = false ;

  //Simu
  //Original_ELab = -1000;
  //Original_ThetaLab = -1000;

  //FPD
  Delta_E      = -1000;
  Micro1_E_row1 = -1000;
  Micro1_E_col4 = -1000;
 	Micro1_E_row1_2 = -1000;
	Micro2_E_row1_2 = -1000;
	Micro2_E      = -1000;
  Plast_E      = -1000;
//by Shuya 170516
  Micro1_E_col1 = -1000;
  Micro1_E_col2 = -1000;
  Micro1_E_col3 = -1000;
  Micro1_E_col5 = -1000;
  Micro1_E_col6 = -1000;
  Micro1_E_col7 = -1000;
  Micro2_E_col1 = -1000;
  Micro2_E_col2 = -1000;
  Micro2_E_col3 = -1000;
  Micro2_E_col4 = -1000;
  Micro2_E_col5 = -1000;
  Micro2_E_col6 = -1000;
  Micro2_E_col7 = -1000;

	for(int i=0; i< kNumAw; ++i) {
		Aw_X[i] = -1000;
		Aw_X[i] = -1000;
	}
	Aw_Theta1_2 = -1000;
	Aw_ThetaFit	= -1000;
	Aw_ThetaFit_R2	= -1000;

  //TAC
  TacSiGeOR     = -1000;
  TacSiMicro    = -1000;
	TacSiMicro_E  = -1000;
	TacSiMicro_dE = -1000;
  TacSiPlastLeft  = -1000;
  TacSiPlastRight = -1000;

	//by Shuya 170524
	//RunNumber = 0;
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void Analysis::InitOutputBranch() {
  //Tiara
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixX",&TiaraIMX,"TiaraImpactMatrixX/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixY",&TiaraIMY,"TiaraImpactMatrixY/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixZ",&TiaraIMZ,"TiaraImpactMatrixZ/D");

  //GeTamu
  // stuff goes here
  RootOutput::getInstance()->GetTree()->Branch("GammaSinglesE",&GammaSinglesE,"GammaSinglesE/D");


  //FPD
  RootOutput::getInstance()->GetTree()->Branch("Delta_E",&Delta_E,"Delta_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_row1",&Micro1_E_row1,"Micro1_E_row1/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col4",&Micro1_E_col4,"Micro1_E_col4/D");
	RootOutput::getInstance()->GetTree()->Branch("Micro1_E_row1_2", &Micro1_E_row1_2, "Micro1_E_row1_2/D");
	RootOutput::getInstance()->GetTree()->Branch("Micro2_E_row1_2", &Micro2_E_row1_2, "Micro2_E_row1_2/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E",&Micro2_E,"Micro2_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Plast_E",&Plast_E,"Plast_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Aw_X",Aw_X,Form("Aw_X[%i]/D", kNumAw));
  RootOutput::getInstance()->GetTree()->Branch("Aw_Z",Aw_Z,Form("Aw_Z[%i]/D", kNumAw));
  RootOutput::getInstance()->GetTree()->Branch("Aw_Theta1_2",&Aw_Theta1_2,"Aw_Theta1_2/D");
  RootOutput::getInstance()->GetTree()->Branch("Aw_ThetaFit",&Aw_ThetaFit,"Aw_ThetaFit/D");
  RootOutput::getInstance()->GetTree()->Branch("Aw_ThetaFit_R2",&Aw_ThetaFit_R2,"Aw_ThetaFit_R2/D");
  //by Shuya 170516
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col1",&Micro1_E_col1,"Micro1_E_col1/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col2",&Micro1_E_col2,"Micro1_E_col2/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col3",&Micro1_E_col3,"Micro1_E_col3/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col5",&Micro1_E_col5,"Micro1_E_col5/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col6",&Micro1_E_col6,"Micro1_E_col6/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro1_E_col7",&Micro1_E_col7,"Micro1_E_col7/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col1",&Micro2_E_col1,"Micro2_E_col1/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col2",&Micro2_E_col2,"Micro2_E_col2/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col3",&Micro2_E_col3,"Micro2_E_col3/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col4",&Micro2_E_col4,"Micro2_E_col4/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col5",&Micro2_E_col5,"Micro2_E_col5/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col6",&Micro2_E_col6,"Micro2_E_col6/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro2_E_col7",&Micro2_E_col7,"Micro2_E_col7/D");

//TACS
  RootOutput::getInstance()->GetTree()->Branch("TacSiGeOR",&TacSiGeOR,"TacSiGeOR/D");
	RootOutput::getInstance()->GetTree()->Branch("TacSiMicro",&TacSiMicro,"TacSiMicro/D");
	RootOutput::getInstance()->GetTree()->Branch("TacSiMicro_E",&TacSiMicro_E,"TacSiMicro_E/D");
	RootOutput::getInstance()->GetTree()->Branch("TacSiMicro_dE",&TacSiMicro_dE,"TacSiMicro_dE/D");
	RootOutput::getInstance()->GetTree()->Branch("TacSiPlastLeft",& TacSiPlastLeft," TacSiPlastLeft/D");
  RootOutput::getInstance()->GetTree()->Branch("TacSiPlastRight",& TacSiPlastRight," TacSiPlastRight/D");

// Other
	RootOutput::getInstance()->GetTree()->Branch("RunNumber", &RunNumber," RunNumber/I");
// by Shuya 170524.
	RootOutput::getInstance()->GetTree()->Branch("EntryNumber", &EntryNumber," EntryNumber/I");

  //Simulation
  //RootOutput::getInstance()->GetTree()->Branch("Original_ELab",&Original_ELab,"Original_ELab/D");
  //RootOutput::getInstance()->GetTree()->Branch("Original_ThetaLab",&Original_ThetaLab,"Original_ThetaLab/D");

}
/////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  //RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial );
  //RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
  return (NPL::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy p;
}
