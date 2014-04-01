/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 *  This class hold LaBr3  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TLaBr3Physics.h"
#include "../include/RootOutput.h"
#include "../include/RootInput.h"

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
void TLaBr3Physics::ReadConfiguration(string Path) 
   {
      ifstream ConfigFile           ;
      ConfigFile.open(Path.c_str()) ;
      string LineBuffer             ;
      string DataBuffer             ;

      bool check_Theta = false          ;
      bool check_Phi  = false           ;
      bool check_R     = false          ;
      bool check_Radius = false         ;
      bool check_Height = false         ;
      bool check_X = false              ;
      bool check_Y = false              ;
      bool check_Z = false              ;      
      bool ReadingStatus = false        ;

    while (!ConfigFile.eof()) 
       {
         
         getline(ConfigFile, LineBuffer);

         //   If line is a Start Up LaBr3 bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 5, "LaBr3") == 0) 
            {
               cout << "///" << endl ;
               cout << "LaBr3 found: " << endl ;        
               ReadingStatus = true ;
            }
            
         //   Else don't toggle to Reading Block Status
         else ReadingStatus = false ;
         
         //   Reading Block
         while(ReadingStatus)
            {
               // Pickup Next Word 
               ConfigFile >> DataBuffer ;

               //   Comment Line 
               if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

                  //   Finding another telescope (safety), toggle out
               else if (DataBuffer.compare(0, 5, "LaBr3") == 0) {
                  cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
                  ReadingStatus = false ;
               }
                              
                                    //Angle method
               else if (DataBuffer=="THETA=") {
                  check_Theta = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Theta:  " << atof(DataBuffer.c_str()) << "deg" << endl;
               }

               else if (DataBuffer=="PHI=") {
                  check_Phi = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Phi:  " << atof( DataBuffer.c_str() ) << "deg" << endl;
               }

               else if (DataBuffer=="R=") {
                  check_R = true;
                  ConfigFile >> DataBuffer ;
                  cout << "R:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               //Position method
               else if (DataBuffer=="X=") {
                  check_X = true;
                  ConfigFile >> DataBuffer ;
                  cout << "X:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }

               else if (DataBuffer=="Y=") {
                  check_Y = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Y:  " << atof( DataBuffer.c_str() ) << "mm"<< endl;
               }

               else if (DataBuffer=="Z=") {
                  check_Z = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Z:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               
                 
               // Cylindrical shape
               else if (DataBuffer== "Radius=") {
                  check_Radius = true;
                  ConfigFile >> DataBuffer ;
                  cout << "LaBr3 Radius:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
 
               
               else if (DataBuffer== "Height=") {
                  check_Height = true;
                  ConfigFile >> DataBuffer ;
                  cout << "LaBr3 Height:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
                                                 
               ///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if ( check_Theta && check_Phi && check_R &&  check_Radius &&      check_Height &&    check_X && check_Y && check_Z ) 
                  { 
                     NumberOfDetector++;
                     
                     //   Reinitialisation of Check Boolean  
                     check_Theta = false          ;
                     check_Phi  = false           ;
                     check_R     = false          ;
                     check_Radius = false         ;
                     check_Height = false         ;
                     check_X = false              ;
                     check_Y = false              ;
                     check_Z = false              ;
                     ReadingStatus = false        ;   
                     cout << "///"<< endl         ;                
                  }
            }
      }
   }

///////////////////////////////////////////////////////////////////////////
void TLaBr3Physics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < NumberOfDetector ; i++)
         {
                  Cal->AddParameter("LaBr3", "Detector"+itoa(i+1)+"_E","LaBr3_Detector"+itoa(i+1)+"_E")   ;
                  Cal->AddParameter("LaBr3", "Detector"+itoa(i+1)+"_T","LaBr3_Detector"+itoa(i+1)+"_T")   ;         
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
map< vector<string> , TH1*> TLaBr3Physics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< vector<string> , TH1*> empty;
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
		E=CalibrationManager::getInstance()->ApplyCalibration("LaBr3/Detector" + itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i));
    		if(E>m_LaBr3_E_Threshold)
    		{
        		PreTreatedData->SetENumber( EventData->GetENumber(i) );
        		PreTreatedData->SetEEnergy( E );
    		}
    	}
    }
  for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; ++i)
    {
	T=CalibrationManager::getInstance()->ApplyCalibration("LaBr3/Detector"+itoa(EventData->GetTNumber(i))+"_T",EventData->GetTTime(i) ) ;      
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


