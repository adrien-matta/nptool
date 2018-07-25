/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "TActarPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

//   NPLNPSystemOfUnits.h
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"
#include "NPSystemOfUnits.h"

//   ROOT
#include "TChain.h"
#include "TH2F.h"
#include "TCanvas.h"

ClassImp(TActarPhysics)


///////////////////////////////////////////////////////////////////////////
TActarPhysics::TActarPhysics()
   : m_EventData(new TActarData),
     m_PreTreatedData(new TActarData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}

///////////////////////////////////////////////////////////////////////////
/// A usefull method to bundle all operation to add a detector
void TActarPhysics::AddDetector(TVector3 , string ){
  // In That simple case nothing is done
  // Typically for more complex detector one would calculate the relevant
  // positions (stripped silicon) or angles (gamma array)
  m_NumberOfDetectors++;
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::AddDetector(double R, double Theta, double Phi, string shape){
  // Compute the TVector3 corresponding
  TVector3 Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
  // Call the cartesian method
  AddDetector(Pos,shape);
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration

  PreTreat();

  // match Charge and time together
  unsigned int mysizeE = m_PreTreatedData->GetMultCharge();

  for (UShort_t e = 0; e < mysizeE ; e++) {
    PadNumber.push_back(m_PreTreatedData->Get_PadNumber(e));
    PadRow.push_back(m_PreTreatedData->Get_PadRow(e));
    PadColumn.push_back(m_PreTreatedData->Get_PadColumn(e));
    PadCharge.push_back(m_PreTreatedData->Get_Charge(e));
    PadTime.push_back(m_PreTreatedData->Get_Time(e));
  }

  HoughTransform(PadRow, PadColumn, PadTime);
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Charge
  unsigned int mysize = m_EventData->GetMultCharge();

  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->Get_Charge(i) > m_E_RAW_Threshold) {
      //Double_t Charge = Cal->ApplyCalibration("Actar/CHARGE"+NPL::itoa(m_EventData->Get_PadNumber(i)),m_EventData->Get_Charge(i));
      //Double_t Time= Cal->ApplyCalibration("Actar/TIME"+NPL::itoa(m_EventData->Get_PadNumber(i)),m_EventData->Get_Time(i));
      if (m_EventData->Get_Charge(i) > m_E_Threshold) {
        m_PreTreatedData->SetCharge(m_EventData->Get_Charge(i));
        m_PreTreatedData->SetPadNumber(m_EventData->Get_PadNumber(i));
        m_PreTreatedData->SetRowNumber(m_EventData->Get_PadRow(i));
        m_PreTreatedData->SetColumnNumber(m_EventData->Get_PadColumn(i));
        m_PreTreatedData->SetTime(m_EventData->Get_Time(i));
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::HoughTransform(vector<int> v1, vector<int> v2, vector<int> v3){
  for(unsigned int i=0; i<v1.size(); i++){
    for(int itheta=0; itheta<180; itheta++){
      HoughAngle.push_back(itheta);
      double tmp= v1[i]*cos(itheta*NPUNITS::deg)+v2[i]*sin(itheta*NPUNITS::deg);
      HoughRadius.push_back(tmp);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigActar.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigActar.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigActar.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigActar.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigActar";
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
void TActarPhysics::Clear() {
  PadNumber.clear();
  PadRow.clear();
  PadColumn.clear();
  PadCharge.clear();
  PadTime.clear();

  HoughRadius.clear();
  HoughAngle.clear();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Actar");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl;

  vector<string> cart = {"POS","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Actar " << i+1 <<  endl;

      TVector3 Pos = blocks[i]->GetTVector3("POS","mm");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(Pos,Shape);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Actar " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(R,Theta,Phi,Shape);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitSpectra() {
  m_Spectra = new TActarSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TActarPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TActarPhysics::GetCanvas() {
  //if(m_Spectra)
    //return m_Spectra->GetCanvas();
  //else{
    //vector<TCanvas*> empty;
    //return empty;
  //}
  return vector<TCanvas*>();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("Actar", "D"+ NPL::itoa(i+1)+"_CHARGE","Actar_D"+ NPL::itoa(i+1)+"_CHARGE");
    Cal->AddParameter("Actar", "D"+ NPL::itoa(i+1)+"_TIME","Actar_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Actar",  true );
  inputChain->SetBranchAddress("Actar", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("Actar", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TActarPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Actar", "TActarPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TActarPhysics::Construct() {
  return (NPL::VDetector*) new TActarPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_Actar{
  public:
    proxy_Actar(){
      NPL::DetectorFactory::getInstance()->AddToken("Actar","Actar");
      NPL::DetectorFactory::getInstance()->AddDetector("Actar",TActarPhysics::Construct);
    }
};

proxy_Actar p_Actar;
}
