/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
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
void TExlPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("EXL");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl;

  vector<string> token= {"POS"};
  vector<TVector3> Center_CsI_Crystals;
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      TVector3 pos = blocks[i]->GetTVector3("POS","mm");
      Center_CsI_Crystals.push_back(pos);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
  AddEXL(Center_CsI_Crystals) ;
}
///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddEXL(vector <TVector3> Center_CsI_Crystals)
{     
      // In progress
      // Needs the detectors dimensions
      // Crystals in this detector can be mapped differently for every module
      // Rows_y (0,1,2) Cols_x (0,1,2,3,4,5)
      //local zero is at row 1, col 2.5
      double arbit_length = 100; // detector arbit. face length and width, 100 mm
      double col_pitch = arbit_length/6;
      double row_pitch = arbit_length/3;

	for(unsigned i=0; i<Center_CsI_Crystals.size(); i++)
	{
    TVector3 direction = Center_CsI_Crystals.at(i).Unit();
    //place all 18 crystals in place
	  for(unsigned iCry=0; iCry<18; iCry++)
	  { 
      int Row_y = iCry/6;
      int Col_x = iCry%6;
      // Define Detector position localy 
      TVector3 localPos(  (Row_y-1)*row_pitch,(Col_x-2.5)*row_pitch,0  );
      // Rotate 
      localPos.RotateUz(direction);
      TVector3 globalPos = Center_CsI_Crystals.at(i)+localPos; 
      cout << globalPos.X() << " " << globalPos.Y() << " " << globalPos.Z() << endl; 
		  CsIPosition.push_back(globalPos);
    }
	}
}

///////////////////////////////////////////////////////////////////////////
TVector3 TExlPhysics::GetPositionOfInteraction(int det, int cry)
   {
      int N = det*18+cry; // hyper crystal number
      TVector3 Position = TVector3 (CsIPosition.at(N).X(),CsIPosition.at(N).Y(),CsIPosition.at(N).Z()) ;

      return(Position) ;

   }

///////////////////////////////////////////////////////////////////////////
void TExlPhysics::AddParameterToCalibrationManager()
   {
      CalibrationManager* Cal = CalibrationManager::getInstance();

      for(unsigned int i = 0 ; i < NumberOfCrystal ; i++)
         {
            Cal->AddParameter("EXL", "_E_"+ NPL::itoa(i+1),"EXL_E_"+ NPL::itoa(i+1))   ;
            Cal->AddParameter("EXL", "_T_"+ NPL::itoa(i+1),"EXL_T_"+ NPL::itoa(i+1))   ;
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
void TExlPhysics::BuildSimplePhysicalEvent(){
	if(EventData->GetMult()>0){
		for(unsigned int i = 0 ; i < EventData->GetMult() ; i++){
			if(EventData->GetEnergy(i)>m_E_RAW_Threshold){
				CrystalNumber.push_back( EventData->GetCrystalNumber(i))   ;
				EXL_Energy.push_back( CalibrationManager::getInstance()->ApplyCalibration("EXL/_E_" + NPL::itoa( EventData->GetExlNumber(i) ),EventData->GetEnergy(i) ) );
			}
		}
	}
	else{
		CrystalNumber.push_back(-1000)   ;
		EXL_Energy.push_back(-1000);
	}

	if(EventData->GetMult()>0){
		for(unsigned int i = 0 ; i < EventData->GetMult() ; i++){
			//EXL_Time.push_back( CalibrationManager::getInstance()->ApplyCalibration("EXL/_T_" + NPL::itoa( EventData->GetExlNumber(i) ),EventData->GetTime(i) ) );
			EXL_Time=EventData->GetTime(i);
		}
	}
	else{
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

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TExlPhysics::Construct(){
  return (NPL::VDetector*) new TExlPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_exl{
  public:
    proxy_exl(){
      NPL::DetectorFactory::getInstance()->AddToken("EXL","Exl");
      NPL::DetectorFactory::getInstance()->AddDetector("EXL",TExlPhysics::Construct);
    }
};

proxy_exl p;
}
