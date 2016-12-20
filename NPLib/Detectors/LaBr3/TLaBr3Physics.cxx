/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold LaBr3  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TLaBr3Physics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;

//   ROOT
#include "TChain.h"

//   tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TLaBr3Physics)
///////////////////////////////////////////////////////////////////////////
TLaBr3Physics::TLaBr3Physics()
   {      
      NumberOfDetector = 0 ;
      EventData = new TLaBr3Data ;
      PreTreatedData = new TLaBr3Data ;
      EventPhysics = this ;
      m_Spectra = NULL;
      m_LaBr3_E_Threshold   = 0;   
      m_LaBr3_RAW_Threshold   = 0;   
   }
   
///////////////////////////////////////////////////////////////////////////
TLaBr3Physics::~TLaBr3Physics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::Clear()
   {

      DetectorNumber.clear() ;
      Energy.clear() ;
      Time.clear() ;
   }
   
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("LaBr3");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token_sphe = {"R","THETA","PHI"};
  vector<string> token_cart = {"X","Y","Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token_cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  LaBr3" << i+1 <<  endl;
      blocks[i]->GetDouble("X","mm");
      blocks[i]->GetDouble("Y","mm");
      blocks[i]->GetDouble("Z","mm");
    }
    else if(blocks[i]->HasTokenList(token_sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  LaBr3" << i+1 <<  endl;
      blocks[i]->GetDouble("R","deg");
      blocks[i]->GetDouble("THETA","deg");
      blocks[i]->GetDouble("PHI","deg");
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < NumberOfDetector ; i++)
         {
                  Cal->AddParameter("LaBr3", "Detector"+ NPL::itoa(i+1)+"_E","LaBr3_Detector"+ NPL::itoa(i+1)+"_E")   ;
                  Cal->AddParameter("LaBr3", "Detector"+ NPL::itoa(i+1)+"_T","LaBr3_Detector"+ NPL::itoa(i+1)+"_T")   ;         
         }
   }
   
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "LaBr3"       , true )        ;
      inputChain->SetBranchStatus ( "fLaBr3_*"    , true )        ;
      inputChain->SetBranchAddress( "LaBr3"       , &EventData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "LaBr3", true );
      inputChain->SetBranchStatus ( "DetectorNumber", true );
      inputChain->SetBranchStatus ( "Energy", true );
      inputChain->SetBranchStatus ( "Time", true );
      inputChain->SetBranchAddress( "LaBr3", &EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "LaBr3" , "TLaBr3Physics" , &EventPhysics ) ;
   }
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::InitSpectra(){  
   m_Spectra = new TLaBr3Spectra(NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::FillSpectra(){  
   m_Spectra -> FillRawSpectra(EventData);
   m_Spectra -> FillPreTreatedSpectra(PreTreatedData);
   m_Spectra -> FillPhysicsSpectra(EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TLaBr3Physics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
///////////////////////////////////////////////////////////////////////////

void TLaBr3Physics::PreTreat(){  

  //   X
  //   E
  ClearPreTreatedData();
  double E,T;
  for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; ++i)
    {
    	if( EventData->GetEEnergy(i)>m_LaBr3_RAW_Threshold )
    	{
		E=CalibrationManager::getInstance()->ApplyCalibration("LaBr3/Detector" + NPL::itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i));
    		if(E>m_LaBr3_E_Threshold)
    		{
        		PreTreatedData->SetENumber( EventData->GetENumber(i) );
        		PreTreatedData->SetEEnergy( E );
    		}
    	}
    }
  for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; ++i)
    {
	T=CalibrationManager::getInstance()->ApplyCalibration("LaBr3/Detector"+ NPL::itoa(EventData->GetTNumber(i))+"_T",EventData->GetTTime(i) ) ;      
	PreTreatedData->SetTNumber( EventData->GetTNumber(i) );
        PreTreatedData->SetTTime( T );
            
    }  
    
}
///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::BuildPhysicalEvent()
   {
      BuildSimplePhysicalEvent()   ;
   }

///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::BuildSimplePhysicalEvent()
   {
      PreTreat();

      double LaBr3_T=-1000;  
      double LaBr3_E=-1000;  
      int LaBr3_N=-1000;  
      vector<double> LaBr3_E_vect,LaBr3_T_vect;
      vector<int> LaBr3_N_vect;
      for(unsigned int j = 0 ; j < EventData->GetEnergyMult() ; j++)  
      {
        LaBr3_E=PreTreatedData->GetEEnergy(j);  
        LaBr3_N=PreTreatedData->GetENumber(j);  
     	for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; i++)  
      	{
      		if(PreTreatedData->GetENumber(j)==PreTreatedData->GetTNumber(i)) 
      		{
      			LaBr3_T=EventData->GetTTime(i);
      		}
      	}
      	LaBr3_E_vect.push_back( LaBr3_E); 
      	LaBr3_T_vect.push_back( LaBr3_T); 
      	LaBr3_N_vect.push_back( LaBr3_N); 
      }
      for(unsigned int i = 0 ; i < LaBr3_E_vect.size() ; i++)
         {
            DetectorNumber.push_back( LaBr3_N_vect[i] )   ;
            Energy.push_back( LaBr3_E_vect[i]  );
            Time.push_back( LaBr3_T_vect[i]  );
         }

   }


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TLaBr3Physics::Construct(){
  return (NPL::VDetector*) new TLaBr3Physics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_labr3{
  public:
    proxy_labr3(){
      NPL::DetectorFactory::getInstance()->AddToken("LaBr3","LaBr3");
      NPL::DetectorFactory::getInstance()->AddDetector("LaBr3",TLaBr3Physics::Construct);
    }
};

proxy_labr3 p;
}

