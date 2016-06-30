/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Microball Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TMicroballPhysics.h"

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

ClassImp(TMicroballPhysics)


///////////////////////////////////////////////////////////////////////////
TMicroballPhysics::TMicroballPhysics()
   : m_EventData(new TMicroballData),
     m_PreTreatedData(new TMicroballData),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::BuildPhysicalEvent() {
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
void TMicroballPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Energy
  for (UShort_t i = 0; i < m_EventData->GetMultEnergy(); ++i) {
    if (m_EventData->Get_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Microball/ENERGY"+NPL::itoa(m_EventData->GetE_DetectorNbr(i)),m_EventData->Get_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEnergy(m_EventData->GetE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("Microball/TIME"+NPL::itoa(m_EventData->GetT_DetectorNbr(i)),m_EventData->Get_Time(i));
    m_PreTreatedData->SetTime(m_EventData->GetT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigMicroball.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigMicroball.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigMicroball.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigMicroball.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigMicroball";
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
void TMicroballPhysics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::ReadConfiguration(string Path) {
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

    //   If line is a Start Up Microball bloc, Reading toggle to true
    string name="Microball";
    if (LineBuffer.compare(0, name.length(), name) == 0){
      cout << "///" << endl ;
      cout << "Microball found: " << endl ;
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
void TMicroballPhysics::InitSpectra() {
  m_Spectra = new TMicroballSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TMicroballPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TMicroballPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("Microball", "D"+ NPL::itoa(i+1)+"_ENERGY","Microball_D"+ NPL::itoa(i+1)+"_ENERGY");
    Cal->AddParameter("Microball", "D"+ NPL::itoa(i+1)+"_TIME","Microball_D"+ NPL::itoa(i+1)+"_TIME");
  }
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Microball",  true );
  inputChain->SetBranchAddress("Microball", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("Microball", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TMicroballPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Microball", "TMicroballPhysics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TMicroballPhysics::Construct() {
  return (NPL::VDetector*) new TMicroballPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_Microball{
  public:
    proxy_Microball(){
      NPL::DetectorFactory::getInstance()->AddToken("Microball","Microball");
      NPL::DetectorFactory::getInstance()->AddDetector("Microball",TMicroballPhysics::Construct);
    }
};

proxy_Microball p_Microball;
}

