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
void TSiLiPhysics::ReadConfiguration(string Path) 
   {
      ifstream ConfigFile           ;
      ConfigFile.open(Path.c_str()) ;
      string LineBuffer             ;
      string DataBuffer             ;

      bool check_Thickness = false      ;
      bool check_Height = false         ;
      bool check_Width = false          ;
      bool check_X = false              ;
      bool check_Y = false              ;
      bool check_Z = false              ;      
      bool ReadingStatus = false        ;

    while (!ConfigFile.eof()) 
       {
         
         getline(ConfigFile, LineBuffer);

         //   If line is a Start Up SiLi bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 4, "SiLi") == 0) 
            {
               cout << "///" << endl ;
               cout << "SiLi found: " << endl ;        
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
               else if (DataBuffer.compare(0, 4, "SiLi") == 0) {
                  cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
                  ReadingStatus = false ;
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
               
                                                
               ///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if ( check_Thickness &&    check_Height &&   check_Width &&  check_X && check_Y && check_Z ) 
                  { 
                     NumberOfDetector++;
                     
                     //   Reinitialisation of Check Boolean  
                     check_Thickness = false      ;
                     check_Height = false         ;
                     check_Width = false          ;
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

