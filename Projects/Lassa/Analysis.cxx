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
    Lassa = (TLassaPhysics*) m_DetectorManager->GetDetector("LASSAArray");
    InitialConditions = new TInitialConditions();
    InitOutputBranch();
    InitInputBranch();
    EDelta = 0.;
    totalEvents = 0;
    detectedEvents = 0;
    peakEvents = 0;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
    // Reinitiate calculated variable
    ReInitValue();
    
    totalEvents++;

    double BeamEnergy = 120.0;
    //double BeamEnergy = InitialConditions->GetIncidentInitialKineticEnergy();

    //cout << BeamEnergy << endl;
    
    EDelta = 2.0;

    //cout << Lassa->ThickSi_E.size() << endl;
    ///////////////////////////LOOP on Lassa Hit//////////////////////////////////
    if(Lassa->ThickSi_E.size() == 1){

        detectedEvents++;

        //for(unsigned int countLassa = 0 ; countLassa < Lassa->ThickSi_E.size(); countLassa++){
        TelescopeNumber = Lassa->TelescopeNumber[0];

        X = Lassa->GetPositionOfInteraction(0).X();
        Y = Lassa->GetPositionOfInteraction(0).Y();
        Z = Lassa->GetPositionOfInteraction(0).Z();
  
        TVector3 PositionOnLassa = TVector3(X,Y,Z);
        TVector3 ZUnit = TVector3(0,0,1);

        double X_target = InitialConditions->GetIncidentPositionX();
        double Y_target = InitialConditions->GetIncidentPositionY();
        double Z_target = InitialConditions->GetIncidentPositionZ();

        TVector3 PositionOnTarget = TVector3(X_target,Y_target,Z_target);
        TVector3 HitDirection = PositionOnLassa-PositionOnTarget;
        TVector3 HitDirectionUnit = HitDirection.Unit();

        //TVector3 BeamDirection = InitialConditions->GetBeamDirection();
        //double XBeam = BeamDirection.X();
        //double YBeam = BeamDirection.Y();
        //double ZBeam = BeamDirection.Z();

        //ThetaLab = BeamDirection.Angle(HitDirection);
        ThetaLab = 0;//ThetaLab/deg;

        PhiLab = HitDirection.Phi()/deg;

        E_ThickSi = Lassa->ThickSi_E[0];
      

        if(Lassa->CsI_E.size()>=1){  

            E_CsI = Lassa->CsI_E[0];
        
            ELab = E_ThickSi + E_CsI;
    
            if(ELab > (BeamEnergy-EDelta) && ELab < (BeamEnergy+EDelta) && ELab > 0.){
                peakEvents++;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){

  geomEff = 100*((double)detectedEvents)/((double)totalEvents);

  peakEff = 100*((double)peakEvents)/((double)detectedEvents);

  cout << endl;
  cout << "Total Events: " << totalEvents << endl;
  cout << "Detected Events: " << detectedEvents << endl;
  cout << "PeakEvents: " << peakEvents << endl;

  cout << "Geometric Efficiency: " << geomEff << endl;
  cout << "Peak Efficiency: " << peakEff << endl;

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
    //RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex,"Ex/D");
    RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab,"ELab/D");
    RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab,"ThetaLab/D");
    RootOutput::getInstance()->GetTree()->Branch("PhiLab",&PhiLab,"PhiLab/D");
    //  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM,"ThetaCM/D");
    //  RootOutput::getInstance()->GetTree()->Branch("totalEvents",&totalEvents,"totalEvents/I");
    //  RootOutput::getInstance()->GetTree()->Branch("detectedEvents",&detectedEvents,"detectedEvents/I");
    //  RootOutput::getInstance()->GetTree()->Branch("peakEvents",&peakEvents,"peakEvents/I");
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&InitialConditions);
}

////////////////////////////////////////////////////////////////////////////////     
void Analysis::ReInitValue(){
    E_ThickSi = 0.;
    E_CsI = 0.;
    ELab = 0.;
    ThetaLab = -1000;
    PhiLab = -1000;
    X = -1000;
    Y = -1000;
    Z = -1000;
    TelescopeNumber = -1;
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

