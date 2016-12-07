/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : April 2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS1 treated data                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TAnnularS1Physics.h"
using namespace ANNULARS1_LOCAL;

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
#include "NPDetectorFactory.h"
#include "NPSystemOfUnits.h"
using namespace NPUNITS;
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TAnnularS1Physics)
  ///////////////////////////////////////////////////////////////////////////
  TAnnularS1Physics::TAnnularS1Physics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TS1Data ;
    m_PreTreatedData    = new TS1Data ;
    m_EventPhysics      = this ;
    m_NumberOfDetector = 0 ;
    m_MaximumStripMultiplicityAllowed = 10;
    m_StripEnergyMatchingSigma = 0.060    ;
    m_StripEnergyMatchingNumberOfSigma = 3;

    // Threshold
    m_StripRing_E_RAW_Threshold = 0 ;
    m_StripRing_E_Threshold = 0.400 ;

    m_StripSector_E_RAW_Threshold = 0 ;
    m_StripSector_E_Threshold = 0.400 ;

    m_Take_E_Ring=false;
    m_Take_T_Sector=true;
  }

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////

void TAnnularS1Physics::BuildPhysicalEvent(){
  PreTreat();
  if( CheckEvent() == 1 ){
    vector< TVector2 > couple = Match_Ring_Sector() ;
    EventMultiplicity = couple.size();

    unsigned int size = couple.size();
    for(unsigned int i = 0 ; i < size ; ++i){
      int N = m_PreTreatedData->GetS1ThetaEDetectorNbr(couple[i].X()) ;

      int Ring = m_PreTreatedData->GetS1ThetaEStripNbr(couple[i].X()) ;
      int Sector  = m_PreTreatedData->GetS1PhiEStripNbr(couple[i].Y()) ;

      double Ring_E = m_PreTreatedData->GetS1ThetaEEnergy( couple[i].X() ) ;
      double Sector_E  = m_PreTreatedData->GetS1PhiEEnergy( couple[i].Y() ) ;

      double Ring_T = m_PreTreatedData->GetS1ThetaTTime( couple[i].X() ) ;
      double Sector_T  = m_PreTreatedData->GetS1PhiTTime ( couple[i].Y() ) ;

      DetectorNumber.push_back(N);
      StripRing_E.push_back(Ring_E);
      StripRing_T.push_back(Ring_T) ;
      StripSector_E.push_back(Sector_E) ;
      StripSector_T.push_back(Sector_T) ;

      if(m_Take_E_Ring)
        Strip_E.push_back(Ring_E) ;
      else
        Strip_E.push_back(Sector_E) ;

      if(m_Take_T_Sector)
        Strip_T.push_back(Sector_T) ;
      else
        Strip_T.push_back(Ring_T) ;

      Strip_Ring.push_back(Ring) ;
      Strip_Sector.push_back(Sector) ;

    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::PreTreat(){
  ClearPreTreatedData();
  //   Ring E
  unsigned int sizeRingE = m_EventData->GetS1ThetaEMult();
  for(unsigned int i = 0 ; i < sizeRingE ; ++i){
    if( m_EventData->GetS1ThetaEEnergy(i)>m_StripRing_E_RAW_Threshold && IsValidChannel("Ring", m_EventData->GetS1ThetaEDetectorNbr(i), m_EventData->GetS1ThetaEStripNbr(i)) ){
      double Ring_E = fStrip_Ring_E(m_EventData , i);
      if( Ring_E > m_StripRing_E_Threshold ){
        m_PreTreatedData->SetS1ThetaEDetectorNbr( m_EventData->GetS1ThetaEDetectorNbr(i) );
        m_PreTreatedData->SetS1ThetaEStripNbr( m_EventData->GetS1ThetaEStripNbr(i) );
        m_PreTreatedData->SetS1ThetaEEnergy( Ring_E );
      }
    }
  }

  //   Ring T
  unsigned int sizeRingT = m_EventData->GetS1ThetaTMult();
  for(unsigned int i = 0 ; i < sizeRingT ; ++i){
    m_PreTreatedData->SetS1ThetaTDetectorNbr( m_EventData->GetS1ThetaTDetectorNbr(i) );
    m_PreTreatedData->SetS1ThetaTStripNbr( m_EventData->GetS1ThetaTStripNbr(i) );
    m_PreTreatedData->SetS1ThetaTTime( m_EventData->GetS1ThetaTTime(i) );
  }

  //   Sector E
  unsigned int sizeSectorE = m_EventData->GetS1PhiEMult();
  for(unsigned int i = 0 ; i < sizeSectorE ; ++i){
    if( m_EventData->GetS1PhiEEnergy(i)>m_StripSector_E_RAW_Threshold && IsValidChannel("Sector", m_EventData->GetS1PhiEDetectorNbr(i), m_EventData->GetS1PhiEStripNbr(i)) ){
      double Sector_E = fStrip_Sector_E(m_EventData , i);
      if( Sector_E > m_StripSector_E_Threshold ){
        m_PreTreatedData->SetS1PhiEDetectorNbr( m_EventData->GetS1PhiEDetectorNbr(i) );
        m_PreTreatedData->SetS1PhiEStripNbr( m_EventData->GetS1PhiEStripNbr(i) );
        m_PreTreatedData->SetS1PhiEEnergy( Sector_E );
      }
    }
  }

  //   Sector T
  unsigned int sizeSectorT = m_EventData->GetS1ThetaTMult();
  for(unsigned int i = 0 ; i < sizeSectorT ; ++i){
    m_PreTreatedData->SetS1PhiTDetectorNbr( m_EventData->GetS1PhiTDetectorNbr(i) );
    m_PreTreatedData->SetS1PhiTStripNbr( m_EventData->GetS1PhiTStripNbr(i) );
    m_PreTreatedData->SetS1PhiTTime( m_EventData->GetS1PhiTTime(i) );
  }

  return;
}


///////////////////////////////////////////////////////////////////////////
int TAnnularS1Physics :: CheckEvent(){
  // Check the size of the different elements
  if(         m_PreTreatedData->GetS1ThetaEMult() == m_PreTreatedData->GetS1PhiEMult() )
    return 1 ; // Regular Event

  else
    return -1 ; // Rejected Event

}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TAnnularS1Physics :: Match_Ring_Sector(){
  vector < TVector2 > ArrayOfGoodCouple ;

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_PreTreatedData->GetS1ThetaEMult() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetS1PhiEMult() > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  for(unsigned int i = 0 ; i < m_PreTreatedData->GetS1ThetaEMult(); i++) {
    for(unsigned int j = 0 ; j < m_PreTreatedData->GetS1PhiEMult(); j++){
      //   if same detector check energy
      if ( m_PreTreatedData->GetS1ThetaEDetectorNbr(i) == m_PreTreatedData->GetS1PhiEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetS1ThetaEEnergy(i)-m_PreTreatedData->GetS1PhiEEnergy(j))/2. ) < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma )
          ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
      }
    }
  }

  //   Prevent to treat event with ambiguous matchin beetween X and Y
  if( ArrayOfGoodCouple.size() > m_PreTreatedData->GetS1ThetaEMult() ) ArrayOfGoodCouple.clear() ;
  return ArrayOfGoodCouple;
}


////////////////////////////////////////////////////////////////////////////
bool TAnnularS1Physics :: IsValidChannel(const string DetectorType, const int detector , const int channel){

  if(DetectorType == "Ring"){
    return *(m_RingChannelStatus[detector-1].begin()+channel-1);
  }
  else if(DetectorType == "Sector")
    return *(m_SectorChannelStatus[detector-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigAnnularS1.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigAnnularS1.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigAnnularS1.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigAnnularS1.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigAnnularS1") == 0) ReadingStatus = true;

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
        ChannelStatus.resize(64,false);
        m_RingChannelStatus[Detector-1] = ChannelStatus;
        ChannelStatus.resize(16,false);
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

        else cout << "Warning: detector type for AnnularS1 unknown!" << endl;

      }

      else if (whatToDo=="TAKE_E_FRONT") {
        m_Take_E_Ring = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_E_BACK") {
        m_Take_E_Ring = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_FRONT") {
        m_Take_T_Sector = false;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="TAKE_T_BACK") {
        m_Take_T_Sector = true;
        cout << whatToDo << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripRing_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripRing_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_E_RAW_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripSector_E_RAW_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripSector_E_RAW_Threshold << endl;
      }

      else if (whatToDo=="STRIP_FRONT_E_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripRing_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripRing_E_Threshold << endl;
      }

      else if (whatToDo=="STRIP_BACK_THRESHOLD") {
        AnalysisConfigFile >> DataBuffer;
        m_StripSector_E_Threshold = atoi(DataBuffer.c_str());
        cout << whatToDo << " " << m_StripSector_E_Threshold << endl;
      }

      else {
        ReadingStatus = false;
      }

    }
  }

}


///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::Clear(){
  EventMultiplicity = 0;

  //   Provide a Classification of Event
  EventType.clear() ;

  // Detector
  DetectorNumber.clear() ;

  //   DSSD
  Strip_E.clear() ;
  Strip_T.clear() ;
  StripRing_E.clear() ;
  StripRing_T.clear();
  StripSector_E.clear() ;
  StripSector_T.clear() ;
  Strip_Ring.clear() ;
  Strip_Sector.clear() ;

}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::ReadConfiguration(NPL::InputParser parser){

 vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularS1");
  cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double Z = blocks[i]->GetDouble("Z","mm");
      AddDetector(Z);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  InitializeStandardParameter();
  ReadAnalysisConfig();
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){

    for( int j = 0 ; j < 24 ; ++j){
      Cal->AddParameter("ANNULARS1", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E",
          "ANNULARS1_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_E");
      Cal->AddParameter("ANNULARS1", "D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T",
          "ANNULARS1_D"+ NPL::itoa(i+1)+"_STRIP_FRONT"+ NPL::itoa(j+1)+"_T");
    }

    for( int j = 0 ; j < 48 ; ++j){
      Cal->AddParameter("ANNULARS1", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E",
          "ANNULARS1_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_E");
      Cal->AddParameter("ANNULARS1", "D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T",
          "ANNULARS1_D"+ NPL::itoa(i+1)+"_STRIP_BACK"+ NPL::itoa(j+1)+"_T");
    }
  }

  return;

}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "AnnularS1" , true );
  inputChain->SetBranchStatus( "fS1_*" , true );
  inputChain->SetBranchAddress( "AnnularS1" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "AnnularS1.*" , true );
}

///////////////////////////////////////////////////////////////////////////
void TAnnularS1Physics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "AnnularS1." , "TAnnularS1Physics" , &m_EventPhysics );
}

/////   Specific to AnnularS1Array   ////
void TAnnularS1Physics::AddDetector(double Z){

  double R_Min = 24;
  double R_Max = 48;

  double Phi_Min = 0  ;
  double Phi_Max = 360;

  int Ring_NumberOfStrip = 16 ;
  int Sector_NumberOfStrip = 16 ;
  int Ring_NumberOfQuadrant = 4 ;
  
  double StripPitchSector = (Phi_Max-Phi_Min)/Sector_NumberOfStrip ; //radial strip spacing in rad
  double StripPitchRing = (R_Max-R_Min)/Ring_NumberOfStrip  ; // ring strip spacing in mm

  TVector3 Strip_1_1;

  m_NumberOfDetector++;
  Strip_1_1=TVector3(0,0,Z);

  //   Buffer object to fill Position Array
  vector<double> lineX ; vector<double> lineY ; vector<double> lineZ ;

  vector< vector< double > >   OneStripPositionX   ;
  vector< vector< double > >   OneStripPositionY   ;
  vector< vector< double > >   OneStripPositionZ   ;

  TVector3 StripCenter = Strip_1_1;
  for(int q = 0 ; q < Ring_NumberOfQuadrant ; q++){
    for(int f = 0 ; f < Ring_NumberOfStrip ; f++){
    lineX.clear() ;
    lineY.clear() ;
    lineZ.clear() ;

    for(int b = 0 ; b < Sector_NumberOfStrip ; b++){
      StripCenter = Strip_1_1;
      StripCenter = TVector3(R_Min+(f+0.5)*StripPitchRing,0, Z);
      StripCenter.RotateZ((Phi_Max-(b+0.5)*StripPitchSector)*M_PI/180.);
      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneStripPositionX.push_back(lineX);
    OneStripPositionY.push_back(lineY);
    OneStripPositionZ.push_back(lineZ);
    }
  }
  m_StripPositionX.push_back( OneStripPositionX ) ;
  m_StripPositionY.push_back( OneStripPositionY ) ;
  m_StripPositionZ.push_back( OneStripPositionZ ) ;

  return;
}
///////////////////////////////
TVector3 TAnnularS1Physics::GetDetectorNormal( const int i) const{
  return (TVector3(0,0,1));
}
//////////////////////////////
TVector3 TAnnularS1Physics::GetPositionOfInteraction(const int i) const{
  TVector3    Position = TVector3 (  GetStripPositionX( DetectorNumber[i] , Strip_Ring[i] , Strip_Sector[i] )    ,
      GetStripPositionY( DetectorNumber[i] , Strip_Ring[i] , Strip_Sector[i] )    ,
      GetStripPositionZ( DetectorNumber[i] , Strip_Ring[i] , Strip_Sector[i] )    ) ;

  return(Position) ;

}
/////////////////////////////
void TAnnularS1Physics::InitializeStandardParameter(){
  //   Enable all channel
  vector< bool > ChannelStatus;
  m_RingChannelStatus.clear();
  m_SectorChannelStatus.clear();

  ChannelStatus.resize(64,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_RingChannelStatus[i] = ChannelStatus;
  }

  ChannelStatus.resize(16,true);
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    m_SectorChannelStatus[i] = ChannelStatus;
  }

  m_MaximumStripMultiplicityAllowed = m_NumberOfDetector   ;

  return;
}

///////////////////////////////////////////////////////////////////////////
namespace ANNULARS1_LOCAL{
  //   DSSD
  //   Ring
  double fStrip_Ring_E(const TS1Data* m_EventData , const int& i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "ANNULARS1/D" +NPL::itoa( m_EventData->GetS1ThetaEDetectorNbr(i) ) + "_STRIP_FRONT" +NPL::itoa( m_EventData->GetS1ThetaEStripNbr(i) ) + "_E",
        m_EventData->GetS1ThetaEEnergy(i) );
  }

  double fStrip_Ring_T(const TS1Data* m_EventData , const int& i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "ANNULARS1/D" +NPL::itoa( m_EventData->GetS1ThetaTDetectorNbr(i) ) + "_STRIP_FRONT" +NPL::itoa( m_EventData->GetS1ThetaTStripNbr(i) ) +"_T",
        m_EventData->GetS1ThetaTTime(i) );
  }

  //   Sector
  double fStrip_Sector_E(const TS1Data* m_EventData , const int& i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "ANNULARS1/D" +NPL::itoa( m_EventData->GetS1PhiEDetectorNbr(i) ) + "_STRIP_BACK" +NPL::itoa( m_EventData->GetS1PhiEStripNbr(i) ) +"_E",
        m_EventData->GetS1PhiEEnergy(i) );
  }

  double fStrip_Sector_T(const TS1Data* m_EventData , const int& i){
    return CalibrationManager::getInstance()->ApplyCalibration(   "ANNULARS1/D" +NPL::itoa( m_EventData->GetS1PhiTDetectorNbr(i) ) + "_STRIP_BACK" +NPL::itoa( m_EventData->GetS1PhiTStripNbr(i) ) +"_T",
        m_EventData->GetS1PhiTTime(i) );
  }
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TAnnularS1Physics::Construct(){
  return (NPL::VDetector*) new TAnnularS1Physics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_annulars1{
  public:
    proxy_annulars1(){
      NPL::DetectorFactory::getInstance()->AddToken("AnnularS1","AnnularS1");
      NPL::DetectorFactory::getInstance()->AddDetector("AnnularS1",TAnnularS1Physics::Construct);
    }
};

proxy_annulars1 p;
}

