/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: E611                                                     *
 *                                                                           *
 * Creation Date  : May 2012                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds Chateau de Cristal Physics                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TChateauCristalPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"

// C++
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <cmath>
using namespace std;
using namespace ChateauCristal_LOCAL;

//  ROOT
#include "TChain.h"

//  transform an integer to a string
string itoa(int value)
{
   std::ostringstream o;
   if (!(o << value))
      return ""  ;
   return o.str();
}

ClassImp(TChateauCristalPhysics)

///////////////////////////////////////////////////////////////////////////
TChateauCristalPhysics::TChateauCristalPhysics()
{    
   NumberOfDetectors = 0;
   EventData = new TChateauCristalData;
   PreTreatedData = new TChateauCristalData;
   EventPhysics = this;
   E_Threshold = 0.;
   Pedestal_Threshold = 0;
}

///////////////////////////////////////////////////////////////////////////
TChateauCristalPhysics::~TChateauCristalPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::Clear()
{
   DetectorNumber  .clear() ;
   Energy          .clear() ;
   Time            .clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::ReadConfiguration(string Path) 
{
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer             ;
   string DataBuffer             ;
   string AngleFile;

   bool check_A       = false;
   bool ReadingStatus = false;

   while (!ConfigFile.eof()) {

      getline(ConfigFile, LineBuffer);

      //  If line is a Start Up ChateauCristal bloc, Reading toggle to true      
      if (LineBuffer.compare(0, 14, "ChateauCristal") == 0) //whole array at once
         //if (LineBuffer.compare(0, 4, "BAF2") == 0) //individual detector
      {
         cout << "Chateau de Cristal found: " << endl   ;        
         //         NumberOfDetectors=64;//simplify matters to read whole array at once sf
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

         //       //  Finding another telescope (safety), toggle out
         //     else if (DataBuffer=="BAF2") {
         //       cout << "WARNING: Another Telescope is founnd before standard sequence of Token, Error may occured in detector definition" << endl ;
         //       ReadingStatus = false ;
         //     }

         //  File angle method 

         if (DataBuffer.compare(0, 12, "ANGLES_FILE=") == 0) {
            check_A = true;
            ConfigFile >> DataBuffer ;
            AngleFile = DataBuffer;

            cout << "File angle used : " << DataBuffer << endl;
         }

         //  End File angle Method

         /////////////////////////////////////////////////
         //   If All necessary information there, toggle out
         if (check_A)
         {
            ReadingStatus = false;

            ///Add The previously define telescope

            AddModule(AngleFile);

            check_A = false;
         }
      }
   }

   InitializeStandardParameter() ;
   ReadAnalysisConfig()          ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::AddParameterToCalibrationManager()
{
   CalibrationManager* Cal = CalibrationManager::getInstance();

   for(int i = 0 ; i < NumberOfDetectors ; ++i){
      Cal->AddParameter("ChateauCristal", "Detector"+itoa(i+1)+"_E","ChateauCristal_DETECTOR_"+itoa(i+1)+"_E")  ;
      Cal->AddParameter("ChateauCristal", "Detector"+itoa(i+1)+"_T","ChateauCristal_DETECTOR_"+itoa(i+1)+"_T")  ;  
   }
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootInputRaw()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "ChateauCristal"     , true );
   inputChain->SetBranchStatus ( "fChateauCristal_*"  , true );
   inputChain->SetBranchAddress( "ChateauCristal"     , &EventData );
}     

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootInputPhysics()
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus ( "ChateauCristal"          , true );
   inputChain->SetBranchStatus ( "DetectorNumber", true );
   inputChain->SetBranchStatus ( "Energy"        , true );
   inputChain->SetBranchStatus ( "Time"          , true );
   inputChain->SetBranchAddress( "ChateauCristal"          , &EventPhysics );
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeRootOutput()
{
   TTree* outputTree = RootOutput::getInstance()->GetTree()      ;
   outputTree->Branch( "ChateauCristal" , "TChateauCristalPhysics" , &EventPhysics )  ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::BuildPhysicalEvent()
{
   BuildSimplePhysicalEvent()  ;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::BuildSimplePhysicalEvent()
{
   PreTreat();

   for(unsigned int i = 0 ; i <   PreTreatedData->GetChateauCristalEnergyMult() ; ++i)
   {
      DetectorNumber  .push_back(   PreTreatedData->GetChateauCristalEDetectorNbr(i) )  ;
      Energy          .push_back(   PreTreatedData->GetChateauCristalEnergy(i)       )  ; 
      // Look for associated time
      for(unsigned int j = 0 ; j <   PreTreatedData->GetChateauCristalTimeMult() ; ++j )
      {
         if(PreTreatedData->GetChateauCristalEDetectorNbr(i) == PreTreatedData->GetChateauCristalTDetectorNbr(j))
            Time.push_back(PreTreatedData->GetChateauCristalTime(j));
      }                        
   }
   return;    
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::PreTreat()
{
   ClearPreTreatedData();

   //  E
   for(int i = 0 ; i < EventData->GetChateauCristalEnergyMult() ; ++i)
   {
      if(EventData->GetChateauCristalEnergy(i) > Pedestal_Threshold && ChannelStatus[EventData->GetChateauCristalEDetectorNbr(i)-1])
      {
         double E = fChateauCristal_E(EventData , i); 
         if( E > E_Threshold )
         {
            PreTreatedData->SetChateauCristalEDetectorNbr( EventData->GetChateauCristalEDetectorNbr(i) )  ;
            PreTreatedData->SetChateauCristalEnergy( E )                                              ;
         }
      } 
   }

   //  T
   for(int i = 0 ; i < EventData->GetChateauCristalTimeMult() ; ++i)
   {
      if(ChannelStatus[EventData->GetChateauCristalTDetectorNbr(i)-1])
      {
         PreTreatedData->SetChateauCristalTDetectorNbr( EventData->GetChateauCristalTDetectorNbr(i) )  ;
         PreTreatedData->SetChateauCristalTime( fChateauCristal_T(EventData , i) )                         ;
      }
   }
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::InitializeStandardParameter()
{
   //  Enable all channels
   bool TempChannelStatus;
   ChannelStatus.clear();
   TempChannelStatus=true;
   for(int i = 0 ; i < NumberOfDetectors ; ++i)   
      ChannelStatus[i] = TempChannelStatus;
}

///////////////////////////////////////////////////////////////////////////
void TChateauCristalPhysics::ReadAnalysisConfig()
{
   bool ReadingStatus = false;

   // path to file
   string FileName = string(getenv("E611")) + string("/configs/ConfigChateauCristal.dat");
   //string FileName = "./configs/ConfigChateauCristal.dat";

   // open analysis config file
   ifstream AnalysisConfigFile;
   AnalysisConfigFile.open(FileName.c_str());

   if (!AnalysisConfigFile.is_open()) {
      cout << "No ConfigChateauCristal.dat found: default parameters loaded for analysis " << FileName << endl;
      return;
   }
   cout << "Loading user parameters for analysis from ConfigChateauCristal.dat " << endl;

   // Save it in a TAsciiFile
   TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
   asciiConfig->AppendLine("%%% ConfigChateauCristal.dat %%%");
   asciiConfig->Append(FileName.c_str());
   asciiConfig->AppendLine("");

   // read analysis config file
   string LineBuffer,DataBuffer,whatToDo;
   while (!AnalysisConfigFile.eof()) {
      // Pick-up next line
      getline(AnalysisConfigFile, LineBuffer);

      // search for "header"
      if (LineBuffer.compare(0, 9, "ConfigChateauCristal") == 0) ReadingStatus = true;

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
            Pedestal_Threshold = atoi(DataBuffer.c_str() );
            cout << "PEDESTAL THRESHOLD  " << Pedestal_Threshold << endl;
         }

         else if (whatToDo=="DISABLE") {
            AnalysisConfigFile >> DataBuffer;
            cout << whatToDo << "  " << DataBuffer << endl;
            int Detector = atoi(DataBuffer.substr(2,1).c_str());
            bool ChannelStatusBuffer;
            ChannelStatusBuffer=false;
            ChannelStatus[Detector-1] = ChannelStatusBuffer;
         }

         else {
            ReadingStatus = false;
         }
      }
   }
}   



void TChateauCristalPhysics::AddModule(string AngleFile)
{
   ifstream file;
   //  TString filename = Form("posBaptiste/angles_exogam_clover%d.txt",NumberOfClover);
   //  TString filename = Form("posz42_simu50mm/angles_exogam_clover%d.txt",NumberOfClover);
   //  TString filename = Form("posz42_exp_stat_demiring/angles_exogam_clover%d.txt",NumberOfClover);

   string path = "posz42_exp_stat_demiring/";
   TString filename = path + AngleFile;

   cout << filename << endl;
   file.open(filename);
   if (!file) cout << filename << " was not opened" << endl;

   map<int, double> mapDetectorAngleCorrec, mapDetectorAngleGeo;


   double angle_correc, angle_geo; 
   string buffer;
   int det;

   while (!file.eof()) {
      file >> buffer >> det >> angle_correc >> angle_geo;
      mapDetectorAngleCorrec[det] = angle_correc; 
      mapDetectorAngleGeo[det]    = angle_geo; 
      NumberOfDetectors++;
   }

   file.close();
}



double TChateauCristalPhysics::DopplerCorrection(double E, double Theta)
{
   double Pi = 3.141592654 ;

   double E_corr = 0;
   //double beta = 0.197;     // baptiste value
   double beta = 0.23;
   double gamma = 1./ sqrt(1-beta*beta);

   E_corr = gamma * E * ( 1. - beta * cos(Theta*Pi/180.));

   return E_corr;
}

///////////////////////////////////////////////////////////////////////////
double ChateauCristal_LOCAL::fChateauCristal_E( const TChateauCristalData* EventData , const int i )
{
   return CalibrationManager::getInstance()->ApplyCalibration("ChateauCristal/Detector" + itoa( EventData->GetChateauCristalEDetectorNbr(i) ) +"_E",  
         EventData->GetChateauCristalEnergy(i) );
}

double ChateauCristal_LOCAL::fChateauCristal_T( const TChateauCristalData* EventData , const int i )
{
   return CalibrationManager::getInstance()->ApplyCalibration("ChateauCristal/Detector" + itoa( EventData->GetChateauCristalTDetectorNbr(i) ) +"_T",  
         EventData->GetChateauCristalTime(i) );
}  



