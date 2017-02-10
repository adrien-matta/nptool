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

  for(unsigned int iPixel = 0 ; iPixel < Singles_E.size() ; iPixel++){
    int clv = Singles_Clover[iPixel];
    int cry = Singles_Crystal[iPixel];
    int seg = Singles_Segment[iPixel];
    double energy = Singles_E[iPixel];
    double X = Singles_X[iPixel];
    double Y = Singles_Y[iPixel];
    double Z = Singles_Z[iPixel];
    double Theta = Singles_Theta[iPixel];
    //cout << clv << " " << cry << " " << seg << " " << X << " " << Y << " " << Z << " "<< energy << endl;
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
  map<int,double> max_cry;
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
    if(energy > max_cry[clv]){
      max_cry[clv] = energy;
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
    if(energy > max_cry[clv]){
      max_cry[clv] = energy;
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

} // end

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
      Singles_CloverMap_CryEN[clover].push_back(crystal);
      Singles_CloverMap_CryE[clover].push_back(Energy);
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
      Singles_CloverMap_CryTN[clover].push_back(crystal);
      Singles_CloverMap_CryT[clover].push_back(Time);
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
      Singles_CloverMap_SegEN[clover].push_back(segment);
      Singles_CloverMap_SegE[clover].push_back(Energy);
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
      Singles_CloverMap_CryTN[clover].push_back(segment);
      Singles_CloverMap_CryT[clover].push_back(Time);
      m_PreTreatedData->SetSegmentT(clover,segment,Time);
    }
  }

if(m_PreTreatedData->GetMultiplicityCoreE())   FillSingles();

}

void TGeTAMUPhysics::FillSingles(void){
//match segments data to cores data,
// Rule 0: If more than 1 core is fired, there are as many hits as there are fired cores
// Rule 1: segments are auxilliary to cores, they only point out the reduced "pixel" if any, (pixel== overlap segment/core)
//Ideas: try to kick one segment out by sum rule

//cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
//m_PreTreatedData->Dump();

vector <int>    CryEN, SegEN;
vector <double> CryE, SegE;

for (unsigned iClover=0; iClover<4; iClover++){
  int clover = iClover+1;
  CryEN.clear();
  SegEN.clear();
  CryE.clear();
  SegE.clear();
  //Energy
  if(Singles_CloverMap_CryEN.find(iClover+1) != Singles_CloverMap_CryEN.end()){
    CryEN  = Singles_CloverMap_CryEN[iClover+1];
    CryE   = Singles_CloverMap_CryE[iClover+1];
  }
  else
    continue; // no need to go further if Cores energies are non existant

  if(Singles_CloverMap_SegEN.find(iClover+1) != Singles_CloverMap_SegEN.end()){
    SegEN   = Singles_CloverMap_SegEN[iClover+1];
    SegE    = Singles_CloverMap_SegE[iClover+1];
  }

/*
//calculate total energy for sum rules
  double totCryE =0;
  double totSegE =0;
  for (unsigned i=0 ; i < CryE.size();i++)    totCryE+=CryE[i];
  for (unsigned i=0 ; i < SegE.size();i++)   totSegE+=SegE[i];
*/

//sort the crystal energies;
  int swapEN;
  double swapE;
  int size = (int) CryE.size();
for (int i=0; i< (size -1); i++){    // element to be compared
  for(int j = (i+1); j < size; j++){   // rest of the elements
    if (CryE[i] < CryE[j]){          // descending order
      swapE= CryE[i];    CryE[i] = CryE[j];   CryE[j] = swapE;
      swapEN= CryEN[i];   CryEN[i] = CryEN[j];   CryEN[j] = swapEN;
    }
  }
}

  //create hit matrix
  int pixel[4][3];
  memset(pixel,0,sizeof(pixel)); // filling the matrix with zeros
  //fill the cores
  for (unsigned iCry = 0 ; iCry < CryEN.size() ; iCry++){
    int Cry = CryEN[iCry] ;
    for (unsigned iSeg = 1 ; iSeg <= 3 ; iSeg++){
      // (Segment 3 (right) + Cry 1 or 4) or (Segment 1 (Left)  + Cry 2 or 3) are impossible
      if( ((Cry==1 || Cry==4) && iSeg!=3) ||  ((Cry==2 || Cry==3) && iSeg!=1) ){
        pixel[Cry-1][iSeg-1]++;
      }
    }
  }
  //fill the segments
  for (unsigned iSeg = 0 ; iSeg < SegEN.size() ; iSeg++){
    int Seg = SegEN[iSeg];
    for (unsigned iCry = 1 ; iCry <= 4 ; iCry++){
      if( pixel[iCry-1][Seg-1] != 0){ // Only if the pixel was filled by a Cry signal previously, access it
        pixel[iCry-1][Seg-1]++;
      }
    }
  }

  // show
  /*
  int* apixel = *pixel;
  cout <<endl<<"------- Clover "<< clover << endl;
  for (unsigned i = 0 ; i < 4 ; i++){
    for (unsigned j = 0 ; j < 3 ; j++)
        cout << *(apixel+(i*3)+j) << " ";
    cout << endl;
  }
   cout <<"----------------------- " <<endl;
  */
   //Calculate the singles
  for (unsigned i = 0 ; i < CryEN.size() ; i++){
    int segment = -1;
    unsigned crystal = CryEN[i];
    unsigned segmentA = 2;
    unsigned segmentB = 3;
    if (crystal==1 || crystal==4){ // if Core 1 or 4 change the segments to segment 1 and 2
      segmentA = 1;
      segmentB = 2;
    }
    //pick between segment A or B for each case
    if (pixel[crystal-1][segmentA-1] == pixel[crystal-1][segmentB-1])
      segment = 0; // system can't be resolved
    else if (pixel[crystal-1][segmentA-1] > pixel[crystal-1][segmentB-1])
      segment = segmentA;
    else if (pixel[crystal-1][segmentA-1] < pixel[crystal-1][segmentB-1])
       segment = segmentB;

    //cout << i <<" picked: crystal " << crystal << "   segment " << segment << "  Energy " << CryE[i] << endl;
    Singles_Clover.push_back(clover);
    Singles_Crystal.push_back(CryEN[i]);
    Singles_Segment.push_back(segment);
    Singles_E.push_back(CryE[i]);
    TVector3 Pos = GetSegmentPosition(clover,CryEN[i],segment);
    Singles_X.push_back(Pos.X());
    Singles_Y.push_back(Pos.Y());
    Singles_Z.push_back(Pos.Z());
    Singles_Theta.push_back(Pos.Theta());
    //cout << " XYZ "<< Pos.X() << " "<< Pos.Y() << " "<< Pos.Z() << " Theta: " <<Pos.Theta()/deg<< endl ;
    }
  //cin.get();
//Time

  } // end of Clover loop on map

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
  direction.SetMag(1);
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
  double offset = 20; // mm
  double depth = 40; // mm
  TVector3 Pos;
  TVector3 CloverPos = GetCloverPosition(CloverNbr);

  if(CoreNbr==1)
    Pos.SetXYZ(offset,offset,depth);
  else if(CoreNbr==2)
    Pos.SetXYZ(-offset,offset,depth);
  else if(CoreNbr==3)
    Pos.SetXYZ(-offset,-offset,depth);
  else if(CoreNbr==4)
    Pos.SetXYZ(offset,-offset,depth);
  else
    cout << "Warning in GetCorePosition: GeTAMU crystal number " << CoreNbr << " is out of range (1 to 4)" << endl;

  // Define reference axis as the clover direction
  Pos.RotateUz(CloverPos.Unit());
  Pos+=CloverPos;
  return (Pos);
}
/////////////////////////////////////////////////
TVector3 TGeTAMUPhysics::GetSegmentPosition(int& CloverNbr,int& CoreNbr, int& SegmentNbr){

  if (SegmentNbr==0) return GetCorePosition(CloverNbr,CoreNbr);

   double offsetX = 20; // 20mm in GeTAMU according to measurments, 33.4 mm in TIGRESS
   double offsetY = 20;
   double depth = 40;  // 40mm in GeTAMU according to measurments, 45 mm in TIGRESS

  // Changes signs with segment/core combinations
  if (CoreNbr==3||CoreNbr==2)
    offsetX = -offsetX;
  if (CoreNbr==3||CoreNbr==4)
    offsetY = -offsetY;

  TVector3 CloverPos = GetCloverPosition(CloverNbr);
  TVector3 Pos;

  if(SegmentNbr==1 || SegmentNbr==3){ // LEFT OR RIGHT
    offsetX = 1.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
    }
  else if(SegmentNbr==2){ // MIDDLE
    offsetX = 0.5*offsetX ;
    Pos.SetXYZ(offsetX,offsetY,depth);
    }
  else

    cout << "Warning: GeTAMU segment number " << SegmentNbr
    << " is out of range\n accepted values: 0 (reserved for core) or 1-3 (Left, Middle, Right segment) " << endl;

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

  Singles_CloverMap_CryEN.clear(); // cry number energy
  Singles_CloverMap_SegEN.clear(); // seg number
  Singles_CloverMap_CryE.clear(); // cry energy
  Singles_CloverMap_SegE.clear(); // seg energy
  Singles_CloverMap_CryTN.clear(); // cry number time
  Singles_CloverMap_SegTN.clear(); // seg number
  Singles_CloverMap_CryT.clear(); // cry energy
  Singles_CloverMap_SegT.clear(); // seg energy
  Singles_E.clear();
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
  for(int det = 0 ; det < 4; det++){ //4 detectors

    for(int cry = 0 ; cry < 4 ; cry++){ // 4 crystals
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E");
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_CRY"+NPL::itoa(cry+1)+"_T");
    }
    for( int seg = 0 ; seg < 3 ; seg++){ // 3 segments
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_SEG"+ NPL::itoa(seg+1)+"_E","GETAMU_D"+ NPL::itoa(det+1)+"_SEG"+NPL::itoa(seg+1)+"_E");
      Cal->AddParameter("GETAMU", "D"+ NPL::itoa(det+1)+"_SEG"+ NPL::itoa(seg+1)+"_T","GETAMU_D"+ NPL::itoa(det+1)+"_SEG"+NPL::itoa(seg+1)+"_T");
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
