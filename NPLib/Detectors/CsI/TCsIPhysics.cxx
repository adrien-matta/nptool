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
 *  This class hold CsI  Physics                                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TCsIPhysics.h"
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPCalibrationManager.h"
#include "NPOptionManager.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include <cmath>
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

ClassImp(TCsIPhysics)
///////////////////////////////////////////////////////////////////////////
TCsIPhysics::TCsIPhysics()
{
    NumberOfDetector = 0 ;
    EventData = new TCsIData ;
    EventPhysics = this ;
}

///////////////////////////////////////////////////////////////////////////
TCsIPhysics::~TCsIPhysics()
{}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::Clear()
{
    DetectorNumber.clear() ;
    Energy.clear() ;
    Time.clear() ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("CsI");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"X","Y","Z"};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> trapez= {"Shape","FaceFront","FaceBack","Thickness","Scintillator","LeadThickness"};
  vector<string> cylind= {"Shape","Radius","Thickness","Scintillator","LeadThickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      double R = sqrt (X*X+Y*Y+Z*Z);
      double Theta = acos(Z / (R) );
      double Phi = atan2(Y,X);

      if(blocks[i]->HasTokenList(trapez)){
        string Shape = blocks[i]->GetString("Shape");
        double FaceFront = blocks[i]->GetDouble("FaceFront","mm");
        double FaceBack = blocks[i]->GetDouble("FaceBack","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator= blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Radius = blocks[i]->GetDouble("Radius","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator= blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
      }

    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");

      if(blocks[i]->HasTokenList(trapez)){
        string Shape = blocks[i]->GetString("Shape");
        double FaceFront = blocks[i]->GetDouble("FaceFront","mm");
        double FaceBack = blocks[i]->GetDouble("FaceBack","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator = blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Radius = blocks[i]->GetDouble("Radius","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator = blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
      }

    }


    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::AddParameterToCalibrationManager()
{
    CalibrationManager* Cal = CalibrationManager::getInstance();
    
    for(int i = 0 ; i < NumberOfDetector ; i++)
    {
        for( int j = 0 ; j < 16 ; j++)
        {
            Cal->AddParameter("CsI", "Detector"+itoa(i+1)+"_E","CsI_Detector"+itoa(i+1)+"_E")   ;
            Cal->AddParameter("CsI", "Detector"+itoa(i+1)+"_T","CsI_Detector"+itoa(i+1)+"_T")   ;
        }
        
    }
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootInputRaw()
{
    TChain* inputChain = RootInput::getInstance()->GetChain()     ;
    inputChain->SetBranchStatus ( "CsI"       , true )        ;
    inputChain->SetBranchStatus ( "fCsI_*"    , true )        ;
    inputChain->SetBranchAddress( "CsI"       , &EventData )  ;
}
///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootInputPhysics()
{
    TChain* inputChain = RootInput::getInstance()->GetChain();
    inputChain->SetBranchStatus ( "CsI", true );
    inputChain->SetBranchStatus ( "DetectorNumber", true );
    inputChain->SetBranchStatus ( "Energy", true );
    inputChain->SetBranchStatus ( "Time", true );
    inputChain->SetBranchAddress( "CsI", &EventPhysics );
}
///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::InitializeRootOutput()
{
    TTree* outputTree = RootOutput::getInstance()->GetTree()            ;
    outputTree->Branch( "CsI" , "TCsIPhysics" , &EventPhysics ) ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::BuildPhysicalEvent()
{
    BuildSimplePhysicalEvent()   ;
}

///////////////////////////////////////////////////////////////////////////
void TCsIPhysics::BuildSimplePhysicalEvent()
{
    for(unsigned int i = 0 ; i < EventData->GetEnergyMult() ; i++)
    {
        DetectorNumber.push_back( EventData->GetENumber(i) )   ;
        Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("CsI/Detector" + itoa( EventData->GetENumber(i) ) +"_E",EventData->GetEEnergy(i) ) );
        Time.push_back( CalibrationManager::getInstance()->ApplyCalibration(   "CsI/Detector" + itoa( EventData->GetENumber(i) ) +"_T",EventData->GetTTime(i) ) );
    }
    
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TCsIPhysics::Construct(){
    return (NPL::VDetector*) new TCsIPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
    class proxy_csi{
    public:
        proxy_csi(){
            NPL::DetectorFactory::getInstance()->AddToken("CsI","CsI");
            NPL::DetectorFactory::getInstance()->AddDetector("CsI",TCsIPhysics::Construct);
        }
    };
    
    proxy_csi p;
}

