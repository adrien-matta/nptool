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
 *             for the NANA detector.                                       *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of NPAnalysis.                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    Last update: Merging of Nana and TNanaPhyscis class                  *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TNanaPhysics.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"

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

ClassImp(TNanaPhysics)

  ////////////////////////////////////////////////////////////////////////////////
  TNanaPhysics::TNanaPhysics(){
    m_NumberOfModule = 0;
    m_EventData    = new TNanaData();
    m_EventPhysics =this; 
  }

////////////////////////////////////////////////////////////////////////////////
TNanaPhysics::~TNanaPhysics(){
  Clear();
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::BuildPhysicalEvent(){

  int multLaBrE = m_EventData->GetNanaLaBr3Mult();

  NanaEventMult=multLaBrE;
  // get energy from strips and store it
  if(NanaEventMult>=1){
    double EnergyTot=0.;
    double EnergyStripFront;
    double EnergyStrip;

    for(int j=0;j<multLaBrE;j++){
      DetectorNumber.push_back(m_EventData->GetNanaLaBr3DetectorNbr(j));
      EnergyStripFront= m_EventData->GetNanaLaBr3EnergyLong(j);

      EnergyStrip  = EnergyStripFront;
      LaBr_E.push_back(EnergyStrip);
      


      EnergyTot += EnergyStrip;
      //cout << "Energytot LaBr=" << EnergyTot << endl;
    }

    // Fill total energy in inner shell
    InTotalEnergy.push_back(EnergyTot);
  }
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::Clear(){
  //EventMultiplicity= 0;
  NanaEventMult= 0;
 
  //ModuleNumber.clear();
  //EventType.clear();
  InTotalEnergy.clear();   // inner shell
  OutTotalEnergy.clear();  // outter shell
  TotalEnergy.clear();

  // LaBr
  DetectorNumber.clear();
  LaBr_E.clear();
  //First_T.clear();
  //First_X.clear();
  //First_Y.clear();

  // NaI
  NaI_E.clear();
  //Second_T.clear();
  //Second_N.clear();

  /*
  // NaI   
  Third_E.clear();
  Third_T.clear();
  Third_N.clear();
  */
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::ReadConfiguration(string Path)    {
  ifstream ConfigFile              ;
  ConfigFile.open(Path.c_str())    ;
  string LineBuffer                ;
  string DataBuffer                ;

  double   Ax, Bx, Cx, Dx, Ay, By, Cy, Dy, Az, Bz, Cz, Dz;
  TVector3 A, B, C, D;
  double   Theta = 0, Phi = 0, R = 0, beta_u = 0 , beta_v = 0 , beta_w = 0;

  bool check_A = false;
  bool check_C = false;
  bool check_B = false;
  bool check_D = false;

  bool check_Theta = false;
  bool check_Phi   = false;
  bool check_R     = false;
  bool check_beta  = false;

  bool ReadingStatus = false;

  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    // If line is a Nana bloc, reading toggle to true
    // and toggle to true flags indicating which shape is treated.
    if (LineBuffer.compare(0, 12, "NanaCluster") == 0 ) {
      cout << "///////////////////////" << endl;
      cout << "Detector found:" << endl;
      ReadingStatus = true;
    }
    // Reading Block
    while (ReadingStatus) {
      ConfigFile >> DataBuffer ;
      // Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {
        ConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
      }
      // Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, 13, "NanaDetector") == 0) {
        cout << "WARNING: Another Module is find before standard sequence of Token, Error may occured in Telecope definition" << endl;
        ReadingStatus = false;
      }

      // Position method
      else if (DataBuffer=="A=") {
        check_A = true;
        ConfigFile >> DataBuffer ;
        Ax = atof(DataBuffer.c_str()) ;
        Ax = Ax  ;
        ConfigFile >> DataBuffer ;
        Ay = atof(DataBuffer.c_str()) ;
        Ay = Ay  ;
        ConfigFile >> DataBuffer ;
        Az = atof(DataBuffer.c_str()) ;
        Az = Az  ;

        A = TVector3(Ax, Ay, Az);
        cout << "X1 Y1 corner position : (" << A.X() << ";" << A.Y() << ";" << A.Z() << ")" << endl;
      }
      else if (DataBuffer=="B=") {
        check_B = true;
        ConfigFile >> DataBuffer ;
        Bx = atof(DataBuffer.c_str()) ;
        Bx = Bx  ;
        ConfigFile >> DataBuffer ;
        By = atof(DataBuffer.c_str()) ;
        By = By  ;
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;
        Bz = Bz  ;

        B = TVector3(Bx, By, Bz);
        cout << "X128 Y1 corner position : (" << B.X() << ";" << B.Y() << ";" << B.Z() << ")" << endl;
      }
      else if (DataBuffer == "C=") {
        check_C = true;
        ConfigFile >> DataBuffer ;
        Cx = atof(DataBuffer.c_str()) ;
        Cx = Cx  ;
        ConfigFile >> DataBuffer ;
        Cy = atof(DataBuffer.c_str()) ;
        Cy = Cy  ;
        ConfigFile >> DataBuffer ;
        Cz = atof(DataBuffer.c_str()) ;
        Cz = Cz  ;

        C = TVector3(Cx, Cy, Cz);
        cout << "X1 Y128 corner position : (" << C.X() << ";" << C.Y() << ";" << C.Z() << ")" << endl;
      }
      else if (DataBuffer=="D=") {
        check_D = true;
        ConfigFile >> DataBuffer ;
        Dx = atof(DataBuffer.c_str()) ;
        Dx = Dx  ;
        ConfigFile >> DataBuffer ;
        Dy = atof(DataBuffer.c_str()) ;
        Dy = Dy  ;
        ConfigFile >> DataBuffer ;
        Dz = atof(DataBuffer.c_str()) ;
        Dz = Dz  ;

        D = TVector3(Dx, Dy, Dz);
        cout << "X128 Y128 corner position : (" << D.X() << ";" << D.Y() << ";" << D.Z() << ")" << endl;
      } // End Position Method

      // Angle method
      else if (DataBuffer.compare(0, 6, "Theta=") == 0) {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str()) ;
        Theta = Theta ;
        cout << "Theta:  " << Theta << endl;
      }
      else if (DataBuffer.compare(0, 4, "Phi=") == 0) {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str()) ;
        Phi = Phi ;
        cout << "Phi:  " << Phi << endl;
      }
      else if (DataBuffer.compare(0, 2, "R=") == 0) {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str()) ;
        R = R ;
        cout << "R:  " << R << endl;
      }
      else if (DataBuffer.compare(0, 5, "Beta=") == 0) {
        check_beta = true;
        ConfigFile >> DataBuffer ;
        beta_u = atof(DataBuffer.c_str()) ;
        beta_u = beta_u    ;
        ConfigFile >> DataBuffer ;
        beta_v = atof(DataBuffer.c_str()) ;
        beta_v = beta_v    ;
        ConfigFile >> DataBuffer ;
        beta_w = atof(DataBuffer.c_str()) ;
        beta_w = beta_w    ;
        cout << "Beta:  " << beta_u << " " << beta_v << " " << beta_w << endl  ;
      }

      /////////////////////////////////////////////////
      // If All necessary information there, toggle out
      if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta)  ) {
        ReadingStatus = false;

        // Add The previously define telescope
        // With position method
        if ( check_A && check_B && check_C && check_D ) {
          AddDetector(A   ,
              B   ,
              C   ,
              D   ) ;
        }

        // with angle method
        else if ( check_Theta && check_Phi && check_R && check_beta ) {
          AddDetector(Theta,
              Phi,
              R,
              beta_u,
              beta_v,
              beta_w);
        }

        // reset boolean flag for point positioning
        check_A = false;
        check_B = false;
        check_C = false;
        check_D = false;

        // reset boolean flag for angle positioning
        check_Theta = false;
        check_Phi   = false;
        check_R     = false;
        check_beta  = false;

      } // end test for adding a module


    } // end while for reading block
  } // end while for reading file

  cout << endl << "/////////////////////////////" << endl<<endl;
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::ReadCalibrationFile(string Path){
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
void TNanaPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("Nana", true);
  inputChain->SetBranchStatus("fNANA*", true);
  inputChain->SetBranchAddress("Nana", &m_EventData);
}


////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("Nana", "TNanaPhysics", &m_EventPhysics);
}
void TNanaPhysics::AddDetector(TVector3 A,
    TVector3 B,
    TVector3 C,
    TVector3 D)
{
  m_NumberOfModule++;

  // remove warning using D
  D.Unit();

  // Vector U on Module Face (paralelle to Y Strip) (NB: remember that Y strip are allong X axis)
  TVector3 U = B - A;
  U = U.Unit();

  // Vector V on Module Face (parallele to X Strip)
  TVector3 V = C - A;
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

  vector< vector< double > >   OneModulePositionX;
  vector< vector< double > >   OneModulePositionY;
  vector< vector< double > >   OneModulePositionZ;

  // Moving StripCenter to 1.1 corner:
  Strip_1_1 = A + (U+V) * (StripPitch/2.);

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

    OneModulePositionX.push_back(lineX);
    OneModulePositionY.push_back(lineY);
    OneModulePositionZ.push_back(lineZ);
  }

  m_PositionX.push_back( OneModulePositionX );
  m_PositionY.push_back( OneModulePositionY );
  m_PositionZ.push_back( OneModulePositionZ );
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::AddDetector(double theta,
    double phi,
    double R,
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

  C = TVector3(R * sin(theta) * cos(phi),
      R * sin(theta) * sin(phi),
      R * cos(theta));

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

  vector< vector< double > >   OneModulePositionX;
  vector< vector< double > >   OneModulePositionY;
  vector< vector< double > >   OneModulePositionZ;

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

    OneModulePositionX.push_back(lineX);
    OneModulePositionY.push_back(lineY);
    OneModulePositionZ.push_back(lineZ);
  }

  m_PositionX.push_back( OneModulePositionX );
  m_PositionY.push_back( OneModulePositionY );
  m_PositionZ.push_back( OneModulePositionZ );
}

////////////////////////////////////////////////////////////////////////////////
double TNanaPhysics::GetEnergyDeposit(){
  if (m_EventPhysics->TotalEnergy.size() > 0)
    return m_EventPhysics->TotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
double TNanaPhysics::GetEnergyInDeposit(){// inner Layer
  if (m_EventPhysics->InTotalEnergy.size() > 0)
    return m_EventPhysics->InTotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
double TNanaPhysics::GetEnergyOutDeposit(){// Outer Layer
  if (m_EventPhysics->OutTotalEnergy.size() > 0)
    return m_EventPhysics->OutTotalEnergy[0];
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
TVector3 TNanaPhysics::GetPositionOfInteraction(){
  TVector3 Position = TVector3(-1000,-1000,-1000);
  return(Position);
}

////////////////////////////////////////////////////////////////////////////////
void TNanaPhysics::Print(){
  cout << "Number of Modules: " << m_NumberOfModule << endl;

  for (int f = 0; f < m_NumberOfModule; f++) {
    cout << "Module " << f+1 << endl;

    for (int i = 0; i < 128; i++) {
      for (int j = 0; j < 128; j++) {
        cout << i+1 << "  "<< j+1 << "  "
          << m_PositionX[f][i][j]   << "  "
          << m_PositionY[f][i][j]   << "  "
          << m_PositionZ[f][i][j]   << "  "
          << endl ;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TNanaPhysics::Construct(){
  return (NPL::VDetector*) new TNanaPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_nana{
  public:
    proxy_nana(){
      NPL::DetectorFactory::getInstance()->AddToken("Nana","Nana");
      NPL::DetectorFactory::getInstance()->AddDetector("Nana",TNanaPhysics::Construct);
    }
};

proxy_nana p;
}

