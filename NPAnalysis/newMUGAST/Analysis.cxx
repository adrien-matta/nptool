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
  
  M2  = (TMust2Physics*) m_DetectorManager -> GetDetector("MUST2Array");
  GD = (GaspardTracker*)  m_DetectorManager -> GetDetector("GaspardTracker");
  LightCD2 = EnergyLoss("proton_CD2.G4table","G4Table",100 );
  LightAl = EnergyLoss("proton_Al.G4table","G4Table",100);
  LightSi = EnergyLoss("proton_Si.G4table","G4Table",100);
  BeamCD2 = EnergyLoss("Na24[0.0]_CD2.G4table","G4Table",100);
  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
   TargetThickness = 18*micrometer;
  OriginalBeamEnergy = myReaction->GetBeamEnergy();
   Rand = TRandom3();
   DetectorNumber = 0 ;
   ThetaNormalTarget = 0 ;
   ThetaM2Surface = 0;
   Si_E_M2 = 0 ;
   CsI_E_M2 = 0 ;
   Energy = 0;
   E_M2 = 0;
  
   ThetaGDSurface = 0;
   X_GD = 0 ;
   Y_GD = 0 ;
   Z_GD = 0 ;
   Si_E_GD = 0 ;
   E_GD = 0;
   Si_X_GD = 0;
   Si_Y_GD = 0;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();
  double XTarget = 0;
  double YTarget = 0;
  TVector3 BeamDirection = TVector3(0,0,1);
  double BeamEnergy = BeamCD2.Slow(OriginalBeamEnergy,Rand.Uniform(0,TargetThickness),0);
  myReaction->SetBeamEnergy(BeamEnergy);
  //////////////////////////// LOOP on MUST2 //////////////////
  for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
    /************************************************/
    //Part 0 : Get the usefull Data
    // MUST2
    int TelescopeNumber = M2->TelescopeNumber[countMust2];
    
    /************************************************/
    // Part 1 : Impact Angle
    ThetaM2Surface = 0;
    ThetaNormalTarget = 0;
    TVector3 HitDirection = M2 -> GetPositionOfInteraction(countMust2) - BeamImpact ;
    ThetaLab = HitDirection.Angle( BeamDirection );
    
    ThetaM2Surface = HitDirection.Angle(- M2 -> GetTelescopeNormal(countMust2) );
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    
    /************************************************/
    
    /************************************************/
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Si_E_M2 = M2->Si_E[countMust2];
    CsI_E_M2= M2->CsI_E[countMust2];
    
    // if CsI
    if(CsI_E_M2>0 ){
      // The energy in CsI is calculate form dE/dx Table because
      Energy = CsI_E_M2;
      Energy = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
      Energy+=Si_E_M2;
    }
    
    else
      Energy = Si_E_M2;
    
    // Evaluate energy using the thickness
    ELab = LightAl.EvaluateInitialEnergy( Energy ,0.4*micrometer , ThetaM2Surface);
    // Target Correction
    ELab   = LightCD2.EvaluateInitialEnergy( ELab ,TargetThickness/2., ThetaNormalTarget);
    /************************************************/
    
    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
    ThetaLab=ThetaLab/deg;
    
    /************************************************/
    
    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    /************************************************/
  }//end loop MUST2
  
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on GASPARD //////////////////
  if(GD->GetEnergyDeposit()>0){
    /************************************************/
    // Part 1 : Impact Angle
    ThetaGDSurface = 0;
    ThetaNormalTarget = 0;
    if(XTarget>-1000 && YTarget>-1000){
      TVector3 HitDirection = GD -> GetPositionOfInteraction() - BeamImpact ;
      ThetaLab = HitDirection.Angle( BeamDirection );
      
      ThetaGDSurface = HitDirection.Angle( TVector3(0,0,1) ) ;
      ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    }
    
    else{
      BeamDirection = TVector3(-1000,-1000,-1000);
      ThetaGDSurface    = -1000  ;
      ThetaNormalTarget = -1000  ;
    }
    
    /************************************************/
    
    /************************************************/
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Energy = GD->GetEnergyDeposit();
    // Target Correction
    
   ELab   = LightCD2.EvaluateInitialEnergy( Energy ,TargetThickness/2., ThetaNormalTarget);
   /************************************************/
    
    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
    
    /************************************************/
    
    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;
    
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
NPA::VAnalysis* Analysis::Construct(){
  return (NPA::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPA::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy p;
}

