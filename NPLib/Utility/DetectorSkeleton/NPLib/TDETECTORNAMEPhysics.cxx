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
  return;
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::PreTreat(){
  ClearPreTreatedData();
  //   Front
  unsigned int sizeFront = m_EventData->GetMultiplicityFront();
  for(unsigned int i = 0 ; i < sizeFront ; ++i){
    if( m_EventData->GetFront_Energy(i)>m_StripFront_E_RAW_Threshold && IsValidChannel("Front", m_EventData->GetFront_DetectorNbr(i), m_EventData->GetFront_StripNbr(i)) ){
      double Front_E = fStrip_Front_E(m_EventData , i);
      if( Front_E > m_StripFront_E_Threshold ){
        if( m_EventData->GetFront_StripNbr(i)==0)
          cout << m_EventData->GetFront_StripNbr(i) << endl;
        m_PreTreatedData->SetFront_DetectorNbr( m_EventData->GetFront_DetectorNbr(i) );
        m_PreTreatedData->SetFront_StripNbr( m_EventData->GetFront_StripNbr(i) );
        m_PreTreatedData->SetFront_TimeCFD( m_EventData->GetFront_TimeCFD(i) );
        m_PreTreatedData->SetFront_Energy( Front_E );
      }
    }
  }
  return;
}


///////////////////////////////////////////////////////////////////////////
int TDETECTORNAMEPhysics :: CheckEvent(){
  // Check the size of the different elements
  if( m_PreTreatedData->GetMultiplicityBack() == m_PreTreatedData->GetMultiplicityFront() )
    return 1 ; // Regular Event
  
  else
    return -1 ; // Rejected Event
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
    if (LineBuffer.compare(0, 11, "ConfigDETECTORNAME") == 0) ReadingStatus = true;
    
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
        m_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_RAW_Threshold << endl;
      }
      
      else if (whatToDo=="STRIP_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_Threshold = atoi(DataBuffer.c_str());
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
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  
  double R,Phi,Z;
  R = 0 ; Phi = 0 ; Z = 0;
  TVector3 Pos;
  bool check_R   = false ;
  bool check_Phi = false ;
  bool check_Z   = false ;
  
  bool ReadingStatusQQQ = false ;
  bool ReadingStatusBOX = false ;
  bool ReadingStatus    = false ;
  
  bool VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel(); ;
  
  while (!ConfigFile.eof()){
    
    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 5, "DETECTORNAME") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      //   CD case
      if (DataBuffer=="DETECTORNAMEQQQ"){
       if(VerboseLevel) cout << "///" << endl           ;
        if(VerboseLevel) cout << "QQQ Quadrant found: " << endl   ;
        ReadingStatusQQQ = true ;
      }
      
      //  Box case
      else if (DataBuffer=="DETECTORNAMEBOX"){
        if(VerboseLevel) cout << "///" << endl           ;
        if(VerboseLevel) cout << "Box Detector found: " << endl   ;
        ReadingStatusBOX = true ;
      }
      
      //   Reading Block
      while(ReadingStatusQQQ){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Z= " << Z << "mm" << endl;
        }
        
        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  R= " << R << "mm" << endl;
        }
        
        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Phi= " << Phi << "deg" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector=") {
          /*ignore that*/
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusQQQ = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_R && check_Phi && check_Z){
          
          ReadingStatusQQQ = false;
          AddQQQDetector(R,Phi,Z);
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
        }
        
      }
      
      while(ReadingStatusBOX){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str());
          if(VerboseLevel) cout << "  Z= " << Z << "mm" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector1=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector2=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector3=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessDector4=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD1=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD2=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD3=") {
          /*ignore this */
        }
        
        else if (DataBuffer == "ThicknessPAD4=") {
          /*ignore this */
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusBOX = false;
          cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_Z){
          ReadingStatusBOX = false;
          AddBoxDetector(Z);
          //   Reinitialisation of Check Boolean
          check_R = false ;
          check_Phi = false ;
          check_Z = false ;
          
        }
      }
    }
  }
  
  InitializeStandardParameter();
  ReadAnalysisConfig();
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
map< vector<string> , TH1*> TDETECTORNAMEPhysics::GetSpectra() {
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

