/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TDETECTORNAMEPhysics.h"

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

//   ROOT
#include "TChain.h"

ClassImp(TDETECTORNAMEPhysics)


///////////////////////////////////////////////////////////////////////////
TDETECTORNAMEPhysics::TDETECTORNAMEPhysics()
   : m_EventData(new TDETECTORNAMEData),
     m_PreTreatedData(new TDETECTORNAMEData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // match energy and time together
  for (UShort_t e = 0; e < m_PreTreatedData->GetMultEnergy(); e++) {
    for (UShort_t t = 0; t < m_PreTreatedData->GetMultTime(); t++) {
      if (m_PreTreatedData->GetE_DetectorNbr(e) == m_PreTreatedData->GetT_DetectorNbr(t)) {
        DetectorNumber.push_back(m_PreTreatedData->GetE_DetectorNbr(e));
        Energy.push_back(m_PreTreatedData->Get_Energy(e));
        Time.push_back(m_PreTreatedData->Get_Time(t));
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Energy
  for (UShort_t i = 0; i < m_EventData->GetMultEnergy(); ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("DETECTORNAME/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("DETECTORNAME/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigDETECTORNAME.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigDETECTORNAME.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigDETECTORNAME.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigDETECTORNAME.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigDETECTORNAME";
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
void TDETECTORNAMEPhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ReadConfiguration(string Path) {
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;

  bool check_Theta = false          ;
  bool check_Phi  = false           ;
  bool check_R     = false          ;
  bool check_Shape = false          ;
  bool check_X = false              ;
  bool check_Y = false              ;
  bool check_Z = false              ;
  bool ReadingStatus = false        ;

  while (!ConfigFile.eof()){

    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up DETECTORNAME bloc, Reading toggle to true
    string name="DETECTORNAME";
    if (LineBuffer.compare(0, name.length(), name) == 0){
      cout << "///" << endl ;
      cout << "DETECTORNAME found: " << endl ;
      ReadingStatus = true ; 
    }

    //   Reading Block
    while(ReadingStatus)
    {
      // Pickup Next Word
      ConfigFile >> DataBuffer ;

      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, name.length(), name) == 0) {
        cout << "\033[1;311mWARNING: Another detector is find before standard sequence of Token, Error may occured in detector definition\033[0m" << endl ;
        ReadingStatus = false ;
      }

      //Angle method
      else if (DataBuffer=="THETA=") {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        cout << "Theta:  " << atof(DataBuffer.c_str()) << "deg" << endl;
      }

      else if (DataBuffer=="PHI=") {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        cout << "Phi:  " << atof( DataBuffer.c_str() ) << "deg" << endl;
      }

      else if (DataBuffer=="R=") {
        check_R = true;
        ConfigFile >> DataBuffer ;
        cout << "R:  " << atof( DataBuffer.c_str() ) << "mm" << endl;
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


      //General
      else if (DataBuffer=="Shape=") {
        check_Shape = true;
        ConfigFile >> DataBuffer ;
        cout << "Shape:  " << DataBuffer << endl;
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if ( ((check_Theta && check_Phi && check_R) ||( check_X && check_Y && check_Z)  ) && check_Shape ){
        m_NumberOfDetectors++;

        //   Reinitialisation of Check Boolean
        check_Theta = false          ;
        check_Phi  = false           ;
        check_R     = false          ;
        check_Shape = false          ;
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
void TDETECTORNAMEPhysics::InitSpectra() {
  m_Spectra = new TDETECTORNAMESpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TDETECTORNAMEPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TDETECTORNAMEPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("DETECTORNAME", "D"+ NPL::itoa(i+1)+"_ENERGY","DETECTORNAME_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("DETECTORNAME", "D"+ NPL::itoa(i+1)+"_TIME","DETECTORNAME_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("DETECTORNAME",  true );
  inputChain->SetBranchAddress("DETECTORNAME", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("DETECTORNAME", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TDETECTORNAMEPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("DETECTORNAME", "TDETECTORNAMEPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TDETECTORNAMEPhysics::Construct() {
  return (NPL::VDetector*) new TDETECTORNAMEPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_DETECTORNAME{
  public:
    proxy_DETECTORNAME(){
      NPL::DetectorFactory::getInstance()->AddToken("DETECTORNAME","DETECTORNAME");
      NPL::DetectorFactory::getInstance()->AddDetector("DETECTORNAME",TDETECTORNAMEPhysics::Construct);
    }
};

proxy_DETECTORNAME p_DETECTORNAME;
}

