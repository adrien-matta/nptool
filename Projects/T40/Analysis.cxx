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
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class describing the property of an Analysis object                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include<iostream>
using namespace std;
#include"Analysis.h"
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"NPFunction.h" 
#include"RootOutput.h"
#include"RootInput.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
  TH  = (TTiaraHyballPhysics*) m_DetectorManager -> GetDetector("TiaraHyballWedge");
  TB  = (TTiaraBarrelPhysics*) m_DetectorManager -> GetDetector("TiaraInnerBarrel=");

  // get reaction information
  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = myReaction->GetBeamEnergy();

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

  XTarget = 0;
  YTarget =0;
  BeamDirection = TVector3(0,0,1);
  InitOutputBranch();
  InitInputBranch();

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();


  ////////////////////////////////////////// LOOP on TiaraHyball + SSSD Hit //////////////////////////////////////////
  for(unsigned int countTiaraHyball = 0 ; countTiaraHyball < TH->Strip_E.size() ; countTiaraHyball++){
    /************************************************/

    // TiaraHyball

    /************************************************/

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

    /************************************************/

    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Si_E_TH = TH->Strip_E[countTiaraHyball];
    Energy = Si_E_TH;

    // Evaluate energy using the thickness 
//    ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaTHSurface); 
    ELab = Energy;
    // Target Correction
    ELab = LightTarget.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget); 

    /************************************************/

    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );

    /************************************************/

    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

    /************************************************/

    // Part 5 : Implementing impact matrix for the entire Hyball (all 6 wedges)
    /*TVector3 HyballImpactPosition = TH -> GetPositionOfInteraction(countTiaraHyball);
      HyballIMX = HyballImpactPosition.X();
      HyballIMY = HyballImpactPosition.Y();
      HyballIMZ = HyballImpactPosition.Z();*/

    /************************************************/

    // Part 6 : Implementing randomised position impact matrix for the Hyball
    TVector3 HyballRandomImpactPosition = TH -> GetRandomisedPositionOfInteraction(countTiaraHyball);
    TiaraIMX = HyballRandomImpactPosition.X();
    TiaraIMY = HyballRandomImpactPosition.Y();
    TiaraIMZ = HyballRandomImpactPosition.Z();

    /************************************************/

  } // end loop TiaraHyball

  /////////////////////////// LOOP on TiaraBarrel /////////////////////////////
  for(unsigned int countTiaraBarrel = 0 ; countTiaraBarrel < TB->Strip_E.size() ; countTiaraBarrel++){
    /************************************************/
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
    /************************************************/

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

    /************************************************/

    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );

    /************************************************/

    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;

    /************************************************/      

    // Part 5 : Implementing impact matrix for the Tiara Barrel (all 8 detecting strips)
    /*TVector3 BarrelImpactPosition = TB -> GetPositionOfInteraction(countTiaraBarrel);
      BarrelIMX = BarrelImpactPosition.X();
      BarrelIMY = BarrelImpactPosition.Y();
      BarrelIMZ = BarrelImpactPosition.Z();*/

    /************************************************/

    // Part 6 : Implementing randomised position impact matrix for both the entire Barrel (all 8 strips) and each strip making up the octagonal Barrel individually
    TVector3 BarrelRandomImpactPosition = TB -> GetRandomisedPositionOfInteraction(countTiaraBarrel);
    TiaraIMX = BarrelRandomImpactPosition.X();
    TiaraIMY = BarrelRandomImpactPosition.Y();
    TiaraIMZ = BarrelRandomImpactPosition.Z();

    /************************************************/

  } // end loop TiaraBarrel
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}


void Analysis::ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
}
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixX",&TiaraIMX,"TiaraImpactMatrixX/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixY",&TiaraIMY,"TiaraImpactMatrixY/D");
  RootOutput::getInstance()->GetTree()->Branch("TiaraImpactMatrixZ",&TiaraIMZ,"TiaraImpactMatrixZ/D");
}
/////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
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

