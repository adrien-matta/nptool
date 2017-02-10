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
 * Last update    : December 2016 m.moukaddam@surrey.ac.uk                   *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold GeTAMU treated data                                      *
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
#include "NPOptionManager.h"
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

 //Treat singles
  for(unsigned int iSeg = 0 ; iSeg < Singles_SegE.size() ; iSeg++){
    int clv = Singles_Clover[iSeg];
    int cry = Singles_Crystal[iSeg];
    int seg = Singles_Segment[iSeg];
    double energy = Singles_SegE[iSeg];
    TVector3 pos = GetPositionOfInteraction(iSeg);

    Singles_Theta.push_back(pos.Theta()/deg);
    Singles_X.push_back(pos.X());
    Singles_Y.push_back(pos.Y());
    Singles_Z.push_back(pos.Z());
    }

  // Treat addback
  unsigned int c_size_e = m_PreTreatedData->GetMultiplicityCoreE();
  unsigned int s_size_e = m_PreTreatedData->GetMultiplicitySegmentE();
  unsigned int c_size_t = m_PreTreatedData->GetMultiplicityCoreT();
  unsigned int s_size_t = m_PreTreatedData->GetMultiplicitySegmentT();

  // map for add back
  map<int,double> clv_energy;
  map<int,int> clv_segment;
  map<int,int> clv_crystal;
  map<int,double> max_core;
  map<int,double> max_segment;

  map<int,TVector3> max_pos; // first hit
  map<int,int> cry_segment;

/*


  for(unsigned int i = 0 ; i < c_size_e ; i++){
    int clv = m_PreTreatedData->GetCoreCloverNbrE(i);
    int cry = m_PreTreatedData->GetCoreCrystalNbrE(i);
    double energy = m_PreTreatedData->GetCoreEnergy(i);
    // Add back energy
    clv_energy[clv] += energy;
    // Pick up the crystal with the maximum energy in every clover
    if(energy > max_core[clv]){
      max_core[clv] = energy;
      clv_crystal[clv] = cry;
    }
    // Pick up the segment with the maximum energy in every clover
    for(unsigned int j = 0 ; j < s_size_e ; j++){
      double s_energy = m_PreTreatedData->GetSegmentEnergy(j);
      if(s_energy > max_segment[clv]){
        max_segment[clv] = s_energy;
        clv_segment[clv] = m_PreTreatedData->GetSegmentSegmentNbrE(j);
      }
    }
  }

    // Pick up the segment with the maximum energy in every clover
  for(unsigned int i = 0 ; i < c_size_e ; i++){
    int clv = m_PreTreatedData->GetCoreCloverNbrE(i);
    int cry = m_PreTreatedData->GetCoreCrystalNbrE(i);
    double energy = m_PreTreatedData->GetCoreEnergy(i);
    }

    for(unsigned int j = 0 ; j < s_size_e ; j++){
      double s_energy = m_PreTreatedData->GetSegmentEnergy(j);
      if(s_energy > max_segment[clv]){
        max_segment[clv] = s_energy;
        clv_segment[clv] = m_PreTreatedData->GetSegmentSegmentNbrE(j);
      }
    }

*/


  for(unsigned int i = 0 ; i < c_size_e ; i++){
    int clv = m_PreTreatedData->GetCoreCloverNbrE(i);
    int cry = m_PreTreatedData->GetCoreCrystalNbrE(i);
    double energy = m_PreTreatedData->GetCoreEnergy(i);
    // Add back energy
    clv_energy[clv] += energy;
    // Pick up the crystal with the maximum energy in every clover
    if(energy > max_core[clv]){
      max_core[clv] = energy;
      clv_crystal[clv] = cry;
    }
    // Pick up the segment with the maximum energy in every clover
    for(unsigned int j = 0 ; j < s_size_e ; j++){
      double s_energy = m_PreTreatedData->GetSegmentEnergy(j);
      int s_seg = m_PreTreatedData->GetSegmentSegmentNbrE(i);
      int s_cry = m_PreTreatedData->GetSegmentSegmentNbrE(i);
      if(s_energy > max_segment[clv]){
        max_segment[clv] = s_energy;
        cry_segment[clv] = s_cry;
        clv_segment[clv] = s_seg;
        //max_pos[clv] =
      }
    }
  }

  // Fill in the info using the map
  map<int,double>::iterator it;
  for (it = clv_energy.begin(); it != clv_energy.end(); ++it) {
    int clv = it->first;
    //energy
    AddBack_E.push_back(it->second);
    //AddBack_DC.push_back(-1000);
    //time
    AddBack_T.push_back(-1000);

    //geometry for first hit
    AddBack_Clover.push_back(clv);
    AddBack_Crystal.push_back(clv_crystal[clv]);
    AddBack_Segment.push_back(clv_segment[clv]);

    AddBack_Theta.push_back(-1000);
    AddBack_X.push_back(-1000);
    AddBack_Y.push_back(-1000);
    AddBack_Z.push_back(-1000);
  }

//Fill the time OR
for (unsigned i = 0 ; i < m_PreTreatedData->GetMultiplicityCoreT(); i++)
  GeTime.push_back(m_PreTreatedData->GetCoreTime(i));

}

/////////////////////////////////////////////////
void TGeTAMUPhysics::PreTreat(){

  ClearPreTreatedData();

  static CalibrationManager* cal = CalibrationManager::getInstance();
  static string name;
  unsigned int mysizeE = m_EventData->GetMultiplicityCoreE();
  unsigned int mysizeT = m_EventData->GetMultiplicityCoreT();
  double Eraw,Energy;
  double Traw,Time;
  int clover, crystal, segment;

  for(unsigned int i = 0 ; i < mysizeE ; i++){
    Eraw = m_EventData->GetCoreEnergy(i);
    if(Eraw>0){
      clover = m_EventData->GetCoreCloverNbrE(i);
      crystal = m_EventData->GetCoreCrystalNbrE(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(crystal);
      Energy =  cal->ApplyCalibration(name+"_E", Eraw);
      m_PreTreatedData->SetCoreE(clover,crystal,Energy);
    }
  }

  for(unsigned int i = 0 ; i < mysizeT ; i++){
    Traw = m_EventData->GetCoreTime(i);
    if(Traw>0){
      clover = m_EventData->GetCoreCloverNbrT(i);
      crystal = m_EventData->GetCoreCrystalNbrT(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_CRY"+ NPL::itoa(crystal);
      Time =  cal->ApplyCalibration(name+"_T", Traw);
      m_PreTreatedData->SetCoreT(clover,crystal,Time);
    }
  }

 mysizeE = m_EventData->GetMultiplicitySegmentE();
  for(unsigned int i = 0 ; i < mysizeE ; i++){
    Eraw = m_EventData->GetSegmentEnergy(i);
    if(Eraw>0){
      clover = m_EventData->GetSegmentCloverNbrE(i);
      segment = m_EventData->GetSegmentSegmentNbrE(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_SEG"+ NPL::itoa(segment);
      Energy =  cal->ApplyCalibration(name+"_E", Eraw);
      m_PreTreatedData->SetSegmentE(clover,segment,Energy);
    }
  }

 mysizeT = m_EventData->GetMultiplicitySegmentT();
  for(unsigned int i = 0 ; i < mysizeT ; i++){
    Traw = m_EventData->GetSegmentTime(i);
    if(Traw>0){
      clover = m_EventData->GetSegmentCloverNbrT(i);
      segment = m_EventData->GetSegmentSegmentNbrT(i);
      name = "GETAMU/D"+ NPL::itoa(clover)+"_SEG"+ NPL::itoa(segment);
      Time =  cal->ApplyCalibration(name+"_T", Traw);
      m_PreTreatedData->SetSegmentT(clover,segment,Time);
    }
  }

//match data and fill singles
  unsigned int sizec = m_PreTreatedData->GetMultiplicityCoreE();
  unsigned int sizes = m_PreTreatedData->GetMultiplicitySegmentE();
  //cores
  for(unsigned int iCore = 0 ; iCore < sizec ; iCore++){
    Energy = m_PreTreatedData->GetCoreEnergy(iCore);
    Singles_CoreE.push_back(Energy);
  }
  //segments
  // For geometry parameters (clover, crystal, segment) we use the segment vectors
  for(unsigned int iSeg = 0 ; iSeg < sizes ; iSeg++){
    clover = m_PreTreatedData->GetSegmentCloverNbrE(iSeg);
    crystal = m_PreTreatedData->GetSegmentCloverNbrE(iSeg);
    segment = m_PreTreatedData->GetSegmentSegmentNbrE(iSeg);
    Energy = m_PreTreatedData->GetSegmentEnergy(iSeg);
    //avoid impossible cases
    if ( !(segment==0 && (crystal==2||crystal==3)) || (segment==2 && (crystal==1||crystal==4)) ){
      Singles_Clover.push_back(clover);
      Singles_Crystal.push_back(crystal);
      Singles_Segment.push_back(segment);
      Singles_SegE.push_back(Energy);
    }
  }


}

/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetPositionOfInteraction(unsigned int& i){
  //return TVector3();
  return GetSegmentPosition(Singles_Clover[i],Singles_Crystal[i],Singles_Segment[i]);
}
/////////////////////////////////////////////////
// original energy, position, beta
double TGeTAMUPhysics::GetDopplerCorrectedEnergy(double& energy , TVector3 direction, TVector3& beta){
  // renormalise pos vector
  position.SetMag(1);
  m_GammaLV.SetPx(energy*direction.X());
  m_GammaLV.SetPy(energy*direction.Y());
  m_GammaLV.SetPz(energy*direction.Z());
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
  static double depth = 45; // mm
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
    cout << "Warning: GeTAMU crystal number " << CoreNbr << " is out of range (1 to 4)" << endl;

  // Define reference axis as the clover direction
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos);
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetSegmentPosition(int& CloverNbr,int& CoreNbr, int& SegmentNbr){

  static double offsetX = 33.4; // mm assumed the same as TIGRESS, CHECK
  static double offsetY = 33.4; //mm in case of left and right segments, CHECK
  static double depth = 45; // mm in crystal, assumed the same as TIGRESS, CHECK

  // Changes signs with segment/core combinations
  if (CoreNbr==1||CoreNbr==4)
    offsetX = -offsetX;
  if (CoreNbr==3||CoreNbr==4)
    offsetY = -offsetY;

  //impossible cases
  if ( (SegmentNbr==0 && (CoreNbr==2||CoreNbr==3)) || (SegmentNbr==2 && (CoreNbr==1||CoreNbr==4)) )
    cout << "Warning: GeTAMU segment number " << SegmentNbr
    << " and core number " << CoreNbr << " are not compatible " << endl;

  TVector3 CorePos = GetCorePosition(CloverNbr,CoreNbr);
  TVector3 CloverPos = GetCloverPosition(CloverNbr);
  static TVector3 Pos;

  if(SegmentNbr == 0)
    return CorePos;
  else if(SegmentNbr==1 || SegmentNbr==3){ // LEFT OR RIGHT
    offsetX = 1.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
    }
  else if(SegmentNbr==2){ // MIDDLE
    offsetX = 0.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
    }
  else
    cout << "Warning: GeTAMU segment number " << SegmentNbr
    << " is out of range\n accepted range: 0 (core) or 1-3 (L,M,R) " << endl;

/* // Not need in case of geTAMU, CHECK
  // Each crystal is a rotation of the previous one
  if (CoreNbr == 2 )
    Pos.RotateZ(90*deg);
  else if (CoreNbr == 3 )
    Pos.RotateZ(180*deg);
  else if (CoreNbr == 4)
    Pos.RotateZ(270*deg);
*/

  // Define reference axis as the Clover position,
  // This is a special case to GeTAMU where segmentation is with respect to clover
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos);

}


/////////////////////////////////////////////////

void TGeTAMUPhysics::ReadConfiguration(NPL::InputParser parser)  {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GeTAMU");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " clovers found " << endl;

  vector<string> token = {"CloverID","R","Theta","Phi","Beta"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      int     id = blocks[i]->GetInt("CloverID");
      vector<double> Beta = blocks[i]->GetVectorDouble("Beta","deg");
      cout << "WARNING: beta not used, need to be fixed!" << endl;
      AddClover(id,R,Theta,Phi);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}
///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus( "GeTAMU" , true );
  if(inputChain->FindBranch( "fTIG_*" ))
    inputChain->SetBranchStatus( "fTIG_*" , true ); // CHECK
  inputChain->SetBranchAddress( "GeTAMU" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::InitializeRootOutput()    {
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "GeTAMU" , "TGeTAMUPhysics" , &m_EventPhysics );
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::Clear() {

  Singles_CoreE.clear();
  Singles_SegE.clear();
  Singles_DC.clear();
  Singles_Theta.clear();
  Singles_X.clear();
  Singles_Y.clear();
  Singles_Z.clear();
  Singles_Clover.clear();
  Singles_Crystal.clear();
  Singles_Segment.clear();

  AddBack_E.clear();
  AddBack_DC.clear();
  AddBack_Theta.clear();
  AddBack_X.clear();
  AddBack_Y.clear();
  AddBack_Z.clear();
  AddBack_Clover.clear();
  AddBack_Crystal.clear();
  AddBack_Segment.clear();
  AddBack_T.clear();

  GeTime.clear();
}

///////////////////////////////////////////////////////////////////////////
void TGeTAMUPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  for(int i = 0 ; i < 16; ++i){
    for(int cry = 0 ; cry < 4 ; cry++){
      // core are 0 and 9 , segment 1 to 8
      for( int j = 0 ; j < 10 ; ++j){
        Cal->AddParameter("GETAMU", "D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+ NPL::itoa(j)+"_E","GETAMU_D"+ NPL::itoa(i+1)+"_CRY"+NPL::itoa(cry+1)+"_SEG"+NPL::itoa(j)+"_E");
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
