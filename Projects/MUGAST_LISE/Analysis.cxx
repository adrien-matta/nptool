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

#include "Analysis.h"
#include "NPFunction.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init() {
  // initialize input and output branches
  InitOutputBranch();
  InitInputBranch();
  myInit = new TInitialConditions();
  Coord = new TInteractionCoordinates();
  GD = (GaspardTracker*) m_DetectorManager -> GetDetector("GaspardTracker");

  // get reaction information
  myReaction.ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = myReaction.GetBeamEnergy();

  // target thickness
  TargetThickness = m_DetectorManager->GetTargetThickness();
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();
  // Cryo target case
  WindowsThickness = m_DetectorManager->GetWindowsThickness(); 
  string WindowsMaterial = m_DetectorManager->GetWindowsMaterial();

  // energy losses
  string light=NPL::ChangeNameToG4Standard(myReaction.GetNucleus3().GetName());
  string beam=NPL::ChangeNameToG4Standard(myReaction.GetNucleus1().GetName());

  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
  LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
  LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);
  BeamCD2 = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);

  if(WindowsThickness){
    BeamWindow= new NPL::EnergyLoss(beam+"_"+WindowsMaterial+".G4table","G4Table",100); 
    LightWindow=  new NPL::EnergyLoss(light+"_"+WindowsMaterial+".G4table","G4Table",100);  
  }

  else{
    BeamWindow= NULL;
    LightWindow=NULL;
  }

  // initialize various parameters
  Rand = TRandom3();
  DetectorNumber = 0;
  ThetaNormalTarget = 0;
  ThetaM2Surface = 0;
  Si_E_M2 = 0;
  CsI_E_M2 = 0;
  Energy = 0;
  ThetaGDSurface = 0;
  X = 0;
  Y = 0;
  Z = 0;
  dE = 0;
  dTheta=0;
  BeamDirection = TVector3(0,0,1);

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent() {
  // Reinitiate calculated variable
  ReInitValue();
  //double zImpact = Rand.Uniform(-TargetThickness*0.5,TargetThickness*0.5);
  double zImpact = 0 ;
  BeamImpact = TVector3(0,0,zImpact); 
  // determine beam energy for a randomized interaction point in target
  // 1% FWHM randominastion (E/100)/2.35
  myReaction.SetBeamEnergy(Rand.Gaus(myInit->GetIncidentFinalKineticEnergy(),myInit->GetIncidentFinalKineticEnergy()/235));
  OriginalThetaLab = myInit->GetThetaLab_WorldFrame(0);
  OriginalELab = myInit->GetKineticEnergy(0);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on GASPARD //////////////////
  if(GD->GetEnergyDeposit()>0){
    /************************************************/
    // Part 1 : Impact Angle
    ThetaGDSurface = 0;
    ThetaNormalTarget = 0;
    TVector3 HitDirection = GD -> GetPositionOfInteraction() - BeamImpact ;
    ThetaLab = HitDirection.Angle( BeamDirection );

    X =  GD -> GetPositionOfInteraction().X();
    Y =  GD -> GetPositionOfInteraction().Y();
    Z =  GD -> GetPositionOfInteraction().Z();
    cout << " -- " << endl;
     cout << X-Coord->GetDetectedPositionX(0) << " " << Y-Coord->GetDetectedPositionY(0) << " " << Z << endl;
    ThetaGDSurface = HitDirection.Angle( TVector3(0,0,1) ) ;
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;

    /************************************************/

    /************************************************/
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    Energy = GD->GetEnergyDeposit();
    // Target Correction
    ELab   = LightTarget.EvaluateInitialEnergy( Energy ,TargetThickness*0.5-zImpact, ThetaNormalTarget);

    if(LightWindow)
      ELab = LightWindow->EvaluateInitialEnergy( ELab ,WindowsThickness, ThetaNormalTarget);
    
      dE= myInit->GetKineticEnergy(0) - ELab ;
      dTheta = (myInit->GetThetaLab_WorldFrame(0)*deg-ThetaLab)/deg ;
    /************************************************/

    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction.ReconstructRelativistic( ELab , ThetaLab );

    /************************************************/

    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction.EnergyLabToThetaCM( ELab , ThetaLab)/deg;
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
  RootOutput::getInstance()->GetTree()->Branch("Run",&Run,"Run/I");
  RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D");
  RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D");
  RootOutput::getInstance()->GetTree()->Branch("Z",&Z,"Z/D");
  RootOutput::getInstance()->GetTree()->Branch("dE",&dE,"dE/D");
  RootOutput::getInstance()->GetTree()->Branch("OriginalThetaLab",&OriginalThetaLab,"OriginalThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("OriginalELab",&OriginalELab,"OriginalELab/D");


  RootOutput::getInstance()->GetTree()->Branch("dTheta",&dTheta,"dTheta/D");
}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput::getInstance()->GetChain()->SetBranchStatus("Run",true);
  RootInput::getInstance()->GetChain()->SetBranchAddress("Run",&Run);
  RootInput::getInstance()->GetChain()->SetBranchStatus("InitialConditions",true);
  RootInput::getInstance()->GetChain()->SetBranchAddress("InitialConditions",&myInit);
  RootInput::getInstance()->GetChain()->SetBranchAddress("InteractionCoordinates",&Coord);

}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  X = -1000;
  Y = -1000;
  Z = -1000;
  dE= -1000;
  dTheta= -1000;
  OriginalThetaLab = -1000;
  OriginalELab = -1000;
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
class proxy_analysis{
  public:
    proxy_analysis(){
      NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
    }
};

proxy_analysis p_analysis;
}

