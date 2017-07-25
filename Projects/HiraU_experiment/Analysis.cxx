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
    
    neutron = new NPL::Nucleus("1n");
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
        X_Hira = Hira->GetPositionOfInteraction(i).X();
        Y_Hira = Hira->GetPositionOfInteraction(i).Y();
        Z_Hira = Hira->GetPositionOfInteraction(i).Z();
        
        TVector3 PositionOnHira = TVector3(X_Hira,Y_Hira,Z_Hira);
        TVector3 HitDirection	= PositionOnHira - PositionOnTarget;
        
        ThetaLabHira = BeamDirection.Angle(HitDirection)/deg;
        PhiLabHira = HitDirection.Phi()/deg;
        
        EF = Hira->ThickSi_E[i];
        HiraELab = EF;
        if(Hira->CsI_E[i] > 0){
            E_CsI = Hira->CsI_E[i];
            HiraELab += E_CsI;
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
        ThetaLabNW = InteractionCoordinates->GetDetectedAngleTheta(i);
        PhiLabNW = InteractionCoordinates->GetDetectedAnglePhi(i);
        
        double X = InteractionCoordinates->GetDetectedPositionX(i);
        double Y = InteractionCoordinates->GetDetectedPositionY(i);
        double Z = InteractionCoordinates->GetDetectedPositionZ(i);
        
        double R = sqrt(pow(X,2) + pow(Y,2) + pow(Z,2))*1e-3;

        NWEnergy    = NW->NW_Energy[i];
        NWTOF       = NW->NW_Time[i];

        neutron->SetTimeOfFlight(NWTOF*1e-9/R);
        NWELab      = neutron->GetEnergy();
    }
    NWMultiplicity = NW->NW_Energy.size();
    
    if(PhiLabNW>0) PhiLabNW = 180-PhiLabNW;
    else if(PhiLabNW<0) PhiLabNW = -PhiLabNW-180;*/

    ///////////////////////////
    // ForwardArray Analysis //
    ///////////////////////////
    for(int i=0; i<FA->Energy.size(); i++){
        ThetaLabFA = InteractionCoordinates->GetDetectedAngleTheta(i);
        PhiLabFA = InteractionCoordinates->GetDetectedAnglePhi(i);
    }
    
    FAMultiplicity = FA->DetectorNumber.size();
    
    if(PhiLabFA>0) PhiLabFA = 180-PhiLabFA;
    else if(PhiLabFA<0) PhiLabFA = -PhiLabFA-180;

}

///////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
	RootOutput::getInstance()->GetTree()->Branch("ThetaLabMB",&ThetaLabMB,"ThetaLabMB/D");
	RootOutput::getInstance()->GetTree()->Branch("PhiLabMB",&PhiLabMB,"PhiLabMB/D");
    
    RootOutput::getInstance()->GetTree()->Branch("X_Hira",&X_Hira,"X_Hira/D");
    RootOutput::getInstance()->GetTree()->Branch("Y_Hira",&Y_Hira,"Y_Hira/D");
    RootOutput::getInstance()->GetTree()->Branch("Z_Hira",&Z_Hira,"Z_Hira/D");
    
    RootOutput::getInstance()->GetTree()->Branch("EF",&EF,"EF/D");
    RootOutput::getInstance()->GetTree()->Branch("E_CsI",&E_CsI,"E_CsI/D");
    RootOutput::getInstance()->GetTree()->Branch("HiraELab",&HiraELab,"HiraELab/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabHira",&ThetaLabHira,"ThetaLabHira/D");
	RootOutput::getInstance()->GetTree()->Branch("PhiLabHira",&PhiLabHira,"PhiLabHira/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabNW",&ThetaLabNW,"ThetaLabNW/D");
    RootOutput::getInstance()->GetTree()->Branch("PhiLabNW",&PhiLabNW,"PhiLabNW/D");
    RootOutput::getInstance()->GetTree()->Branch("NWEnergy",&NWEnergy,"NWEnergy/D");
    RootOutput::getInstance()->GetTree()->Branch("NWTOF",&NWTOF,"NWTOF/D");
    RootOutput::getInstance()->GetTree()->Branch("NWELab",&NWELab,"NWELab/D");
    
    RootOutput::getInstance()->GetTree()->Branch("ThetaLabFA",&ThetaLabFA,"ThetaLabFA/D");
    RootOutput::getInstance()->GetTree()->Branch("PhiLabFA",&PhiLabFA,"PhiLabFA/D");
    
    RootOutput::getInstance()->GetTree()->Branch("MBMultiplicity",&MBMultiplicity,"MBMultiplicity/I");
    RootOutput::getInstance()->GetTree()->Branch("NWMultiplicity",&NWMultiplicity,"NWMultiplicity/I");
    RootOutput::getInstance()->GetTree()->Branch("FAMultiplicity",&FAMultiplicity,"FAMultiplicity/I");
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
	ThetaLabMB      = -100;
	PhiLabMB        = -100;
	ThetaLabHira    = -100;
	PhiLabHira      = -100;
    ThetaLabNW      = -100;
    PhiLabNW        = -100;
    NWEnergy        = -100;
    NWTOF           = -100;
    NWELab          = -100;
    ThetaLabFA      = -100;
    PhiLabFA        = -100;
    MBMultiplicity  = -100;
    X_Hira          = -100;
    Y_Hira          = -100;
    Z_Hira          = -100;
    EF              = -100;
    HiraELab        = -100;
    E_CsI           = -100;
    
    FAMultiplicity  = -1;
    NWMultiplicity  = -1;
    MBMultiplicity  = -1;
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

