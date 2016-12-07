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
 *  This class hold QQQ  Physics                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//  NPL
#include "TQQQPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;
using namespace QQQ_LOCAL;
//  ROOT
#include "TChain.h"

//  tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TQQQPhysics)
///////////////////////////////////////////////////////////////////////////
TQQQPhysics::TQQQPhysics(){    
    NumberOfDetector = 0;
    EventData = new TQQQData;
    PreTreatedData = new TQQQData;
    EventPhysics = this;
    m_E_Threshold = 0;
    m_Pedestal_Threshold = 0;
  }
///////////////////////////////////////////////////////////////////////////
TQQQPhysics::~TQQQPhysics()
  {}
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::Clear(){
    DetectorNumber  .clear() ;
    StripNumber     .clear() ;
    Energy          .clear() ;
    Time            .clear() ;
  }
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("QQQ");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"A","B","C","D"};
  vector<string> sphe = {"R","THETA","PHI","BETA"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  QQQ " << i+1 <<  endl;
      TVector3 A = blocks[i]->GetTVector3("A","mm");
      TVector3 B = blocks[i]->GetTVector3("B","mm");
      TVector3 C = blocks[i]->GetTVector3("C","mm");
      TVector3 D = blocks[i]->GetTVector3("D","mm");
      NumberOfDetector++;  
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  QQQ " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> Beta = blocks[i]->GetVectorDouble("BETA","deg");
      NumberOfDetector++;
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}


///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::AddParameterToCalibrationManager(){
    CalibrationManager* Cal = CalibrationManager::getInstance();
    
    for(int i = 0 ; i < NumberOfDetector ; ++i){
      
        for( int j = 0 ; j < 16 ; ++j){
            Cal->AddParameter("QQQ", "Detector"+ NPL::itoa(i+1)+"_Strip"+ NPL::itoa(j+1)+"_E","QQQ_DETECTOR_"+ NPL::itoa(i+1)+"_STRIP_"+ NPL::itoa(j+1)+"_E")  ;
            Cal->AddParameter("QQQ", "Detector"+ NPL::itoa(i+1)+"_Strip"+ NPL::itoa(j+1)+"_T","QQQ_DETECTOR_"+ NPL::itoa(i+1)+"_STRIP_"+ NPL::itoa(j+1)+"_T")  ;  
          }
      }
  }
 
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::InitializeRootInputRaw(){
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus ( "QQQ"     , true );
    inputChain->SetBranchStatus ( "fQQQ_*"  , true );
    inputChain->SetBranchAddress( "QQQ"     , &EventData );
  }     
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::InitializeRootInputPhysics(){
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "QQQ"          , true );
   inputChain->SetBranchStatus ( "DetectorNumber", true );
   inputChain->SetBranchStatus ( "StripNumber"   , true );
   inputChain->SetBranchStatus ( "Energy"        , true );
   inputChain->SetBranchStatus ( "Time"          , true );
   inputChain->SetBranchAddress( "QQQ"          , &EventPhysics );

   }
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::InitializeRootOutput(){
    TTree* outputTree = RootOutput::getInstance()->GetTree()      ;
    outputTree->Branch( "QQQ" , "TQQQPhysics" , &EventPhysics )  ;
  }

///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::BuildPhysicalEvent(){
    BuildSimplePhysicalEvent()  ;
  }

///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::BuildSimplePhysicalEvent(){
        PreTreat();
        for(unsigned int i = 0 ; i <   PreTreatedData->GetEnergyMult() ; ++i){
            DetectorNumber  .push_back(   PreTreatedData->GetEnergyDetectorNbr(i) )  ;
            StripNumber     .push_back(   PreTreatedData->GetEnergyStripNbr(i)    )  ;
            Energy          .push_back(   PreTreatedData->GetEnergy(i)            )  ; 
            // Look For associate Time
            for(unsigned int j = 0 ; j <   PreTreatedData->GetTimeMult() ; ++j ){
                if(PreTreatedData->GetEnergyDetectorNbr(i) == PreTreatedData->GetTimeDetectorNbr(j) && PreTreatedData->GetEnergyStripNbr(i)==PreTreatedData->GetTimeStripNbr(j))
                  Time.push_back(PreTreatedData->GetTime(j));
              }                        
          }
    return;    
  }
  
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::PreTreat(){
  ClearPreTreatedData();
  
  //  E
      for(int i = 0 ; i < EventData->GetEnergyMult() ; ++i){
        if(EventData->GetEnergy(i) > m_Pedestal_Threshold && ChannelStatus[EventData->GetEnergyDetectorNbr(i)-1][EventData->GetEnergyStripNbr(i)-1]){
               double E = fSi_E(EventData , i); 
               if( E > m_E_Threshold ){
                     PreTreatedData->SetEnergyDetectorNbr( EventData->GetEnergyDetectorNbr(i) )  ;
                     PreTreatedData->SetEnergyStripNbr( EventData->GetEnergyStripNbr(i) )        ;
                     PreTreatedData->SetEnergy( E )                                              ;
                   }
              } 
          }
         //  T
         for(int i = 0 ; i < EventData->GetTimeMult() ; ++i){
            if(ChannelStatus[EventData->GetTimeDetectorNbr(i)-1][EventData->GetTimeStripNbr(i)-1]){
                 PreTreatedData->SetTimeDetectorNbr( EventData->GetTimeDetectorNbr(i) )  ;
                 PreTreatedData->SetTimeStripNbr( EventData->GetTimeStripNbr(i) )        ;
                 PreTreatedData->SetTime( fSi_T(EventData , i) )                         ;
                }
            }
  }

///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::InitializeStandardParameter(){
      //  Enable all channel
      vector<bool> TempChannelStatus;
      ChannelStatus.clear();
      TempChannelStatus.resize(16,true);
      for(int i = 0 ; i < NumberOfDetector ; ++i)   
          ChannelStatus[i] = TempChannelStatus;
  }
///////////////////////////////////////////////////////////////////////////
void TQQQPhysics::ReadAnalysisConfig(){
   bool ReadingStatus = false;

   // path to file
   string FileName = "./configs/ConfigQQQ.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigMust2.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigQQQ.dat " << endl;
    
   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigQQQ.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");
   
   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 11, "ConfigQQQ") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
      
         whatToDo= "" ;
         AnalysisConfigFile >> whatToDo;

         // Search for comment symbol (%)
         if (whatToDo.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }
         
         else if (whatToDo=="PEDESTAL_THRESHOLD") {
            AnalysisConfigFile >> DataBuffer;
            m_Pedestal_Threshold = atoi(DataBuffer.c_str() );
            cout << "PEDESTAL THRESHOLD  " << m_Pedestal_Threshold << endl;
         }
         
         
       else if (whatToDo=="DISABLE_ALL") {
             AnalysisConfigFile >> DataBuffer;
             cout << whatToDo << "  " << DataBuffer << endl;
             int Detector = atoi(DataBuffer.substr(2,1).c_str());
             vector< bool > ChannelStatusBuffer;
             ChannelStatusBuffer.resize(16,false);
             ChannelStatus[Detector-1] = ChannelStatusBuffer;
          }
          
        else if (whatToDo=="DISABLE_CHANNEL") {
             AnalysisConfigFile >> DataBuffer;
             cout << whatToDo << "  " << DataBuffer << endl;
             int telescope = atoi(DataBuffer.substr(2,1).c_str());
             int channel = -1;
             if (DataBuffer.compare(3,3,"STR") == 0) {
                channel = atoi(DataBuffer.substr(6).c_str());
                *(ChannelStatus[telescope-1].begin()+channel-1) = false;
             }
             
             else {
                cout << "Warning: detector type for QQQ unknown!" << endl;
             }
          }

         else {
            ReadingStatus = false;
         }
      }
   }
}   


///////////////////////////////////////////////////////////////////////////
double QQQ_LOCAL::fSi_E( const TQQQData* EventData , const int i )
  {
    return CalibrationManager::getInstance()->ApplyCalibration(  "QQQ/Detector" + NPL::itoa( EventData->GetEnergyDetectorNbr(i) ) + "_Strip" + NPL::itoa( EventData->GetEnergyStripNbr(i) ) +"_E",  
                            EventData->GetEnergy(i) );
  }
  
  
double QQQ_LOCAL::fSi_T( const TQQQData* EventData , const int i )
  {
    return CalibrationManager::getInstance()->ApplyCalibration(  "QQQ/Detector" + NPL::itoa( EventData->GetEnergyDetectorNbr(i) ) + "_Strip" + NPL::itoa( EventData->GetEnergyStripNbr(i) ) +"_T",  
                            EventData->GetTime(i) );
  }  
     
     
     
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TQQQPhysics::Construct(){
  return (NPL::VDetector*) new TQQQPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_qqq{
  public:
    proxy_qqq(){
      NPL::DetectorFactory::getInstance()->AddToken("QQQ","QQQ");
      NPL::DetectorFactory::getInstance()->AddDetector("QQQ",TQQQPhysics::Construct);
    }
};

proxy_qqq p;
}

