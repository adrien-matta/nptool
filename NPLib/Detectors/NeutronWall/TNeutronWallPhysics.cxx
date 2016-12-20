/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : June 2016                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Treated  data                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TNeutronWallPhysics.h"

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

ClassImp(TNeutronWallPhysics)


  ///////////////////////////////////////////////////////////////////////////
TNeutronWallPhysics::TNeutronWallPhysics()
  : m_EventData(new TNeutronWallData),
  m_PreTreatedData(new TNeutronWallData),
  m_EventPhysics(this),
  m_Spectra(0),
  m_E_RAW_Threshold(0), // adc channels
  m_E_Threshold(0),     // MeV
  m_NumberOfDetectors(0) {
  }



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // Neutron Wall
  for (UShort_t e = 0; e < m_PreTreatedData->GetMultEnergy(); e++) {
    if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(e)) {
      DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
      NW_PadNumber.push_back(m_PreTreatedData->GetE_PadNbr(e));
      NW_Energy.push_back(m_PreTreatedData->Get_Energy(e));
      NW_Time.push_back(m_PreTreatedData->Get_Time(e));

      NW_PosX.push_back(m_PreTreatedData->Get_NeutronWall_PosX(e));
      NW_PosY.push_back(m_PreTreatedData->Get_NeutronWall_PosY(e));
      NW_PosZ.push_back(m_PreTreatedData->Get_NeutronWall_PosZ(e));
    }
  }


  // Veto Wall
  for (UShort_t e = 0; e < m_PreTreatedData->GetVetoMultEnergy(); e++) {
    if (m_PreTreatedData->GetE_VetoDetectorNbr(e) == m_PreTreatedData->GetT_VetoDetectorNbr(e)) {
      VW_PadNumber.push_back(m_PreTreatedData->GetE_VetoPadNbr(e));
      VW_Energy.push_back(m_PreTreatedData->Get_VetoEnergy(e));
      VW_Time.push_back(m_PreTreatedData->Get_VetoTime(e));

      VW_PosX.push_back(m_PreTreatedData->Get_VetoWall_PosX(e));
      VW_PosY.push_back(m_PreTreatedData->Get_VetoWall_PosY(e));
      VW_PosZ.push_back(m_PreTreatedData->Get_VetoWall_PosZ(e));
    }
  }

}

///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Neutron Wall
  // Energy
  for (UShort_t i = 0; i < m_EventData->GetMultEnergy(); ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t EnergyNW = Cal->ApplyCalibration("NeutronWall/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (EnergyNW > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i),m_EventData->GetE_PadNbr(i), EnergyNW);
      }
    }
  }
  // Time
  for (UShort_t i = 0; i < m_EventData->GetMultTime(); ++i) {
    Double_t TimeNW= Cal->ApplyCalibration("NeutronWall/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i),m_EventData->GetT_PadNbr(i), TimeNW);
  }
  // Position
  for(UShort_t i = 0; i < m_EventData->GetMult_NeutronWall_Position(); ++i){
    m_PreTreatedData->Set_NeutronWall_PosX(m_EventData->Get_NeutronWall_PosX(i));
    m_PreTreatedData->Set_NeutronWall_PosY(m_EventData->Get_NeutronWall_PosY(i));
    m_PreTreatedData->Set_NeutronWall_PosZ(m_EventData->Get_NeutronWall_PosZ(i));
  }


  // Veto Wall
  // Energy
  for (UShort_t i = 0; i < m_EventData->GetVetoMultEnergy(); ++i) {
    if (m_EventData->Get_VetoEnergy(i) > m_E_RAW_Threshold) {
      Double_t EnergyVW = Cal->ApplyCalibration("VetoWall/ENERGY"+NPL::itoa(m_EventData->GetE_VetoPadNbr(i)),m_EventData->Get_VetoEnergy(i));

      if (EnergyVW > m_E_Threshold) {
        m_PreTreatedData->SetVetoEnergy(m_EventData->GetE_VetoDetectorNbr(i),m_EventData->GetE_VetoPadNbr(i), EnergyVW);
      }
    }
  }
  //Time
  for (UShort_t i = 0; i < m_EventData->GetVetoMultTime(); ++i) {
    Double_t TimeVW= Cal->ApplyCalibration("VetoWall/TIME"+NPL::itoa(m_EventData->GetT_VetoPadNbr(i)),m_EventData->Get_VetoTime(i));
    m_PreTreatedData->SetVetoTime(m_EventData->GetT_VetoDetectorNbr(i),m_EventData->GetT_VetoPadNbr(i), TimeVW);
  }
  // Position
  for(UShort_t i = 0; i < m_EventData->GetMult_VetoWall_Position(); ++i){
    m_PreTreatedData->Set_VetoWall_PosX(m_EventData->Get_VetoWall_PosX(i));
    m_PreTreatedData->Set_VetoWall_PosY(m_EventData->Get_VetoWall_PosY(i));
    m_PreTreatedData->Set_VetoWall_PosZ(m_EventData->Get_VetoWall_PosZ(i));
  }







}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ReadConfiguration(NPL::InputParser parser) {

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("NeutronWall");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"R","THETA","PHI","BARS","VETOWALL","VWDISTANCE","NWMATERIAL","OVERLAP"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Neutron Wall " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      int Bars = blocks[i]->GetInt("BARS");
      int VetoWall = blocks[i]->GetInt("VETOWALL");
      double VWDistance = blocks[i]->GetDouble("VWDISTANCE","mm");
      string NWMaterial = blocks[i]->GetString("NWMATERIAL");
      int Overlap = blocks[i]->GetInt("OVERLAP");
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  ReadAnalysisConfig();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::Clear() {
  DetectorNumber.clear();
  NW_PadNumber.clear();
  NW_Energy.clear();
  NW_Time.clear();
  VW_PadNumber.clear();
  VW_Energy.clear();
  VW_Time.clear();

  NW_PosX.clear();
  NW_PosY.clear();
  NW_PosZ.clear();
  VW_PosX.clear();
  VW_PosY.clear();
  VW_PosZ.clear();
}


///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigNeutronWall.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigNeutronWall.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigNeutronWall.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigNeutronWall.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigNeutronWall";
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
void TNeutronWallPhysics::InitSpectra() {
  m_Spectra = new TNeutronWallSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TNeutronWallPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TNeutronWallPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("NeutronWall", "D"+ NPL::itoa(i+1)+"_ENERGY","NeutronWall_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("NeutronWall", "D"+ NPL::itoa(i+1)+"_TIME","NeutronWall_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("NeutronWall",  true );
  inputChain->SetBranchAddress("NeutronWall", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("NeutronWall", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TNeutronWallPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("NeutronWall", "TNeutronWallPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TNeutronWallPhysics::Construct() {
  return (NPL::VDetector*) new TNeutronWallPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_NeutronWall{
  public:
    proxy_NeutronWall(){
      NPL::DetectorFactory::getInstance()->AddToken("NeutronWall","NeutronWall");
      NPL::DetectorFactory::getInstance()->AddDetector("NeutronWall",TNeutronWallPhysics::Construct);
    }
};

proxy_NeutronWall p_NeutronWall;
}

