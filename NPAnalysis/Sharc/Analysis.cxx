/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : march 2025                                               *
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
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
  InitOutputBranch();
  InitInputBranch();
  
  Sharc = (TSharcPhysics*)  m_DetectorManager -> GetDetector("Sharc");
  LightCD2 = EnergyLoss("proton_CD2.G4table","G4Table",100 );
  LightAl = EnergyLoss("proton_Al.G4table","G4Table",100);
  LightSi = EnergyLoss("proton_Si.G4table","G4Table",100);
  BeamCD2 = EnergyLoss("Rb88_CD2.G4table","G4Table",100);
  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
   TargetThickness = m_DetectorManager->GetTargetThickness()*micrometer;
  OriginalBeamEnergy = myReaction->GetBeamEnergy();
   Rand = TRandom3();
   DetectorNumber = 0 ;
   ThetaNormalTarget = 0 ;
   ThetaM2Surface = 0;
   Si_E_M2 = 0 ;
   CsI_E_M2 = 0 ;
   Energy = 0;
   E_M2 = 0;
  
   ThetaSharcSurface = 0;
   X_Sharc = 0 ;
   Y_Sharc = 0 ;
   Z_Sharc = 0 ;
   Si_E_Sharc = 0 ;
   E_Sharc = 0;
   Si_X_Sharc = 0;
   Si_Y_Sharc = 0;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();
  double XTarget = 0;
  double YTarget = 0;
  TVector3 BeamDirection = TVector3(0,0,1);
  double BeamEnergy = BeamCD2.Slow(OriginalBeamEnergy,TargetThickness*0.5,0);
  myReaction->SetBeamEnergy(BeamEnergy);
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on Sharc//////////////////
  if(Sharc->Strip_E.size()>0){
    /************************************************/
    // Part 1 : Impact Angle
    ThetaSharcSurface = 0;
    ThetaNormalTarget = 0;
    if(XTarget>-1000 && YTarget>-1000){
      TVector3 HitDirection = Sharc -> GetPositionOfInteraction(0) - BeamImpact ;
      ThetaLab = HitDirection.Angle( BeamDirection );
      
      ThetaSharcSurface = HitDirection.Angle( TVector3(0,0,1) ) ;
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    }
    
    else{
      BeamDirection = TVector3(-1000,-1000,-1000);
      ThetaSharcSurface    = -1000  ;
      ThetaNormalTarget = -1000  ;
    }
    
    /************************************************/
    
    /************************************************/
    // Part 2 : Impact Energy

    Energy = ELab = 0;
    if(Sharc->PAD_E[0]>0){
      Energy = Sharc->PAD_E[0];
    }

    Energy += Sharc->Strip_E[0];
    // Target Correction
    
   ELab   = LightCD2.EvaluateInitialEnergy( Energy ,TargetThickness*0.5, ThetaNormalTarget);
   /************************************************/
    
    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
    
    /************************************************/
    
    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;
    ThetaLab=Rand.Uniform(ThetaLab-0.5,ThetaLab+0.5);
    /************************************************/
  }//end loop GASPARD

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the AnalysisFactory              //
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

