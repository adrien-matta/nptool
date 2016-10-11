/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold SSSD  Physics                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//  NPL
#include "TSSSDPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;
using namespace SSSD_LOCAL;
//  ROOT
#include "TChain.h"

//  tranform an integer to a string
string itoa(int value)
{
   char buffer [33];
   sprintf(buffer,"%d",value);
   return buffer;
}

ClassImp(TSSSDPhysics)
///////////////////////////////////////////////////////////////////////////
TSSSDPhysics::TSSSDPhysics(){    
    NumberOfDetector = 0;
    EventData = new TSSSDData;
    PreTreatedData = new TSSSDData;
    EventPhysics = this;
    m_E_Threshold = 0;
    m_Pedestal_Threshold = 0;
  }
///////////////////////////////////////////////////////////////////////////
TSSSDPhysics::~TSSSDPhysics()
  {}
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::Clear(){
    DetectorNumber  .clear() ;
    StripNumber     .clear() ;
    Energy          .clear() ;
    Time            .clear() ;
  }
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::ReadConfiguration(string Path) {
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer             ;
   string DataBuffer             ;

   double TLX , BLX , BRX , TRX , TLY , BLY , BRY , TRY , TLZ , BLZ , BRZ , TRZ   ;
   double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0      ;
   bool check_A = false ;
   bool check_B = false ;
   bool check_C = false ;
   bool check_D = false ;

   bool check_Theta = false   ;
   bool check_Phi  = false    ;
   bool check_R     = false   ;
   bool check_beta = false    ;
   bool ReadingStatus = false ;

 while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);
    //  If line is a Start Up SSSD bloc, Reading toggle to true      
        if (LineBuffer.compare(0, 4, "SSSD") == 0 &&LineBuffer.compare(0, 5, "SSSDA") != 0) {
            cout << "SSSD found: " << endl   ;        
            ReadingStatus = true ;
          }

    //  Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //  Reading Block
    while(ReadingStatus){
          // Pickup Next Word 
        ConfigFile >> DataBuffer ;

        //  Comment Line 
        if (DataBuffer.compare(0, 1, "%") == 0) {  ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

          //  Finding another telescope (safety), toggle out
        else if (DataBuffer=="SSSD") {
          cout << "WARNING: Another Telescope is founnd before standard sequence of Token, Error may occured in detector definition" << endl ;
          ReadingStatus = false ;
        }

        //Position method
       else if (DataBuffer=="A=") {
          check_A = true;
          ConfigFile >> DataBuffer ;
          TLX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TLY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TLZ = atof(DataBuffer.c_str()) ;
          cout << " A corner Position : (" << TLX << ";"<< TLY << ";"<< TLZ << ")"<<endl;
       }
        
        else if (DataBuffer=="B=") {
          check_B = true;
          ConfigFile >> DataBuffer ;
          BLX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLZ = atof(DataBuffer.c_str()) ;
          cout << " B corner Position : (" << BLX << ";"<< BLY << ";"<< BLZ << ")"<<endl;

        }

        else if (DataBuffer=="C=") {
          check_C = true;
          ConfigFile >> DataBuffer ;
          BRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRZ = atof(DataBuffer.c_str()) ;
          cout << " C corner Position : (" << BRX << ";"<< BRY << ";"<< BRZ << ")"<<endl;

        }

        else if (DataBuffer=="D=") {
          check_D = true;
          ConfigFile >> DataBuffer ;
          TRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRZ = atof(DataBuffer.c_str()) ;
          cout << " D corner Position : (" << TRX << ";"<< TRY << ";"<< TRZ << ")"<<endl;
        }

                  
        //Angle method
        else if (DataBuffer=="THETA=") {
          check_Theta = true;
          ConfigFile >> DataBuffer ;
          Theta = atof(DataBuffer.c_str()) ;
          cout << " Theta : " << Theta << "deg"<<endl;
        }

        else if (DataBuffer=="PHI=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi = atof(DataBuffer.c_str()) ;
          cout << " Phi : " << Phi << "deg"<<endl;
        }

        else if (DataBuffer=="R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R = atof(DataBuffer.c_str()) ;
          cout << " R : " << R << "mm"<<endl;
        }


        else if (DataBuffer=="BETA=") {
          check_beta = true;
          ConfigFile >> DataBuffer ;
          beta_u = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_v = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_w = atof(DataBuffer.c_str()) ;
          cout << " Beta : (" << beta_u << "deg ; "<< beta_v << "deg ; "<< beta_w << "deg"<<endl;
        }
          
               ///////////////////////////////////////////////////
          //  If no Detector Token and no comment, toggle out
             else 
               {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
             
               /////////////////////////////////////////////////
               //  If All necessary information there, toggle out
             
             if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta) ) { 
                   ReadingStatus = false; 
                   
                   ///Add The previously define telescope
                   //With position method
                    if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) {
                           NumberOfDetector++;
                     }

                   //with angle method
                   else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) {
                           NumberOfDetector++;
                     }
                     
                    //  Reinitialisation of Check Boolean 
                    
                check_A = false ;
                check_B = false ;
                check_C = false ;
                check_D = false ;

                check_Theta   = false ;
                check_Phi     = false ;
                check_R       = false ;
                check_beta    = false ;
                ReadingStatus = false ;
                       
               }
            }
          }
          
    InitializeStandardParameter() ;
    ReadAnalysisConfig()          ;
}


///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::AddParameterToCalibrationManager(){
    CalibrationManager* Cal = CalibrationManager::getInstance();
    
    for(int i = 0 ; i < NumberOfDetector ; ++i){
      
        for( int j = 0 ; j < 16 ; ++j){
            Cal->AddParameter("SSSD", "Detector"+ NPL::itoa(i+1)+"_Strip"+ NPL::itoa(j+1)+"_E","SSSD_DETECTOR_"+ NPL::itoa(i+1)+"_STRIP_"+ NPL::itoa(j+1)+"_E")  ;
            Cal->AddParameter("SSSD", "Detector"+ NPL::itoa(i+1)+"_Strip"+ NPL::itoa(j+1)+"_T","SSSD_DETECTOR_"+ NPL::itoa(i+1)+"_STRIP_"+ NPL::itoa(j+1)+"_T")  ;  
          }
      }
  }
 
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::InitializeRootInputRaw(){
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus ( "SSSD"     , true );
    inputChain->SetBranchStatus ( "fSSSD_*"  , true );
    inputChain->SetBranchAddress( "SSSD"     , &EventData );
  }     
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::InitializeRootInputPhysics(){
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "SSSD"          , true );
   inputChain->SetBranchStatus ( "DetectorNumber", true );
   inputChain->SetBranchStatus ( "StripNumber"   , true );
   inputChain->SetBranchStatus ( "Energy"        , true );
   inputChain->SetBranchStatus ( "Time"          , true );
   inputChain->SetBranchAddress( "SSSD"          , &EventPhysics );

   }
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::InitializeRootOutput(){
    TTree* outputTree = RootOutput::getInstance()->GetTree()      ;
    outputTree->Branch( "SSSD" , "TSSSDPhysics" , &EventPhysics )  ;
  }

///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::BuildPhysicalEvent(){
    BuildSimplePhysicalEvent()  ;
  }

///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::BuildSimplePhysicalEvent(){
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
void TSSSDPhysics::PreTreat(){
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
void TSSSDPhysics::InitializeStandardParameter(){
      //  Enable all channel
      vector<bool> TempChannelStatus;
      ChannelStatus.clear();
      TempChannelStatus.resize(16,true);
      for(int i = 0 ; i < NumberOfDetector ; ++i)   
          ChannelStatus[i] = TempChannelStatus;
  }
///////////////////////////////////////////////////////////////////////////
void TSSSDPhysics::ReadAnalysisConfig(){
   bool ReadingStatus = false;

   // path to file
   string FileName = "./configs/ConfigSSSD.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigMust2.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigSSSD.dat " << endl;
    
   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigSSSD.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");
   
   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 11, "ConfigSSSD") == 0) ReadingStatus = true;

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
            m_Pedestal_Threshold = atof(DataBuffer.c_str() );
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
                cout << "Warning: detector type for SSSD unknown!" << endl;
             }
          }

         else {
            ReadingStatus = false;
         }
      }
   }
}   


///////////////////////////////////////////////////////////////////////////
double SSSD_LOCAL::fSi_E( const TSSSDData* EventData , const int i )
  {
    return CalibrationManager::getInstance()->ApplyCalibration(  "SSSD/Detector" + NPL::itoa( EventData->GetEnergyDetectorNbr(i) ) + "_Strip" + NPL::itoa( EventData->GetEnergyStripNbr(i) ) +"_E",  
                            EventData->GetEnergy(i) );
  }
  
  
double SSSD_LOCAL::fSi_T( const TSSSDData* EventData , const int i )
  {
    return CalibrationManager::getInstance()->ApplyCalibration(  "SSSD/Detector" + NPL::itoa( EventData->GetEnergyDetectorNbr(i) ) + "_Strip" + NPL::itoa( EventData->GetEnergyStripNbr(i) ) +"_T",  
                            EventData->GetTime(i) );
  }  
     
     
     
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TSSSDPhysics::Construct(){
  return (NPL::VDetector*) new TSSSDPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_sssd{
  public:
    proxy_sssd(){
      NPL::DetectorFactory::getInstance()->AddToken("SSSD","SSSD");
      NPL::DetectorFactory::getInstance()->AddDetector("SSSD",TSSSDPhysics::Construct);
    }
};

proxy_sssd p;
}

