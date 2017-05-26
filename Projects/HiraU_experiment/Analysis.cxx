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
    MB= (TMicroballPhysics*) m_DetectorManager->GetDetector("Microball");
	Hira= (THiraPhysics*) m_DetectorManager->GetDetector("HiraTelescope");
    FA= (TForwardArrayPhysics*) m_DetectorManager->GetDetector("ForwardArray");
    //NW= (TNeutronWallPhysics*) m_DetectorManager->GetDetector("NeutronWall");
	InitialConditions = new TInitialConditions();
	InteractionCoordinates = new TInteractionCoordinates();
	
    InitInputBranch();
	InitOutputBranch();
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
	ReInitValue();
    
    double X_target = 0;
    double Y_target = 0;
    double Z_target = 0;
    //double Z_target	= InitialConditions->GetIncidentPositionZ();
    //cout << Z_target << endl;
    TVector3 PositionOnTarget = TVector3(X_target,Y_target,Z_target);
    TVector3 BeamDirection = TVector3(0,0,1);
    ////////////////////////
    // MicroBall Analysis //
    ///////////////////////
	for(int i=0; i<MB->DetectorNumber.size(); i++){
		ThetaLabMB = InteractionCoordinates->GetDetectedAngleTheta(i);
		PhiLabMB = InteractionCoordinates->GetDetectedAnglePhi(i);	
	}
	MBMultiplicity = MB->DetectorNumber.size();

	if(PhiLabMB>0) PhiLabMB = 180-PhiLabMB;
	else if(PhiLabMB<0) PhiLabMB = -PhiLabMB-180;
    
    ///////////////////
    // Hira Analysis //
    ///////////////////
	for(int i=0; i<Hira->TelescopeNumber.size(); i++){
        X = Hira->GetPositionOfInteraction(i).X();
        Y = Hira->GetPositionOfInteraction(i).Y();
        Z = Hira->GetPositionOfInteraction(i).Z();
        
        TVector3 PositionOnHira = TVector3(X,Y,Z);
        TVector3 HitDirection	= PositionOnHira - PositionOnTarget;
        
        ThetaLabHira = BeamDirection.Angle(HitDirection)/deg;
        PhiLabHira = HitDirection.Phi()/deg;
        
        EF = Hira->ThickSi_E[i];
        
        if(Hira->CsI_E[i] > 0){
            E_CsI = Hira->CsI_E[i];
            ELab = EF + E_CsI;
        }
        
		//ThetaLabHira = InteractionCoordinates->GetDetectedAngleTheta(i);
		//PhiLabHira = InteractionCoordinates->GetDetectedAnglePhi(i);
	}	

	if(PhiLabHira>0) PhiLabHira = 180-PhiLabHira;
	else if(PhiLabHira<0) PhiLabHira = -PhiLabHira-180;
    
    //////////////////////////
    // NeutronWall Analysis //
    //////////////////////////
    /*for(int i=0; i<NW->NW_Energy.size(); i++){
        ThetaLabVW = InteractionCoordinates->GetDetectedAngleTheta(i);
        PhiLabVW = InteractionCoordinates->GetDetectedAnglePhi(i);
    }
    VWMultiplicity = NW->VW_Energy.size();
    
    if(PhiLabVW>0) PhiLabVW = 180-PhiLabVW;
    else if(PhiLabVW<0) PhiLabVW = -PhiLabVW-180;*/

    ///////////////////////////
    // ForwardArray Analysis //
    ///////////////////////////
    for(int i=0; i<FA->Energy.size(); i++){
        ThetaLabFA = InteractionCoordinates->GetDetectedAngleTheta(i);
        PhiLabFA = InteractionCoordinates->GetDetectedAnglePhi(i);
    }
    if(PhiLabFA>0) PhiLabFA = 180-PhiLabFA;
    else if(PhiLabFA<0) PhiLabFA = -PhiLabFA-180;

}

///////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
	RootOutput::getInstance()->GetTree()->Branch("ThetaLabMB",&ThetaLabMB,"ThetaLabMB/D");
	RootOutput::getInstance()->GetTree()->Branch("PhiLabMB",&PhiLabMB,"PhiLabMB/D");
    
    RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D");
    RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D");
    RootOutput::getInstance()->GetTree()->Branch("Z",&Z,"Z/D");
    
    RootOutput::getInstance()->GetTree()->Branch("EF",&EF,"EF/D");
    RootOutput::getInstance()->GetTree()->Branch("E_CsI",&E_CsI,"E_CsI/D");
    RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabHira",&ThetaLabHira,"ThetaLabHira/D");
	RootOutput::getInstance()->GetTree()->Branch("PhiLabHira",&PhiLabHira,"PhiLabHira/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabVW",&ThetaLabVW,"ThetaLabVW/D");
    RootOutput::getInstance()->GetTree()->Branch("PhiLabVW",&PhiLabVW,"PhiLabVW/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabFA",&ThetaLabFA,"ThetaLabFA/D");
    RootOutput::getInstance()->GetTree()->Branch("PhiLabFA",&PhiLabFA,"PhiLabFA/D");
    
    RootOutput::getInstance()->GetTree()->Branch("MBMultiplicity",&MBMultiplicity,"MBMultiplicity/I");
    RootOutput::getInstance()->GetTree()->Branch("VWMultiplicity",&VWMultiplicity,"VWMultiplicity/I");
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  	RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  	RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
  	RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&InitialConditions);

	RootInput:: getInstance()->GetChain()->SetBranchStatus("InteractionCoordinates",true );
  	RootInput:: getInstance()->GetChain()->SetBranchStatus("fDetected_*",true );
  	RootInput:: getInstance()->GetChain()->SetBranchAddress("InteractionCoordinates",&InteractionCoordinates);
}

////////////////////////////////////////////////////////////////////////////////     
void Analysis::ReInitValue(){
	ThetaLabMB = -100;
	PhiLabMB = -100;
	ThetaLabHira = -100;
	PhiLabHira = -100;	
    ThetaLabVW = -100;
    PhiLabVW = -100;
    ThetaLabFA = -100;
    PhiLabFA = -100;
    MBMultiplicity = -100;
    X = -100;
    Y = -100;
    Z = -100;
    EF = -100;
    ELab = -100;
    E_CsI = -100;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
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

