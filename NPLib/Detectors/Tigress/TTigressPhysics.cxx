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
 *  This class hold Tigress treated data                                     *
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

#include "TTigressPhysics.h"

//   NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
#include "NPSystemOfUnits.h"
#include "NPOptionManager.h"
using namespace NPUNITS;

//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTigressPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTigressPhysics::TTigressPhysics()  {
    m_EventData         = new TTigressData ;
    m_PreTreatedData    = new TTigressData ;
    m_EventPhysics      = this ;
  };


/////////////////////////////////////////////////
void TTigressPhysics::BuildPhysicalEvent(){
  PreTreat();
  // Addback Map
  unsigned int mysize = Gamma_Energy.size();
  for(unsigned int i = 0 ; i < 16 ; i ++) {
    for(unsigned int g = 0 ; g < mysize ; g++){
      if(Clover_Number[g] == i+1 && Segment_Number[g]==0){
        m_map_E[i] += Gamma_Energy[g];
        if( Gamma_Energy[g]> m_map_Core_MaxE[i] ){
          m_map_Core_MaxE[i] = Gamma_Energy[g];
          m_map_Core_Crystal[i] = Crystal_Number[g];
        }
      }
      if(Clover_Number[g] == i+1 &&  Segment_Number[g]>0 &&  Segment_Number[g]<9){
        if( Gamma_Energy[g]>m_map_Segment_MaxE[i]){
          m_map_Segment_MaxE[i] = Gamma_Energy[g];
          m_map_Segment_Crystal[i] = Crystal_Number[g];
          m_map_Segment[i] = Segment_Number[g];
        }
      }
    }
  }

  // Final Addback and Doppler Correction 
  int zero = 0;
  for(int i = 0 ; i < 16 ; i++) {
    if(m_map_E.find(i)!=m_map_E.end()){
      int clover = i+1;
      TVector3 Pos;
      if(m_map_Segment_MaxE[i]>0)
        Pos = GetSegmentPosition(clover,m_map_Segment_Crystal[i],m_map_Segment[i]);
      else if(m_map_Core_MaxE[i]>0)
        Pos = GetSegmentPosition(clover,m_map_Core_Crystal[i],zero);

      if(Pos.Mag()!=0){
        static TVector3 Beta = TVector3(0,0,0.10);
        double E = GetDopplerCorrectedEnergy(m_map_E[i],Pos,Beta);
        AddBack_DC.push_back(E);
        AddBack_E.push_back(m_map_E[i]);
        AddBack_Theta.push_back(Pos.Angle(Beta)*180./3.141592653589793);
        AddBack_Clover.push_back(clover); 
        if(m_map_Segment_MaxE[i]>0){
          AddBack_Crystal.push_back(m_map_Segment_Crystal[i]);
          AddBack_Segment.push_back(m_map_Segment[i]);
        }
        else{
          AddBack_Crystal.push_back(m_map_Core_Crystal[i]);
          AddBack_Segment.push_back(0);
        }
        AddBack_X.push_back(Pos.X());
        AddBack_Y.push_back(Pos.Y());
        AddBack_Z.push_back(Pos.Z());
      }    
    }
  }
}

/////////////////////////////////////////////////
void TTigressPhysics::PreTreat(){
  static CalibrationManager* cal = CalibrationManager::getInstance();
  static string name;
  unsigned int mysize = m_EventData->GetMultiplicityGe();
  double Eraw,Energy;
  int clover, crystal, segment;
  for(unsigned int i = 0 ; i < mysize ; i++){
    Eraw = m_EventData->GetGeEnergy(i);
    if( Eraw>20000){
      clover = m_EventData->GetGeCloverNbr(i);
      crystal = m_EventData->GetGeCrystalNbr(i);
      segment = m_EventData->GetGeSegmentNbr(i);
      name = "TIGRESS/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(crystal)+"_SEG"+ NPL::itoa(segment)+"_E";
      Energy =  cal->ApplyCalibration(name, Eraw);
      Gamma_Energy.push_back(Energy);
      Clover_Number.push_back(clover);
      Crystal_Number.push_back(crystal);
      Segment_Number.push_back(segment);
      Gamma_Time.push_back(m_EventData->GetGeTimeCFD(i));

      // Look for Associate BGO
      bool BGOcheck = false ;
      for(unsigned j = 0 ;  j <  m_EventData->GetMultiplicityBGO() ; j++){

        if( m_EventData->GetBGOCloverNbr(j)== m_EventData->GetGeCloverNbr(i) && m_EventData->GetBGOEnergy(j)>20 )
          BGOcheck = true ;
      }
      BGO.push_back(BGOcheck);
    }
  }
}

/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetPositionOfInteraction(unsigned int& i){
  return GetSegmentPosition(Clover_Number[i],Crystal_Number[i],Segment_Number[i]);
}
/////////////////////////////////////////////////
// original energy, position, beta
double TTigressPhysics::GetDopplerCorrectedEnergy(double& energy , TVector3 position, TVector3& beta){
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
void TTigressPhysics::AddClover(unsigned int ID ,double R, double Theta, double Phi){
  TVector3 Pos(0,0,1);
  Pos.SetTheta(Theta);
  Pos.SetPhi(Phi);
  Pos.SetMag(R);
  m_CloverPosition[ID] = Pos;
  return;
}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetCloverPosition(int& CloverNbr){
  return m_CloverPosition[CloverNbr];
}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetCorePosition(int& CloverNbr,int& CoreNbr){
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
    cout << "Warning: Tigress crystal number " << CoreNbr << " is out of range (1 to 4)" << endl;

  // Define reference axis as the clover direction
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos); 
}
/////////////////////////////////////////////////
TVector3 TTigressPhysics::GetSegmentPosition(int& CloverNbr,int& CoreNbr, int& SegmentNbr){
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
    cout << "Warning: Tigress segment number " << SegmentNbr << " is out of range (0 to 9)" << endl;


  // Each crystal is a rotation of the previous one
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
void TTigressPhysics::ReadConfiguration(NPL::InputParser parser)  {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("TigressClover");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " clovers found " << endl; 

  vector<string> token = {"CloverID","R","Theta","Phi"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      int     id = blocks[i]->GetInt("CloverID");
      AddClover(id,R,Theta,Phi);
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }
}
///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "Tigress" , true );
  if(inputChain->FindBranch( "fTIG_*" ))
    inputChain->SetBranchStatus( "fTIG_*" , true );
  inputChain->SetBranchAddress( "Tigress" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::InitializeRootOutput()    {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "Tigress" , "TTigressPhysics" , &m_EventPhysics );
}
///////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::Clear() {
  Gamma_Energy.clear();
  Gamma_Time.clear();
  Crystal_Number.clear();
  Clover_Number.clear();
  Segment_Number.clear();
  BGO.clear();
  AddBack_E.clear();
  AddBack_DC.clear();
  AddBack_Theta.clear();
  AddBack_X.clear();
  AddBack_Y.clear();
  AddBack_Z.clear();
  m_map_E.clear();
  m_map_Core_Crystal.clear();
  m_map_Core_MaxE.clear(); 
  m_map_Segment_Crystal.clear(); 
  m_map_Segment.clear(); 
  m_map_Segment_MaxE.clear(); 
  AddBack_Clover.clear();
  AddBack_Crystal.clear();
  AddBack_Segment.clear();
}
///////////////////////////////////////////////////////////////////////////  
void TTigressPhysics::ClearEventData() {
  m_EventData->Clear();
  m_PreTreatedData->Clear();
}
///////////////////////////////////////////////////////////////////////////
void TTigressPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < 16; ++i){
    for(int cry = 0 ; cry < 4 ; cry++){
      // core are 0 and 9 , segment 1 to 8
      for( int j = 0 ; j < 10 ; ++j){
        Cal->AddParameter("TIGRESS", "D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+ NPL::itoa(j)+"_E","TIGRESS_D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+NPL::itoa(j)+"_E");
      }
    }
  } 
  return;
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTigressPhysics::Construct(){
  return (NPL::VDetector*) new TTigressPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_tigress{
  public:
    proxy_tigress(){
      NPL::DetectorFactory::getInstance()->AddToken("Tigress","Tigress");
      NPL::DetectorFactory::getInstance()->AddDetector("Tigress",TTigressPhysics::Construct);
    }
};

proxy_tigress p;
}

