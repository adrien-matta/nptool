/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Jurogam Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TJurogamPhysics.h"

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
#include "TChain.h"

ClassImp(TJurogamPhysics)


///////////////////////////////////////////////////////////////////////////
TJurogamPhysics::TJurogamPhysics()
   : m_EventData(new TJurogamData),
     m_PreTreatedData(new TJurogamData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TJurogamPhysics::AddDetector(TVector3 , string , bool ){
  // In That simple case nothing is done
  // Typically for more complex detector one would calculate the relevant 
  // positions (stripped silicon) or angles (gamma array)
  m_NumberOfDetectors++;
} 

///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::AddDetector(double R, double Theta, double Phi, string shape, bool BGO){
  // Compute the TVector3 corresponding
  TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
  // Call the cartesian method
  AddDetector(Pos,shape,BGO);
} 
  
///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::AddDetector(TVector3 Pos, double Theta, double Phi, string shape, bool BGO){
  // Compute the TVector3 corresponding
	//TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
  // Call the cartesian method
  AddDetector(Pos,shape,BGO);
} 
///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // match energy and time together
  unsigned int mysizeE = m_PreTreatedData->GetMultEnergy();
  unsigned int mysizeT = m_PreTreatedData->GetMultTime();
  for (UShort_t e = 0; e < mysizeE ; e++) {
    for (UShort_t t = 0; t < mysizeT ; t++) {
      if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(t)) {
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        Energy.push_back(m_PreTreatedData->Get_Energy(e));
        Time.push_back(m_PreTreatedData->Get_Time(t));
      }
    }
  }
  unsigned int mysizeBGOE = m_PreTreatedData->Get_BGO_MultEnergy();
  unsigned int mysizeBGOT = m_PreTreatedData->Get_BGO_MultTime();
  for (UShort_t e = 0; e < mysizeBGOE ; e++) {
    for (UShort_t t = 0; t < mysizeBGOT ; t++) {
      if (m_PreTreatedData->Get_BGO_E_DetectorNbr(e) == m_PreTreatedData->Get_BGO_T_DetectorNbr(t)) {
        //DetectorNumber.push_back(m_PreTreatedData->Get_BGO_E_DetectorNbr(e));
        BGO_E.push_back(m_PreTreatedData->Get_BGO_Energy(e));
        //Time.push_back(m_PreTreatedData->Get_BGO_Time(t));
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Energy
  unsigned int mysize = m_EventData->GetMultEnergy();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Jurogam/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  mysize = m_EventData->GetMultTime();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("Jurogam/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }

  // Energy
  mysize = m_EventData->Get_BGO_MultEnergy();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_BGO_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Jurogam/BGO_ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->Set_BGO_Energy(m_EventData->Get_BGO_E_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  mysize = m_EventData->Get_BGO_MultTime();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("Jurogam/BGO_TIME"+NPL::itoa(m_EventData->Get_BGO_T_DetectorNbr(i)),m_EventData->Get_BGO_Time(i));
    m_PreTreatedData->Set_BGO_Time(m_EventData->Get_BGO_T_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigJurogam.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigJurogam.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigJurogam.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigJurogam.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigJurogam";
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

      else if (whatToDo=="E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_RAW_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_E_Threshold = atof(DataBuffer.c_str());
        cout << whatToDo << " " << m_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }
    }
  }
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
  BGO_E.clear();
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Jurogam");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","Shape","BGO"};
  vector<string> sphe = {"R","Theta","Phi","Shape","BGO"};
  vector<string> combi = {"Loc","Theta","Phi","Shape","BGO"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Jurogam " << i+1 <<  endl;
    
      TVector3 Pos = blocks[i]->GetTVector3("POS","mm");
      string Shape = blocks[i]->GetString("Shape");
	  int BGO_int = blocks[i]->GetInt("BGO");
	  bool BGO = false;
	  if (BGO_int == 1) BGO = true;
      AddDetector(Pos,Shape,BGO);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Jurogam " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
	  int BGO_int = blocks[i]->GetInt("BGO");
	  bool BGO = false;
	  if (BGO_int == 1) BGO = true;
      AddDetector(R,Theta,Phi,Shape,BGO);
    }
    else if(blocks[i]->HasTokenList(combi)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Jurogam " << i+1 <<  endl;
      TVector3 Loc = blocks[i]->GetTVector3("Loc","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
	  int BGO_int = blocks[i]->GetInt("BGO");
	  bool BGO = false;
	  if (BGO_int == 1) BGO = true;
      AddDetector(Loc,Theta,Phi,Shape,BGO);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::InitSpectra() {
  m_Spectra = new TJurogamSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TJurogamPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
//vector<TCanvas*> TJurogamPhysics::GetCanvas() {
//  if(m_Spectra)
//    return m_Spectra->GetCanvas();
//  else{
//    vector<TCanvas*> empty;
//    return empty;
//  }
//}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("Jurogam", "D"+ NPL::itoa(i+1)+"_ENERGY","Jurogam_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("Jurogam", "D"+ NPL::itoa(i+1)+"_TIME","Jurogam_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Jurogam",  true );
  inputChain->SetBranchAddress("Jurogam", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("Jurogam", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TJurogamPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Jurogam", "TJurogamPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TJurogamPhysics::Construct() {
  return (NPL::VDetector*) new TJurogamPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_Jurogam{
  public:
    proxy_Jurogam(){
      NPL::DetectorFactory::getInstance()->AddToken("Jurogam","Jurogam");
      NPL::DetectorFactory::getInstance()->AddDetector("Jurogam",TJurogamPhysics::Construct);
    }
};

proxy_Jurogam p_Jurogam;
}

