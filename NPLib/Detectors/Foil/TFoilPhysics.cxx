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
 *  This class hold Foil  Physics                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TFoilPhysics.h"
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

ClassImp(TFoilPhysics)
  ///////////////////////////////////////////////////////////////////////////
TFoilPhysics::TFoilPhysics()
{      
  NumberOfDetector = 0 ;
  EventData = new TFoilData ;
  EventPhysics = this ;
}

///////////////////////////////////////////////////////////////////////////
TFoilPhysics::~TFoilPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::Clear()
{
  DetectorNumber.clear() ;
  Energy.clear() ;
  Time.clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Foil");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS"};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> square= {"Shape","Height","Width","Thickness","Material"};
  vector<string> cylind= {"Shape","Radius","Thickness","Material"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Foil " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Material");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double R = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Material");
      }

    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Foil " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Material");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double R = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Material");
      }

    }


    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::AddParameterToCalibrationManager()
{
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < NumberOfDetector ; i++)
  {
    for( int j = 0 ; j < 16 ; j++)
    {
      Cal->AddParameter("Foil", "Detector"+ NPL::itoa(i+1)+"_E","Foil_Detector"+ NPL::itoa(i+1)+"_E")   ;
      Cal->AddParameter("Foil", "Detector"+ NPL::itoa(i+1)+"_T","Foil_Detector"+ NPL::itoa(i+1)+"_T")   ;   
    }

  }
}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::InitializeRootInputRaw() 
{
  TChain* inputChain = RootInput::getInstance()->GetChain()     ;
  inputChain->SetBranchStatus ( "Foil"       , true )        ;
  inputChain->SetBranchStatus ( "fFoil_*"    , true )        ;
  inputChain->SetBranchAddress( "Foil"       , &EventData )  ;
}
///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::InitializeRootInputPhysics()
{
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus ( "Foil", true );
  inputChain->SetBranchStatus ( "DetectorNumber", true );
  inputChain->SetBranchStatus ( "Energy", true );
  inputChain->SetBranchStatus ( "Time", true );
  inputChain->SetBranchAddress( "Foil", &EventPhysics );
}
///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::InitializeRootOutput()
{
  TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
  outputTree->Branch( "Foil" , "TFoilPhysics" , &EventPhysics ) ;
}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::BuildPhysicalEvent()
{
  BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TFoilPhysics::BuildSimplePhysicalEvent()
{
  for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
  {
    DetectorNumber.push_back( EventData->GetFoilNumber(i) )   ;
    static string str;
    str = "Foil/Detector" + NPL::itoa( EventData->GetFoilNumber(i) ) +"_E";
    Energy.push_back(CalibrationManager::getInstance()->ApplyCalibration(str ,EventData->GetEnergy(i) ) );
    str = "Foil/Detector" + NPL::itoa( EventData->GetFoilNumber(i) ) +"_T";
    Time.push_back(CalibrationManager::getInstance()->ApplyCalibration(str ,EventData->GetTime(i) ) );
  }

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TFoilPhysics::Construct(){
  return (NPL::VDetector*) new TFoilPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_foil{
  public:
    proxy_foil(){
      NPL::DetectorFactory::getInstance()->AddToken("Foil","Foil");
      NPL::DetectorFactory::getInstance()->AddDetector("Foil",TFoilPhysics::Construct);
    }
};

proxy_foil p;
}

