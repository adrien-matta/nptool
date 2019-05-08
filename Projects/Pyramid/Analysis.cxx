/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Moukaddam                                             *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : 23 Nov 2018                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Pyramid analysis project                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

//c++
#include<iostream>
using namespace std;

//Nptool
#include "Analysis.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "NPFunction.h"
#include "NPNucleus.h"

bool reaction = true;  // false = source
string beam; // irrelevant in case of alpha
string TargetMaterial;
double BeamEnergy;
double BeamContEnergy;

////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){

	//stage specific
	EventsFile = TFile::Open("EventsFile.root","RECREATE");
	EventsFile->cd();
	// Create a TTree
  EventsTree = new TTree("EventsTree","Tree With Simulated Data");
  EventsTree->Branch("Detector_N",&Detector_N);
  //EventsTree->Branch("Strip_E",&Strip_E);
  //EventsTree->Branch("Strip_T",&Strip_T);
  EventsTree->Branch("Strip_N",&Strip_N);
  EventsTree->Branch("DownStream_E",&DownStream_E);
  EventsTree->Branch("DownStream_T",&DownStream_T);
  EventsTree->Branch("UpStream_E",&UpStream_E);
  EventsTree->Branch("UpStream_T",&UpStream_T);
  //Original events
  EventsTree->Branch("OriginalVertexX",&vertexX,"OriginalVertexX/D");
  EventsTree->Branch("OriginalVertexY",&vertexY,"OriginalVertexY/D");
  EventsTree->Branch("OriginalVertexZ",&vertexZ,"OriginalVertexZ/D");
  EventsTree->Branch("OriginalThetaCM",&thetaCM,"OriginalThetaCM/D");
  EventsTree->Branch("OriginalThetaLab_BF_0",&thetaLab_BF_0,"OriginalThetaLab_BF_0/D");
  EventsTree->Branch("OriginalThetaLab_WF_0",&thetaLab_WF_0,"OriginalThetaLab_WF_0/D");
  EventsTree->Branch("OriginalEnergy_0",&energy_0,"OriginalEnergy_0/D");
  EventsTree->Branch("OriginalThetaLab_BF_1",&thetaLab_BF_1,"OriginalThetaLab_BF_1/D");
  EventsTree->Branch("OriginalThetaLab_WF_1",&thetaLab_WF_1,"OriginalThetaLab_WF_1/D");
  EventsTree->Branch("OriginalEnergy_1",&energy_1,"OriginalEnergy_1/D");
  //
  EventsTree->Branch("OriginalDetected_X_det",&X_det);
  EventsTree->Branch("OriginalDetected_Y_det",&Y_det);
  EventsTree->Branch("OriginalDetected_Z_det",&Z_det);
  EventsTree->Branch("OriginalDetected_Theta_det",&Theta_det);
  EventsTree->Branch("OriginalDetected_Phi_det",&Phi_det);

  //Connect 
  initCond = new TInitialConditions();
  interCoord = new TInteractionCoordinates();
  reacCond = new TReactionConditions();
  RootInput::getInstance()->GetChain()->SetBranchAddress("InitialConditions",&initCond);
  RootInput::getInstance()->GetChain()->SetBranchAddress("ReactionConditions",&reacCond);
  RootInput::getInstance()->GetChain()->SetBranchAddress("InteractionCoordinates",&interCoord);


  //Analysis
	//InitOutputBranch();
  //InitInputBranch();

if(reaction){
    Reaction = new NPL::Reaction();
    Reaction->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());

    TargetThickness = m_DetectorManager->GetTargetThickness();
    TargetMaterial = m_DetectorManager->GetTargetMaterial();
    
    /*
    NPL::Reaction* Reaction;          // 12C(12C,12C)12C
    NPL::Reaction* ReactionCont;      // 12C(12C,4He)20Ne (check)
    NPL::Reaction* ReactionElast;     // 12C(12C,12C)12C  Mott Scatt.
    */
    
    //energies will be set later on 
    ReactionElast= new NPL::Reaction("23Ne(d,d)23Ne@1"); // check 
    ReactionCont= new NPL::Reaction("12C(12C,4He)20Ne@1");

    // set beam 
    beam=NPL::ChangeNameToG4Standard(Reaction->GetNucleus1().GetName());
    OriginalBeamEnergy = Reaction->GetBeamEnergy();

    BeamTarget = NPL::EnergyLoss(beam+"_"+TargetMaterial+".SRIM","SRIM",20);
    BeamEnergy = BeamTarget.Slow(OriginalBeamEnergy,TargetThickness*0.5,0);
    Reaction->SetBeamEnergy(BeamEnergy);
    ReactionElast->SetBeamEnergy(BeamEnergy);
    cout << "\nSet Beam energy to: " <<  BeamEnergy << " MeV\n";

    //inspect all beams
    Reaction->Print();
    ReactionElast->Print();
    ReactionCont->Print();

    // contaminant
    alphaTarget = NPL::EnergyLoss("He4_"+TargetMaterial+".SRIM","SRIM",20 );
    alphaAl     = NPL::EnergyLoss("He4_Al.SRIM","SRIM",20);
    alphaSi     = NPL::EnergyLoss("He4_Si.SRIM","SRIM",20);
    
    // interest
    CarbonTarget = NPL::EnergyLoss("C12_"+TargetMaterial+".SRIM","SRIM",20 );
    CarbonAl     = NPL::EnergyLoss("C12_Al.SRIM","SRIM",20);
    CarbonSi     = NPL::EnergyLoss("C12_Si.SRIM","SRIM",20);

  }
  else { // Source simulated data 
    TargetThickness = 0;
    TargetMaterial = "Al"; // always check normally it's a gold Layer
    LightTarget = NPL::EnergyLoss("He4_"+TargetMaterial+".SRIM","SRIM",20 );
    LightAl = NPL::EnergyLoss("He4_Al.SRIM","SRIM",20);
    //  LightAl = EnergyLoss("alpha_Al.G4table","G4Table",10); 
  }

   Pyramid= (TPyramidPhysics*) m_DetectorManager->GetDetector("Pyramid");
   //Paris= (TParisPhysics*) m_DetectorManager->GetDetector("Paris");

  Rand = TRandom3();
  PyramidDetNumber = 0 ;
  ThetaNormalTarget = 0 ;
  Energy = 0;

  X_Pyramid = 0;
  Y_Pyramid = 0;
  Z_Pyramid = 0;
  Si_E_Pyramid = 0;
  Pad_E_Pyramid = 0;
  ThetaDetector = 0;
  TargetPosition = TVector3(m_DetectorManager->GetTargetX(),m_DetectorManager->GetTargetY(),m_DetectorManager->GetTargetZ() );

//Beam
  BeamDirection = TVector3(0,0,1);  
  double finalEnergy =0;

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){

// Reinitiate calculated variable
  ReInitValue();

  // stage specific
  for(int i = 0 ; i < Pyramid->Detector_N.size() ; i++)
  	Detector_N.push_back(Pyramid->Detector_N[i]); 
  for(int i = 0 ; i < Pyramid->Strip_N.size() ; i++)
  	Strip_N.push_back(Pyramid->Strip_N[i]);  

  Strip_E = Pyramid->Strip_E;
  Strip_T = Pyramid->Strip_T;
  DownStream_E = Pyramid->DownStream_E;
  DownStream_T = Pyramid->DownStream_T;
  UpStream_E = Pyramid->UpStream_E;
  UpStream_T = Pyramid->UpStream_T;

  if(reaction){
   //reacCond->Dump();
   vertexX = reacCond->GetVertexPositionX();
   vertexY = reacCond->GetVertexPositionY();
   vertexZ = reacCond->GetVertexPositionZ();
   //EmittanceThetaX = reacCond->GetBeamEmittanceThetaX();
   //EmittancePhiY = reacCond->GetBeamEmittancePhiY();
   //EmittanceTheta = reacCond->GetBeamEmittanceTheta();
   //EmittancePhi = reacCond->GetBeamEmittancePhi();
   thetaCM = reacCond->GetThetaCM();
   thetaLab_BF_0 = reacCond->GetThetaLab_BeamFrame(0);
   thetaLab_WF_0 = reacCond->GetThetaLab_WorldFrame(0);
   energy_0 = reacCond->GetKineticEnergy(0);
   thetaLab_BF_1 = reacCond->GetThetaLab_BeamFrame(1);
   thetaLab_WF_1 = reacCond->GetThetaLab_WorldFrame(1);
   energy_1 = reacCond->GetKineticEnergy(1);
  }
  else{
   vertexX = initCond->GetIncidentPositionX();
   vertexY = initCond->GetIncidentPositionY();
   vertexZ = initCond->GetIncidentPositionZ();
   //EmittanceThetaX = initCond->GetIncidentEmittanceThetaX();
   //EmittancePhiY = initCond->GetIncidentEmittancePhiY();
   //EmittanceTheta = initCond->GetIncidentEmittanceTheta();
   //EmittancePhi = initCond->GetIncidentEmittancePhi();
   thetaCM = initCond->GetThetaCM(0);
   thetaLab_BF_0 = initCond->GetThetaLab_IncidentFrame(0);
   thetaLab_WF_0 = initCond->GetThetaLab_WorldFrame(0);
   energy_0 = initCond->GetKineticEnergy(0);
   thetaLab_BF_1 = initCond->GetThetaLab_IncidentFrame(1);
   thetaLab_WF_1 = initCond->GetThetaLab_WorldFrame(1);
   energy_1 = initCond->GetKineticEnergy(1); 
  }

	for (unsigned i = 0 ; i <  interCoord->GetDetectedMultiplicity() ; i++) {
	  X_det.push_back(interCoord->GetDetectedPositionX(i));
	  Y_det.push_back(interCoord->GetDetectedPositionY(i));
	  Z_det.push_back(interCoord->GetDetectedPositionZ(i));
	  Theta_det.push_back(interCoord->GetDetectedAngleTheta(i));
	  Phi_det.push_back(interCoord->GetDetectedAnglePhi(i));
	 }

  EventsTree->Fill();

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// LOOP on Pyramid//////////////////

  if(0 /*Pyramid->Strip_E.size()==1*/){
    /************************************************/
    // Part 1 : Impact Angle
    PyramidDetNumber= Pyramid->Detector_N[0];
    TVector3 HitDirection = Pyramid -> GetPositionOfInteraction(0)-TargetPosition;
    X_Pyramid = Pyramid -> GetPositionOfInteraction(0).X();
    Y_Pyramid = Pyramid -> GetPositionOfInteraction(0).Y();
    Z_Pyramid = Pyramid -> GetPositionOfInteraction(0).Z();
 
    ThetaLab = HitDirection.Angle( BeamDirection );
    ThetaNormalTarget = HitDirection.Angle( TVector3(0,0,1) ) ;
    ThetaDetector = HitDirection.Angle(-Pyramid->GetDetectorNormal(0));
    LightDistance = HitDirection.Mag();
    /************************************************/

    /************************************************/
    // Part 2 : Impact Energy

    // select particle type
    if(ThetaLab>90*deg) { // 12C + 12C 
      LightAl=CarbonAl;
      LightTarget=CarbonTarget;
    }
    else{ // alpha + Ne
      LightAl=alphaAl;
      LightTarget=alphaTarget; 
    }

    Energy = ELab = 0;
    Si_E_Pyramid=Pad_E_Pyramid=0; //calibrated but uncorrected from dead layers

    if(Pyramid->Back_E[0]>0 /*&& Pyramid->Back_E[0] <100*/){
      Pad_E_Pyramid = Pyramid->Back_E[0];
    }

    if(Pyramid->Strip_E[0]>0)
      Si_E_Pyramid = Pyramid->Strip_E[0];
    

    // Si dead-layer Correction
    //PAD layer
    Energy +=Pad_E_Pyramid;
    double deadLayer = 0.0*micrometer; // arbtrary value

    if(Pyramid->Back_E[0]>0)
      Energy = LightAl.EvaluateInitialEnergy(Energy,deadLayer,0); 
    //DSSD layer
    Energy +=Si_E_Pyramid;
    Energy = LightAl.EvaluateInitialEnergy(Energy,deadLayer,0);
    
    // Target Correction
    ELab = LightTarget.EvaluateInitialEnergy( Energy ,TargetThickness*0.5, ThetaNormalTarget);
    
 
    /************************************************/
    // Part 3 : Excitation Energy Calculation
    if(reaction){

      if(1 /*ThetaLab>90*deg*/){ // out: 12C+12C
        Ex = Reaction -> ReconstructRelativistic( ELab , ThetaLab );
        //double Ex_contam = ReactionCont -> ReconstructRelativistic( ELab , ThetaLab );
      }
      else{ 
        double Ex_elast = ReactionElast -> ReconstructRelativistic( ELab , ThetaLab );
        Ex=Ex_elast;
      }

    }

    /************************************************/
    // Part 4 : Theta CM Calculation
    if(reaction)
      ThetaCM  = Reaction -> EnergyLabToThetaCM( ELab , ThetaLab)/deg;
    
    /************************************************/
    // Part 5: Compute the X and Y coordinate of the Heavy in the Trifoil plan
    if(reaction){
      Reaction->SetThetaCM(ThetaCM*deg);
      Reaction->SetExcitationHeavy(Ex);

      //double LabAngle3,LabKineE3,LabKineE4,LabAngle4;  
      //Reaction->KineRelativistic(LabAngle3,LabKineE3,LabAngle4,LabKineE4);
      //cout << " " << LabAngle3<< " " <<LabKineE3<< " " <<LabKineE4<< " " <<LabAngle4<< endl;  
     
      //Get Time of Flight
      //double m4 = Reaction->GetNucleus4().Mass();
      //Reaction->GetNucleus4().SetKineticEnergy(LabKineE4);

      //double beta4 = sqrt(  1-(m4*m4)/(LabKineE4+m4)/(LabKineE4+m4)  ) ;
      //double v4 = beta4 * (c_light*1e6); // m/s
      //RecoilTOF = 1/v4 * 1e6; // in s/m * 1e6 => ns/mm 
      //RecoilTOF *= R_Trifoil; // in ns
    }
        
/************************************************/
  }//end loop Pyramid 

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// PARIS //////////////////
  //Singles
    // ...
  //Add-back
    // ...

/************************************************/
// Unit conversion
ThetaLab=ThetaLab/deg;
//RecoilTheta=RecoilTheta/deg;

}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch(){
  //RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
  //RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
  //RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
  //RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
  //RootOutput::getInstance()->GetTree()->Branch("ThetaDetector",&ThetaDetector,"ThetaDetector/D");

  //RootOutput::getInstance()->GetTree()->Branch("X_Pyramid",&X_Pyramid,"X_Pyramid/D");
  //RootOutput::getInstance()->GetTree()->Branch("Y_Pyramid",&Y_Pyramid,"Y_Pyramid/D");
  //RootOutput::getInstance()->GetTree()->Branch("Z_Pyramid",&Z_Pyramid,"Z_Pyramid/D");
  //RootOutput::getInstance()->GetTree()->Branch("LightDistance",&LightDistance,"LightDistance/D");

  //other
  // RootOutput::getInstance()->GetTree()->Branch("RecoilTOF",&RecoilTOF,"RecoilTOF/D"); 
  // RootOutput::getInstance()->GetTree()->Branch("RecoilTheta",&RecoilTheta,"RecoilTheta/D");

}


////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  
  ThetaDetector=-1000;
  ELab = -1000;
  ThetaLab = -1000;
  ThetaCM = -1000;
  Ex = -1000 ;

  X_Pyramid = -1000;
  Y_Pyramid = -1000;
  Z_Pyramid = -1000;
  LightDistance=-1000;

  Detector_N.clear() ; 
  Strip_E.clear() ;
  Strip_T.clear() ;
  Strip_N.clear() ;
  DownStream_E.clear() ;
  DownStream_T.clear() ;
  UpStream_E.clear() ;
  UpStream_T.clear() ;

  vertexX = -1000;
  vertexY = -1000;
  vertexZ = -1000;
  //EmittanceThetaX = -1000;
  //EmittancePhiY = -1000;
  //EmittanceTheta = -1000;
  //EmittancePhi = -1000;
  thetaCM = -1000;
  thetaLab_BF_0 = -1000;
  thetaLab_WF_0 = -1000;
  energy_0 = -1000;
  thetaLab_BF_1 = -1000;
  thetaLab_WF_1 = -1000;
  energy_1 = -1000;

  X_det.clear();
  Y_det.clear();
  Z_det.clear();
  Theta_det.clear();
  Phi_det.clear();
  
  //RecoilTheta= -1000;
  //RecoilTOF= -1000;

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
  //stage specific;
	EventsFile->Write();
	delete EventsFile;
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

