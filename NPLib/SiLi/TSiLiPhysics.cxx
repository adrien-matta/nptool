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
 *  This class hold SiLi  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TSiLiPhysics.h"
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

ClassImp(TSiLiPhysics)
///////////////////////////////////////////////////////////////////////////
TSiLiPhysics::TSiLiPhysics()
   {      
      NumberOfDetector = 0 ;
      EventData = new TSiLiData ;
      EventPhysics = this ;
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
void TSiLiPhysics::ReadConfiguration(string Path) 
   {
      ifstream ConfigFile           ;
      ConfigFile.open(Path.c_str()) ;
      string LineBuffer             ;
      string DataBuffer             ;

      bool check_Theta = false          ;
      bool check_Phi  = false           ;
      bool check_R     = false          ;
      bool check_Thickness = false      ;
      bool check_Radius = false         ;
      bool check_LeadThickness = false  ;
      bool check_Scintillator = false   ;
      bool check_Height = false         ;
      bool check_Width = false          ;
      bool check_Shape = false          ;
      bool check_X = false              ;
      bool check_Y = false              ;
      bool check_Z = false              ;      
      bool ReadingStatus = false        ;

    while (!ConfigFile.eof()) 
       {
         
         getline(ConfigFile, LineBuffer);

         //   If line is a Start Up SiLi bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 5, "SiLi") == 0) 
            {
               cout << "///" << endl ;
               cout << "Platic found: " << endl ;        
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
               else if (DataBuffer.compare(0, 5, "SiLi") == 0) {
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
               
               
               //General
               else if (DataBuffer=="Shape=") {
                  check_Shape = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Shape:  " << DataBuffer << endl;
               }
               
               // Cylindrical shape
               else if (DataBuffer== "Radius=") {
                  check_Radius = true;
                  ConfigFile >> DataBuffer ;
                  cout << "SiLi Radius:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               // Squared shape
               else if (DataBuffer=="Width=") {
                  check_Width = true;
                  ConfigFile >> DataBuffer ;
                  cout << "SiLi Width:  " <<atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               else if (DataBuffer== "Height=") {
                  check_Height = true;
                  ConfigFile >> DataBuffer ;
                  cout << "SiLi Height:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               // Common
               else if (DataBuffer=="Thickness=") {
                  check_Thickness = true;
                  ConfigFile >> DataBuffer ;
                  cout << "SiLi Thickness:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
               
               else if (DataBuffer== "Scintillator=") {
                  check_Scintillator = true ;
                  ConfigFile >> DataBuffer ;
                  cout << "SiLi Scintillator type:  " << DataBuffer << endl;
               }
               
               else if (DataBuffer=="LeadThickness=") {
                  check_LeadThickness = true;
                  ConfigFile >> DataBuffer ;
                  cout << "Lead Thickness :  " << atof( DataBuffer.c_str() ) << "mm" << endl;
               }
                                                
               ///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if ( check_Theta && check_Phi && check_R && check_Thickness && check_Radius &&   check_LeadThickness && check_Scintillator &&   check_Height &&   check_Width && check_Shape && check_X && check_Y && check_Z ) 
                  { 
                     NumberOfDetector++;
                     
                     //   Reinitialisation of Check Boolean  
                     check_Theta = false          ;
                     check_Phi  = false           ;
                     check_R     = false          ;
                     check_Thickness = false      ;
                     check_Radius = false         ;
                     check_LeadThickness = false  ;
                     check_Scintillator = false   ;
                     check_Height = false         ;
                     check_Width = false          ;
                     check_Shape = false          ;
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
void TSiLiPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(int i = 0 ; i < NumberOfDetector ; i++)
         {
                  Cal->AddParameter("SiLi", "Detector"+itoa(i+1)+"_E","SiLi_Detector"+itoa(i+1)+"_E")   ;
                  Cal->AddParameter("SiLi", "Detector"+itoa(i+1)+"_T","SiLi_Detector"+itoa(i+1)+"_T")   ;         
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
      inputChain->SetBranchStatus ( "DetectorENumber", true );
      inputChain->SetBranchStatus ( "DetectorTNumber", true );
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
void TSiLiPhysics::BuildPhysicalEvent()
   {
      BuildSimplePhysicalEvent()   ;
   }

///////////////////////////////////////////////////////////////////////////
void TSiLiPhysics::BuildSimplePhysicalEvent()
   {
      for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
         {
            DetectorNumber.push_back( EventData->GetENumber(i) )   ;
            Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("SiLi/Detector" + itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i) ) );
             Time.push_back( CalibrationManager::getInstance()->ApplyCalibration(   "SiLi/Detector" + itoa( EventData->GetTNumber(i) ) +"_T",EventData->GetTTime(i) ) );
         }

   }
