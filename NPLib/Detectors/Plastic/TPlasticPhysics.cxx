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
 *  This class hold Plastic  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TPlasticPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

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

ClassImp(TPlasticPhysics)
  ///////////////////////////////////////////////////////////////////////////
TPlasticPhysics::TPlasticPhysics()
{      
  NumberOfDetector = 0 ;
  EventData = new TPlasticData ;
  EventPhysics = this ;
}

///////////////////////////////////////////////////////////////////////////
TPlasticPhysics::~TPlasticPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::Clear()
{
  DetectorNumber.clear() ;
  Energy.clear() ;
  Time.clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Plastic");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"X","Y","Z"};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> square= {"Shape","Height","Width","Thickness","Scintillator","LeadThickness"};
  vector<string> cylind= {"Shape","Radius","Thickness","Scintillator","LeadThickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double R = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
      }

    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double R = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
      }

    }


    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < NumberOfDetector ; i++)
  {
    for( int j = 0 ; j < 16 ; j++)
    {
      Cal->AddParameter("Plastic", "Detector"+ NPL::itoa(i+1)+"_E","Plastic_Detector"+ NPL::itoa(i+1)+"_E")   ;
      Cal->AddParameter("Plastic", "Detector"+ NPL::itoa(i+1)+"_T","Plastic_Detector"+ NPL::itoa(i+1)+"_T")   ;   
    }

  }
}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::InitializeRootInputRaw() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain()     ;
  inputChain->SetBranchStatus ( "Plastic"       , true )        ;
  inputChain->SetBranchStatus ( "fPlastic_*"    , true )        ;
  inputChain->SetBranchAddress( "Plastic"       , &EventData )  ;
}
///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus ( "Plastic", true );
  inputChain->SetBranchStatus ( "DetectorNumber", true );
  inputChain->SetBranchStatus ( "Energy", true );
  inputChain->SetBranchStatus ( "Time", true );
  inputChain->SetBranchAddress( "Plastic", &EventPhysics );
}
///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
  outputTree->Branch( "Plastic" , "TPlasticPhysics" , &EventPhysics ) ;
}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::BuildPhysicalEvent()
{
  BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TPlasticPhysics::BuildSimplePhysicalEvent()
{
  for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
  {
    DetectorNumber.push_back( EventData->GetPlasticNumber(i) )   ;
    Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("Plastic/Detector" + NPL::itoa( EventData->GetPlasticNumber(i) ) +"_E",EventData->GetEnergy(i) ) );
    Time.push_back( CalibrationManager::getInstance()->ApplyCalibration(   "Plastic/Detector" + NPL::itoa( EventData->GetPlasticNumber(i) ) +"_T",EventData->GetTime(i) ) );
  }

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TPlasticPhysics::Construct(){
  return (NPL::VDetector*) new TPlasticPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_plastic{
  public:
    proxy_plastic(){
      NPL::DetectorFactory::getInstance()->AddToken("Plastic","Plastic");
      NPL::DetectorFactory::getInstance()->AddDetector("Plastic",TPlasticPhysics::Construct);
    }
};

proxy_plastic p;
}

