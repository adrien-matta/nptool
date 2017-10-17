/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : October 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Treated  data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TMDMPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//   ROOT
#include "TMath.h"
#include "TChain.h"

ClassImp(TMDMPhysics)

namespace {

const double ZPOS_[4] = { 2.0, 17.1, 33.4, 49.7 }; // cm

}


///////////////////////////////////////////////////////////////////////////
TMDMPhysics::TMDMPhysics()
: m_EventData(new TMDMData),
m_PreTreatedData(new TMDMData),
m_EventPhysics(this),
m_Spectra(0),
m_X_Threshold(1000000), // junk value
m_Y_Threshold(1000000), // junk value
m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TMDMPhysics::AddDetector(){
  // In That simple case nothing is done
  // Typically for more complex detector one would calculate the relevant 
  // positions (stripped silicon) or angles (gamma array)
  m_NumberOfDetectors++;
} 

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // match x and y
  UInt_t mysizeX = m_PreTreatedData->GetMultX();
  UInt_t mysizeY = m_PreTreatedData->GetMultY();
	UInt_t i0=-1, i1=-1;
  for (UShort_t ix = 0; ix < mysizeX ; ix++) {
    for (UShort_t iy = 0; iy < mysizeY ; iy++) {
      if (m_PreTreatedData->GetX_DetectorNbr(ix) == m_PreTreatedData->GetY_DetectorNbr(iy)) {
				int detno = m_PreTreatedData->GetX_DetectorNbr(ix);

				if(detno >= 0 && detno < 4) {
					DetectorNumber.push_back(detno);
					Xpos.push_back(m_PreTreatedData->Get_Xpos(ix));
					Ypos.push_back(m_PreTreatedData->Get_Ypos(iy));
					Zpos.push_back(ZPOS_[detno]);
				}
						
				if(detno == 0) { i0 = ix; }
				if(detno == 1) { i1 = ix; }
      }
    }
  }

	// calculate angles from first 2 wires
	if(i0!=-1 && i1!=-1) {
		Xang = atan((Xpos[i1] - Xpos[i0]) / (Zpos[i1] - Zpos[i0]));
		Yang = atan((Ypos[i1] - Ypos[i0]) / (Zpos[i1] - Zpos[i0]));
		Xang *= (180/TMath::Pi());
		Yang *= (180/TMath::Pi());
	}
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // X - position
  UInt_t mysize = m_EventData->GetMultX();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Xpos(i) < m_X_Threshold) {
      Double_t Xpos = Cal->ApplyCalibration("MDM/XPOS"+NPL::itoa(m_EventData->GetX_DetectorNbr(i)),m_EventData->Get_Xpos(i));
      if (true) {
        m_PreTreatedData->SetXpos(m_EventData->GetX_DetectorNbr(i), Xpos);
      }
    }
  }

	// Y - position
  mysize = m_EventData->GetMultY();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Ypos(i) < m_Y_Threshold) {
      Double_t Ypos = Cal->ApplyCalibration("MDM/YPOS"+NPL::itoa(m_EventData->GetY_DetectorNbr(i)),m_EventData->Get_Ypos(i));
      if (true) {
        m_PreTreatedData->SetYpos(m_EventData->GetY_DetectorNbr(i), Ypos);
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigMDM.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigMDM.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigMDM.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigMDM.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigMDM";
    if (LineBuffer.compare(0, name.length(), name) == 0) 
      ReadingStatus = true;

    // loop on tokens and data
    while (ReadingStatus ) {
      whatToDo="";
      AnalysisConfigFile >> whatToDo;

      // Search for comment symbol (%)
      if (whatToDo.compare(0, 1, "%") == 0) {
        AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
      }

      else if (whatToDo=="X_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_X_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_X_Threshold << endl;
      }

      else if (whatToDo=="Y_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_Y_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_Y_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }
    }
  }
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::Clear() {
  DetectorNumber.clear();
  Xpos.clear();
  Ypos.clear();
	Zpos.clear();
	Xang = -1000;
	Yang = -1000;
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("MDM");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> tokens = {"Angle","Field","Xaccept","Yaccept","Rayin"};

  for(UInt_t i = 0 ; i < blocks.size() ; i++){
		if(blocks[i]->HasTokenList(tokens)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  MDM " << i+1 <<  endl;
      double Angle = blocks[i]->GetDouble("Angle","deg");
			double Field = blocks[i]->GetDouble("Field","gauss");
      double XA    = blocks[i]->GetDouble("XAccept","deg");
      double YA    = blocks[i]->GetDouble("YAccept","deg");
      string Rayin = blocks[i]->GetString("Rayin");
      AddDetector();
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitSpectra() {
  m_Spectra = new TMDMSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TMDMPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}

///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("MDM", "D"+ NPL::itoa(i+1)+"_ENERGY","MDM_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("MDM", "D"+ NPL::itoa(i+1)+"_TIME","MDM_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("MDM",  true );
  inputChain->SetBranchAddress("MDM", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("MDM", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMDMPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("MDM", "TMDMPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TMDMPhysics::Construct() {
  return (NPL::VDetector*) new TMDMPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_MDM{
public:
	proxy_MDM(){
		NPL::DetectorFactory::getInstance()->AddToken("MDM","MDM");
		NPL::DetectorFactory::getInstance()->AddDetector("MDM",TMDMPhysics::Construct);
	}
};

proxy_MDM p_MDM;
}

