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
#include "NPFunction.h"
#include"TF1.h"
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
  Trifoil = (TPlasticPhysics*)  m_DetectorManager -> GetDetector("Plastic");

  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());

  // target thickness
  TargetThickness = m_DetectorManager->GetTargetThickness();
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();

  // energy losses
  string light=NPL::ChangeNameToG4Standard(myReaction->GetNucleus3().GetName());
  string beam=NPL::ChangeNameToG4Standard(myReaction->GetNucleus1().GetName());

  LightCD2 = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
  LightAl = NPL::EnergyLoss(light+"_Al.G4table","G4Table",100);
  LightSi = NPL::EnergyLoss(light+"_Si.G4table","G4Table",100);
  BeamCD2 = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);

  OriginalBeamEnergy = myReaction->GetBeamEnergy();
  Rand = TRandom3();
  DetectorNumber = 0 ;
  ThetaNormalTarget = 0 ;
  Energy = 0;

  RunNumber = 0;
  RunNumberMinor=0;

  ThetaSharcSurface = 0;
  X_Sharc = 0 ;
  Y_Sharc = 0 ;
  Z_Sharc = 0 ;
  X_Trifoil = 0;
  Y_Trifoil = 0 ;

  Si_E_Sharc = 0 ;
  E_Sharc = 0;
  ThetaDetector = 0   ;
  BeamDirection = TVector3(0,0,1);
  // S1554
    TargetPosition = TVector3(0.1635909,0.910980,m_DetectorManager->GetTargetZ() );
  // S1107
  //TargetPosition = TVector3(0.0808323,0.177073,m_DetectorManager->GetTargetZ() );
  double finalEnergy = BeamCD2.Slow(myReaction->GetBeamEnergy(),TargetThickness*0.5,0);
  myReaction->SetBeamEnergy(finalEnergy);
  cout << "Set Beam energy to: " <<  finalEnergy << " MeV" << endl;

  myInit = new TInitialConditions();
  RootInput::getInstance()->GetChain()->SetBranchAddress("InitialConditions",&myInit);

  HistoFile = new TFile("SharcEfficiency.root","RECREATE");
  ThetaCM_emmitted = new TH1F("ThetaCM_emmitted","ThetaCM_emmitted",180,0,180);
  ThetaCM_detected = new TH1F("ThetaCM_detected","ThetaCM_detected",180,0,180);
  ThetaLab_emmitted = new TH1F("ThetaLab_emmitted","ThetaLab_emmitted",180,0,180);
  ThetaLab_detected = new TH1F("ThetaLab_detected","ThetaLab_detected",180,0,180);
/*
  ThetaCM_emmitted_2D = new TH2F("ThetaCM_emmitted_2D","ThetaCM_emmitted_2D",72,0,180,400,-8,8);
  ThetaCM_detected_2D = new TH2F("ThetaCM_detected_2D","ThetaCM_detected_2D",72,0,180,400,-8,8);
  ThetaLab_emmitted_2D = new TH2F("ThetaLab_emmitted_2D","ThetaLab_emmitted_2D",72,0,180,400,-8,8);
  ThetaLab_detected_2D = new TH2F("ThetaLab_detected_2D","ThetaLab_detected_2D",72,0,180,400,-8,8);
*/
  ThetaCM_emmitted_2D = new TH2F("ThetaCM_emmitted_2D","ThetaCM_emmitted_2D",180,0,180,1100,-1,10);
  ThetaCM_detected_2D = new TH2F("ThetaCM_detected_2D","ThetaCM_detected_2D",180,0,180,1100,-1,10);
  ThetaLab_emmitted_2D = new TH2F("ThetaLab_emmitted_2D","ThetaLab_emmitted_2D",180,0,180,1100,-1,10);
  ThetaLab_detected_2D = new TH2F("ThetaLab_detected_2D","ThetaLab_detected_2D",180,0,180,1100,-1,10);

  Kine_2D = new TH2F("Kine_2D","Kine_2D",1800,0,180,600,0,60);

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on Sharc//////////////////
  // Fill Initial condition Histo
  ThetaCM_emmitted->Fill(myInit->GetThetaCM(0));
  ThetaLab_emmitted->Fill(myInit->GetThetaLab_WorldFrame(0));

  myReaction->SetBeamEnergy(myInit->GetIncidentInitialKineticEnergy());
  double EXD = myReaction->ReconstructRelativistic(myInit->GetKineticEnergy(0),myInit->GetThetaLab_IncidentFrame(0)*deg);
  ThetaCM_emmitted_2D->Fill(myInit->GetThetaCM(0),EXD);
  ThetaLab_emmitted_2D->Fill(myInit->GetThetaLab_WorldFrame(0),EXD);

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
    ELab = Energy;
    ELab = LightCD2.EvaluateInitialEnergy( Energy ,TargetThickness*0.5, ThetaNormalTarget);
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

    /* 
       if(Trifoil->Energy.size()>0){
       if( abs(Ex-EXD)<0.5 && Trifoil->Energy[0]>0 && Sharc->Strip_E[0]>0.8) {

       ThetaCM_detected->Fill(myInit->GetThetaCM(0));
       ThetaLab_detected->Fill(myInit->GetThetaLab_WorldFrame(0));
       ThetaCM_detected_2D->Fill(myInit->GetThetaCM(0),Ex);
       ThetaLab_detected_2D->Fill(myInit->GetThetaLab_WorldFrame(0),Ex);
       }
       }
       */

    // S1107
    bool check = false;
/*    if(Sharc->DetectorNumber[0] == 1 && Sharc->Strip_E[0]>1.83)
      check = true;
    
    else if(Sharc->DetectorNumber[0] == 2 && Sharc->Strip_E[0]>1.88)
      check = true;
   
    else if(Sharc->DetectorNumber[0] == 3 && Sharc->Strip_E[0]>1.74)
      check = true;

    else if(Sharc->DetectorNumber[0] == 4 && Sharc->Strip_E[0]>1.)
      check = true;

    else if(Sharc->DetectorNumber[0] == 5 && Sharc->Strip_E[0]>2.10)
      check = true;

    else if(Sharc->DetectorNumber[0] == 6 && Sharc->Strip_E[0]>2.16)
      check = true;

    else if(Sharc->DetectorNumber[0] == 7 && Sharc->Strip_E[0]>2.06)
      check = true;

    else if(Sharc->DetectorNumber[0] == 8 && Sharc->Strip_E[0]>2.09)
      check = true;

    else if(Sharc->DetectorNumber[0] == 9 && Sharc->Strip_E[0]>1.97)
      check = true;

    else if(Sharc->DetectorNumber[0] == 10 && Sharc->Strip_E[0]>2.03)
      check = true;

    else if(Sharc->DetectorNumber[0] == 12 && Sharc->Strip_E[0]>1.99)
      check = true;

   if(Sharc->DetectorNumber[0] == 4 && Sharc->Strip_E[0]>1.)
      check = true;
*/
check =true;
    if(Trifoil->Energy.size()>0){
      if( abs(Ex-EXD)<0.5 &&  Trifoil->Energy.size()>0 && Trifoil->Energy[0]>0) { 
        ThetaCM_detected->Fill(myInit->GetThetaCM(0));
        ThetaLab_detected->Fill(myInit->GetThetaLab_WorldFrame(0));
        ThetaCM_detected_2D->Fill(myInit->GetThetaCM(0),EXD);
        ThetaLab_detected_2D->Fill(myInit->GetThetaLab_WorldFrame(0),EXD);
        Kine_2D->Fill(ThetaLab,ELab);

      }
    }
    /************************************************/
  }//end loop Sharc 
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
  Efficiency_CM = new TH1F(*ThetaCM_detected); 
  Efficiency_CM->SetName("EfficiencyCM");
  Efficiency_CM->SetTitle("EfficiencyCM");
  Efficiency_CM->Sumw2();

  Efficiency_Lab = new TH1F(*ThetaLab_detected); 
  Efficiency_Lab->SetName("EfficiencyLab");
  Efficiency_Lab->SetTitle("EfficiencyLab");
  Efficiency_Lab->Sumw2();

  Efficiency_CM_2D = new TH2F(*ThetaCM_detected_2D); 
  Efficiency_CM_2D->SetName("EfficiencyCM_2D");
  Efficiency_CM_2D->SetTitle("EfficiencyCM_2D");
  Efficiency_CM_2D->Sumw2();

  Efficiency_Lab_2D = new TH2F(*ThetaLab_detected_2D); 
  Efficiency_Lab_2D->SetName("EfficiencyLab_2D");
  Efficiency_Lab_2D->SetTitle("EfficiencyLab_2D");
  Efficiency_Lab_2D->Sumw2();


  ThetaCM_detected->Sumw2();
  ThetaLab_detected->Sumw2();

  SolidAngle_CM = new TH1F(*ThetaCM_detected); 
  SolidAngle_CM->SetName("SolidAngleCM");
  SolidAngle_CM->SetTitle("SolidAngleCM");

  SolidAngle_Lab = new TH1F(*ThetaLab_detected); 
  SolidAngle_Lab->SetName("SolidAngleLab");
  SolidAngle_Lab->SetTitle("SolidAngleLab");

  SolidAngle_CM_2D = new TH2F(*ThetaCM_detected_2D); 
  SolidAngle_CM_2D->SetName("SolidAngleCM_2D");
  SolidAngle_CM_2D->SetTitle("SolidAngleCM_2D");
  SolidAngle_CM_2D->Sumw2();

  SolidAngle_Lab_2D = new TH2F(*ThetaLab_detected_2D); 
  SolidAngle_Lab_2D->SetName("SolidAngleLab_2D");
  SolidAngle_Lab_2D->SetTitle("SolidAngleLab_2D");
  SolidAngle_Lab_2D->Sumw2();  

  Efficiency_CM->Divide(ThetaCM_emmitted);
  Efficiency_Lab->Divide(ThetaLab_emmitted);

  Efficiency_CM_2D->Divide(ThetaCM_emmitted_2D);
  Efficiency_Lab_2D->Divide(ThetaLab_emmitted_2D);

  double dt = 180./Efficiency_Lab->GetNbinsX();
  cout << "Angular infinitesimal = " << dt << "deg " << endl;
  TF1* C = new TF1("C",Form("1./(2*%f*sin(x*%f/180.)*%f*%f/180.)",M_PI,M_PI,dt,M_PI),0,180);


  SolidAngle_CM->Divide(ThetaCM_emmitted);
  SolidAngle_CM->Divide(C,1);

  SolidAngle_Lab->Divide(ThetaLab_emmitted);
  SolidAngle_Lab->Divide(C,1);

  SolidAngle_CM_2D->Divide(ThetaCM_emmitted_2D);
  SolidAngle_CM_2D->Divide(C,1);

  SolidAngle_Lab_2D->Divide(ThetaLab_emmitted_2D);
  SolidAngle_Lab_2D->Divide(C,1);


  HistoFile->Write();
  HistoFile->Close();
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch(){
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  RootOutput::getInstance()->GetTree()->Branch("ThetaDetector",&ThetaDetector,"ThetaDetector/D");
  RootOutput::getInstance()->GetTree()->Branch("X_Sharc",&X_Sharc,"X_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("Y_Sharc",&Y_Sharc,"Y_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("Z_Sharc",&Z_Sharc,"Z_Sharc/D");
  RootOutput::getInstance()->GetTree()->Branch("X_Trifoil",&X_Trifoil,"X_Trifoil/D");
  RootOutput::getInstance()->GetTree()->Branch("Y_Trifoil",&Y_Trifoil,"Y_Trifoil/D");


}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput::getInstance()->GetChain()->SetBranchAddress("RunNumber",&RunNumber);
  RootInput::getInstance()->GetChain()->SetBranchAddress("RunNumberMinor",&RunNumberMinor);
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  Ex = -1000 ;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  ThetaDetector=-1000;
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

