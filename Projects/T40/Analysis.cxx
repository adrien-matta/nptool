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

#include <iostream>
using namespace std;

#include "Analysis.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "NPFunction.h" 
#include "RootOutput.h"
#include "RootInput.h"
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
  TargetThickness = m_DetectorManager->GetTargetThickness()*micrometer;
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();

  // energy losses
  string light=NPL::ChangeNameToG4Standard(myReaction->GetNucleus3().GetName());
  string beam=NPL::ChangeNameToG4Standard(myReaction->GetNucleus1().GetName());

  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",10 );
  LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",10);
  LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",10);
  BeamTarget = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",10);
  FinalBeamEnergy = BeamTarget.Slow(OriginalBeamEnergy, TargetThickness*0.5, 0);

  cout << "Final Beam energy (middle of target): " << FinalBeamEnergy << endl;
  myReaction->SetBeamEnergy(FinalBeamEnergy);

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

  //FPD
  Delta_E = 0; // Energy ionisation chamber
  Micro_E = 0; // Energy from micromega total
  Micro_E_row1 = 0 ;// Energy from micromega row 1 
  Micro_E_col4 = 0 ;// energy from micromega col 1 
  Plast_E = 0; // Energy Plastic
  XPlastic_aw = 0; // X on plastic from avalanche wire
  Theta_aw    = 0; // ion direction in the FPD
  XPlastic    = 0; // X on plastic from plastic PMTs
  
  //TAC
  TacSiGeOR     = -1000;
  TacSiMicroOR    = -1000;
  TacSiPlast1  = -1000;
  TacSiPlast2 = -1000;

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
      ThetaTHSurface = HitDirection.Angle(TVector3(0,0,-1) );
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
    Energy = Si_E_TH;

    // Evaluate energy using the thickness 
    ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTHSurface); 
    // Target Correction
    ELab = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); 

   /////////////////////////////
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );

    /////////////////////////////
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

    /////////////////////////////
    // Part 5 : Implementing randomised position impact matrix for the Hyball
    TVector3 HyballRandomImpactPosition = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball);
    TiaraIMX = HyballRandomImpactPosition.X();
    TiaraIMY = HyballRandomImpactPosition.Y();
    TiaraIMZ = HyballRandomImpactPosition.Z();

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
      ThetaLab = HitDirection.Angle( BeamDirection );
      ThetaTBSurface = HitDirection.Angle(TVector3(0,0,-1) ) - TMath::PiOver2();
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
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
    if(TB->Outer_Strip_E[countTiaraBarrel]>0){
      Si_E_OuterTB = TB->Outer_Strip_E[countTiaraBarrel];
      Energy = Si_E_InnerTB + Si_E_OuterTB;
    }
    else {
      Energy = Si_E_InnerTB;
    }

    // Evaluate energy using the thickness 
    ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTBSurface); 
    // Target Correction
    ELab = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);

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

  } // end loop TiaraBarrel

  /////////////////////////// LOOP on Ge TAMU /////////////////////////////
//for(unsigned int countGe = 0 ; countGe < TG->something.size() ; countGe++) // multiplicity treated for now is zero 
//  { 
  // Things goes here ...
//  }

 ////////////////////////////////////////// LOOP on FPD  //////////////////////////////////////////
  //for(unsigned int countFPD = 0 ; countFPD < TF->Delta.size() ; countFPD++) // multiplicity treated for now is zero 
  { 
    //TF->Dump();
    if(0){ // some condition
      // Part 1 : Collect the energis from the different sub-detectors
      Delta_E      = TF->DeltaEnergy[0];
      Micro_E_row1 = TF->GetMicroGroupEnergy(1,1,1,7); // energy sum from the row 1 
      Micro_E_col4 = TF->GetMicroGroupEnergy(1,4,4,4); // energy sum from the col 4
      Micro_E      = TF->GetMicroGroupEnergy(1,4,1,7); // energy sum from all the pads 
      Plast_E      = TF->PlastCharge[0];

      // Part 2 : Reconstruct ion direction from Avalanche Wire
      Theta_aw          = TF->IonDirection.Theta()/deg; // calculate Theta from AWire
      XPlastic_aw       = TF->PlastPositionX_AW; // calculate position on plastic, provided the Ion Direction, and Z plastic 
      XPlastic          = TF->PlastPositionX[0]; // calculate  position on plastic from Right and Left PMT signals 
    }
    else{
      Delta_E      = -1000;
      Micro_E_row1 = -1000;   
      Micro_E_col4 = -1000; 
      Micro_E      = -1000;  
      Plast_E      = -1000;
      Theta_aw     = -1000;
      XPlastic_aw  = -1000;
      XPlastic     = -1000;
    }

  } // end loop on FPD
  

  ////////////////////////////////////////// TAC  //////////////////////////////////////////
  // The Physics classes for FPDTamu are made to hold the time information from every channel
  // i.e. there can not be channels providing time more than channels providing energies 
  // In Tiara@TAMU campaign the times are provided as OR from the Micro and the Germanium
  // i.e. one time channel per detector. Typically this information is given to the first channel
  // of the detector, e.g. for micro: FpdTAMU MICRO_R1_C1_T and for HPGe: GeTamu CLOVER01_CRYSTAL01_T 
  if(TF->MicroTime.size()==1) 
    TacSiMicroOR = TF->MicroTime[0];
  else 
    TacSiMicroOR = -999;
  if(TG->GeTime.size()==1)
    TacSiGeOR = TG->GeTime[0];
  else 
    TacSiGeOR = -999;
  // For the plastic there's two ways to calculate the times, both ar OR. 
  // The two available time channels i.e. Plast Right and Plast left are used in this case
  if(TF->PlastRightTime.size()==1)
      TacSiPlast1 = TF->PlastRightTime[0];
  else
    TacSiPlast1 = -999;
  
  if(TF->PlastLeftTime.size()==1)
      TacSiPlast2 = TF->PlastLeftTime[0];
  else
    TacSiPlast2 = -999;

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
  
  //Simu
  //Original_ELab = -1000;
  //Original_ThetaLab = -1000;
  
  //FPD
  Delta_E      = -1000;
  Micro_E_row1 = -1000;   
  Micro_E_col4 = -1000; 
  Micro_E      = -1000;  
  Plast_E      = -1000;
  Theta_aw     = -1000;
  XPlastic_aw  = -1000;
  XPlastic     = -1000;
  
  //TAC
  TacSiGeOR     = -1000;
  TacSiMicroOR    = -1000;
  TacSiPlast1  = -1000;
  TacSiPlast2 = -1000;
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
  //FPD
  RootOutput::getInstance()->GetTree()->Branch("Delta_E",&Delta_E,"Delta_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro_E_row1",&Micro_E_row1,"Micro_E_row1/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro_E_col4",&Micro_E_col4,"Micro_E_col4/D");
  RootOutput::getInstance()->GetTree()->Branch("Micro_E",&Micro_E,"Micro_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Plast_E",&Plast_E,"Plast_E/D");
  RootOutput::getInstance()->GetTree()->Branch("Theta_aw",&Theta_aw,"Theta_aw/D");
  RootOutput::getInstance()->GetTree()->Branch("XPlastic_aw",&XPlastic_aw,"XPlastic_aw/D");
  RootOutput::getInstance()->GetTree()->Branch("XPlastic",&XPlastic,"XPlastic/D"); 
  //TACS
  RootOutput::getInstance()->GetTree()->Branch("TacSiGeOR",&TacSiGeOR,"TacSiGeOR/D");
  RootOutput::getInstance()->GetTree()->Branch("TacSiMicroOR",&TacSiMicroOR,"TacSiMicroOR/D");
  RootOutput::getInstance()->GetTree()->Branch("TacSiPlast1",& TacSiPlast1," TacSiPlast1/D");
  RootOutput::getInstance()->GetTree()->Branch("TacSiPlast2",& TacSiPlast2," TacSiPlast2/D");
  
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
