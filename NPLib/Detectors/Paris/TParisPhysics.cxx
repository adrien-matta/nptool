/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:  M. Labiche    contact address: marc.labiche@stfc.ac.uk  *
 *                                                                           *
 * Creation Date  : 04/12/09                                                 *
 * Last update    : 8/12/14 by A. MATTA                                      *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the physical results after NPAnalysis is run*
 *             for the PARIS detector.                                       *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    Last update: Merging of Paris and TParisPhyscis class                  *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TParisPhysics.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "NPOptionManager.h"
//  STL
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <cmath>

// Root
#include "TChain.h"
#include "TTree.h"

using namespace std;

ClassImp(TParisPhysics)

////////////////////////////////////////////////////////////////////////////////
  TParisPhysics::TParisPhysics(){
    m_NumberOfModule = 0;
    m_EventData    = new TParisData();
    m_EventPhysics = this; 
  }

////////////////////////////////////////////////////////////////////////////////
TParisPhysics::~TParisPhysics(){
  Clear();
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::BuildPhysicalEvent(){

  int multLaBrE = m_EventData->GetPARISLaBr3StageEMult();
  int multNaIE = m_EventData->GetPARISNaIStageEMult();

  //cout << "multLaBr= " << multLaBrE << endl;
  //cout << "multNaI= " << multNaIE << endl;

  ParisEventMult=multLaBrE+multNaIE;
  // get energy from strips and store it
  if(ParisEventMult>=1)
  {

    double EnergyTot=0.;

    if(multLaBrE>=1){
      //cout << "cava1b" <<endl;
      //cout <<  m_EventData->GetPARISLaBr3StageEEnergy(0) <<endl;
      //cout << "cava1b" <<endl;

      double EnergyStripFront;
      double EnergyStrip;

      for(int j=0;j<multLaBrE;j++)
      {
        EnergyStripFront= m_EventData->GetPARISLaBr3StageEEnergy(j);

        EnergyStrip  = EnergyStripFront;
        ParisLaBr_E.push_back(EnergyStrip);

        EnergyTot += EnergyStrip;
        //cout << "Energytot LaBr=" << EnergyTot << endl;
      }

      // Fill total energy in inner shell
      ParisInTotalEnergy.push_back(EnergyTot);
    }

    if(multNaIE>=1){
      double EnergySecond;
      double EnergyTotSecond;
      for(int j=0;j<multNaIE;j++)
      {
        EnergySecond = m_EventData->GetPARISNaIStageEEnergy(j);
        ParisNaI_E.push_back(EnergySecond);
        EnergyTotSecond +=EnergySecond;

        EnergyTot += EnergySecond;
        //cout << "Energy NaI=" << EnergySecond << endl;
        //cout << "Energytot NaI=" << EnergyTot << endl;
      }

      // Fill total energy in outter shell
      ParisOutTotalEnergy.push_back(EnergyTotSecond);

    }

    // Fill total energy
    ParisTotalEnergy.push_back(EnergyTot);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::Clear(){
  //EventMultiplicity= 0;
  ParisEventMult= 0;
  //ModuleNumber.clear();
  //EventType.clear();
  ParisInTotalEnergy.clear();   // inner shell
  ParisOutTotalEnergy.clear();  // outter shell
  ParisTotalEnergy.clear();

  // LaBr
  ParisLaBr_E.clear();
  //FirstStage_T.clear();
  //FirstStage_X.clear();
  //FirstStage_Y.clear();

  // NaI
  ParisNaI_E.clear();
  //SecondStage_T.clear();
  //SecondStage_N.clear();

  /*
  // NaI   
  ThirdStage_E.clear();
  ThirdStage_T.clear();
  ThirdStage_N.clear();
  */
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ParisCluster");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"A","B","C","D"};
  vector<string> sphe = {"R","THETA","PHI","BETA"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  ParisCluster " << i+1 <<  endl;
      TVector3 A = blocks[i]->GetTVector3("A","mm");
      TVector3 B = blocks[i]->GetTVector3("B","mm");
      TVector3 C = blocks[i]->GetTVector3("C","mm");
      TVector3 D = blocks[i]->GetTVector3("D","mm");

      AddDetector(A,B,C,D);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  ParisCluster " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> Beta = blocks[i]->GetVectorDouble("BETA","deg");

      AddDetector(R,Theta,Phi,Beta[0],Beta[1],Beta[2]);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::ReadCalibrationFile(string Path){
  // Order of Polynom function used for calibration
  /*   int Calibration_Si_E_Order;
       int Calibration_Si_T_Order;
       int Calibration_SiLi_E_Order;
       int Calibration_NaI_E_Order;
       */
  // Calibration_Si_X_E[DetectorNumber][StripNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_Si_X_E   ;
  vector< vector< vector< double > > >   Calibration_Si_X_T   ;
  vector< vector< vector< double > > >   Calibration_Si_Y_E   ;
  vector< vector< vector< double > > >   Calibration_Si_Y_T   ;

  // Calibration_SiLi_E[DetectorNumber][PadNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_SiLi_E   ;

  // Calibration_SiLi_E[DetectorNumber][CrystalNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_NaI_E   ;

  if (Path == "Simulation") {   // Simulation case: data already calibrated
    /*      Calibration_Si_E_Order   = 1;
            Calibration_Si_T_Order   = 1;
            Calibration_SiLi_E_Order = 1;
            Calibration_NaI_E_Order  = 1;
            */
    vector<double> Coef;
    // Order 0            Order 1
    Coef.push_back(0) ; Coef.push_back(1)    ;

    vector< vector<double> > StripLine       ;
    StripLine.resize( 128 , Coef)         ;

    Calibration_Si_X_E.resize( m_NumberOfModule , StripLine)   ;
    Calibration_Si_X_T.resize( m_NumberOfModule , StripLine)   ;
    Calibration_Si_Y_E.resize( m_NumberOfModule , StripLine)   ;
    Calibration_Si_Y_T.resize( m_NumberOfModule , StripLine)   ;

    Calibration_SiLi_E.resize( m_NumberOfModule , StripLine)   ;
    Calibration_NaI_E .resize( m_NumberOfModule , StripLine)   ;
  }
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("PARIS", true);
  inputChain->SetBranchStatus("fPARIS*", true);
  inputChain->SetBranchAddress("PARIS", &m_EventData);
}


////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("PARIS", "TParisPhysics", &m_EventPhysics);
}
void TParisPhysics::AddModuleSquare(TVector3 C_X1_Y1,
    TVector3 C_X128_Y1,
    TVector3 C_X1_Y128,
    TVector3 C_X128_Y128)
{
  m_NumberOfModule++;

  // remove warning using C_X128_Y128
  C_X128_Y128.Unit();

  // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U = C_X128_Y1 - C_X1_Y1;
  U = U.Unit();

  // Vector V on Module Face (parallele to X Strip)
  TVector3 V = C_X1_Y128 - C_X1_Y1;
  V = V.Unit();

  // Position Vector of Strip Center
  TVector3 StripCenter = TVector3(0,0,0);
  // Position Vector of X=1 Y=1 Strip
  TVector3 Strip_1_1;

  // Geometry Parameter
  double Face = 98;      // mm
  double NumberOfStrip = 128;
  double StripPitch = Face/NumberOfStrip; // mm

  // Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector< vector< double > >   OneModuleStripPositionX;
  vector< vector< double > >   OneModuleStripPositionY;
  vector< vector< double > >   OneModuleStripPositionZ;

  // Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.);

  for (int i = 0; i < NumberOfStrip; i++) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < NumberOfStrip; j++) {
      StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  );
      //         StripCenter += -TargetPosition;

      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneModuleStripPositionX.push_back(lineX);
    OneModuleStripPositionY.push_back(lineY);
    OneModuleStripPositionZ.push_back(lineZ);
  }

  m_StripPositionX.push_back( OneModuleStripPositionX );
  m_StripPositionY.push_back( OneModuleStripPositionY );
  m_StripPositionZ.push_back( OneModuleStripPositionZ );
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::AddModuleSquare(double theta,
    double phi,
    double distance,
    double beta_u,
    double beta_v,
    double beta_w)
{
  m_NumberOfModule++;

  // convert from degree to radian:
  double Pi = 3.141592654;
  theta = theta * Pi/180. ;
  phi   = phi   * Pi/180. ;

  // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U ;
  // Vector V on Module Face (parallele to X Strip)
  TVector3 V ;
  // Vector W normal to Module Face (pointing NaI)
  TVector3 W ;
  // Vector position of Module Face center
  TVector3 C ;

  C = TVector3(distance * sin(theta) * cos(phi),
      distance * sin(theta) * sin(phi),
      distance * cos(theta));

  TVector3 YperpC = TVector3( cos(theta) * cos(phi),
      cos(theta) * sin(phi),
      -sin(theta));

  W = C.Unit();
  U = W.Cross(YperpC);
  V = W.Cross(U);

  U = U.Unit();
  V = V.Unit();

  U.Rotate( beta_u * Pi/180. , U ) ;
  V.Rotate( beta_u * Pi/180. , U ) ;

  U.Rotate( beta_v * Pi/180. , V ) ;
  V.Rotate( beta_v * Pi/180. , V ) ;

  U.Rotate( beta_w * Pi/180. , W ) ;
  V.Rotate( beta_w * Pi/180. , W ) ;

  double Face = 98; // mm
  double NumberOfStrip = 128;
  double StripPitch = Face/NumberOfStrip; // mm

  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector< vector< double > >   OneModuleStripPositionX;
  vector< vector< double > >   OneModuleStripPositionY;
  vector< vector< double > >   OneModuleStripPositionZ;

  double X, Y, Z;

  // Moving C to the 1.1 corner:
  C.SetX( C.X() - ( Face/2 - StripPitch/2 ) * ( V.X() + U.X() ) )  ;
  C.SetY( C.Y() - ( Face/2 - StripPitch/2 ) * ( V.Y() + U.Y() ) )  ;
  C.SetZ( C.Z() - ( Face/2 - StripPitch/2 ) * ( V.Z() + U.Z() ) )  ;

  for (int i = 0; i < NumberOfStrip; i++) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < NumberOfStrip; j++) {
      X = C.X() + StripPitch * ( U.X()*i + V.X()*j );
      Y = C.Y() + StripPitch * ( U.Y()*i + V.Y()*j );
      Z = C.Z() + StripPitch * ( U.Z()*i + V.Z()*j );

      lineX.push_back(X);
      lineY.push_back(Y);
      lineZ.push_back(Z);
    }

    OneModuleStripPositionX.push_back(lineX);
    OneModuleStripPositionY.push_back(lineY);
    OneModuleStripPositionZ.push_back(lineZ);
  }

  m_StripPositionX.push_back( OneModuleStripPositionX );
  m_StripPositionY.push_back( OneModuleStripPositionY );
  m_StripPositionZ.push_back( OneModuleStripPositionZ );
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::AddDetector(TVector3 C_X1_Y1,
    TVector3 C_X128_Y1,
    TVector3 C_X1_Y128,
    TVector3 C_X128_Y128)
{
  m_NumberOfModule++;

  // remove warning using C_X128_Y128
  C_X128_Y128.Unit();

  // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U = C_X128_Y1 - C_X1_Y1;
  U = U.Unit();

  // Vector V on Module Face (parallele to X Strip)
  TVector3 V = C_X1_Y128 - C_X1_Y1;
  V = V.Unit();

  // Position Vector of Strip Center
  TVector3 StripCenter = TVector3(0,0,0);
  // Position Vector of X=1 Y=1 Strip
  TVector3 Strip_1_1;

  // Geometry Parameter
  double Face = 50;      // mm
  double NumberOfStrip = 128;
  double StripPitch = Face/NumberOfStrip; // mm

  // Buffer object to fill Position Array
  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector< vector< double > >   OneModuleStripPositionX;
  vector< vector< double > >   OneModuleStripPositionY;
  vector< vector< double > >   OneModuleStripPositionZ;

  // Moving StripCenter to 1.1 corner:
  Strip_1_1 = C_X1_Y1 + (U+V) * (StripPitch/2.);

  for (int i = 0; i < NumberOfStrip; i++) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < NumberOfStrip; j++) {
      StripCenter  = Strip_1_1 + StripPitch*( i*U + j*V  );
      //         StripCenter += -TargetPosition;

      lineX.push_back( StripCenter.X() );
      lineY.push_back( StripCenter.Y() );
      lineZ.push_back( StripCenter.Z() );
    }

    OneModuleStripPositionX.push_back(lineX);
    OneModuleStripPositionY.push_back(lineY);
    OneModuleStripPositionZ.push_back(lineZ);
  }

  m_StripPositionX.push_back( OneModuleStripPositionX );
  m_StripPositionY.push_back( OneModuleStripPositionY );
  m_StripPositionZ.push_back( OneModuleStripPositionZ );
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::AddDetector(double theta,
    double phi,
    double distance,
    double beta_u,
    double beta_v,
    double beta_w)
{
  m_NumberOfModule++;

  // convert from degree to radian:
  double Pi = 3.141592654;
  theta = theta * Pi/180. ;
  phi   = phi   * Pi/180. ;

  // Vector U on Module Face 
  TVector3 U ;
  // Vector V on Module Face
  TVector3 V ;
  // Vector W normal to Module Face 
  TVector3 W ;
  // Vector position of Module Face center
  TVector3 C ;

  C = TVector3(distance * sin(theta) * cos(phi),
      distance * sin(theta) * sin(phi),
      distance * cos(theta));

  TVector3 YperpW = TVector3( cos(theta) * cos(phi),
      cos(theta) * sin(phi),
      -sin(theta));

  W = C.Unit();
  U = W.Cross(YperpW);
  V = W.Cross(U);

  U = U.Unit();
  V = V.Unit();

  U.Rotate( beta_u * Pi/180. , U ) ;
  V.Rotate( beta_u * Pi/180. , U ) ;

  U.Rotate( beta_v * Pi/180. , V ) ;
  V.Rotate( beta_v * Pi/180. , V ) ;

  U.Rotate( beta_w * Pi/180. , W ) ;
  V.Rotate( beta_w * Pi/180. , W ) ;

  double Face = 50; // mm
  double NumberOfStrip = 25;
  double StripPitch = Face/NumberOfStrip; // mm

  vector<double> lineX;
  vector<double> lineY;
  vector<double> lineZ;

  vector< vector< double > >   OneModuleStripPositionX;
  vector< vector< double > >   OneModuleStripPositionY;
  vector< vector< double > >   OneModuleStripPositionZ;

  double X, Y, Z;

  // Moving C to the 1.1 corner:
  C.SetX( C.X() - ( Face/2 - StripPitch/2 ) * ( V.X() + U.X() ) )  ;
  C.SetY( C.Y() - ( Face/2 - StripPitch/2 ) * ( V.Y() + U.Y() ) )  ;
  C.SetZ( C.Z() - ( Face/2 - StripPitch/2 ) * ( V.Z() + U.Z() ) )  ;

  for (int i = 0; i < NumberOfStrip; i++) {
    lineX.clear();
    lineY.clear();
    lineZ.clear();

    for (int j = 0; j < NumberOfStrip; j++) {
      X = C.X() + StripPitch * ( U.X()*i + V.X()*j );
      Y = C.Y() + StripPitch * ( U.Y()*i + V.Y()*j );
      Z = C.Z() + StripPitch * ( U.Z()*i + V.Z()*j );

      lineX.push_back(X);
      lineY.push_back(Y);
      lineZ.push_back(Z);
    }

    OneModuleStripPositionX.push_back(lineX);
    OneModuleStripPositionY.push_back(lineY);
    OneModuleStripPositionZ.push_back(lineZ);
  }

  m_StripPositionX.push_back( OneModuleStripPositionX );
  m_StripPositionY.push_back( OneModuleStripPositionY );
  m_StripPositionZ.push_back( OneModuleStripPositionZ );
}

////////////////////////////////////////////////////////////////////////////////
double TParisPhysics::GetEnergyDeposit(){
  if (m_EventPhysics->ParisTotalEnergy.size() > 0)
    return m_EventPhysics->ParisTotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
double TParisPhysics::GetEnergyInDeposit(){// inner Layer
  if (m_EventPhysics->ParisInTotalEnergy.size() > 0)
    return m_EventPhysics->ParisInTotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
double TParisPhysics::GetEnergyOutDeposit(){// Outer Layer
  if (m_EventPhysics->ParisOutTotalEnergy.size() > 0)
    return m_EventPhysics->ParisOutTotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
TVector3 TParisPhysics::GetPositionOfInteraction(){
  TVector3 Position = TVector3(-1000,-1000,-1000);

  /*  if (m_EventPhysics->ModuleNumber.size() == 1) {
      Position = TVector3(GetStripPositionX(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0]),
      GetStripPositionY(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0]),
      GetStripPositionZ(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0]));
      }*/

  return(Position);
}

////////////////////////////////////////////////////////////////////////////////
void TParisPhysics::Print(){
  cout << "Number of Modules: " << m_NumberOfModule << endl;

  for (int f = 0; f < m_NumberOfModule; f++) {
    cout << "Module " << f+1 << endl;

    for (int i = 0; i < 128; i++) {
      for (int j = 0; j < 128; j++) {
        cout << i+1 << "  "<< j+1 << "  "
          << m_StripPositionX[f][i][j]   << "  "
          << m_StripPositionY[f][i][j]   << "  "
          << m_StripPositionZ[f][i][j]   << "  "
          << endl ;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TParisPhysics::Construct(){
  return (NPL::VDetector*) new TParisPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_paris{
  public:
    proxy_paris(){
      NPL::DetectorFactory::getInstance()->AddToken("Paris","Paris");
      NPL::DetectorFactory::getInstance()->AddDetector("Paris",TParisPhysics::Construct);
    }
};

proxy_paris p;
}

