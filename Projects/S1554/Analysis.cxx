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

  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());

  string ligth,heavy;
  if(myReaction->GetNucleus3().GetZ()==1 && myReaction->GetNucleus3().GetA()==1)
    ligth = "proton";
  else
    ligth = "deuteron";

  if(myReaction->GetNucleus4().GetZ()==12)
    heavy = "Mg28";
  else
    heavy = "Si28";


  Sharc = (TSharcPhysics*)  m_DetectorManager -> GetDetector("Sharc");
  //Tigress = (TTigressPhysics*)  m_DetectorManager -> GetDetector("Tigress");

  LightCD2 = EnergyLoss(ligth+"_CD2.G4table","G4Table",10);
  LightAl = EnergyLoss(ligth+"_Al.G4table","G4Table",10);

  //  LightAl = EnergyLoss("alpha_Al.G4table","G4Table",10);

  BeamCD2 = EnergyLoss(heavy+"_CD2.G4table","G4Table",10);
  TargetThickness = m_DetectorManager->GetTargetThickness();
  OriginalBeamEnergy = myReaction->GetBeamEnergy();
  Rand = TRandom3();
  DetectorNumber = 0 ;
  ThetaNormalTarget = 0 ;
  Energy = 0;

  RunNumber = 0;
  RunNumberMinor=0;

  ThetaSharcSurface = 0;
  X_Sharc = 0;
  Y_Sharc = 0;
  Z_Sharc = 0;
  X_Trifoil = 0;
  Y_Trifoil = 0 ;
  
  Si_E_Sharc = 0;
  E_Sharc = 0;
  ThetaDetector = 0;
  BeamDirection = TVector3(0,0,1);
  TargetPosition = TVector3(m_DetectorManager->GetTargetX(),m_DetectorManager->GetTargetY(),m_DetectorManager->GetTargetZ() );
  double finalEnergy = BeamCD2.Slow(224,TargetThickness*0.5,0);
  myReaction->SetBeamEnergy(finalEnergy);
  cout << "Set Beam energy to: " <<  finalEnergy << " MeV" << endl;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on Sharc//////////////////

  if(Sharc->Strip_E.size()==1){
    /************************************************/
    // Part 1 : Impact Angle
    TVector3 HitDirection = Sharc -> GetPositionOfInteraction(0,true)-TargetPosition;
    X_Sharc = Sharc -> GetPositionOfInteraction(0,true).X();
    Y_Sharc = Sharc -> GetPositionOfInteraction(0,true).Y();
    Z_Sharc = Sharc -> GetPositionOfInteraction(0,true).Z();
 
    ThetaLab = HitDirection.Angle( BeamDirection );
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    ThetaDetector = HitDirection.Angle(-Sharc->GetDetectorNormal(0));
    Distance = HitDirection.Mag();
    /************************************************/

    /************************************************/
    // Part 2 : Impact Energy
    Energy = ELab = 0;
    if(Sharc->PAD_E[0]>0 && Sharc->PAD_E[0] <100){
      Energy = Sharc->PAD_E[0];
    }
    if(Sharc->Strip_E[0]>0)
      Energy += Sharc->Strip_E[0];

    Energy =  LightAl.EvaluateInitialEnergy(Energy,Sharc->GetDeadLayer(0)*micrometer,0);

    // Target Correction
    ELab = LightCD2.EvaluateInitialEnergy( Energy ,TargetThickness*0.5, ThetaNormalTarget);

    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );

    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    
    /************************************************/
    // Part 5: Compute the X and Y coordinate of the Heavy in the Trifoil plan
    myReaction->SetThetaCM(ThetaCM*deg);
    myReaction->SetExcitationHeavy(Ex);

    double d1,d2,d3,t;  
    myReaction->KineRelativistic(d1,d2,t,d3);
    double x = tan(t)*400;
    TVector3 P;
    P.SetXYZ(x,0,400);
    P.RotateZ(HitDirection.Phi()+3.14159);
    X_Trifoil = P.X();
    Y_Trifoil = P.Y();
   
    myReaction->SetThetaCM(0);
    myReaction->SetExcitationHeavy(0);
  
    
    /************************************************/
    // Unit conversion
    ThetaLab=ThetaLab/deg;
    /************************************************/
  }//end loop Sharc 
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch(){
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaDetector",&ThetaDetector,"ThetaDetector/D");
  RootOutput::getInstance()->GetTree()->Branch("Distance",&Distance,"Distance/D");
  RootOutput::getInstance()->GetTree()->Branch("X_Sharc",&X_Sharc,"X_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("Y_Sharc",&Y_Sharc,"Y_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("Z_Sharc",&Z_Sharc,"Z_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("X_Trifoil",&X_Trifoil,"X_Trifoil/D");
  RootOutput::getInstance()->GetTree()->Branch("Y_Trifoil",&Y_Trifoil,"Y_Trifoil/D");

  RootOutput::getInstance()->GetTree()->Branch("RunNumber",&RunNumber,"RunNumber/I");
  RootOutput::getInstance()->GetTree()->Branch("RunNumberMinor",&RunNumberMinor,"RunNumberMinor/I");
  RootOutput::getInstance()->GetTree()->Branch("Run",&Run,"Run/I");

}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput::getInstance()->GetChain()->SetBranchAddress("RunNumber",&RunNumber);
  RootInput::getInstance()->GetChain()->SetBranchAddress("RunNumberMinor",&RunNumberMinor);
  RootInput::getInstance()->GetChain()->SetBranchAddress("Run",&Run);

}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  ThetaDetector=-1000;
  X_Sharc = -1000;
  Y_Sharc = -1000;
  Z_Sharc = -1000;
  X_Trifoil = -1000;
  Y_Trifoil = -1000;
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

