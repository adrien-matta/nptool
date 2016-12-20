/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : octobre 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios2 Treated  data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "THelios2Physics.h"

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
#include "NPPhysicalConstants.h"
#include "NPOptionManager.h"
//   ROOT
#include "TChain.h"

ClassImp(THelios2Physics)


///////////////////////////////////////////////////////////////////////////
THelios2Physics::THelios2Physics()
   : m_EventData(new THelios2Data),
     m_PreTreatedData(new THelios2Data),
     m_EventPhysics(this),
     m_Spectra(0),
     m_E_RAW_Threshold(0), // adc channels
     m_E_Threshold(0),     // MeV
     m_NumberOfDetectors(0) {
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // Look for Eup and EDw on the same detector
  unsigned int mysizeEBack = m_PreTreatedData->GetMultEBack();
  unsigned int mysizeEUp = m_PreTreatedData->GetMultEUp();
  unsigned int mysizeEDw = m_PreTreatedData->GetMultEDw();
  unsigned int mysizeTUp = m_PreTreatedData->GetMultTUp();
  unsigned int DetNbr = 0;
  for(unsigned int u = 0 ; u < mysizeEUp ; u++){
    DetNbr = m_PreTreatedData->GetEUp_DetectorNbr(u);
    for(unsigned int d = 0 ; d < mysizeEDw ; d++){
      if( DetNbr == m_PreTreatedData->GetEDw_DetectorNbr(d)){
        
        DetectorNumber.push_back(DetNbr); 
        // Total Energy deposit is taken from the back
        double E = -1000;
        for(unsigned int b = 0 ; b < mysizeEBack ; b++){  
          if(DetNbr == m_PreTreatedData->GetEBack_DetectorNbr(b))
            E = m_PreTreatedData->GetEBack_Energy(b);
        }
        Energy.push_back(E);
        
        // Position along the detector lenght between -1 and 1
        double P = m_PreTreatedData->GetEDw_Energy(d)-m_PreTreatedData->GetEUp_Energy(u);
        P = P / E;
        Position.push_back(P);
        // Compute Z based on the Position and detector position
        double Detector_length = 56;
        Z.push_back(P*0.5 * Detector_length + m_Z[m_PreTreatedData->GetEUp_DetectorNbr(u)-1]); 
        
        // Look for associated time
        double T = -1000;
        for(unsigned int t = 0 ; t < mysizeTUp ; t++){  
          if(DetNbr == m_PreTreatedData->GetTUp_DetectorNbr(t)){  
            T = m_PreTreatedData->GetTUp_Time(t);
          }
         Time.push_back(T);
        }// for T Up
      }
    }// for E Dw
  }// for E Up
}

///////////////////////////////////////////////////////////////////////////
void THelios2Physics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector

  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // EBack
  unsigned int mysize = m_EventData->GetMultEBack();
  for (UShort_t i = 0; i < mysize; ++i) {
    if (m_EventData->GetEBack_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Helios2/EBACK"
        +NPL::itoa(m_EventData->GetEBack_DetectorNbr(i)),m_EventData->GetEBack_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEBack(m_EventData->GetEBack_DetectorNbr(i), Energy);
      }
    }
  }

  // EUp
  mysize = m_EventData->GetMultEUp();
  for (UShort_t i = 0; i < mysize; ++i) {
    if (m_EventData->GetEUp_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Helios2/EUP"
        +NPL::itoa(m_EventData->GetEUp_DetectorNbr(i)),m_EventData->GetEUp_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEUp(m_EventData->GetEUp_DetectorNbr(i), Energy);
      }
    }
  }

  // EDw
  mysize = m_EventData->GetMultEDw();
  for (UShort_t i = 0; i < mysize ; ++i) {
    if (m_EventData->GetEDw_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("Helios2/EDW"
        +NPL::itoa(m_EventData->GetEDw_DetectorNbr(i)),m_EventData->GetEDw_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetEDw(m_EventData->GetEDw_DetectorNbr(i), Energy);
      }
    }
  }

  // TBack
  mysize = m_EventData->GetMultTBack();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("Helios2/TBACK"+NPL::itoa(m_EventData->GetTBack_DetectorNbr(i)),m_EventData->GetTBack_Time(i));
    m_PreTreatedData->SetTBack(m_EventData->GetTBack_DetectorNbr(i), Time);
  }

  // TUp
  mysize = m_EventData->GetMultTUp();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("Helios2/TUP"+NPL::itoa(m_EventData->GetTUp_DetectorNbr(i)),m_EventData->GetTUp_Time(i));
    m_PreTreatedData->SetTUp(m_EventData->GetTUp_DetectorNbr(i), Time);
  }
  // TDw
  mysize = m_EventData->GetMultTDw();
  for (UShort_t i = 0; i < mysize; ++i) {
    Double_t Time= Cal->ApplyCalibration("Helios2/TDW"+NPL::itoa(m_EventData->GetTDw_DetectorNbr(i)),m_EventData->GetTDw_Time(i));
    m_PreTreatedData->SetTDw(m_EventData->GetTDw_DetectorNbr(i), Time);
  }

}

///////////////////////////////////////////////////////////////////////////
double THelios2Physics::ThetaLab(unsigned int i, double m, double B, double q){
  // If default value of B, use the private membes Field variable
  // This allow the user to vary B in its analysis to adjust field to data
  if(B == -1000){
    B = m_B;
  }

  else
    B *=NPUNITS::tesla;

  // If q is not provided, use the measured time
  double T = 0 ; 
  if(q == -1000){
    T = Time[i];
  }
 
  // else use the calculated time
  else{
    T = (NPUNITS::twopi*m*NPUNITS::MeV/(NPUNITS::c_squared)) / (q*B); 
  }

  double v_par = Z[i]/T;
  double v = sqrt(2*(Energy[i]*NPUNITS::MeV)
            /(m*NPUNITS::MeV/(NPUNITS::c_squared)));
  double result = acos(v_par/v);
  
  // in case ThetaLab is not a number (nan)
  // return -1000;
  if(result!=result){
    result= -1000;
  }
  return result;
}

///////////////////////////////////////////////////////////////////////////
void THelios2Physics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigHelios2.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigHelios2.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigHelios2.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigHelios2.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigHelios2";
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
void THelios2Physics::Clear() {
  DetectorNumber.clear();
  Energy.clear();
  Time.clear();
  Z.clear();
  Position.clear();
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Helios2");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z","Face"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasToken("MagneticField"))
      m_B=blocks[i]->GetDouble("MagneticField","T");

    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Helios2 " << i+1 <<  endl;
      double Z = blocks[i]->GetDouble("Z","mm");
      string Face = blocks[i]->GetString("Face");

      m_Z.push_back(Z);
      m_Face.push_back(Face);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  ReadAnalysisConfig();
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::InitSpectra() {
  m_Spectra = new THelios2Spectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> THelios2Physics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}



////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> THelios2Physics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_EBACK","Helios2_D"+ NPL::itoa(i+1)+"_EBACK");
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_TBACK","Helios2_D"+ NPL::itoa(i+1)+"_TBACK");
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_EUP","Helios2_D"+ NPL::itoa(i+1)+"_EUP");
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_TUP","Helios2_D"+ NPL::itoa(i+1)+"_TUP");
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_EDW","Helios2_D"+ NPL::itoa(i+1)+"_EDW");
    Cal->AddParameter("Helios2", "D"+ NPL::itoa(i+1)+"_TDW","Helios2_D"+ NPL::itoa(i+1)+"_TDW");

  }
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Helios2",  true );
  inputChain->SetBranchAddress("Helios2", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("Helios2", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void THelios2Physics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Helios2", "THelios2Physics", &m_EventPhysics);
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* THelios2Physics::Construct() {
  return (NPL::VDetector*) new THelios2Physics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_Helios2{
  public:
    proxy_Helios2(){
      NPL::DetectorFactory::getInstance()->AddToken("Helios2","Helios2");
      NPL::DetectorFactory::getInstance()->AddDetector("Helios2",THelios2Physics::Construct);
    }
};

proxy_Helios2 p_Helios2;
}

