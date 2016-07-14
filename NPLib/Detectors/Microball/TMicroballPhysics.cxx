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
  string LineBuffer          ;
  string DataBuffer          ;


bool bR1 = false;
bool bR2 = false;
bool bR3 = false;
bool bR4 = false;
bool bR5 = false;
bool bR6 = false;
bool bR7 = false;
bool bR8 = false;
bool bR9 = false;
vector<int> copyNumArray;
bool bFlip = false;
     
  bool ReadingStatus = false ;

bool check_Ring1 = false;
bool check_Ring2 = false;
bool check_Ring3 = false;
bool check_Ring4 = false;
bool check_Ring5 = false;
bool check_Ring6 = false;
bool check_Ring7 = false;
bool check_Ring8 = false;
bool check_Ring9 = false;
bool check_Flip = false;


  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up Microball bloc, Reading toggle to true      
    string name = "Microball";

    if (LineBuffer.compare(0, name.length(), name) == 0) {
      cout << "///" << endl           ;
      cout << "Microball found: " << endl   ;        
      ReadingStatus = true ;
    }

    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus){
      // Pickup Next Word 
      ConfigFile >> DataBuffer ;

      //   Comment Line 
      if (DataBuffer.compare(0, 1, "%") == 0) {   
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, name.length(),name) == 0) {
        cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
        ReadingStatus = false ;
      }

      //Angle method
      else if (DataBuffer.compare(0, 6, "RING1=") == 0) {
        check_Ring1 = true;
        ConfigFile >> DataBuffer ;
        bR1 = atoi(DataBuffer.c_str()) ;
        cout << "Ring1:  " << bR1 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING2=") == 0) {
        check_Ring2 = true;
        ConfigFile >> DataBuffer ;
        bR2 = atoi(DataBuffer.c_str()) ;
        cout << "Ring2:  " << bR2 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING3=") == 0) {
        check_Ring3 = true;
        ConfigFile >> DataBuffer ;
        bR3 = atoi(DataBuffer.c_str()) ;
        cout << "Ring3:  " << bR3 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING4=") == 0) {
        check_Ring4 = true;
        ConfigFile >> DataBuffer ;
        bR4 = atoi(DataBuffer.c_str()) ;
        cout << "Ring4:  " << bR4 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING5=") == 0) {
        check_Ring5 = true;
        ConfigFile >> DataBuffer ;
        bR5 = atoi(DataBuffer.c_str()) ;
        cout << "Ring5:  " << bR5 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING6=") == 0) {
        check_Ring6 = true;
        ConfigFile >> DataBuffer ;
        bR6 = atoi(DataBuffer.c_str()) ;
        cout << "Ring6:  " << bR6 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING7=") == 0) {
        check_Ring7 = true;
        ConfigFile >> DataBuffer ;
        bR7 = atoi(DataBuffer.c_str()) ;
        cout << "Ring7:  " << bR7 << endl;
      }

      else if (DataBuffer.compare(0, 6, "RING8=") == 0) {
        check_Ring8 = true;
        ConfigFile >> DataBuffer ;
        bR8 = atoi(DataBuffer.c_str()) ;
        cout << "Ring8:  " << bR8 << endl;
      }


      else if (DataBuffer.compare(0, 6, "RING9=") == 0) {
        check_Ring9 = true;
        ConfigFile >> DataBuffer ;
        bR9 = atoi(DataBuffer.c_str()) ;
        cout << "Ring9:  " << bR9 << endl;
      }

      else if (DataBuffer.compare(0, 15, "DISABLE_CRYSTAL") == 0) {        
	ConfigFile >> DataBuffer ;
	int item = atoi(DataBuffer.c_str());
        copyNumArray.push_back(item) ;
        cout << "Disabled crystal:  " << item << endl;
      }

      else if (DataBuffer.compare(0, 14, "DETECTOR_FLIP=") == 0) {
        check_Flip = true;
        ConfigFile >> DataBuffer ;
        bFlip = atoi(DataBuffer.c_str()) ;
        cout << "Flip Detector:  " << bFlip << endl;
      }


      //General


      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
	ReadingStatus = false; 
        cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
      }
	if(check_Ring1 && check_Ring2 && check_Ring3 &&
		check_Ring4 && check_Ring5 && check_Ring6 &&
		check_Ring7 && check_Ring8 && check_Ring9 &&
		check_Flip){
      

	//   Reinitialisation of Check Boolean 
        check_Ring1 = false ;
        check_Ring2 = false ;
        check_Ring3 = false ;
        check_Ring4 = false ;
        check_Ring5 = false ;
        check_Ring6 = false ;
        check_Ring7 = false ;
        check_Ring8 = false ;
        check_Ring9 = false ;
	check_Flip = false ;

        ReadingStatus = false ;   
        cout << "///"<< endl ; 
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

