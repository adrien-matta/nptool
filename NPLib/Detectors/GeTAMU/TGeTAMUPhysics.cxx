/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           * 
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold GeTAMU treated data                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// STL
#include <cmath>
#include <stdlib.h>
#include <limits>
using namespace std;

#include "TGeTAMUPhysics.h"

//   NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPSystemOfUnits.h"
using namespace NPUNITS;

//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TGeTAMUPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TGeTAMUPhysics::TGeTAMUPhysics()  {
    m_EventData         = new TGeTAMUData ;
    m_PreTreatedData    = new TGeTAMUData ;
    m_EventPhysics      = this ;
  };


/////////////////////////////////////////////////
void TGeTAMUPhysics::BuildPhysicalEvent(){
  PreTreat();
  unsigned int c_size = m_PreTreatedData->GetMultiplicityCore();
  unsigned int s_size = m_PreTreatedData->GetMultiplicitySegment();
  // map for add back
  map<int,double> clv_energy;   
  map<int,int> clv_segment;
  map<int,int> clv_cristal;
  map<int,double> max_core;
  map<int,double> max_segment; 
  for(unsigned int i = 0 ; i < c_size ; i++){
    int clv = m_PreTreatedData->GetCoreCloverNbr(i);
    int cri = m_PreTreatedData->GetCoreCristalNbr(i);
    double energy = m_PreTreatedData->GetCoreEnergy(i);
    // Add back energy
    clv_energy[clv] += energy;
    // Pick up the cristal
    if(energy > max_core[clv]){
      max_core[clv] = energy;
      clv_cristal[clv] = cri;
    }
    // Pick up the segment
    for(unsigned int j = 0 ; j < s_size ; j++){
      double s_energy = m_PreTreatedData->GetSegmentEnergy(j); 
      if(s_energy > max_segment[clv]){
        max_segment[clv] = s_energy;
        clv_segment[clv] = m_PreTreatedData->GetSegmentSegmentNbr(j);
      }
    }
  }

  // Fill in the info using the map
  map<int,double>::iterator it;
  for (it = clv_energy.begin(); it != clv_energy.end(); ++it) {
    int clv = it->first;
    AddBack_E.push_back(it->second);
    AddBack_T.push_back(-1000);
    AddBack_DC.push_back(-1000);
    AddBack_Theta.push_back(-1000);
    AddBack_X.push_back(-1000);
    AddBack_Y.push_back(-1000);
    AddBack_Z.push_back(-1000);
    AddBack_Clover.push_back(clv);
    AddBack_Cristal.push_back(clv_cristal[clv]);
    AddBack_Segment.push_back(clv_segment[clv]);
  }
}

/////////////////////////////////////////////////
void TGeTAMUPhysics::PreTreat(){
  static CalibrationManager* cal = CalibrationManager::getInstance();
  static string name;
  unsigned int mysize = m_EventData->GetMultiplicityCore();
  double Eraw,Energy;
  int clover, cristal, segment;
  for(unsigned int i = 0 ; i < mysize ; i++){
    Eraw = m_EventData->GetCoreEnergy(i);
    if(Eraw>0){
      clover = m_EventData->GetCoreCloverNbr(i);
      cristal = m_EventData->GetCoreCristalNbr(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_CRI"+ NPL::itoa(cristal)+"_E";
      Energy =  cal->ApplyCalibration(name, Eraw);
      m_PreTreatedData->SetCore(clover,cristal,Energy,-1000);
    }
  } 
  mysize = m_EventData->GetMultiplicitySegment();
  for(unsigned int i = 0 ; i < mysize ; i++){
    Eraw = m_EventData->GetSegmentEnergy(i);
    if(Eraw>0){
      clover = m_EventData->GetSegmentCloverNbr(i);
      segment = m_EventData->GetSegmentSegmentNbr(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_SEG"+ NPL::itoa(segment)+"_E";
      Energy =  cal->ApplyCalibration(name, Eraw);
      m_PreTreatedData->SetSegment(clover,cristal,Energy,-1000);
    }

  }
}

/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetPositionOfInteraction(unsigned int& i){
  return TVector3();
  //return GetSegmentPosition(Clover_Number[i],Cristal_Number[i],Segment_Number[i]);
}
/////////////////////////////////////////////////
// original energy, position, beta
double TGeTAMUPhysics::GetDopplerCorrectedEnergy(double& energy , TVector3 position, TVector3& beta){
  // renorm pos vector
  position.SetMag(1); 
  m_GammaLV.SetPx(energy*position.X());
  m_GammaLV.SetPy(energy*position.Y());
  m_GammaLV.SetPz(energy*position.Z());
  m_GammaLV.SetE(energy);
  m_GammaLV.Boost(-beta);
  return m_GammaLV.Energy();
}

/////////////////////////////////////////////////
void TGeTAMUPhysics::AddClover(unsigned int ID ,double R, double Theta, double Phi){
  TVector3 Pos(0,0,1);
  Pos.SetTheta(Theta);
  Pos.SetPhi(Phi);
  Pos.SetMag(R);
  m_CloverPosition[ID] = Pos;
  return;
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetCloverPosition(int& CloverNbr){
  return m_CloverPosition[CloverNbr];
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetCorePosition(int& CloverNbr,int& CoreNbr){
  static double offset = 33.4; // mm
  static double depth = 45;
  static TVector3 Pos;
  TVector3 CloverPos = m_CloverPosition[CloverNbr];

  if(CoreNbr==1)
    Pos.SetXYZ(-offset,offset,depth);
  else if(CoreNbr==2)
    Pos.SetXYZ(offset,offset,depth);
  else if(CoreNbr==3)
    Pos.SetXYZ(offset,-offset,depth);
  else if(CoreNbr==4)
    Pos.SetXYZ(-offset,-offset,depth);
  else
    cout << "Warning: GeTAMU cristal number " << CoreNbr << " is out of range (1 to 4)" << endl;

  // Define reference axis as the clover direction
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos); 
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetSegmentPosition(int& CloverNbr,int& CoreNbr, int& SegmentNbr){
  static double offsetXY1 = 10.4; // mm
  static double offsetXY2 = 16.7; // mm
  static double offsetZ1 = 15.5; // mm
  static double offsetZ2 = 60.5; // mm
  TVector3 CorePos = GetCorePosition(CloverNbr,CoreNbr);
  TVector3 CloverPos = GetCloverPosition(CloverNbr);
  static TVector3 Pos;

  if(SegmentNbr == 0 || SegmentNbr == 9)
    return GetCorePosition(CloverNbr,CoreNbr);
  else if(SegmentNbr==1)
    Pos.SetXYZ(-offsetXY1,offsetXY1,offsetZ1);
  else if(SegmentNbr==2)
    Pos.SetXYZ(offsetXY1,offsetXY1,offsetZ1);
  else if(SegmentNbr==3)
    Pos.SetXYZ(offsetXY1,-offsetXY1,offsetZ1);
  else if(SegmentNbr==4)
    Pos.SetXYZ(-offsetXY1,-offsetXY1,offsetZ1);
  else if(SegmentNbr==5)
    Pos.SetXYZ(-offsetXY2,offsetXY2,offsetZ2);
  else if(SegmentNbr==6)
    Pos.SetXYZ(offsetXY2,offsetXY2,offsetZ2);
  else if(SegmentNbr==7)
    Pos.SetXYZ(offsetXY2,-offsetXY2,offsetZ2);
  else if(SegmentNbr==8)
    Pos.SetXYZ(-offsetXY2,-offsetXY2,offsetZ2);
  else
    cout << "Warning: GeTAMU segment number " << SegmentNbr << " is out of range (0 to 9)" << endl;


  // Each cristal is a rotation of the previous one
  if (CoreNbr == 2 )
    Pos.RotateZ(90*deg);
  else if (CoreNbr == 3 )
    Pos.RotateZ(180*deg);
  else if (CoreNbr == 4)
    Pos.RotateZ(270*deg);

  // Define reference axis as the core position
  Pos.RotateUz(CorePos.Unit());
  Pos+=CorePos;
  return (Pos); 

}


/////////////////////////////////////////////////
void TGeTAMUPhysics::ReadConfiguration(string Path)  {
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;

  if(!ConfigFile.is_open()) cout << "Config File not Found" << endl ;

  string LineBuffer;
  string DataBuffer;

  bool check_CloverId= false;
  bool check_R= false; 
  bool check_Theta= false;
  bool check_Phi= false;
  bool ReadingStatus = true;

  int CloverId=0;
  double R=0;
  double Theta=0;
  double Phi=0;

  while (!ConfigFile.eof()) {

    getline(ConfigFile, LineBuffer);
    //   If line is a Start Up GeTAMU bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 13, "GeTAMUClover") == 0) {
      cout << "///" << endl ;
      cout << "GeTAMU Clover found: " << endl ;        
      ReadingStatus = true ;
    }
    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus)  {
      // Pickup Next Word 

      ConfigFile >> DataBuffer ;
      //   Comment Line 

      if (DataBuffer.compare(0, 1, "%") == 0) {ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      //   Finding another Clover toggle out (safety)
      else if (DataBuffer.compare(0, 13, "GeTAMUClover") == 0) {
        cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Clover definition" << endl ;
        ReadingStatus = false ;
      }

      else if (DataBuffer=="CloverId=") {
        check_CloverId = true;
        ConfigFile >> DataBuffer ;
        CloverId=atoi(DataBuffer.c_str());
        cout << "CloverId:  " << CloverId << endl;
      }

      else if (DataBuffer=="R=") {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str());
        cout << "R:  " << R << "mm" << endl;
      }

      else if (DataBuffer=="Theta=") {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str());
        cout << "Theta:  " << Theta << "deg" << endl;
      }

      else if (DataBuffer=="Phi=") {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str());
        cout << "Phi:  " << Phi << "deg" << endl;
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else {
        ReadingStatus = false; cout << "Wrong Token Sequence: Getting out " << DataBuffer << endl ;}
      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out
      if ( check_Theta && check_Phi && check_R && check_CloverId) { 
        ReadingStatus = false;
        check_CloverId= false;
        check_R= false; 
        check_Theta= false;
        check_Phi= false;
        AddClover(CloverId,R,Theta*3.141592653589793/180.,Phi*3.141592653589793/180.);
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "GeTAMU" , true );
  if(inputChain->FindBranch( "fTIG_*" ))
    inputChain->SetBranchStatus( "fTIG_*" , true );
  inputChain->SetBranchAddress( "GeTAMU" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::InitializeRootOutput()    {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "GeTAMU" , "TGeTAMUPhysics" , &m_EventPhysics );
}
///////////////////////////////////////////////////////////////////////////  
void TGeTAMUPhysics::Clear() {
  AddBack_E.clear();
  AddBack_DC.clear();
  AddBack_Theta.clear();
  AddBack_X.clear();
  AddBack_Y.clear();
  AddBack_Z.clear();
  AddBack_Clover.clear();
  AddBack_Cristal.clear();
  AddBack_Segment.clear();
}
///////////////////////////////////////////////////////////////////////////  
void TGeTAMUPhysics::ClearEventData() {
  m_EventData->Clear();
  m_PreTreatedData->Clear();
}
///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < 16; ++i){
    for(int cry = 0 ; cry < 4 ; cry++){
      // core are 0 and 9 , segment 1 to 8
      for( int j = 0 ; j < 10 ; ++j){
        Cal->AddParameter("GETAMU", "D"+ NPL::itoa(i+1)+"_CRI"+NPL::itoa(cry+1)+"_SEG"+ NPL::itoa(j)+"_E","GETAMU_D"+ NPL::itoa(i+1)+"_CRI"+NPL::itoa(cry+1)+"_SEG"+NPL::itoa(j)+"_E");
      }
    }
  } 
  return;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TGeTAMUPhysics::Construct(){
  return (NPL::VDetector*) new TGeTAMUPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_getamu{
  public:
    proxy_getamu(){
      NPL::DetectorFactory::getInstance()->AddToken("GeTAMU","GeTAMU");
      NPL::DetectorFactory::getInstance()->AddDetector("GeTAMU",TGeTAMUPhysics::Construct);
    }
};

proxy_getamu p;
}

