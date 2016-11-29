/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : nov 2016                                                 *
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

#include"NPPhysicalConstants.h"


////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}
////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init(){
   
  myInit= new TInitialConditions();
  //  myIntCoord= new TInteractionCoordinates();
  
  Fat = (TFatimaPhysics*)  m_DetectorManager -> GetDetector("Fatima");



  myBeam = new NPL::Beam();
  myBeam->ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());

  //  myDetector= new NPL::DetectorManager();
  //  myDetector->ReadConfigurationFile(NPOptionManager::getInstance()->GetDetectorFile());

  ANucleus = myBeam->GetA();
  cout << "Beam nucleus A= " << ANucleus <<endl;

  OriginalBeamEnergy = myBeam->GetEnergy();
  cout << "Beam Energy before target= " <<  OriginalBeamEnergy << " MeV" << endl; 

  beta= sqrt(pow(OriginalBeamEnergy,2)+2*OriginalBeamEnergy*25*931.5)/(OriginalBeamEnergy+25*931.5);
  // To do: calculate beta form middle of the target.
  cout << "beta beam= " << beta << endl; 

   Mult_Fatima= 0;
   E_Fatima = 0 ;

  // Output histo
  HistoFile=new TFile("../../Outputs/Analysis/HistoList.root","RECREATE");
  E_DopplerCor = new TH1D("EDop","Gamma Doppler corrected energy",4000,0,4);
  E_Recorded = new TH1D("ERec","Raw Gamma energy",4000,0,4);

  // Output tree:
  InitOutputBranch();
  // Input tree
  InitInputBranch();
   
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue(); 
  // 32 module - symetrical configuration -> 16 polar angles 
  const double ThetaDet[]= {19.02390167, 37.70329404, 57.52298638, 77.29968198, 
  	      25.83989928, 46.55103678, 67.02589098, 107.1442293, 
   	      26.85689097, 41.56673389, 59.43554149,  77.97363101, 
  	      115.2395483, 46.94600569, 72.54645084, 103.1039385 };


  Mult_Fatima=Fat->GetEventData()->GetFatimaLaBr3EMult();

  //cout << "Multiplicity in Fatima= " << Mult_Fatima << endl; 

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //////////////////////////// Fatima evt //////////////////  

  if( Mult_Fatima==1){

    Det_Fatima= Fat->GetEventData()->GetFatimaLaBr3EDetectorNbr(0);
    //cout << "Multiplicity in Fatima= " << Mult_Fatima << endl; 
    //cout << "Fatima Det id = " << Det_Fatima << endl; 

    if(Det_Fatima<17)
      {
	//cout << "Theta detector = " << ThetaDet[Det_Fatima-1] << endl;
	ThetaDop=ThetaDet[Det_Fatima-1];
      }else
      {
	//cout << "Theta detector = " << ThetaDet[Det_Fatima-17] << endl;
	ThetaDop=ThetaDet[Det_Fatima-17];
      }
    
    Energy = Fat->GetEventData()->GetFatimaLaBr3EEnergy(0);
    //cout << "Fatima Energy = " << Energy << endl; 
    
    // Detector intrinsic energy resolution
    EDepCor   =  Energy*(1-beta*cos((ThetaDop)*3.14159/180.))/sqrt(1-beta*beta);; // To do: doppler correction

    //Fill hitogram
    E_Recorded->Fill(Energy);
    E_DopplerCor->Fill(EDepCor);

   }//end Fatima evt

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){

    HistoFile->Write();
    HistoFile->Close();
	
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch() {
  RootOutput::getInstance()->GetTree()->Branch("EDepCor",&EDepCor,"EDepCor/D");
  RootOutput::getInstance()->GetTree()->Branch("Det_Fatima",&Det_Fatima,"Det_Fatima/I");

}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
  //  RootInput:: getInstance()->GetChain()->SetBranchAddress("InitialConditions",&myInit );
}
////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
  EDepCor = -10;
  Det_Fatima= -10; 

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

