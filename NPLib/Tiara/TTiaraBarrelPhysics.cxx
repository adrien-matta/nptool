/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TiaraBarrel treated data                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TTiaraBarrelPhysics.h"
using namespace TiaraBarrel_LOCAL;
//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTiaraBarrelPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTiaraBarrelPhysics::TTiaraBarrelPhysics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TTiaraBarrelData ;
    m_PreTreatedData    = new TTiaraBarrelData ;
    m_EventPhysics      = this ;
    m_NumberOfDetector  = 0 ;
    
    m_Take_E_Strip= true;
    m_Take_T_Back=true;
    m_Strip_E_Threshold = 0.40 ;
    m_Back_E_Threshold = 0.40 ;
    m_Maximum_FrontBack_Difference = 0.4;
    m_OuterBack_E_Threshold =0;
  }

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::BuildPhysicalEvent(){
  PreTreat();

  unsigned int sizeU = m_PreTreatedData->GetFrontUpstreamEMult();
  unsigned int sizeD = m_PreTreatedData->GetFrontDownstreamEMult();
  unsigned int sizeB = m_EventData->GetBackEMult(); 
 for(unsigned int k = 0 ; k < sizeB ; k++){
  for(unsigned int i = 0 ; i < sizeU ; i++){  
    if(m_PreTreatedData->GetFrontUpstreamEDetectorNbr(i) == m_PreTreatedData->GetBackEDetectorNbr(k))
    
    for(unsigned int j = 0 ; j < sizeD ; j++){  
        // same detector, same strip
        if( m_PreTreatedData->GetFrontUpstreamEDetectorNbr(i) 
            == m_PreTreatedData->GetFrontDownstreamEDetectorNbr(j)
            && m_PreTreatedData->GetFrontUpstreamEStripNbr(i) 
            == m_PreTreatedData->GetFrontDownstreamEStripNbr(j)){
            
            double EU = m_PreTreatedData->GetFrontUpstreamEEnergy(i) ;
            double ED = m_PreTreatedData->GetFrontDownstreamEEnergy(j); 
      
            // Front back Energy match
   //         if(abs(m_PreTreatedData->GetBackEEnergy(k)-(EU+ED)) < m_Maximum_FrontBack_Difference){
            // I have no calibration of the back so replace by nothing ;)
            if(true){ 
             double POS =
                CalibrationManager::getInstance()
                  ->ApplyResistivePositionCalibration("TIARABARREL/B"
                    +itoa(m_EventData->GetFrontUpstreamEDetectorNbr(i))
                    +"_STRIP"+itoa(m_EventData->GetFrontUpstreamEStripNbr(i))
                    +"_POS",(ED-EU)/(EU+ED));
                
                Strip_Pos.push_back(POS); 
                Strip_N.push_back(m_EventData->GetFrontUpstreamEStripNbr(i));
                DetectorNumber.push_back(m_EventData->GetFrontUpstreamEDetectorNbr(i));
                double E = (EU+ED) / CalibrationManager::getInstance()
                  ->ApplyCalibration("TIARABARREL/BALLISTIC_B" 
                  + itoa(m_EventData->GetFrontDownstreamEDetectorNbr(i)) 
                  + "_STRIP" 
                  + itoa(m_EventData->GetFrontDownstreamEStripNbr(i)),
                  POS);
                  Strip_E.push_back(E);
          }
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::PreTreat(){
  ClearPreTreatedData();
  // Gain Calibration
  // The cal function first call the MatchStick one
  unsigned int sizeU = m_EventData->GetFrontUpstreamEMult();
  unsigned int sizeD = m_EventData->GetFrontDownstreamEMult();
  unsigned int sizeB = m_EventData->GetBackEMult();
  
  for(unsigned int i = 0 ; i < sizeU ; i++){  
    double EU = Cal_Strip_Upstream_E(i) ;
    if(EU > m_Strip_E_Threshold)
      m_PreTreatedData->SetFrontUpstreamE(m_EventData->GetFrontUpstreamEDetectorNbr(i),
          m_EventData->GetFrontUpstreamEStripNbr(i),
          EU);
  }

  for(unsigned int j = 0 ; j < sizeD ; j++){  
    double ED = Cal_Strip_Downstream_E(j) ;
    if(ED>m_Strip_E_Threshold)
      m_PreTreatedData->SetFrontDownstreamE(m_EventData->GetFrontDownstreamEDetectorNbr(j),
          m_EventData->GetFrontDownstreamEStripNbr(j),
          ED);
  }

  for(unsigned int k = 0 ; k < sizeB ; k++){
    double EB = Cal_Back_E(k) ;
     if(EB > m_Back_E_Threshold)
      m_PreTreatedData->SetBackE(m_EventData->GetBackEDetectorNbr(k),EB);
  }    
}

////////////////////////////////////////////////////////////////////////////
bool TTiaraBarrelPhysics :: IsValidChannel(const string DetectorType, const int detector , const int channel){

  if(DetectorType == "InnerBarrelStripUpstream")
    return *(m_InnerBarrelStripUpstreamChannelStatus[detector-1].begin()+channel-1);

  if(DetectorType == "InnerBarrelStripDownstream")
    return *(m_InnerBarrelStripDownstreamChannelStatus[detector-1].begin()+channel-1);

  else if(DetectorType == "OuterBarrelStrip")
    return *(m_OuterBarrelStripChannelStatus[detector-1].begin()+channel-1);

  if(DetectorType == "InnerBarrelBack")
    return *(m_InnerBarrelBackChannelStatus[detector-1].begin()+channel-1);

  else if(DetectorType == "OuterBarrelBack")
    return *(m_OuterBarrelBackChannelStatus[detector-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::ReadAnalysisConfig(){
  /*  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigTiaraBarrel.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
  cout << " No ConfigTiaraBarrel.dat found: Default parameter loaded for Analayis " << FileName << endl;
  return;
  }
  cout << " Loading user parameter for Analysis from ConfigTiaraBarrel.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigTiaraBarrel.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
  // Pick-up next line
  getline(AnalysisConfigFile, LineBuffer);

  // search for "header"
  if (LineBuffer.compare(0, 11, "ConfigTiaraBarrel") == 0) ReadingStatus = true;

  // loop on tokens and data
  while (ReadingStatus ) {

  whatToDo="";
  AnalysisConfigFile >> whatToDo;

  // Search for comment symbol (%)
  if (whatToDo.compare(0, 1, "%") == 0) {
  AnalysisConfigFile.ignore(numeric_limits<streamsize>::max(), '\n' );
  }

  else if (whatToDo=="MAX_STRIP_MULTIPLICITY") {
  AnalysisConfigFile >> DataBuffer;
  m_MaximumStripMultiplicityAllowed = atoi(DataBuffer.c_str() );
  cout << "MAXIMUN STRIP MULTIPLICITY " << m_MaximumStripMultiplicityAllowed << endl;
  }

  else if (whatToDo=="STRIP_ENERGY_MATCHING_SIGMA") {
  AnalysisConfigFile >> DataBuffer;
  m_StripEnergyMatchingSigma = atof(DataBuffer.c_str() );
  cout << "STRIP ENERGY MATCHING SIGMA " << m_StripEnergyMatchingSigma <<endl;
  }

  else if (whatToDo=="STRIP_ENERGY_MATCHING_NUMBER_OF_SIGMA") {
  AnalysisConfigFile >> DataBuffer;
  m_StripEnergyMatchingNumberOfSigma = atoi(DataBuffer.c_str() );
  cout << "STRIP ENERGY MATCHING NUMBER OF SIGMA " << m_StripEnergyMatchingNumberOfSigma << endl;
  }

  else if (whatToDo== "DISABLE_ALL") {
  AnalysisConfigFile >> DataBuffer;
  cout << whatToDo << "  " << DataBuffer << endl;
  int Detector = atoi(DataBuffer.substr(2,1).c_str());
  vector< bool > ChannelStatus;
  ChannelStatus.resize(24,false);
  m_RingChannelStatus[Detector-1] = ChannelStatus;
  ChannelStatus.resize(48,false);
  m_SectorChannelStatus[Detector-1] = ChannelStatus;
  }

  else if (whatToDo == "DISABLE_CHANNEL") {
  AnalysisConfigFile >> DataBuffer;
  cout << whatToDo << "  " << DataBuffer << endl;
  int Detector = atoi(DataBuffer.substr(2,1).c_str());
  int channel = -1;
  if (DataBuffer.compare(3,4,"STRF") == 0) {
    channel = atoi(DataBuffer.substr(7).c_str());
    *(m_RingChannelStatus[Detector-1].begin()+channel-1) = false;
  }

  else if (DataBuffer.compare(3,4,"STRB") == 0) {
    channel = atoi(DataBuffer.substr(7).c_str());
    *(m_SectorChannelStatus[Detector-1].begin()+channel-1) = false;
  }

  else cout << "Warning: detector type for TiaraBarrel unknown!" << endl;

}

else if (whatToDo=="TAKE_E_RING") {
  m_Take_E_Ring = true;
  cout << whatToDo << endl;
}

else if (whatToDo=="TAKE_E_SECTOR") {
  m_Take_E_Ring = false;
  cout << whatToDo << endl;
}

else if (whatToDo=="TAKE_T_RING") {
  m_Take_T_Sector = false;
  cout << whatToDo << endl;
}

else if (whatToDo=="TAKE_T_SECTOR") {
  m_Take_T_Sector = true;
  cout << whatToDo << endl;
}

else if (whatToDo=="STRIP_RING_E_RAW_THRESHOLD") {
  AnalysisConfigFile >> DataBuffer;
  m_StripRing_E_RAW_Threshold = atoi(DataBuffer.c_str());
  cout << whatToDo << " " << m_StripRing_E_RAW_Threshold << endl;
}

else if (whatToDo=="STRIP_SECTOR_E_RAW_THRESHOLD") {
  AnalysisConfigFile >> DataBuffer;
  m_StripSector_E_RAW_Threshold = atoi(DataBuffer.c_str());
  cout << whatToDo << " " << m_StripSector_E_RAW_Threshold << endl;
}

else if (whatToDo=="STRIP_RING_E_THRESHOLD") {
  AnalysisConfigFile >> DataBuffer;
  m_StripRing_E_Threshold = atoi(DataBuffer.c_str());
  cout << whatToDo << " " << m_StripRing_E_Threshold << endl;
}

else if (whatToDo=="STRIP_SECTOR_THRESHOLD") {
  AnalysisConfigFile >> DataBuffer;
  m_StripSector_E_Threshold = atoi(DataBuffer.c_str());
  cout << whatToDo << " " << m_StripSector_E_Threshold << endl;
}

else {
  ReadingStatus = false;
}

}
}
*/
}

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::Clear(){
  EventMultiplicity=0;
  DetectorNumber .clear();
  Strip_E.clear();
  Strip_T.clear();
  Strip_N.clear();
  Strip_Pos.clear();
  DownStream_E.clear();
  DownStream_T.clear();
  UpStream_E.clear();
  UpStream_T.clear();
  Back_E.clear();
  Back_T.clear();
  Outer_Strip_E.clear();
  Outer_Strip_T.clear();
  Outer_Strip_N.clear();
  Outer_Back_E.clear();
  Outer_Back_T.clear();

}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;

  double X,Y,Z;
  X = 0 ; Y = 0 ; Z = 0;
  TVector3 Pos;
  bool check_X   = false ;
  bool check_Y   = false ;
  bool check_Z   = false ;

  //  bool ReadingStatusWedge = false ;
  bool ReadingStatus    = false ;

  bool VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel(); ;

  while (!ConfigFile.eof()){

    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 11, "TiaraBarrel") == 0)
      ReadingStatus = true;

    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      else if (DataBuffer == "X=") {
        check_X = true;
        ConfigFile >> DataBuffer ;
        X= atof(DataBuffer.c_str());
        if(VerboseLevel) cout << "  X= " << X << "mm" << endl;
      }

      else if (DataBuffer == "Y=") {
        check_Y = true;
        ConfigFile >> DataBuffer ;
        Y= atof(DataBuffer.c_str());
        if(VerboseLevel) cout << "  Y= " << Y << "mm" << endl;
      }

      else if (DataBuffer == "Z=") {
        check_Z = true;
        ConfigFile >> DataBuffer ;
        Z= atof(DataBuffer.c_str());
        if(VerboseLevel) cout << "  Z= " << Z << "deg" << endl;
      }

      else if (DataBuffer == "ThicknessDector=") {
        /*ignore that*/
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus= false;
        cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << endl ;
        exit(1);
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if (check_X && check_Y && check_Z){

        ReadingStatus= false;
        AddDetector(X,Y,Z);
        //   Reinitialisation of Check Boolean
        check_X = false ;
        check_Y = false ;
        check_Z = false ;
      }
    }
  }
  InitializeStandardParameter();
  ReadAnalysisConfig();
}
///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::InitSpectra(){  
  m_Spectra = new TTiaraBarrelSpectra(m_NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::CheckSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< vector<string>,TH1* > TTiaraBarrelPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
     map< vector<string>,TH1* > empty;
     return empty;
  }

} 
///////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  // E and T
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    for( int j = 0 ; j < 4 ; ++j){
      Cal->AddParameter("TIARABARREL","B"+itoa(i+1)+"_UPSTREAM"+itoa(j+1)+"_E","TIARABARREL_B"+itoa(i+1)+"_UPSTREAM"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARABARREL","B"+itoa(i+1)+"_DOWNSTREAM"+itoa(j+1)+"_E","TIARABARREL_B"+itoa(i+1)+"_DOWNSTREAM"+itoa(j+1)+"_E")   ;

      Cal->AddParameter("TIARABARREL","MATCHSTICK_B"+itoa(i+1)+"_UPSTREAM"+itoa(j+1)+"_E","TIARABARREL_MATCHSTICK_B"+itoa(i+1)+"_UPSTREAM"+itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARABARREL","MATCHSTICK_B"+itoa(i+1)+"_DOWNSTREAM"+itoa(j+1)+"_E","TIARABARREL_MATCHSTICK_B"+itoa(i+1)+"_DOWNSTREAM"+itoa(j+1)+"_E")   ;

      Cal->AddParameter("TIARABARREL","BALLISTIC_B"+itoa(i+1)+"_STRIP"+itoa(j+1),"TIARABARREL_BALLISTIC_B"+itoa(i+1)+"_STRIP"+itoa(j+1))   ;
      Cal->AddParameter("TIARABARREL","B"+itoa(i+1)+"_STRIP"+itoa(j+1)+"_POS","TIARABARREL_B"+itoa(i+1)+"_STRIP"+itoa(j+1)+"_POS")   ;
    
    }
  
  Cal->AddParameter("TIARABARREL","TIARABARREL/B" + itoa( i+1 ) + "_BACK_E","TIARABARREL_B" + itoa( i+1 ) + "_BACK_E");

  }
  return;

}

///////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "TiaraBarrel" , true );
  inputChain->SetBranchStatus( "fTiaraBarrel_*" , true );
  inputChain->SetBranchAddress( "TiaraBarrel" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("EventMultiplicity",true);
  inputChain->SetBranchStatus("DetectorNumber ",true);
  inputChain->SetBranchStatus("Strip_E",true);
  inputChain->SetBranchStatus("Strip_T",true);
  inputChain->SetBranchStatus("Strip_N",true);
  inputChain->SetBranchStatus("Strip_Pos",true);
  inputChain->SetBranchStatus("DownStream_E",true);
  inputChain->SetBranchStatus("DownStream_T",true);
  inputChain->SetBranchStatus("UpStream_E",true);
  inputChain->SetBranchStatus("UpStream_T",true);
  inputChain->SetBranchStatus("Back_E",true);
  inputChain->SetBranchStatus("Back_T",true);
  inputChain->SetBranchStatus("Outer_Strip_E",true);
  inputChain->SetBranchStatus("Outer_Strip_T",true);
  inputChain->SetBranchStatus("Outer_Strip_N",true);
  inputChain->SetBranchStatus("Outer_Back_E",true);
  inputChain->SetBranchStatus("Outer_Back_T",true);
}

///////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "TiaraBarrel" , "TTiaraBarrelPhysics" , &m_EventPhysics );
}

/////   Specific to TiaraBarrelArray   ////
void TTiaraBarrelPhysics::AddDetector(double X,double Y,double Z){
  cout << "Adding Tiarrel Barrel centered at : ("<<X<<";"<<Y<<";"<<Z<<")"<<endl;

  m_NumberOfDetector+=8;

  /*  
      double StripPitchSector = (Wedge_Phi_Max-Wedge_Phi_Min)/Wedge_Sector_NumberOfStrip ;
      double StripPitchRing   = (Wedge_R_Max-Wedge_R_Min)/Wedge_Ring_NumberOfStrip  ; 

      TVector3 Strip_1_1;

      m_NumberOfDetector++;
      Strip_1_1=TVector3(0,0,Z);

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneWedgeStripPositionX   ;
  vector< vector< double > >   OneWedgeStripPositionY   ;
  vector< vector< double > >   OneWedgeStripPositionZ   ;

  TVector3 StripCenter = Strip_1_1;
  for(int f = 0 ; f < Wedge_Ring_NumberOfStrip ; f++){
  lineX.clear()   ;
  lineY.clear()   ;
  lineZ.clear()   ;

  for(int b = 0 ; b < Wedge_Sector_NumberOfStrip ; b++){
  StripCenter = Strip_1_1;
  StripCenter.SetY(Wedge_R_Max-f*StripPitchRing);
  StripCenter.SetZ(Z);
  StripCenter.RotateZ(Phi+Wedge_Phi_Min+b*StripPitchSector);
  lineX.push_back( StripCenter.X() );
  lineY.push_back( StripCenter.Y() );
  lineZ.push_back( StripCenter.Z() );
  }
  OneWedgeStripPositionX.push_back(lineX);
  OneWedgeStripPositionY.push_back(lineY);
  OneWedgeStripPositionZ.push_back(lineZ);
  }
  m_StripPositionX.push_back( OneWedgeStripPositionX ) ;
  m_StripPositionY.push_back( OneWedgeStripPositionY ) ;
  m_StripPositionZ.push_back( OneWedgeStripPositionZ ) ;

  return;*/
}
///////////////////////////////////////////////////////////////////////////////
TVector3 TTiaraBarrelPhysics::GetDetectorNormal( const int i) const{

  return (TVector3(0,0,i));

}
///////////////////////////////////////////////////////////////////////////////
TVector3 TTiaraBarrelPhysics::GetPositionOfInteraction(const int i) const{
  // All in mm 
  double INNERBARREL_PCB_Width  = 27.76;
  double INNERBARREL_ActiveWafer_Length = 94.80; 
  double INNERBARREL_ActiveWafer_Width = 24.0;

  double StripPitch = INNERBARREL_ActiveWafer_Width/4. ;

  double X = Strip_N[i]*StripPitch-0.5*INNERBARREL_ActiveWafer_Width;
  double Y = INNERBARREL_PCB_Width*(0.5+sin(45*deg))  ; 
  double Z = (Strip_Pos[i]-0.5)*INNERBARREL_ActiveWafer_Length ;
  TVector3 POS(X,Y,-Z);
  POS.RotateZ((DetectorNumber[i]-1)*45*deg);
  return( POS ) ;

}
///////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelPhysics::InitializeStandardParameter(){
  /*  //   Enable all channel
      vector< bool > ChannelStatus;
      m_RingChannelStatus.clear()    ;
      m_SectorChannelStatus.clear()    ;

      ChannelStatus.resize(16,true);
      for(int i = 0 ; i < m_NumberOfDetector ; ++i){
      m_RingChannelStatus[i] = ChannelStatus;
      }

      ChannelStatus.resize(8,true);
      for(int i = 0 ; i < m_NumberOfDetector ; ++i){
      m_SectorChannelStatus[i] = ChannelStatus;
      }

      m_MaximumStripMultiplicityAllowed = m_NumberOfDetector   ;
      */
  return;
}

///////////////////////////////////////////////////////////////////////////////
//   transform an integer to a string
namespace TiaraBarrel_LOCAL{
  string itoa(unsigned int value){
    char buffer [33];
    sprintf(buffer,"%d",value);
    return buffer;
  }
}
///////////////////////////////////////////////////////////////////////////////
double TTiaraBarrelPhysics::Cal_Strip_Upstream_E(const int i){
  return CalibrationManager::getInstance()->ApplyCalibration("TIARABARREL/B" + itoa( m_EventData->GetFrontUpstreamEDetectorNbr(i) ) + "_UPSTREAM" + itoa( m_EventData->GetFrontUpstreamEStripNbr(i) ) + "_E",
      Match_Strip_Upstream_E(i) );
}
///////////////////////////////////////////////////////////////////////////////
double TTiaraBarrelPhysics::Cal_Strip_Downstream_E(const int i){
  return CalibrationManager::getInstance()->ApplyCalibration("TIARABARREL/B" + itoa( m_EventData->GetFrontDownstreamEDetectorNbr(i) ) + "_DOWNSTREAM" + itoa( m_EventData->GetFrontDownstreamEStripNbr(i) ) + "_E",
      Match_Strip_Downstream_E(i) );
}
///////////////////////////////////////////////////////////////////////////////
double TTiaraBarrelPhysics::Match_Strip_Upstream_E(const int i){
  return CalibrationManager::getInstance()->ApplyCalibration("TIARABARREL/MATCHSTICK_B" + itoa( m_EventData->GetFrontUpstreamEDetectorNbr(i) ) + "_UPSTREAM" + itoa( m_EventData->GetFrontUpstreamEStripNbr(i) ) + "_E",
      m_EventData->GetFrontUpstreamEEnergy(i) );
}
///////////////////////////////////////////////////////////////////////////////
double TTiaraBarrelPhysics::Match_Strip_Downstream_E(const int i){
  return CalibrationManager::getInstance()->ApplyCalibration("TIARABARREL/MATCHSTICK_B" + itoa( m_EventData->GetFrontDownstreamEDetectorNbr(i) ) + "_DOWNSTREAM" + itoa( m_EventData->GetFrontDownstreamEStripNbr(i) ) + "_E",
      m_EventData->GetFrontDownstreamEEnergy(i) );
}
///////////////////////////////////////////////////////////////////////////////
double TTiaraBarrelPhysics::Cal_Back_E(const int i){
  return CalibrationManager::getInstance()->ApplyCalibration("TIARABARREL/B" + itoa( m_EventData->GetBackEDetectorNbr(i) ) + "_BACK_E", m_EventData->GetBackEEnergy(i));
}

