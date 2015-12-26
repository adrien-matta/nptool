/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TDETECTORNAMEPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPDetectorFactory.h"
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TDETECTORNAMEPhysics)
///////////////////////////////////////////////////////////////////////////
TDETECTORNAMEPhysics::TDETECTORNAMEPhysics(){
  m_EventData         = new TDETECTORNAMEData ;
  m_PreTreatedData    = new TDETECTORNAMEData ;
  m_EventPhysics      = this ;
  m_Spectra           = NULL;
  m_NumberOfDetector = 0 ;
  // Threshold
  m_E_RAW_Threshold = 0 ;
  m_E_Threshold = 0 ;
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::BuildPhysicalEvent(){
  PreTreat();
  // Make any further calculation on top the Pre Treat
  // In this case match Energy and time together

  unsigned int sizeE = m_PreTreatedData->GetMultEnergy();
  unsigned int sizeT = m_PreTreatedData->GetMultTime();

  for(unigned int e = 0 ; e < sizeE ; e++){
    for(unigned int t = 0 ; t < sizeT ; t++){
      if(m_PreTreatedData->GetE_DetectorNbr(e)==m_PreTreatedData->GetT_DetectorNbr(t)){
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        Energy.push_back(m_PreTreatedData->Get_Energy(e));
        Time.push_back(m_PreTreatedData->Get_Time(t));
      }
    }
  }
  
  return;
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::PreTreat(){
  // Typically apply threshold and Calibration
  // Might test for desabled channel for more complex detector
  ClearPreTreatedData();
  static NPL::CalibrationMangager Cal* = NPL::CalibrationMangager::getInstance();
  //   Energy
  unsigned int sizeE = m_EventData->GetMultEnergy();
  for(unsigned int i = 0 ; i < sizeE ; ++i){
    if( m_EventData->Get_Energy(i)>m_E_RAW_Threshold ){
      double Energy = Cal->ApplyCalibration("DETECTORNAME","ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if( Energy > m_E_Threshold ){
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }
 
   //  Time 
  unsigned int sizeT = m_EventData->GetMultTime();
  for(unsigned int i = 0 ; i < sizeT ; ++i){
      double Time= Cal->ApplyCalibration("DETECTORNAME","TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
        m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
    }
  }

  return;
}

////////////////////////////////////////////////////////////////////////////
bool TDETECTORNAMEPhysics::IsValidChannel( const int detector , const int channel){
    return *(m_FrontChannelStatus[detector-1].begin()+channel-1);
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;
  
  // path to file
  string FileName = "./configs/ConfigDETECTORNAME.dat";
  
  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());
  
  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigDETECTORNAME.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigDETECTORNAME.dat " << endl;
  
  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigDETECTORNAME.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);
    
    // search for "header"
    string name = "ConfigDETECTORNAME";
    if (LineBuffer.compare(0, name.length(), name) == 0) 
      ReadingStatus = true;
    
    // loop on tokens and data
    while (ReadingStatus ) {
      
      whatToDo="";
      AnalysisConfigFile >> whatToDo;
      
      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }
      
      if (whatToDo== "DISABLE_DETECTOR") {
        AnalysisConfigFile >> DataBuffer;
        cout << whatToDo << "  " << DataBuffer << endl;
        int Detector = atoi(DataBuffer.substr(2,1).c_str());
        vector< bool > ChannelStatus;
        ChannelStatus.resize(24,false);
        m_FrontChannelStatus[Detector-1] = ChannelStatus;
        ChannelStatus.resize(48,false);
        m_BackChannelStatus[Detector-1] = ChannelStatus;
        ChannelStatus.resize(1,false);
        m_PADChannelStatus[Detector-1] = ChannelStatus;
      }
      
        else cout << "Warning: detector type for DETECTORNAME unknown!" << endl;
        
      }
      
      else if (whatToDo=="STRIP_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_RAW_Threshold << endl;
      }
      
      else if (whatToDo=="STRIP_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_Threshold << endl;
      }
      
      else {
        ReadingStatus = false;
      }
      
    }
  }
  
}


///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::Clear(){
  EventMultiplicity = 0;
  DetectorNumber.clear() ;
  Energy.clear();
  Time.clear();
}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ReadConfiguration(string Path){
}
      

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitSpectra(){  
  m_Spectra = new TDETECTORNAMESpectra(m_NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::CheckSpectra(){  
  m_Spectra->CheckSpectra();  
}
///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TDETECTORNAMEPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< vector<string> , TH1*> empty;
    return empty;
  }
} 
////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TDETECTORNAMEPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
} 

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::WriteSpectra(){
  m_Spectra->WriteSpectra();
}
///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  
  for(int i = 0 ; i < m_NumberOfDetector ; ++i)
    {
    
    for( int j = 0 ; j < 24 ; ++j)
      {
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T")   ;
      }
    
    for( int j = 0 ; j < 48 ; ++j)
      {
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T","SHARC_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T")   ;
      }
    
    for( int j = 0 ; j < 1 ; ++j)
      {
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_PAD"+ NPL::itoa(j+1)+"_E","SHARC_D"+ NPL::itoa(i+1)+"_PAD_E")   ;
      Cal->AddParameter("SHARC", "D"+ NPL::itoa(i+1)+"_PAD"+ NPL::itoa(j+1)+"_T","SHARC_D"+ NPL::itoa(i+1)+"_PAD_T")   ;
      }
    }
  
  return;
  
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "DETECTORNAME" , true );
  inputChain->SetBranchAddress( "DETECTORNAME" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress( "DETECTORNAME" , &m_EventPhysics );
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "DETECTORNAME" , "TDETECTORNAMEPhysics" , &m_EventPhysics );
}

////////////////////////////////////////////////////////////////////////////////
/////   Specific to DETECTORNAMEArray   ////
void TDETECTORNAMEPhysics::AddDetector(double Z){
}
////////////////////////////////////////////////////////////////////////////////
TVector3 TDETECTORNAMEPhysics::GetDetectorNormal( const int i) const{
  return (TVector3(0,0,i));
}
////////////////////////////////////////////////////////////////////////////////
TVector3 TDETECTORNAMEPhysics::GetPositionOfInteraction(const int i) const{
  TVector3 Position = TVector3(0,0,1);
  return(Position) ;
  
}
////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeStandardParameter(){
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_FrontChannelStatus.clear()    ;
  m_BackChannelStatus.clear()    ;
  m_PADChannelStatus.clear() ;
  
  ChannelStatus.resize(24,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_FrontChannelStatus[i] = ChannelStatus;
    }
  
  ChannelStatus.resize(48,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_BackChannelStatus[i] = ChannelStatus;
    }
  
  ChannelStatus.resize(1,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_PADChannelStatus[i] = ChannelStatus;
    }
  
  m_MaximumStripMultiplicityAllowed = m_NumberOfDetector   ;
  
  return;
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TDETECTORNAMEPhysics::Construct(){
  return (NPL::VDetector*) new TDETECTORNAMEPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_sharc{
  public:
    proxy_sharc(){
      NPL::DetectorFactory::getInstance()->AddToken("DETECTORNAME","DETECTORNAME");
      NPL::DetectorFactory::getInstance()->AddDetector("DETECTORNAME",TDETECTORNAMEPhysics::Construct);
    }
};

proxy_sharc p;
}

