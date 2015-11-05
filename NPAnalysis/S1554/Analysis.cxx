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

  Sharc = (TSharcPhysics*)  m_DetectorManager -> GetDetector("Sharc");
  Tigress = (TTigressPhysics*)  m_DetectorManager -> GetDetector("Tigress");

  LightCD2 = EnergyLoss("proton_CD2.G4table","G4Table",10);
  LightAl = EnergyLoss("proton_Al.G4table","G4Table",10);

  //  LightAl = EnergyLoss("alpha_Al.G4table","G4Table",10);

  BeamCD2 = EnergyLoss("Si28_CD2.G4table","G4Table",10);
  myReaction = new NPL::Reaction();
  myReaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
//  TargetThickness = m_DetectorManager->GetTargetThickness()*micrometer;
    TargetThickness = 0; 
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
  Si_E_Sharc = 0;
  E_Sharc = 0;
  ThetaDetector = 0;
  BeamDirection = TVector3(0,0,1);
  TargetPosition = TVector3(m_DetectorManager->GetTargetX(),m_DetectorManager->GetTargetY(),m_DetectorManager->GetTargetZ() );
  double finalEnergy = BeamCD2.Slow(224,TargetThickness*0.5,0);
  myReaction->SetBeamEnergy(finalEnergy);
  cout << "Set Beam energy to: " <<  finalEnergy << " MeV" << endl;

  // Load cut for second Pad cal
/*  TFile* file ;            
  file = new TFile("cuts/cut_elgs_det10.root","READ");          
  cut_deuton_d10 = (TCutG*) file->FindObjectAny("cut_elgs_det10"); 
  file = new TFile("cuts/cut_elgs_det12.root","READ");          
  cut_deuton_d12 = (TCutG*) file->FindObjectAny("cut_elgs_det12"); 
  file = new TFile("cuts/Cut_dd.root","READ");          
  cut_dd = (TCutG*) file->FindObjectAny("Cut_dd"); 
  PADFile.open("PADEvent.txt");
*/


  // Load cut for PAD cal
/*    TFile* file ;
      file = new TFile("cuts/deuton_d10.root","READ");
      cut_deuton_d10 = (TCutG*) file->FindObjectAny("deuton_d10");
      file = new TFile("cuts/proton_d10.root","READ");
      cut_proton_d10 = (TCutG*) file->FindObjectAny("proton_d10");
      file = new TFile("cuts/deuton_d12.root","READ");
      cut_deuton_d12 = (TCutG*) file->FindObjectAny("deuton_d12");
      file = new TFile("cuts/proton_d12.root","READ");
      cut_proton_d12 = (TCutG*) file->FindObjectAny("proton_d12");
      PADFile.open("PADEvent.txt");
  */    
  /*  // Load the cut for alignement
      for(unsigned int i = 0 ; i < 8 ; i++){
      TFile* file = new TFile(Form("cuts/Ex5_D%i.root",i+1),"READ");
      cut_ex5.push_back((TCutG*) file->FindObjectAny(Form("Ex5_D%i",i+1))); 
      if(! file->FindObjectAny(Form("Ex5_D%i",i+1))){
      cout << "Fail to Load " << Form("Ex5_D%i",i+1) << endl;
      exit(1);
      }
      }

      box_pos.open("BoxPos.txt");
      qqq_pos.open("QQQPos.txt");
      */
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
  /*      int DetectorNumber = Sharc->DetectorNumber[0];     
          if(DetectorNumber==10 && Sharc->PAD_E[0]>0){
          if(cut_proton_d10->IsInside(Sharc->PAD_E[0],Sharc->Strip_E[0]*cos(ThetaDetector))){
            PADFile << "10 proton " << Sharc->Strip_E[0] << " " << Sharc->PAD_E[0] << " " << ThetaDetector << endl;
          }
          else if(cut_deuton_d10->IsInside(Sharc->PAD_E[0],Sharc->Strip_E[0]*cos(ThetaDetector)))
          PADFile << "10 deuton " << Sharc->Strip_E[0] << " " << Sharc->PAD_E[0] << " " << ThetaDetector << endl;
          }

          if(DetectorNumber==12 && Sharc->PAD_E[0]>0){
          if(cut_proton_d12->IsInside(Sharc->PAD_E[0],Sharc->Strip_E[0]*cos(ThetaDetector)))
          PADFile << "12 proton " << Sharc->Strip_E[0] << " " << Sharc->PAD_E[0] << " " << ThetaDetector << endl;
          else if(cut_deuton_d12->IsInside(Sharc->PAD_E[0], Sharc->Strip_E[0]*cos(ThetaDetector)))
          PADFile << "12 deuton " << Sharc->Strip_E[0] << " " << Sharc->PAD_E[0] << " " << ThetaDetector << endl;
          }
    */      

    /************************************************/
 /*   int DetectorNumber = Sharc->DetectorNumber[0];  
    if(cut_dd->IsInside(Sharc->PAD_E[0],Sharc->Strip_E[0]*cos(ThetaDetector))){
      if(DetectorNumber == 10){
        if(cut_deuton_d10->IsInside(ThetaLab/deg,ELab)){
          double a = 9.23351e-13 ;
          double b = 8.35809e-06 ;
          double c = 5.77842e-01-Sharc->PAD_E[0];
          double delta =  b*b-4*a*c;
          double RawE =   (-b + sqrt(delta))/(2*a);
          PADFile << "10 deuton " << Sharc->Strip_E[0] << " " << RawE << " " << ThetaLab << " " << ThetaNormalTarget << endl;
        }
      }

      else if(DetectorNumber == 12){
        if(cut_deuton_d12->IsInside(ThetaLab/deg,ELab)){
          double a = 1.63514e-12;
          double b = 7.52050e-06;
          double c = 5.93687e-01-Sharc->PAD_E[0];
          double delta =  b*b-4*a*c;
          double RawE =   (-b + sqrt(delta))/(2*a);
          PADFile << "12 deuton " << Sharc->Strip_E[0] << " " << RawE<< " " << ThetaLab << " " << ThetaNormalTarget << endl;
        }
      }

    }*/

    /************************************************/
    /*    int DetectorNumber = Sharc->DetectorNumber[0];
          bool checkT = false;
          if(Tigress->AddBack_DC.size()>0){
          if(Tigress->AddBack_DC[0]/1000.> 4.6 && Tigress->AddBack_DC[0]/1000.< 5.4)
          checkT=true;
          }

          if(checkT){
          if(DetectorNumber<5){
          if(cut_ex5[DetectorNumber-1]->IsInside(ThetaLab/deg,ELab))
          qqq_pos << DetectorNumber << " " << Energy << " " << HitDirection.X() << " " << HitDirection.Y() << " " << HitDirection.Z() << endl;
          }

          else if(DetectorNumber<9){
          if(cut_ex5[DetectorNumber-1]->IsInside(ThetaLab/deg,ELab))
          box_pos << DetectorNumber << " " << Energy << " " << HitDirection.X() << " " << HitDirection.Y() << " " << HitDirection.Z() << endl;
          }
          }
          */
    /************************************************/

    /************************************************/
    // Part 3 : Excitation Energy Calculation
    Ex = myReaction -> ReconstructRelativistic( ELab , ThetaLab );
    /************************************************/

    /************************************************/
    // Part 4 : Theta CM Calculation
    ThetaCM  = myReaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    ThetaLab=ThetaLab/deg;
 /*   if(ThetaLab>140)
      ThetaLab=Rand.Uniform(ThetaLab-0.4,ThetaLab+0.4);*/
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

  RootOutput::getInstance()->GetTree()->Branch("RunNumber",&RunNumber,"RunNumber/I");
  RootOutput::getInstance()->GetTree()->Branch("RunNumberMinor",&RunNumberMinor,"RunNumberMinor/I");
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

