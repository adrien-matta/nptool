/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre    contact address: lefebvrl@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date  : October 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Exl Detector  Physics                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TExlPhysics.h"
#include "../include/RootOutput.h"
#include "../include/RootInput.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
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

ClassImp(TExlPhysics)
///////////////////////////////////////////////////////////////////////////
TExlPhysics::TExlPhysics()
   {      
      NumberOfCrystal = 0;
      EventData = new TExlData ;
      EventPhysics = this ;
// Raw Threshold
      m_E_RAW_Threshold = 0;
   }
   
///////////////////////////////////////////////////////////////////////////
TExlPhysics::~TExlPhysics()
   {}
   
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::Clear()
   {
      CrystalNumber.clear() ;
      EXL_Energy.clear() ;
      EXL_Time=-1000 ;
   }
   
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::ReadConfiguration(string Path) 
   {

      ifstream ConfigFile           ;
      ConfigFile.open(Path.c_str()) ;
      string LineBuffer             ;
      string DataBuffer             ;

      double Centerx , Centery , Centerz;
      double CenterR, CenterTheta, CenterPhi;
      TVector3 Center;
      vector <TVector3> Center_CsI_Crystals;

      bool check_CsI_X_Y_Z = false ;
      bool check_NumberOfCrystal = false ;
      bool check_CsI_R_Theta_Phi = false ;
      bool ReadingStatus = false        ;

    while (!ConfigFile.eof()) 
       {
         
         getline(ConfigFile, LineBuffer);

         //   If line is a Start Up EXL bloc, Reading toggle to true      
         if (LineBuffer.compare(0, 3, "EXL") == 0) 
            {
               cout << "///" << endl ;
               cout << "EXL found ! " << endl ;        
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
               else if (DataBuffer.compare(0, 3, "EXL") == 0) {
                  cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in EXL definition" << endl ;
                  ReadingStatus = false ;
               }

 		else if (DataBuffer=="NumberOfCrystal=") {
                  check_NumberOfCrystal = true;
                  ConfigFile >> DataBuffer ;
		  NumberOfCrystal=atof(DataBuffer.c_str());
                  cout << "NumberOfCrystal:  " << atof(DataBuffer.c_str()) << endl;
               }
                              
		//   Position method
               else if (DataBuffer=="CsI_X_Y_Z=") {
                  check_CsI_X_Y_Z = true;
                  ConfigFile >> DataBuffer ;
                  Centerx = atof(DataBuffer.c_str()) ;
                  Centerx = Centerx  ;
                  ConfigFile >> DataBuffer ;
                  Centery = atof(DataBuffer.c_str()) ;
                  Centery = Centery  ;
                  ConfigFile >> DataBuffer ;
                  Centerz = atof(DataBuffer.c_str()) ;
                  Centerz = Centerz  ;

                  Center = TVector3(Centerx, Centery, Centerz);
		  Center_CsI_Crystals.push_back(Center);
                  cout << "CsI crystal position : (" << Center.X() << ";" << Center.Y() << ";" << Center.Z() << ")" << endl;   
               }

		//   Angle method
               else if (DataBuffer=="CsI_R_Theta_Phi=") {
                  check_CsI_R_Theta_Phi = true;
                  ConfigFile >> DataBuffer ;
                  CenterR = atof(DataBuffer.c_str()) ;
                  CenterR = CenterR  ;
                  ConfigFile >> DataBuffer ;
                  CenterTheta = atof(DataBuffer.c_str()) ;
                  CenterTheta = CenterTheta*3.141592654/180  ;
                  ConfigFile >> DataBuffer ;
                  CenterPhi = atof(DataBuffer.c_str()) ;
                  CenterPhi = CenterPhi*3.141592654/180  ;

                  Center = TVector3(CenterR*sin(CenterTheta)*cos(CenterPhi), CenterR*sin(CenterTheta)*sin(CenterPhi), CenterR*cos(CenterTheta));
		  Center_CsI_Crystals.push_back(Center);
                  cout << "CsI crystal position : (" << Center.X() << ";" << Center.Y() << ";" << Center.Z() << ")" << endl;   
               }


                                                
               ///////////////////////////////////////////////////
               //   If no Detector Token and no comment, toggle out
               else 
                  {ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
               
                  /////////////////////////////////////////////////
                  //   If All necessary information there, toggle out
               
               if ((NumberOfCrystal == Center_CsI_Crystals.size() && check_CsI_R_Theta_Phi && check_NumberOfCrystal) || (NumberOfCrystal == Center_CsI_Crystals.size() && check_CsI_X_Y_Z && check_NumberOfCrystal)) 
                  { 
		      //with angle method
                      if (check_CsI_R_Theta_Phi) 
                        {
                           AddEXL(Center_CsI_Crystals) ;
                        }
                      //With position method
                      if (check_CsI_X_Y_Z) 
                        {
                           AddEXL(Center_CsI_Crystals) ;
                        }
                     //   Reinitialisation of Check Boolean  
                     ReadingStatus = false; 
		     check_NumberOfCrystal = false;
	             check_CsI_X_Y_Z = false;
                     check_CsI_R_Theta_Phi  = false ;
		     Center_CsI_Crystals.clear();
                     cout << "///"<< endl ;                
                  }
            }
      }

   }
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddEXL(vector <TVector3> Center_CsI_Crystals)
{
	for(unsigned i=0; i<Center_CsI_Crystals.size(); i++)
	{
		CsIPosition.push_back(Center_CsI_Crystals.at(i));
	}
}

///////////////////////////////////////////////////////////////////////////
TVector3 TExlPhysics::GetPositionOfInteraction(int N)
   {
      TVector3 Position = TVector3 (CsIPosition.at(N).X(),CsIPosition.at(N).Y(),CsIPosition.at(N).Z()) ;
      
      return(Position) ;   
   
   }

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();
      
      for(unsigned int i = 0 ; i < NumberOfCrystal ; i++)
         {
            Cal->AddParameter("EXL", "_E_"+itoa(i+1),"EXL_E_"+itoa(i+1))   ;
            Cal->AddParameter("EXL", "_T_"+itoa(i+1),"EXL_T_"+itoa(i+1))   ;
         }
   }
   
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()     ;
      inputChain->SetBranchStatus ( "Exl"       , true )        ;
      inputChain->SetBranchStatus ( "fExl_*"    , true )        ;
      inputChain->SetBranchAddress( "Exl"       , &EventData )  ;
   }
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootInputPhysics()
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus ( "Exl", true );
      inputChain->SetBranchStatus ( "CrystalNumber", true );
      inputChain->SetBranchStatus ( "EXL_Energy", true );
      inputChain->SetBranchStatus ( "EXL_Time", true );
      inputChain->SetBranchAddress( "Exl", &EventPhysics );
   }
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::InitializeRootOutput()
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
      outputTree->Branch( "Exl" , "TExlPhysics" , &EventPhysics ) ;
   }

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::BuildPhysicalEvent()
   {
      BuildSimplePhysicalEvent()   ;
   }

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::BuildSimplePhysicalEvent()
{
	if(EventData->GetEnergyMult()>0)
	{
		for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
		{
			if(EventData->GetEnergy(i)>m_E_RAW_Threshold)
			{
				CrystalNumber.push_back( EventData->GetExlNumber(i))   ;
				EXL_Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("EXL/_E_" + itoa( EventData->GetExlNumber(i) ),EventData->GetEnergy(i) ) );
			}
		}
	}
	else
	{
		CrystalNumber.push_back(-1000)   ;
		EXL_Energy.push_back(-1000);
	}

	if(EventData->GetTimeMult()>0)
	{
		for(unsigned int i = 0 ; i < EventData->GetTimeMult() ; i++)
		{
			//EXL_Time.push_back( CalibrationManager::getInstance()->ApplyCalibration("EXL/_T_" + itoa( EventData->GetExlNumber(i) ),EventData->GetTime(i) ) );
			EXL_Time=EventData->GetTime(i);
		}
	}
	else
	{
		EXL_Time=-1000;
	}
}

///////////////////////////////////////////////////////////////////////////
double TExlPhysics::DopplerCorrection(double E, double Theta, double beta)
{
  double Pi = 3.141592654 ;

  double E_corr = 0;
  double gamma = 1./sqrt(1-beta*beta);

  E_corr = gamma*E*(1.-beta*cos(Theta*Pi/180.)); 
  
  return(E_corr);

}

