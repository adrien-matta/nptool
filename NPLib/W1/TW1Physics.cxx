/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  This class hold W1  Physics                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
//  NPL
#include "TW1Physics.h"
#include "RootOutput.h"
#include "RootInput.h"

//  STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;
using namespace LOCAL;
//  ROOT
#include "TChain.h"

//  tranform an integer to a string
string itoa(int value)
{
   std::ostringstream o;
      
   if (!(o << value)) return "";

   return o.str();
}



ClassImp(TW1Physics)
///////////////////////////////////////////////////////////////////////////
TW1Physics::TW1Physics()
   : m_NumberOfDetector(0),
     m_EventData(new TW1Data),
     m_PreTreatedData(new TW1Data),
     m_EventPhysics(this),
     m_E_Threshold(0),
     m_Pedestal_Threshold(0)
{    
}



///////////////////////////////////////////////////////////////////////////
TW1Physics::~TW1Physics()
{
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::Clear()
{
   DetectorNumber  .clear()  ;
   StripNumber      .clear()  ;
   Energy          .clear()  ;
   Time            .clear()  ;
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadConfiguration(string Path) 
  {
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   double TLX , BLX , BRX , TRX , TLY , BLY , BRY , TRY , TLZ , BLZ , BRZ , TRZ   ;
   double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0      ;
   bool check_A = false   ;
   bool check_B = false ;
   bool check_C = false   ;
   bool check_D = false ;

   bool check_Theta = false   ;
   bool check_Phi  = false  ;
   bool check_R     = false   ;
   bool check_beta = false  ;
   bool ReadingStatus = false ;

 while (!ConfigFile.eof()) 
   {
      
    getline(ConfigFile, LineBuffer);

    //  If line is a Start Up ThinSi bloc, Reading toggle to true      
        if (LineBuffer.compare(0, 6, "ThinSi") == 0) 
          {
            cout << "Detector found: " << endl   ;        
            ReadingStatus = true ;
          }

    //  Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //  Reading Block
    while(ReadingStatus)
      {
          // Pickup Next Word 
        ConfigFile >> DataBuffer ;

        //  Comment Line 
        if (DataBuffer.compare(0, 1, "%") == 0) {  ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

          //  Finding another telescope (safety), toggle out
        else if (DataBuffer.compare(0, 6, "ThinSi") == 0) {
          cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
          ReadingStatus = false ;
        }

           //Position method
             else if (DataBuffer.compare(0, 3, "A=") == 0) {
                check_A = true;
                ConfigFile >> DataBuffer ;
                TLX = atof(DataBuffer.c_str()) ;
                ConfigFile >> DataBuffer ;
                TLY = atof(DataBuffer.c_str()) ;
                ConfigFile >> DataBuffer ;
                TLZ = atof(DataBuffer.c_str()) ;

             }
        
        else if (DataBuffer.compare(0, 3, "B=") == 0) {
          check_B = true;
          ConfigFile >> DataBuffer ;
          BLX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BLZ = atof(DataBuffer.c_str()) ;

        }

        else if (DataBuffer.compare(0, 3, "C=") == 0) {
          check_C = true;
          ConfigFile >> DataBuffer ;
          BRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          BRZ = atof(DataBuffer.c_str()) ;

        }

        else if (DataBuffer.compare(0, 3, "D=") == 0) {
          check_D = true;
          ConfigFile >> DataBuffer ;
          TRX = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRY = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          TRZ = atof(DataBuffer.c_str()) ;

        }

                  
        //Angle method
        else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
          check_Theta = true;
          ConfigFile >> DataBuffer ;
          Theta = atof(DataBuffer.c_str()) ;
        }

        else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi = atof(DataBuffer.c_str()) ;
        }

        else if (DataBuffer.compare(0, 2, "R=") == 0) {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R = atof(DataBuffer.c_str()) ;
        }


        else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
          check_beta = true;
          ConfigFile >> DataBuffer ;
          beta_u = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_v = atof(DataBuffer.c_str()) ;
          ConfigFile >> DataBuffer ;
          beta_w = atof(DataBuffer.c_str()) ;
        }
          
               ///////////////////////////////////////////////////
          //  If no Detector Token and no comment, toggle out
             else 
               {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
             
               /////////////////////////////////////////////////
               //  If All necessary information there, toggle out
             
             if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta) ) 
               { 
                   ReadingStatus = false; 
                   
                   ///Add The previously define telescope
                   //With position method
                    if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) {
                           m_NumberOfDetector++;
                     }

                   //with angle method
                   else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) {
                           m_NumberOfDetector++;
                     }
                     
                    //  Reinitialisation of Check Boolean 
                    
                check_A = false   ;
                check_B = false ;
                check_C = false   ;
                check_D = false ;

                check_Theta   = false   ;
                check_Phi     = false  ;
                check_R       = false   ;
                check_beta    = false  ;
                ReadingStatus = false ;
                       
               }
               
            }
          }
          
    InitializeStandardParameter() ;
    ReadAnalysisConfig()          ;
}


///////////////////////////////////////////////////////////////////////////
void TW1Physics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();
    
   for (int i = 0; i < m_NumberOfDetector; i++) {
      for (int j = 0; j < 16; j++) {
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Strip"+itoa(j+1)+"_E","W1_DETECTOR_"+itoa(i+1)+"_STRIP_"+itoa(j+1)+"_E")  ;
         Cal->AddParameter("W1", "Detector"+itoa(i+1)+"_Strip"+itoa(j+1)+"_T","W1_DETECTOR_"+itoa(i+1)+"_STRIP_"+itoa(j+1)+"_T")  ;  
      }
   }
}


  
///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeRootInput()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("W1"   , true);
   inputChain->SetBranchStatus("fW1_*", true);
   inputChain->SetBranchAddress("W1"  , &m_EventData);
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("W1", "TW1Physics", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::BuildPhysicalEvent()
{
   BuildSimplePhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::BuildSimplePhysicalEvent()
{
   PreTreat();
/*
   for (unsigned int i = 0; i < m_PreTreatedData->GetEnergyMult(); i++) {
      DetectorNumber  .push_back(   m_PreTreatedData->GetEnergyDetectorNbr(i) );
      StripNumber     .push_back(   m_PreTreatedData->GetEnergyStripNbr(i)    );
      Energy          .push_back(   m_PreTreatedData->GetEnergy(i)            ); 
      // Look For associate Time
      for (unsigned int j = 0; j < m_PreTreatedData->GetTimeMult(); j++) {
         if (m_PreTreatedData->GetEnergyDetectorNbr(i) == m_PreTreatedData->GetTimeDetectorNbr(j) && m_PreTreatedData->GetEnergyStripNbr(i)==m_PreTreatedData->GetTimeStripNbr(j))
            Time.push_back(m_PreTreatedData->GetTime(j));
      }                        
   }
  */ 
   return;    
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::PreTreat()
{
   ClearPreTreatedData();
      
   // (Front, E)
   for (int i = 0; i < m_EventData->GetW1FrontEMult(); i++) {
      if (ChannelStatus[m_EventData->GetW1FrontEDetectorNbr(i)][m_EventData->GetW1FrontEStripNbr(i)]) {
         double E = fSi_E(m_EventData , i);
         if (E > m_E_Threshold && m_EventData->GetW1FrontEEnergy(i) > m_Pedestal_Threshold)	{
            m_PreTreatedData->SetW1FrontEDetectorNbr(m_EventData->GetW1FrontEDetectorNbr(i));
            m_PreTreatedData->SetW1FrontEStripNbr(m_EventData->GetW1FrontEStripNbr(i));
            m_PreTreatedData->SetW1FrontEEnergy(E);
         }
      } 
   }
   // (Front, T)
   for (int i = 0; i < m_EventData->GetW1FrontTMult(); i++) {
      if (ChannelStatus[m_EventData->GetW1FrontTDetectorNbr(i)][m_EventData->GetW1FrontTStripNbr(i)]) {
         double T = fSi_T(m_EventData , i);
         m_PreTreatedData->SetW1FrontTDetectorNbr(m_EventData->GetW1FrontTDetectorNbr(i));
         m_PreTreatedData->SetW1FrontTStripNbr(m_EventData->GetW1FrontTStripNbr(i));
         m_PreTreatedData->SetW1FrontTTime(T);
      } 
   }

   // (Back, E)
   for (int i = 0; i < m_EventData->GetW1BackEMult(); i++) {
      if (ChannelStatus[m_EventData->GetW1BackEDetectorNbr(i)][m_EventData->GetW1BackEStripNbr(i)]) {
         double E = fSi_E(m_EventData , i);
         if (E > m_E_Threshold && m_EventData->GetW1BackEEnergy(i) > m_Pedestal_Threshold)	{
            m_PreTreatedData->SetW1BackEDetectorNbr(m_EventData->GetW1BackEDetectorNbr(i));
            m_PreTreatedData->SetW1BackEStripNbr(m_EventData->GetW1BackEStripNbr(i));
            m_PreTreatedData->SetW1BackEEnergy(E);
         }
      } 
   }
   // (Back, T)
   for (int i = 0; i < m_EventData->GetW1BackTMult(); i++) {
      if (ChannelStatus[m_EventData->GetW1BackTDetectorNbr(i)][m_EventData->GetW1BackTStripNbr(i)]) {
         double T = fSi_T(m_EventData , i);
         m_PreTreatedData->SetW1BackTDetectorNbr(m_EventData->GetW1BackTDetectorNbr(i));
         m_PreTreatedData->SetW1BackTStripNbr(m_EventData->GetW1BackTStripNbr(i));
         m_PreTreatedData->SetW1BackTTime(T);
      } 
   }
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::InitializeStandardParameter()
{
   // Enable all channels
   vector<bool> TempChannelStatus;
   ChannelStatus.clear();
   TempChannelStatus.resize(16,true);
   for (int i = 0; i < m_NumberOfDetector; i ++) ChannelStatus[i+1] = TempChannelStatus;
}



///////////////////////////////////////////////////////////////////////////
void TW1Physics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   // path to file
   string FileName = "./configs/ConfigW1.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << " No ConfigMust2.dat found: Default parameter loaded for Analayis " << FileName << endl;
      return;
   }
   cout << " Loading user parameter for Analysis from ConfigW1.dat " << endl;

   // read analysis config file
   string LineBuffer,DataBuffer;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 11, "ConfigW1") == 0) ReadingStatus = true;

      // loop on tokens and data
      while (ReadingStatus) {
         AnalysisConfigFile >> DataBuffer;

         // Search for comment symbol (%)
         if (DataBuffer.compare(0, 1, "%") == 0) {
            AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
         }
         
         else if (DataBuffer.compare(0, 18, "PEDESTAL_THRESHOLD") == 0) {
            AnalysisConfigFile >> DataBuffer;
            m_Pedestal_Threshold = atoi(DataBuffer.c_str() );
            cout << "Pedestal threshold = " << m_Pedestal_Threshold << endl;
         }
         
         else if (DataBuffer.compare(0, 4, "W1") == 0) {
            AnalysisConfigFile >> DataBuffer;
            string whatToDo = DataBuffer;
            if (whatToDo.compare(0, 11, "DISABLE_ALL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int Detector = atoi(DataBuffer.substr(2,1).c_str());
               vector< bool > ChannelStatusBuffer;
               ChannelStatusBuffer.resize(16,false);
               ChannelStatus[Detector] = ChannelStatusBuffer;
            }
            
          else if (whatToDo.compare(0, 15, "DISABLE_CHANNEL") == 0) {
               AnalysisConfigFile >> DataBuffer;
               cout << whatToDo << "  " << DataBuffer << endl;
               int telescope = atoi(DataBuffer.substr(2,1).c_str());
               int channel = -1;
               if (DataBuffer.compare(3,3,"STR") == 0) {
                  channel = atoi(DataBuffer.substr(6).c_str());
                  *(ChannelStatus[telescope].begin()+channel) = false;
               }
               
               else {
                  cout << "Warning: detector type for Must2 unknown!" << endl;
               }
            }
            else {
               cout << "Warning: don't know what to do (lost in translation)" << endl;
            }
         }
         else {
            ReadingStatus = false;
//            cout << "WARNING: Wrong Token Sequence" << endl;
         }
      }
   }
}	



///////////////////////////////////////////////////////////////////////////
double LOCAL::fSi_E( TW1Data* m_EventData , int i )
{
   return CalibrationManager::getInstance()->ApplyCalibration(  "W1/Detector" + itoa( m_EventData->GetW1FrontEDetectorNbr(i) ) + "_Strip" + itoa( m_EventData->GetW1FrontEStripNbr(i) ) +"_E",  
   m_EventData->GetW1FrontEEnergy(i) );
}
	  
	  
double LOCAL::fSi_T( TW1Data* m_EventData , int i )
{
	    return CalibrationManager::getInstance()->ApplyCalibration(  "W1/Detector" + itoa( m_EventData->GetW1FrontEDetectorNbr(i) ) + "_Strip" + itoa( m_EventData->GetW1FrontEStripNbr(i) ) +"_T",  
									    m_EventData->GetW1FrontTTime(i) );
}  
	  
	  
	  
