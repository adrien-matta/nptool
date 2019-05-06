/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam  contact address: mohamad.moukaddam@iphc.cnrs.fr                        *
 *                                                                           *
 * Creation Date  : November 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Treated  data                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

#include "TPyramidPhysics.h"
using namespace Pyramid_LOCAL;
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
#include "TRandom3.h"

TRandom *Random = new TRandom3();
///////////////////////////////////////////////////////////////////////////

ClassImp(TPyramidPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TPyramidPhysics::TPyramidPhysics(){
    EventMultiplicity   = 0 ;
    m_EventData         = new TPyramidData ;
    m_PreTreatedData    = new TPyramidData ;
    m_PreTreatedMSData  = new TPyramidData ;
    m_EventPhysics      = this ;
    m_NumberOfDetector  = 0 ;

    m_Take_E_Strip= true;
    m_Take_T_Back=true;
    m_Strip_E_Threshold = 300*keV;
    m_Back_E_Threshold = 10*keV;
    m_Maximum_FrontBack_Difference = 30*keV;
    m_OuterBack_E_Threshold = 50*keV;
    m_Spectra = NULL ;
  }

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::BuildPhysicalEvent(){
  PreTreat(); // At this stage all vectors are fo same size, & each event share the same detector/strip

  unsigned int sizeU = m_PreTreatedData->GetFrontUpstreamEMult();
  unsigned int sizeO = m_PreTreatedData->GetOuterEMult();
  EventMultiplicity = sizeU;

  //m_PreTreatedData->Dump();
  static string name; // token 

  for(unsigned int i = 0 ; i < sizeU ; i++){  
    // detector and strip 
    int det =  m_PreTreatedData->GetFrontUpstreamEDetectorNbr(i);
    int strip = m_PreTreatedData->GetFrontUpstreamEStripNbr(i) ;
    // calibrated energy 
    double EU = m_PreTreatedData->GetFrontUpstreamEEnergy(i) ;
    double ED = m_PreTreatedData->GetFrontDownstreamEEnergy(i); 
    // matchsticked energy for position calibration
    double msU = m_PreTreatedMSData->GetFrontUpstreamEEnergy(i) ; 
    double msD = m_PreTreatedMSData->GetFrontDownstreamEEnergy(i); 
 
    double RowPos = (msU-msD)/(msU+msD);
    name =  "PYRAMID/D";
    name+=NPL::itoa(det);
    name+="_STRIP";
    name+=NPL::itoa(strip);
    name+="_POS";
    double Pos = CalibrationManager::getInstance()->ApplyResistivePositionCalibration(name,RowPos); // returns ((RowPos-d)/k)

    //Fix Balistic deficit
    // calibration is applied as: (U+D)*( 1 + BD*(pow(k,2)-pow(pos-d,2)) ), 
    //                               While BD > 0 and |k| >= |pos-d| for good events
    // Get resistive shift and length, this will fix asymetries
    double d = CalibrationManager::getInstance()->GetValue(name,0); // resistive strip length shift
    double k = CalibrationManager::getInstance()->GetValue(name,1); // resistive strip half-length
    name ="PYRAMID/D";
    name+=NPL::itoa(det);
    name+="_STRIP";
    name+=NPL::itoa(strip);
    name+="_BALLISTIC";
    double BD_x_k2 =CalibrationManager::getInstance()->ApplyCalibration(name, k ); 
    double BD_x_Pos2 =CalibrationManager::getInstance()->ApplyCalibration(name, (RowPos-d) );
    double BD = (BD_x_k2 - BD_x_Pos2);

    // Fill the members
    Detector_N.push_back(det);
    Strip_N.push_back(strip);
    Strip_Pos.push_back(Pos); // position expressed in [-1;+1]
    UpStream_E.push_back(EU);
    DownStream_E.push_back(ED);  
    Strip_E.push_back( (EU+ED) * (1+BD) );
    //cout << det << " " << strip << " " << Pos << " " << RowPos <<  " " 
    //     << EU << " " << ED << " " << " " << EU+ED << endl ;
    //cin.get();
  } 

  //Outer 
  for(unsigned int i = 0 ; i < sizeO ; i++){  
    // detector and strip 
    int det =  m_PreTreatedData->GetOuterEDetectorNbr(i);
    int strip = m_PreTreatedData->GetOuterEStripNbr(i) ;
    double EO = m_PreTreatedData->GetOuterEEnergy(i);
    Outer_Detector_N.push_back(det);
    Outer_Strip_N.push_back(strip);
    Outer_Strip_E.push_back(EO); 
  }

}
///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::PreTreat(){
  ClearPreTreatedData();
  // Gain Calibration
  // The cal function first call the MatchStick one
  unsigned int sizeU = m_EventData->GetFrontUpstreamEMult();
  unsigned int sizeD = m_EventData->GetFrontDownstreamEMult();
  unsigned int sizeB = m_EventData->GetBackEMult();
  unsigned int sizeO = m_EventData->GetOuterEMult();

  for(unsigned int i = 0 ; i < sizeU ; i++){  
    double EU = Cal_Strip_Upstream_E(i) ;
    int det = m_EventData->GetFrontUpstreamEDetectorNbr(i);
    int strip = m_EventData->GetFrontUpstreamEStripNbr(i);
    int key = det*10+strip; // key of the map 
    if(EU > 0) { // threshold on energy is applied below
      m_mapU[key].push_back(EU);
      m_mapMSU[key].push_back(Match_Strip_Upstream_E(i));
    }
  }

  for(unsigned int i = 0 ; i < sizeD ; i++){  
    double ED = Cal_Strip_Downstream_E(i) ;
    int det = m_EventData->GetFrontDownstreamEDetectorNbr(i);
    int strip = m_EventData->GetFrontDownstreamEStripNbr(i);
    int key = det*10+strip; // key of the map strip ={1,2,3,4}
    if(ED > 0 ) { // threshold on energy is applied below
      m_mapD[key].push_back(ED);
      m_mapMSD[key].push_back(Match_Strip_Downstream_E(i));
    }
  }

  for(unsigned int i = 0 ; i < sizeB ; i++){  
    double EB = Cal_Back_E(i) ;
    int det = m_EventData->GetBackEDetectorNbr(i);
    int key = det; // key of the map 
    if(EB > m_Back_E_Threshold) m_mapB[key].push_back(EB);
  }

  for(unsigned int i = 0 ; i < sizeO ; i++){  
    double EO = m_EventData->GetOuterEEnergy(i);
    int det = m_EventData->GetOuterEDetectorNbr(i);
    int strip = m_EventData->GetOuterEStripNbr(i);
    int key = det*10+strip; // key of the map  OuterStrip={1,2,3,4} => key  
    if(EO > m_OuterBack_E_Threshold) m_mapO[key].push_back(EO);
  }

//Apply selection and matching 
  //NOTE about  Matching
    // Applying a strong strip-matching condition between the inner and outer  might not be adequate 
    // in the case of a large beam spot, since some particles at specific angles can fire an Inner-strip (n)
    // and an Outer-strip (n+/-1). The strip-matching can be addressed in the user Analysis.cxx

double EU, ED, EUms, EDms, EB, EO ;
map<int,vector <double> >::iterator it;

for (it= m_mapU.begin(); it!=m_mapU.end(); ++it){
  // Define the detector and strip 
  int key = it->first ; 
  int strip = (key)%10;
  int det = (key)/10;
  EU=ED=EUms=EDms=0;
  // skip any event where U and D are not present simultaneously
  if (m_mapU[key].size()==1 && m_mapD[key].size()==1){
    if( (m_mapU[key][0]+m_mapD[key][0])> m_Strip_E_Threshold ){ // U+D greater than threshold
      EU = m_mapU[key][0];
      ED = m_mapD[key][0];
      EUms = m_mapMSU[key][0];
      EDms = m_mapMSD[key][0];
      m_PreTreatedData->SetFrontUpstreamE(det,strip,EU);
      m_PreTreatedData->SetFrontDownstreamE(det,strip,ED);
      m_PreTreatedMSData->SetFrontUpstreamE(det,strip,EUms); 
      m_PreTreatedMSData->SetFrontDownstreamE(det,strip,EDms);
    }
  }
}

for (it= m_mapB.begin(); it!=m_mapB.end(); ++it){
  EB=0 ;
  int det = it->first;
  if (m_mapB[det].size()==1){
    EB = m_mapB[det][0];
    m_PreTreatedData->SetBackE(det,EB);
  }
}

for (it= m_mapO.begin(); it!=m_mapO.end(); ++it){
  EO=0; 
  int key = it->first ; 
  int strip = (key)%10;
  int det = (key)/10;
  if (m_mapO[key].size()==1){
    EO = m_mapO[key][0];
    m_PreTreatedData->SetOuterE(det,strip,EO);
  }
}

}// end

////////////////////////////////////////////////////////////////////////////
bool TPyramidPhysics :: IsValidChannel(const string DetectorType, const int detector , const int channel){

  if(DetectorType == "InnerStripUpstream")
    return *(m_InnerStripUpstreamChannelStatus[detector-1].begin()+channel-1);

  if(DetectorType == "InnerStripDownstream")
    return *(m_InnerStripDownstreamChannelStatus[detector-1].begin()+channel-1);

  else if(DetectorType == "OuterStrip")
    return *(m_OuterStripChannelStatus[detector-1].begin()+channel-1);

  if(DetectorType == "InnerBack")
    return *(m_InnerBackChannelStatus[detector-1].begin()+channel-1);

  else if(DetectorType == "OuterBack")
    return *(m_OuterBackChannelStatus[detector-1].begin()+channel-1);

  else return false;
}

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::ReadAnalysisConfig(){
}

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::Clear(){
  EventMultiplicity=0;
  Outer_Detector_N.clear();
  Detector_N.clear();
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
  m_mapU.clear();
  m_mapD.clear();
  m_mapMSU.clear(); // after matchstick
  m_mapMSD.clear(); // after matchstick
  m_mapB.clear();
  m_mapO.clear();
}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::ReadConfiguration(NPL::InputParser parser){

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Pyramid");

  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Inner","Outer","Z","ANGLE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Pyramid " << i+1 <<  endl;
      m_boolInner.push_back(blocks[i]->GetInt("Inner"));
      m_boolOuter.push_back(blocks[i]->GetInt("Outer"));
      //m_boolChamber   = blocks[i]->GetInt("Chamber");
      m_Z.push_back(blocks[i]->GetDouble("Z","mm"));
      m_ANGLE.push_back(blocks[i]->GetDouble("ANGLE","deg"));
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  blocks.clear();

  InitializeStandardParameter();
  ReadAnalysisConfig();
}
///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::InitSpectra(){  
  m_Spectra = new TPyramidSpectra(m_NumberOfDetector);
}

///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::FillSpectra(){  
  m_Spectra -> FillRawSpectra(m_EventData);
  m_Spectra -> FillPreTreatedSpectra(m_PreTreatedData);
  m_Spectra -> FillPhysicsSpectra(m_EventPhysics);
}
///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::CheckSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::ClearSpectra(){  
  // To be done
}
///////////////////////////////////////////////////////////////////////////
map< string,TH1* > TPyramidPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string,TH1* > empty;
    return empty;
  }

} 
///////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::AddParameterToCalibrationManager(){

  CalibrationManager* Cal = CalibrationManager::getInstance();
  // E and T
  for(int i = 0 ; i < m_NumberOfDetector ; ++i){
    for( int j = 0 ; j < 4 ; ++j){
      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_UPSTREAM"+NPL::itoa(j+1)+"_E","Pyramid_B"+NPL::itoa(i+1)+"_UPSTREAM"+NPL::itoa(j+1)+"_E")   ;
      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_DOWNSTREAM"+NPL::itoa(j+1)+"_E","Pyramid_B"+NPL::itoa(i+1)+"_DOWNSTREAM"+NPL::itoa(j+1)+"_E")   ;

      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_UPSTREAM"+NPL::itoa(j+1)+"_MATCHSTICK","Pyramid_B"+NPL::itoa(i+1)+"_UPSTREAM"+NPL::itoa(j+1)+"_MATCHSTICK")   ;
      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_DOWNSTREAM"+NPL::itoa(j+1)+"_MATCHSTICK","Pyramid_B"+NPL::itoa(i+1)+"_DOWNSTREAM"+NPL::itoa(j+1)+"_MATCHSTICK")   ;

      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_STRIP"+NPL::itoa(j+1)+"_BALLISTIC","Pyramid_B"+NPL::itoa(i+1)+"_STRIP"+NPL::itoa(j+1)+"_BALLISTIC")   ;
      Cal->AddParameter("Pyramid","B"+NPL::itoa(i+1)+"_STRIP"+NPL::itoa(j+1)+"_POS","Pyramid_B"+NPL::itoa(i+1)+"_STRIP"+NPL::itoa(j+1)+"_POS")   ;
    }

    Cal->AddParameter("Pyramid","B" + NPL::itoa( i+1 ) + "_BACK_E","Pyramid_B" + NPL::itoa( i+1 ) + "_BACK_E");
  }
  return;

}

///////////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "Pyramid" , true );
//  inputChain->SetBranchStatus( "fPyramid_*" , true );
  inputChain->SetBranchAddress( "Pyramid" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Pyramid" , true );
  inputChain->SetBranchStatus("EventMultiplicity",true);
  inputChain->SetBranchStatus("Detector_N",true);
  inputChain->SetBranchStatus("Outer_Detector_N",true);
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
  inputChain->SetBranchAddress("Pyramid" , &m_EventPhysics );

}

///////////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "Pyramid" , "TPyramidPhysics" , &m_EventPhysics );
}

/////   Specific to PyramidArray   ////
void TPyramidPhysics::AddDetector(double X,double Y,double Z){
  cout << "Adding Pyramid centered at : ("<<X<<";"<<Y<<";"<<Z<<")"<<endl;

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
TVector3 TPyramidPhysics::GetDetectorNormal( const int i) const{

  return (TVector3(0,0,i));

}
///////////////////////////////////////////////////////////////////////////////
TVector3 TPyramidPhysics::GetPositionOfInteraction(const int i) const{
  // All in mm 
  double INNER_PCB_Width  = 27.10;
  double INNER_ActiveWafer_Length = 94.80; 
  double INNER_ActiveWafer_Width = 22.6;
  double StripPitch = INNER_ActiveWafer_Width/8.0;
  
  //Calculate position locally as if it's detector 3 (at 12'oclock) that is hit 
  double X = (Strip_N[i]*StripPitch-0.5*INNER_ActiveWafer_Width)-(0.5*StripPitch);
  double Y = INNER_PCB_Width*(0.5+sin(45*deg));
  double Z = Strip_Pos[i]*(0.5*INNER_ActiveWafer_Length); 
  TVector3 POS(X,Y,-Z); // since RowPos = (U-D)/(U+D) => Downstream hit (i.e. Z>0) has RowPos<0, thus the sign
  POS.RotateZ((3-Detector_N[i])*45*deg);// looking downstream, Detector 1 is at 3 o'clock 
  return( POS ) ;
}
///////////////////////////////////////////////////////////////////////////////
TVector3 TPyramidPhysics::GetRandomisedPositionOfInteraction(const int i) const{
  TVector3 RandomPOS = GetPositionOfInteraction(i);
  TVector3 v1(-12.0, 27.10*(0.5+sin(45*deg)), 0.0); 
  TVector3 v2(12.0, 27.10*(0.5+sin(45*deg)), 0.0);
  v1.RotateZ((3-Detector_N[i])*45*deg);
  v2.RotateZ((3-Detector_N[i])*45*deg);
  TVector3 u = (v2-v1).Unit();
  double RandomNumber = Random->Rndm();
  TVector3 DeltaHolder((RandomNumber*5.65)-(5.65/2.),(RandomNumber*5.65)-(5.65/2.),0.0);
  TVector3 DeltaVector(u.X()*DeltaHolder.X(),u.Y()*DeltaHolder.Y(),u.Z()*DeltaHolder.Z());
  RandomPOS.SetXYZ(RandomPOS.X()+DeltaVector.X(),RandomPOS.Y()+DeltaVector.Y(),RandomPOS.Z()+DeltaVector.Z());
  return( RandomPOS );
}
///////////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::InitializeStandardParameter(){
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
double TPyramidPhysics::Cal_Strip_Upstream_E(const int i){
  static string name; name ="PYRAMID/D" ;
  name+= NPL::itoa( m_EventData->GetFrontUpstreamEDetectorNbr(i) ) ;
  name+= "_UPSTREAM" ;
  name+= NPL::itoa( m_EventData->GetFrontUpstreamEStripNbr(i) ) ;
  name+= "_E";
  double MSEnergy = Match_Strip_Upstream_E(i);
  double CalEnergy = CalibrationManager::getInstance()->ApplyCalibration(name,MSEnergy);
  return CalEnergy ;
}
///////////////////////////////////////////////////////////////////////////////
double TPyramidPhysics::Cal_Strip_Downstream_E(const int i){
  static string name; name = "PYRAMID/D" ;
  name+= NPL::itoa( m_EventData->GetFrontDownstreamEDetectorNbr(i) ) ;
  name+= "_DOWNSTREAM" ;
  name+= NPL::itoa( m_EventData->GetFrontDownstreamEStripNbr(i) ) ;
  name+= "_E";
  double MSEnergy = Match_Strip_Downstream_E(i);
  double CalEnergy = CalibrationManager::getInstance()->ApplyCalibration(name, MSEnergy);
  return CalEnergy ;
}
///////////////////////////////////////////////////////////////////////////////
double TPyramidPhysics::Match_Strip_Upstream_E(const int i){
  static string name; name = "PYRAMID/D" ;
  name+= NPL::itoa( m_EventData->GetFrontUpstreamEDetectorNbr(i) ) ;
  name+= "_UPSTREAM" ;
  name+= NPL::itoa( m_EventData->GetFrontUpstreamEStripNbr(i) ) ;
  name+= "_MATCHSTICK";
  double RawEnergy = m_EventData->GetFrontUpstreamEEnergy(i);
  double MSEnergy = CalibrationManager::getInstance()->ApplyCalibration(name,RawEnergy);
  return MSEnergy ;
}
///////////////////////////////////////////////////////////////////////////////
double TPyramidPhysics::Match_Strip_Downstream_E(const int i){
  static string name; name ="PYRAMID/D" ;
  name+= NPL::itoa( m_EventData->GetFrontDownstreamEDetectorNbr(i) ) ;
  name+= "_DOWNSTREAM" ;
  name+= NPL::itoa( m_EventData->GetFrontDownstreamEStripNbr(i) ) ;
  name+= "_MATCHSTICK"; 
  double RawEnergy = m_EventData->GetFrontDownstreamEEnergy(i);
  double MSEnergy = CalibrationManager::getInstance()->ApplyCalibration(name,RawEnergy);
  return MSEnergy;
}
///////////////////////////////////////////////////////////////////////////////
double TPyramidPhysics::Cal_Back_E(const int i){
  static string name; name = "PYRAMID/D" ;
  name+= NPL::itoa( m_EventData->GetBackEDetectorNbr(i) ) ;
  name+= "_BACK_E";
  return CalibrationManager::getInstance()->ApplyCalibration(name, m_EventData->GetBackEEnergy(i));
}

////////////////////////////////////////////////////////////////////////////
void TPyramidPhysics::WriteSpectra(){
  m_Spectra->WriteSpectra();
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TPyramidPhysics::Construct(){
  return (NPL::VDetector*) new TPyramidPhysics();
}


////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_Pyramid{
  public:
    proxy_Pyramid(){
      NPL::DetectorFactory::getInstance()->AddToken("Pyramid","Pyramid");
      NPL::DetectorFactory::getInstance()->AddDetector("Pyramid",TPyramidPhysics::Construct);
    }
};

proxy_Pyramid p_Pyramid;
}

