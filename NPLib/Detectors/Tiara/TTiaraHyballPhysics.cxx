/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TiaraHyball treated data                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TTiaraHyballPhysics.h"
using namespace TiaraHyball_LOCAL;

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
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif


//   ROOT
#include "TChain.h"
#include "TVector3.h" 
//#include "TRandom3.h"
//#include "random"

TRandom *Rand = new TRandom3();
///////////////////////////////////////////////////////////////////////////

ClassImp(TTiaraHyballPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTiaraHyballPhysics::TTiaraHyballPhysics(){
    EventMultiplicity   = 0 ;
    //TRandom *Rand = new TRandom3();
    m_EventData         = new TTiaraHyballData ;
    m_PreTreatedData    = new TTiaraHyballData ;
    m_EventPhysics      = this ;
    m_NumberOfDetector = 0 ;
    m_MaximumStripMultiplicityAllowed = 100;
    m_StripEnergyMatchingSigma = 1.00    ;
    m_StripEnergyMatchingNumberOfSigma = 30;

    // Threshold
    m_StripRing_E_RAW_Threshold = 0 ;
    m_StripRing_E_Threshold = 0.0 ;

    m_StripSector_E_RAW_Threshold = 0 ;
    m_StripSector_E_Threshold = 0.0 ;

    m_Take_E_Ring=false;
    m_Take_T_Sector=true;
    m_Spectra = NULL;
  }

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::BuildPhysicalEvent(){
  PreTreat();

  if( CheckEvent() == 1 ){
    vector< TVector2 > couple = Match_Ring_Sector() ;
    EventMultiplicity = couple.size();

    unsigned int MatchSize = couple.size();
    for(unsigned int i = 0 ; i < MatchSize ; ++i){

      int N = m_PreTreatedData->GetRingEDetectorNbr(couple[i].X()) ;
      int Ring = m_PreTreatedData->GetRingEStripNbr(couple[i].X()) ;
      int Sector  = m_PreTreatedData->GetSectorEStripNbr(couple[i].Y()) ;

      double Ring_E = m_PreTreatedData->GetRingEEnergy( couple[i].X() ) ;
      double Sector_E  = m_PreTreatedData->GetSectorEEnergy( couple[i].Y() ) ;

      // Search for associate Time:
      double Ring_T = -1000 ;
      unsigned int StripRingTMult = m_PreTreatedData->GetRingTMult(); 
      for(unsigned int t = 0 ; t < StripRingTMult ; ++t ){
        if(  m_PreTreatedData->GetRingEStripNbr( couple[i].X() ) == m_PreTreatedData->GetRingTStripNbr(t)
            &&m_PreTreatedData->GetRingEDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetRingTDetectorNbr(t))
          Ring_T = m_PreTreatedData->GetRingTTime(t);
      }

      // Search for associate Time:
      double Sector_T = -1000 ;
      unsigned int StripSectorTMult = m_PreTreatedData->GetSectorTMult(); 
      for(unsigned int t = 0 ; t < StripSectorTMult ; ++t ){
        if(  m_PreTreatedData->GetSectorEStripNbr( couple[i].X() ) == m_PreTreatedData->GetSectorTStripNbr(t)
            &&m_PreTreatedData->GetSectorEDetectorNbr( couple[i].X() ) == m_PreTreatedData->GetSectorTDetectorNbr(t))
          Sector_T = m_PreTreatedData->GetSectorTTime(t);
      }

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
void TTiaraHyballPhysics::PreTreat(){
  ClearPreTreatedData();
  //   Ring E
  unsigned int sizeRingE = m_EventData->GetRingEMult();
  for(unsigned int i = 0 ; i < sizeRingE ; ++i){
    if( m_EventData->GetRingEEnergy(i)>m_StripRing_E_RAW_Threshold && IsValidChannel("Ring", m_EventData->GetRingEDetectorNbr(i), m_EventData->GetRingEStripNbr(i)) ){
      double Ring_E = fStrip_Ring_E(m_EventData , i);
      if( Ring_E > m_StripRing_E_Threshold ){
        m_PreTreatedData->SetRingEDetectorNbr( m_EventData->GetRingEDetectorNbr(i) );
        m_PreTreatedData->SetRingEStripNbr( m_EventData->GetRingEStripNbr(i) );
        m_PreTreatedData->SetRingEEnergy( Ring_E );
      }
    }
  }

  //   Ring T
  unsigned int sizeRingT = m_EventData->GetRingTMult();
  for(unsigned int i = 0 ; i < sizeRingT ; ++i){
    if(IsValidChannel("Ring", m_EventData->GetRingTDetectorNbr(i), m_EventData->GetRingTStripNbr(i)) ){
      double Ring_T = fStrip_Ring_T(m_EventData , i);
      m_PreTreatedData->SetRingTDetectorNbr( m_EventData->GetRingTDetectorNbr(i) );
      m_PreTreatedData->SetRingTStripNbr( m_EventData->GetRingTStripNbr(i) );
      m_PreTreatedData->SetRingTTime( Ring_T );
    }
  }

  //  Sector E
  unsigned int sizeSectorE = m_EventData->GetSectorEMult() ;
  for(unsigned int i = 0 ; i < sizeSectorE ; ++i){
    if( m_EventData->GetSectorEEnergy(i)>m_StripSector_E_RAW_Threshold && IsValidChannel("Sector", m_EventData->GetSectorEDetectorNbr(i), m_EventData->GetSectorEStripNbr(i)) ){
      double Sector_E = fStrip_Sector_E(m_EventData , i);
      if( Sector_E > m_StripSector_E_Threshold ){
        m_PreTreatedData->SetSectorEDetectorNbr( m_EventData->GetSectorEDetectorNbr(i) );
        m_PreTreatedData->SetSectorEStripNbr( m_EventData->GetSectorEStripNbr(i) );
        m_PreTreatedData->SetSectorEEnergy( Sector_E );
      }
    }
  }

  //  Sector T
  unsigned int sizeSectorT = m_EventData->GetSectorTMult() ;
  for(unsigned int i = 0 ; i < sizeSectorT ; ++i){
    if(IsValidChannel("Sector", m_EventData->GetSectorTDetectorNbr(i), m_EventData->GetSectorTStripNbr(i)) ){
      double Sector_T = fStrip_Sector_T(m_EventData , i);
      m_PreTreatedData->SetSectorTDetectorNbr( m_EventData->GetSectorTDetectorNbr(i) );
      m_PreTreatedData->SetSectorTStripNbr( m_EventData->GetSectorTStripNbr(i) );
      m_PreTreatedData->SetSectorTTime( Sector_T );
    }
  }

  return;
}


///////////////////////////////////////////////////////////////////////////
int TTiaraHyballPhysics :: CheckEvent(){
  // Check the size of the different elements
  //  if(m_PreTreatedData->GetSectorEMult() == m_PreTreatedData->GetRingEMult() )
  return 1 ; // Regular Event

  // else
  return -1 ; // Rejected Event

}

///////////////////////////////////////////////////////////////////////////
vector < TVector2 > TTiaraHyballPhysics :: Match_Ring_Sector(){
  vector < TVector2 > ArrayOfGoodCouple ;

  // Prevent code from treating very high multiplicity Event
  // Those event are not physical anyway and that improve speed.
  if( m_PreTreatedData->GetRingEMult() > m_MaximumStripMultiplicityAllowed || m_PreTreatedData->GetSectorEMult() > m_MaximumStripMultiplicityAllowed )
    return ArrayOfGoodCouple;

  unsigned int sizeR = m_PreTreatedData->GetRingEMult();
  unsigned int sizeS = m_PreTreatedData->GetSectorEMult();

  for(unsigned int i = 0 ; i < sizeR ; i++) {
    for(unsigned int j = 0 ; j < sizeS ; j++){
      //   if same detector check energy
      if ( m_PreTreatedData->GetRingEDetectorNbr(i) == m_PreTreatedData->GetSectorEDetectorNbr(j) ){
        //   Look if energy match
        if( abs( (m_PreTreatedData->GetRingEEnergy(i)-m_PreTreatedData->GetSectorEEnergy(j))/2. ) 
            < m_StripEnergyMatchingNumberOfSigma*m_StripEnergyMatchingSigma ) {
          ArrayOfGoodCouple . push_back ( TVector2(i,j) ) ;
        }
      }
    }
  }
  //   Prevent to treat event with ambiguous matchin beetween X and Y
  if( ArrayOfGoodCouple.size() > m_PreTreatedData->GetRingEMult() ) ArrayOfGoodCouple.clear() ;

  return ArrayOfGoodCouple;
}

////////////////////////////////////////////////////////////////////////////
bool TTiaraHyballPhysics :: IsValidChannel(const string DetectorType, const int telescope , const int channel){

  if(DetectorType == "Ring")
    return *(m_RingChannelStatus[telescope-1].begin()+channel-1);

  else if(DetectorType == "Sector")
    return *(m_SectorChannelStatus[telescope-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::ReadAnalysisConfig(){
  bool ReadingStatus = false;

  // path to file
  string FileName = "./configs/ConfigTiaraHyball.dat";

  // open analysis config file
  ifstream AnalysisConfigFile;
  AnalysisConfigFile.open(FileName.c_str());

  if (!AnalysisConfigFile.is_open()) {
    cout << " No ConfigTiaraHyball.dat found: Default parameter loaded for Analayis " << FileName << endl;
    return;
  }
  cout << " Loading user parameter for Analysis from ConfigTiaraHyball.dat " << endl;

  // Save it in a TAsciiFile
  TAsciiFile* asciiConfig = RootOutput::getInstance()->GetAsciiFileAnalysisConfig();
  asciiConfig->AppendLine("%%% ConfigTiaraHyball.dat %%%");
  asciiConfig->Append(FileName.c_str());
  asciiConfig->AppendLine("");
  // read analysis config file
  string LineBuffer,DataBuffer,whatToDo;
  while (!AnalysisConfigFile.eof()) {
    // Pick-up next line
    getline(AnalysisConfigFile, LineBuffer);

    // search for "header"
    if (LineBuffer.compare(0, 11, "ConfigTiaraHyball") == 0) ReadingStatus = true;

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

        else cout << "Warning: detector type for TiaraHyball unknown!" << endl;

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
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::Clear(){
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
void TTiaraHyballPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("HyballWedge");

  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z","R","Phi"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Hyball Wedge" << i+1 <<  endl;
      double Z = blocks[i]->GetDouble("Z","mm");
      double R = blocks[i]->GetDouble("R","mm");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      AddWedgeDetector(R,Phi,Z);

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
void TTiaraHyballPhysics::InitSpectra(){  
  m_Spectra = new TTiaraHyballSpectra();
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::CheckSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::WriteSpectra(){
  m_Spectra->WriteSpectra();
}


///////////////////////////////////////////////////////////////////////////
map< string,TH1* > TTiaraHyballPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string,TH1* > empty;
    return empty ;
  }

} 
///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();

  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    for( int j = 0 ; j < 24 ; ++j){
      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_E","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_MATCHSTICK","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_MATCHSTICK")   ;

      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_T","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_RING"+NPL::itoa(j+1)+"_T")   ;
    }

    for( int j = 0 ; j < 48 ; ++j){
      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_E","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_MATCHSTICK","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_MATCHSTICK")   ;
      Cal->AddParameter("TIARAHYBALL", "D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_T","TIARAHYBALL_D"+NPL::itoa(i+1)+"_STRIP_SECTOR"+NPL::itoa(j+1)+"_T")   ;
    }
  }
  return;

}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain()   ;
  inputChain->SetBranchStatus( "TiaraHyball" , true )               ;
  inputChain->SetBranchStatus( "fTiaraHyball_*" , true )               ;
  inputChain->SetBranchAddress( "TiaraHyball" , &m_EventData )      ;
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "EventMultiplicity" , true );
  inputChain->SetBranchStatus( "EventType" , true );
  inputChain->SetBranchStatus( "DetectorNumber" , true );
  inputChain->SetBranchStatus( "Strip_E" , true );
  inputChain->SetBranchStatus( "Strip_T" , true );
  inputChain->SetBranchStatus( "StripRing_E" , true );
  inputChain->SetBranchStatus( "StripSector_T" , true );
  inputChain->SetBranchStatus( "StripRing_E" , true );
  inputChain->SetBranchStatus( "StripSector_T" , true );
  inputChain->SetBranchStatus( "Strip_Ring" , true );
  inputChain->SetBranchStatus( "Strip_Sector" , true );
}

///////////////////////////////////////////////////////////////////////////
void TTiaraHyballPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "TiaraHyball" , "TTiaraHyballPhysics" , &m_EventPhysics );
}

/////   Specific to TiaraHyballArray   ////
void TTiaraHyballPhysics::AddWedgeDetector( double R,double Phi,double Z){

  double Wedge_R_Min = 32.6+R;
  double Wedge_R_Max = 135.1+R;
  double Wedge_Phi_Min = -27.2*deg/rad;
  double Wedge_Phi_Max = 27.2*deg/rad;
  Phi= Phi*deg/rad;

  int Wedge_Ring_NumberOfStrip = 16 ;
  int Wedge_Sector_NumberOfStrip = 8 ;

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
      StripCenter.SetY(Wedge_R_Max-f*StripPitchRing-0.5*StripPitchRing);
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

  return;
}

TVector3 TTiaraHyballPhysics::GetDetectorNormal( const int i) const{

  return (TVector3(0,0,i));

}

//double TTiaraHyballPhysics::GetNumberOfDetector(const int i) const{
//  double DetectorNumber = double ( GetDetectorNumber() );

//  return(DetectorNumber) ;

//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
TVector3 TTiaraHyballPhysics::GetPositionOfInteraction(const int i) const{
  TVector3 Position = TVector3 ( GetStripPositionX(DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] )    ,
      GetStripPositionY( DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] )    ,
      GetStripPositionZ( DetectorNumber[i],Strip_Ring[i],Strip_Sector[i] ) ) ;
  return(Position) ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
TVector3 TTiaraHyballPhysics::GetRandomisedPositionOfInteraction(const int i) const{
  TVector3 RandomPosition = GetPositionOfInteraction(i);
  double Zholder = RandomPosition.Z();
  RandomPosition.SetZ(0.0);
  double R = RandomPosition.Mag();
  double Theta = RandomPosition.Theta(); // defines the inclination coordinate in a spherical coordinate system
  double Phi = RandomPosition.Phi(); // defines the azimuthal coordinate in a spherical coordinate system
  double RandomNumber1 = Rand->Rndm();
  double DeltaR = ((RandomNumber1 * 6.4)-3.2);
  R = R + DeltaR; // randomises R within a given detector ring
  double RandomNumber2 = Rand->Rndm();
  double DeltaAngle = ((RandomNumber2 * 0.118682389)-0.0593411946);
  Phi = Phi + DeltaAngle; // randomises Phi within a given detector sector
  RandomPosition.SetXYZ(R*(sin(Theta))*(cos(Phi)),R*(sin(Phi))*(sin(Theta)),Zholder);
  return(RandomPosition) ;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void TTiaraHyballPhysics::InitializeStandardParameter(){
  //   Enable all channel
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

  return;
}


///////////////////////////////////////////////////////////////////////////
namespace TiaraHyball_LOCAL{
  //   DSSD
  //   Ring
  double fStrip_Ring_E(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetRingEDetectorNbr(i) ) ;
    name+= "_STRIP_RING" ;
    name+= NPL::itoa( m_EventData->GetRingEStripNbr(i) ) ;
    name+= "_E";
    return CalibrationManager::getInstance()->ApplyCalibration(name ,
        fStrip_Ring_Matchstick(m_EventData,i) );
  }
  double fStrip_Ring_Matchstick(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetRingEDetectorNbr(i) ) ;
    name+= "_STRIP_RING" ;
    name+= NPL::itoa( m_EventData->GetRingEStripNbr(i) ) ;
    name+= "_MATCHSTICK";
    return CalibrationManager::getInstance()->ApplyCalibration(name ,
        m_EventData->GetRingEEnergy(i) );
  }

  double fStrip_Ring_T(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetRingTDetectorNbr(i) ) ;
    name+= "_STRIP_RING" ;
    name+= NPL::itoa( m_EventData->GetRingTStripNbr(i) ) ;
    name+="_T";
    return CalibrationManager::getInstance()->ApplyCalibration(name ,
        m_EventData->GetRingTTime(i) );
  }

  //   Sector
  double fStrip_Sector_E(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetSectorEDetectorNbr(i) ) ;
    name+= "_STRIP_SECTOR" ;
    name+= NPL::itoa( m_EventData->GetSectorEStripNbr(i) ) ;
    name+="_E";
    return CalibrationManager::getInstance()->ApplyCalibration(name,
        fStrip_Sector_Matchstick(m_EventData,i) );
  }

  double fStrip_Sector_Matchstick(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetSectorEDetectorNbr(i) ) ;
    name+= "_STRIP_SECTOR" ;
    name+= NPL::itoa( m_EventData->GetSectorEStripNbr(i) ) ;
    name+="_MATCHSTICK";
    return CalibrationManager::getInstance()->ApplyCalibration(name,
        m_EventData->GetSectorEEnergy(i) );
  }

  double fStrip_Sector_T(const TTiaraHyballData* m_EventData , const int i){
    static string name; name = "TIARAHYBALL/D" ;
    name+= NPL::itoa( m_EventData->GetSectorTDetectorNbr(i) ); 
    name+= "_STRIP_SECTOR" ;
    name+= NPL::itoa( m_EventData->GetSectorTStripNbr(i) ); 
    name+="_T";
    return CalibrationManager::getInstance()->ApplyCalibration(name,
        m_EventData->GetRingTTime(i) );
  }

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTiaraHyballPhysics::Construct(){
  return (NPL::VDetector*) new TTiaraHyballPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_hyball{
  public:
    proxy_hyball(){
      NPL::DetectorFactory::getInstance()->AddToken("HyballWedge","Tiara");
      NPL::DetectorFactory::getInstance()->AddDetector("HyballWedge",TTiaraHyballPhysics::Construct);
    }
};

proxy_hyball p;
}

