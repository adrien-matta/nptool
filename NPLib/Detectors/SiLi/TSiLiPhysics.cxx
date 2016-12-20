/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold SiLi  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TSiLiPhysics.h"
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

ClassImp(TSiLiPhysics)
  ///////////////////////////////////////////////////////////////////////////
TSiLiPhysics::TSiLiPhysics()
{      
  NumberOfDetector = 0 ;
  EventData = new TSiLiData ;
  PreTreatedData = new TSiLiData ;
  EventPhysics = this ;
  m_Spectra = NULL;
  m_SiLi_E_Threshold   = 0;   
  m_SiLi_RAW_Threshold   = 0;   
}

///////////////////////////////////////////////////////////////////////////
TSiLiPhysics::~TSiLiPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::Clear()
{

  DetectorNumber.clear() ;
  Energy.clear() ;
  Time.clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::ReadConfiguration(NPL::InputParser parser){

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("SiLi");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"X","Y","Z","Width","Height","Thickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  SiLi " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      double Width = blocks[i]->GetDouble("Width","mm");
      double Height = blocks[i]->GetDouble("Height","mm");
      double Thickness = blocks[i]->GetDouble("Thickness","mm");
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < NumberOfDetector ; i++)
  {
    Cal->AddParameter("SiLi", "Detector"+ NPL::itoa(i+1)+"_E","SiLi_Detector"+ NPL::itoa(i+1)+"_E")   ;
    Cal->AddParameter("SiLi", "Detector"+ NPL::itoa(i+1)+"_T","SiLi_Detector"+ NPL::itoa(i+1)+"_T")   ;         
  }
}

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::InitializeRootInputRaw() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain()     ;
  inputChain->SetBranchStatus ( "SiLi"       , true )        ;
  inputChain->SetBranchStatus ( "fSiLi_*"    , true )        ;
  inputChain->SetBranchAddress( "SiLi"       , &EventData )  ;
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus ( "SiLi", true );
  inputChain->SetBranchStatus ( "DetectorNumber", true );
  inputChain->SetBranchStatus ( "Energy", true );
  inputChain->SetBranchStatus ( "Time", true );
  inputChain->SetBranchAddress( "SiLi", &EventPhysics );
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
  outputTree->Branch( "SiLi" , "TSiLiPhysics" , &EventPhysics ) ;
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::InitSpectra(){  
  m_Spectra = new TSiLiSpectra(NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(EventData);
  m_Spectra -> FillPreTreatedSpectra(PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string, TH1*> TSiLiPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
} 
///////////////////////////////////////////////////////////////////////////

void TSiLiPhysics::PreTreat(){  

  //   X
  //   E
  ClearPreTreatedData();
  double E,T;
  for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; ++i)
  {
    if( EventData->GetEEnergy(i)>m_SiLi_RAW_Threshold )
    {
      E=CalibrationManager::getInstance()->ApplyCalibration("SiLi/Detector" + NPL::itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i));
      if(E>m_SiLi_E_Threshold)
      {
        PreTreatedData->SetENumber( EventData->GetENumber(i) );
        PreTreatedData->SetEEnergy( E );
      }
    }
  }
  for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; ++i)
  {
    T=CalibrationManager::getInstance()->ApplyCalibration("SiLi/Detector"+ NPL::itoa(EventData->GetTNumber(i))+"_T",EventData->GetTTime(i) ) ;      
    PreTreatedData->SetTNumber( EventData->GetTNumber(i) );
    PreTreatedData->SetTTime( T );

  }  

}
///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::BuildPhysicalEvent()
{
  BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::BuildSimplePhysicalEvent()
{
  PreTreat();

  double SiLi_T=-1000;  
  double SiLi_E=-1000;  
  int SiLi_N=-1000;  
  vector<double> SiLi_E_vect,SiLi_T_vect;
  vector<int> SiLi_N_vect;
  for(unsigned int j = 0 ; j < EventData->GetEnergyMult() ; j++)  
  {
    SiLi_E=PreTreatedData->GetEEnergy(j);  
    SiLi_N=PreTreatedData->GetENumber(j);  
    for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; i++)  
    {
      if(PreTreatedData->GetENumber(j)==PreTreatedData->GetTNumber(i)) 
      {
        SiLi_T=EventData->GetTTime(i);
      }
    }
    SiLi_E_vect.push_back( SiLi_E); 
    SiLi_T_vect.push_back( SiLi_T); 
    SiLi_N_vect.push_back( SiLi_N); 
  }
  for(unsigned int i = 0 ; i < SiLi_E_vect.size() ; i++)
  {
    DetectorNumber.push_back( SiLi_N_vect[i] )   ;
    Energy.push_back( SiLi_E_vect[i]  );
    Time.push_back( SiLi_T_vect[i]  );
  }

}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSiLiPhysics::Construct(){
  return (NPL::VDetector*) new TSiLiPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_sili{
  public:
    proxy_sili(){
      NPL::DetectorFactory::getInstance()->AddToken("SiLi","SiLi");
      NPL::DetectorFactory::getInstance()->AddDetector("SiLi",TSiLiPhysics::Construct);
    }
};

proxy_sili p;
}

