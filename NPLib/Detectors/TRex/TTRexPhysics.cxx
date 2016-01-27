/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TRex Treat data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TTRexPhysics.h"

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
ClassImp(TTRexPhysics)


  ///////////////////////////////////////////////////////////////////////////
TTRexPhysics::TTRexPhysics()
  : m_EventData(new TTRexData),
  m_PreTreatedData(new TTRexData),
  m_EventPhysics(this),
  m_Spectra(0),
  m_E_RAW_Threshold(0), // adc channels
  m_E_Threshold(0),     // MeV
  m_NumberOfDetectors(0) {
  }



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::BuildSimplePhysicalEvent() {
  BuildPhysicalEvent();
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::BuildPhysicalEvent() {
  // apply thresholds and calibration
  PreTreat();

  // match Front and Back Energy
  for (UShort_t f = 0; f < m_PreTreatedData->GetFrontMultEnergy(); f++) {
    for (UShort_t b = 0; b < m_PreTreatedData->GetBackMultEnergy(); b++) {
      if (m_PreTreatedData->GetFrontE_DetectorNbr(f) == m_PreTreatedData->GetBackE_DetectorNbr(b)) {
        DetectorNumber.push_back(m_PreTreatedData->GetFrontE_DetectorNbr(f));
        Energy.push_back(m_PreTreatedData->GetFront_Energy(f));
        Strip_Front.push_back(m_PreTreatedData->GetFrontE_StripNbr(f));
        Strip_Back.push_back(m_PreTreatedData->GetBackE_StripNbr(b)); 
        bool checkTime = false;
        double tt;
        // Look for associated time
        for (UShort_t t = 0; t < m_PreTreatedData->GetBackMultTime(); t++) { 
          if(m_PreTreatedData->GetFrontE_DetectorNbr(f) == m_PreTreatedData->GetBackT_DetectorNbr(t)){
            tt=m_PreTreatedData->GetBack_Time(t);
            checkTime =true;
          }
        }
        if(checkTime)
           Time.push_back(tt);
        else
          Time.push_back(-1000);
        
        bool checkPAD = false;
        double pp;
        // Look for associated time
        for (UShort_t t = 0; t < m_PreTreatedData->GetPADMultEnergy(); t++) { 
          if(m_PreTreatedData->GetFrontE_DetectorNbr(f) == m_PreTreatedData->GetBackT_DetectorNbr(t)){
            pp=m_PreTreatedData->GetPAD_Energy(t);
            checkPAD =true;
          }
        }
        if(checkPAD)
           PAD_E.push_back(pp);
        else
          PAD_E.push_back(-1000);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::PreTreat() {
  // This method typically applies thresholds and calibrations
  // Might test for disabled channels for more complex detector
  // clear pre-treated object
  ClearPreTreatedData();

  // instantiate CalibrationManager
  static CalibrationManager* Cal = CalibrationManager::getInstance();

  // Front
  // Energy
  for (UShort_t i = 0; i < m_EventData->GetFrontMultEnergy(); ++i) {
    if (m_EventData->GetFront_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("TRex/ENERGY_FRONT"+NPL::itoa(m_EventData->GetFrontE_DetectorNbr(i)),m_EventData->GetFront_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetFrontEnergy(m_EventData->GetFrontE_DetectorNbr(i),m_EventData->GetFrontE_StripNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetFrontMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("TRex/TIME_FRONT"+NPL::itoa(m_EventData->GetFrontT_DetectorNbr(i)),m_EventData->GetFront_Time(i));
    m_PreTreatedData->SetFrontTime(m_EventData->GetFrontT_DetectorNbr(i),m_EventData->GetFrontT_StripNbr(i), Time);
  }

  // Back
  // Energy
  for (UShort_t i = 0; i < m_EventData->GetBackMultEnergy(); ++i) {
    if (m_EventData->GetBack_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("TRex/ENERGY_BACK"+NPL::itoa(m_EventData->GetBackE_DetectorNbr(i)),m_EventData->GetBack_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetBackEnergy(m_EventData->GetBackE_DetectorNbr(i),m_EventData->GetBackE_StripNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetBackMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("TRex/TIME_BACK"+NPL::itoa(m_EventData->GetBackT_DetectorNbr(i)),m_EventData->GetBack_Time(i));
    m_PreTreatedData->SetBackTime(m_EventData->GetBackT_DetectorNbr(i),m_EventData->GetBackT_StripNbr(i), Time);
  }
  // PAD
  // Energy
  for (UShort_t i = 0; i < m_EventData->GetPADMultEnergy(); ++i) {
    if (m_EventData->GetPAD_Energy(i) > m_E_RAW_Threshold) {
      Double_t Energy = Cal->ApplyCalibration("TRex/ENERGY_PAD"+NPL::itoa(m_EventData->GetPADE_DetectorNbr(i)),m_EventData->GetPAD_Energy(i));
      if (Energy > m_E_Threshold) {
        m_PreTreatedData->SetPADEnergy(m_EventData->GetPADE_DetectorNbr(i), Energy);
      }
    }
  }

  // Time 
  for (UShort_t i = 0; i < m_EventData->GetPADMultTime(); ++i) {
    Double_t Time= Cal->ApplyCalibration("TRex/TIME_PAD"+NPL::itoa(m_EventData->GetPADT_DetectorNbr(i)),m_EventData->GetPAD_Time(i));
    m_PreTreatedData->SetPADTime(m_EventData->GetPADT_DetectorNbr(i), Time);
  }
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::ReadAnalysisConfig() {
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigTRex.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigTRex.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigTRex.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigTRex.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    string name = "ConfigTRex";
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
void TTRexPhysics::Clear() {
  DetectorNumber.clear();
  Strip_Front.clear();
  Strip_Back.clear();
  Energy.clear();
  PAD_E.clear();
  Time.clear();
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::ReadConfiguration(string Path) {
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer;
  string DataBuffer;

  double X,Y,Z;
  bool check_X = false ;
  bool check_Y = false ;
  bool check_Z = false ;      
  bool ReadingStatus = false ;


  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up TRex bloc, Reading toggle to true      
    string name = "TRex";

    if (LineBuffer.compare(0, name.length(), name) == 0) {
      cout << "///" << endl           ;
      cout << "TRex found: " << endl   ;        
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

      //Position method
      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true;
        ConfigFile >> DataBuffer ;
        X = atof(DataBuffer.c_str()) ;
        X = X ;
        cout << "X:  " << X  << endl;
      }

      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true;
        ConfigFile >> DataBuffer ;
        Y = atof(DataBuffer.c_str()) ;
        Y = Y ;
        cout << "Y:  " << Y  << endl;
      }

      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true;
        ConfigFile >> DataBuffer ;
        Z = atof(DataBuffer.c_str()) ;
        Z = Z ;
        cout << "Z:  " << Z  << endl;
      }

      else if (DataBuffer.compare(0, 8, "Chamber=") == 0) {
        ConfigFile >> DataBuffer ; 
      }
      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus = false; 
        cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if (check_X && check_Y && check_Z){
        // Convert Cartesian to Spherical (detector always face the target)
        AddBarrelDetector(X,Y,Z);

        //   Reinitialisation of Check Boolean 
        check_X = false ;
        check_Y = false ;
        check_Z = false ;
        ReadingStatus = false ;   
        cout << "///"<< endl ;            
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::InitSpectra() {
  m_Spectra = new TTRexSpectra(m_NumberOfDetectors);
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::FillSpectra() {
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::CheckSpectra() {
  m_Spectra->CheckSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::ClearSpectra() {
  // To be done
}



///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TTRexPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
}
///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::AddBarrelDetector(double X, double Y, double Z){
  TVector3 U; TVector3 V;TVector3 Strip_1_1;
  m_NumberOfDetectors++;
  double StripPitchFront= 50/16.;
  double StripPitchBack= 50/16.;
  
  // Half the length of the wafer (square wafer)
 double A = (50-StripPitchFront)*0.5;
 double B = (50-StripPitchBack)*0.5;
  double Zc=0; 
  if(Z<0){// Up Stream
    Zc = Z +B; 
    if(X==0 && Y>0)      {U=TVector3(-1,0,0) ;V=TVector3(0,0,-1);  Strip_1_1=TVector3( A , Y  ,Zc); }
    else if(X==0 && Y<0) {U=TVector3(1,0,0) ;V=TVector3(0,0,-1); Strip_1_1=TVector3( -A , Y ,Zc);}
    else if(Y==0 && X>0) {U=TVector3(0,1,0) ;V=TVector3(0,0,-1);  Strip_1_1=TVector3( X  , -A ,Zc); }
    else if(Y==0 && X<0) {U=TVector3(0,-1,0) ;V=TVector3(0,0,-1); Strip_1_1=TVector3( X  , A  ,Zc);}
  }

  else if(Z>0){//Down Stream
    Zc = Z -B; 
    if(X==0 && Y>0)      {U=TVector3(1,0,0) ;V=TVector3(0,0,1); Strip_1_1=TVector3( -A , Y  ,Zc); }
    else if(X==0 && Y<0) {U=TVector3(-1,0,0) ;V=TVector3(0,0,1); Strip_1_1=TVector3( A , Y ,Zc); }
    else if(Y==0 && X>0) {U=TVector3(0,-1,0) ;V=TVector3(0,0,1); Strip_1_1=TVector3( X  , A ,Zc); }
    else if(Y==0 && X<0) {U=TVector3(0,1,0) ;V=TVector3(0,0,1); Strip_1_1=TVector3( X  , -A  ,Zc); }
  }

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneBoxStripPositionX   ;
  vector< vector< double > >   OneBoxStripPositionY   ;
  vector< vector< double > >   OneBoxStripPositionZ   ;

  TVector3 StripCenter = Strip_1_1;
  for(int f = 0 ; f < 16; f++){
    lineX.clear()   ;
    lineY.clear()   ;
    lineZ.clear()   ;

    for(int b = 0 ; b < 16; b++){
      StripCenter = Strip_1_1 + ( StripPitchFront*f*U + StripPitchBack*b*V  );

      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneBoxStripPositionX.push_back(lineX);
    OneBoxStripPositionY.push_back(lineY);
    OneBoxStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back( OneBoxStripPositionX ) ;
  m_StripPositionY.push_back( OneBoxStripPositionY ) ;
  m_StripPositionZ.push_back( OneBoxStripPositionZ ) ;

}


////////////////////////////////////////////////////////////////////////////////
vector<TCanvas*> TTRexPhysics::GetCanvas() {
  if(m_Spectra)
    return m_Spectra->GetCanvas();
  else{
    vector<TCanvas*> empty;
    return empty;
  }
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::WriteSpectra() {
  m_Spectra->WriteSpectra();
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::AddParameterToCalibrationManager() {
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for (int i = 0; i < m_NumberOfDetectors; ++i) {
    Cal->AddParameter("TRex", "D"+ NPL::itoa(i+1)+"_ENERGY_FRONT","TRex_D"+ NPL::itoa(i+1)+"_ENERGY_FRONT");
    Cal->AddParameter("TRex", "D"+ NPL::itoa(i+1)+"_TIME_FRONT","TRex_D"+ NPL::itoa(i+1)+"_TIME_FRONT");
    Cal->AddParameter("TRex", "D"+ NPL::itoa(i+1)+"_ENERGY_FRONT","TRex_D"+ NPL::itoa(i+1)+"_ENERGY_FRONT");
    Cal->AddParameter("TRex", "D"+ NPL::itoa(i+1)+"_TIME_FRONT","TRex_D"+ NPL::itoa(i+1)+"_TIME_FRONT");

  }
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("TRex",  true );
  inputChain->SetBranchAddress("TRex", &m_EventData );
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::InitializeRootInputPhysics() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("TRex", &m_EventPhysics);
}



///////////////////////////////////////////////////////////////////////////
void TTRexPhysics::InitializeRootOutput() {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("TRex", "TTRexPhysics", &m_EventPhysics);
}

////////////////////////////////////////////////////////////////////////////////
TVector3 TTRexPhysics::GetPositionOfInteraction(const int& i,bool random) const{
  static TVector3 Position ;
  static double BoxPitchBack= 50/16.;
  static double BoxPitchFront= 50/16.;
  Position = TVector3 (  GetStripPositionX( DetectorNumber[i], Strip_Front[i], Strip_Back[i] ),
      GetStripPositionY( DetectorNumber[i] , Strip_Front[i], Strip_Back[i] ),
      GetStripPositionZ( DetectorNumber[i] , Strip_Front[i], Strip_Back[i] )) ;
  
  if(random){
    return Position;
  }
  
  return Position ;

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTRexPhysics::Construct() {
  return (NPL::VDetector*) new TTRexPhysics();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_TRex{
  public:
    proxy_TRex(){
      NPL::DetectorFactory::getInstance()->AddToken("TRex","TRex");
      NPL::DetectorFactory::getInstance()->AddDetector("TRex",TTRexPhysics::Construct);
    }
};

proxy_TRex p_TRex;
}

