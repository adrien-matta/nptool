/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : October 2009                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a 20um Silicium detector                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPTool header
#include "ThinSi.hh"
#include "ObsoleteGeneralScorers.hh"
#include "ThinSiScorers.hh"
#include "RootOutput.h"
using namespace THINSI;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ThinSi Specific Method
ThinSi::ThinSi(){
  InitializeMaterial();
  m_Event = new TSSSDData();
}

ThinSi::~ThinSi(){
  delete m_MaterialSilicon;
  delete m_MaterialAl;
  delete m_MaterialVacuum;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ThinSi::AddTelescope( G4ThreeVector TL,
    G4ThreeVector BL,
    G4ThreeVector BR,
    G4ThreeVector TR)
{
  m_DefinitionType.push_back(true);

  m_TL.push_back(TL);
  m_BL.push_back(BL);
  m_BR.push_back(BR);
  m_TR.push_back(TR);

  m_R.push_back(0);
  m_Theta.push_back(0);
  m_Phi.push_back(0);
  m_beta_u.push_back(0);
  m_beta_v.push_back(0);
  m_beta_w.push_back(0);
}

void ThinSi::AddTelescope( G4double R,
    G4double Theta,
    G4double Phi,
    G4double beta_u,
    G4double beta_v,
    G4double beta_w)
{
  G4ThreeVector empty = G4ThreeVector(0, 0, 0);

  m_DefinitionType.push_back(false);

  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_beta_u.push_back(beta_u);
  m_beta_v.push_back(beta_v);
  m_beta_w.push_back(beta_w);

  m_TL.push_back(empty);
  m_BL.push_back(empty);
  m_BR.push_back(empty);
  m_TR.push_back(empty);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ThinSi::VolumeMaker(  G4int             DetNumber ,
    G4ThreeVector     Det_pos ,
    G4RotationMatrix* Det_rot ,
    G4LogicalVolume*  world   )
{
  G4double NbrTelescopes = DetNumber  ;
  G4String DetectorNumber ;
  std::ostringstream Number ;
  Number << NbrTelescopes ;
  DetectorNumber = Number.str() ;

  ////////////////////////////////////////////////////////////////
  /////////General Geometry Parameter Definition /////////////////
  ////////////////////////////////////////////////////////////////
  /////// Starting Volume Definition ///////
  G4String Name = "ThinSi" + DetectorNumber;

  G4Box* solidThinSi = new G4Box(Name+"Solid", 0.5*DetectorSize, 0.5*DetectorSize, 0.5*FrameThickness);

  G4LogicalVolume* logicThinSi =
    new G4LogicalVolume(solidThinSi, m_MaterialVacuum, Name+"_logic", 0, 0);

  new G4PVPlacement(   G4Transform3D(*Det_rot, Det_pos) ,
      logicThinSi         ,
      Name                ,
      world               ,
      false               ,
      0);

  logicThinSi->SetVisAttributes(G4VisAttributes::Invisible);
  // Frame is made of 4 thick box (2 Horizontal and 2 Vertical)
  G4Box* solidFrameHorizontal = new G4Box(Name + "_Frame", 0.5*SiliconSize, 0.5*(DetectorSize - SiliconSize) / 2, 0.5*FrameThickness*mm)   ;
  G4Box* solidFrameVertical  = new G4Box(Name + "_Frame", 0.5*(DetectorSize - SiliconSize) / 2, 0.5*DetectorSize, 0.5*FrameThickness*mm)   ;

  G4LogicalVolume* logicFrameHorizontal = new G4LogicalVolume(solidFrameHorizontal, m_MaterialAl, Name, 0, 0);
  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.0, 0.5, 1));
  logicFrameHorizontal->SetVisAttributes(VisAtt1);


  G4LogicalVolume* logicFrameVertical = new G4LogicalVolume(solidFrameVertical, m_MaterialAl, Name, 0, 0);
  logicFrameVertical->SetVisAttributes(VisAtt1); 
  
  G4ThreeVector FrameTopPosition    = G4ThreeVector(0 ,  0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ; 
  G4ThreeVector FrameBottomPosition = G4ThreeVector(0 , -0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ;
  G4ThreeVector FrameLeftPosition   = G4ThreeVector(0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;
  G4ThreeVector FrameRightPosition  = G4ThreeVector(-0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;


  new G4PVPlacement(   0,
      FrameTopPosition,
      logicFrameHorizontal,
      Name + "_Frame",
      logicThinSi,
      false,
      0);

  new G4PVPlacement(   0,
      FrameBottomPosition,
      logicFrameHorizontal,
      Name + "_Frame",
      logicThinSi,
      false,
      0);

  new G4PVPlacement(   0,
      FrameLeftPosition,
      logicFrameVertical,
      Name + "_Frame",
      logicThinSi,
      false,
      0);

  new G4PVPlacement(   0,
      FrameRightPosition,
      logicFrameVertical,
      Name + "_Frame",
      logicThinSi,
      false,
      0);


  G4ThreeVector posAluFront  = G4ThreeVector(0 ,  0  ,  AluStripFront_PosZ)  ;
  G4ThreeVector posSi        = G4ThreeVector(0 ,  0  ,  0                 )  ;
  G4ThreeVector posAluBack   = G4ThreeVector(0 ,  0  ,  AluStripBack_PosZ )  ;

  G4Box*   solidAlu  =
    new G4Box("ThinSiAlu", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*AluThickness) ;

  G4LogicalVolume* logicAlu  =
    new G4LogicalVolume(solidAlu, m_MaterialAl, "logicAlu", 0, 0, 0)    ;

  
  new G4PVPlacement(0  ,  posAluFront ,  logicAlu ,  Name + "_AluFront"   ,  logicThinSi ,  true, 0)  ;

  new G4PVPlacement(0  ,  posAluBack  ,  logicAlu ,  Name + "_AluBack"    ,  logicThinSi ,  true, 0)  ;

  G4Box*   solidSi  =
    new G4Box("ThinSi", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*SiliconThickness);

  G4LogicalVolume* logicSi  =
    new G4LogicalVolume(solidSi, m_MaterialSilicon, "logicSi", 0, 0, 0);

  G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  logicAlu->SetVisAttributes(VisAtt2);  

  new G4PVPlacement(0, posSi, logicSi, Name + "_Si", logicThinSi, true, 0);

  //attach it to the Silicon plate
  logicSi ->SetSensitiveDetector(m_StripScorer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void ThinSi::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;

  G4double TLX , BLX , BRX , TRX , TLY , BLY , BRY , TRY , TLZ , BLZ , BRZ , TRZ;
  G4ThreeVector TL , BL , BR , TR ;
  G4double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0 ;
  bool check_A = false ;
  bool check_B = false ;
  bool check_C = false ;
  bool check_D = false ;

  bool check_Theta   = false;
  bool check_Phi     = false;
  bool check_R       = false;
  bool check_beta    = false;
  bool ReadingStatus = false;

  while (!ConfigFile.eof()) 
  {

    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up ThinSi bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 4, "SSSD") == 0 && LineBuffer.compare(0, 5, "SSSDA") != 0) 
    {
      G4cout << "///" << G4endl           ;
      G4cout << "Detector found: " << G4endl   ;        
      ReadingStatus = true ;

    }

    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus)
    {
      // Pickup Next Word 
      ConfigFile >> DataBuffer ;

      //   Comment Line 
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer=="SSSD") {
        cout << "WARNING: Another Telescope is found before standard sequence of Token, Error may occured in detector definition" << endl ;
        ReadingStatus = false ;
      }

      //Position method
      else if (DataBuffer.compare(0, 3, "A=") == 0) {
        check_A = true;
        ConfigFile >> DataBuffer ;
        TLX = atof(DataBuffer.c_str()) ;
        TLX = TLX * mm;
        ConfigFile >> DataBuffer ;
        TLY = atof(DataBuffer.c_str()) ;
        TLY = TLY * mm;
        ConfigFile >> DataBuffer ;
        TLZ = atof(DataBuffer.c_str()) ;
        TLZ = TLZ * mm;

        TL = G4ThreeVector(TLX, TLY, TLZ);
        cout << "Top Left position : (" << TLX << ";" << TLY << ";" << TLZ << ")" << endl;
      }

      else if (DataBuffer.compare(0, 3, "B=") == 0) {
        check_B = true;
        ConfigFile >> DataBuffer ;
        BLX = atof(DataBuffer.c_str()) ;
        BLX = BLX * mm;
        ConfigFile >> DataBuffer ;
        BLY = atof(DataBuffer.c_str()) ;
        BLY = BLY * mm;
        ConfigFile >> DataBuffer ;
        BLZ = atof(DataBuffer.c_str()) ;
        BLZ = BLZ * mm;

        BL = G4ThreeVector(BLX, BLY, BLZ);
        cout << "Top Right position : (" << BLX << ";" << BLY << ";" << BLZ << ")" << endl;
      }

      else if (DataBuffer.compare(0, 3, "C=") == 0) {
        check_C = true;
        ConfigFile >> DataBuffer ;
        BRX = atof(DataBuffer.c_str()) ;
        BRX = BRX * mm;
        ConfigFile >> DataBuffer ;
        BRY = atof(DataBuffer.c_str()) ;
        BRY = BRY * mm;
        ConfigFile >> DataBuffer ;
        BRZ = atof(DataBuffer.c_str()) ;
        BRZ = BRZ * mm;

        BR = G4ThreeVector(BRX, BRY, BRZ);
        cout << "Bottom Right position : (" << BRX << ";" << BRY << ";" << BRZ << ")" << endl;
      }

      else if (DataBuffer.compare(0, 3, "D=") == 0) {
        check_D = true;
        ConfigFile >> DataBuffer ;
        TRX = atof(DataBuffer.c_str()) ;
        TRX = TRX * mm;
        ConfigFile >> DataBuffer ;
        TRY = atof(DataBuffer.c_str()) ;
        TRY = TRY * mm;
        ConfigFile >> DataBuffer ;
        TRZ = atof(DataBuffer.c_str()) ;
        TRZ = TRZ * mm;

        TR = G4ThreeVector(TRX, TRY, TRZ);
        G4cout << "Center position : (" << TRX << ";" << TRY << ";" << TRZ << ")" << G4endl << G4endl;
      }


      //Angle method
      else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str()) ;
        Theta = Theta * deg;
        cout << "Theta:  " << Theta / deg << endl;
      }

      else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str()) ;
        Phi = Phi * deg;
        cout << "Phi:  " << Phi / deg << endl;
      }

      else if (DataBuffer.compare(0, 2, "R=") == 0) {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str()) ;
        R = R * mm;
        cout << "R:  " << R*mm << endl;
      }


      else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
        check_beta = true;
        ConfigFile >> DataBuffer ;
        beta_u = atof(DataBuffer.c_str()) ;
        beta_u = beta_u * deg   ;
        ConfigFile >> DataBuffer ;
        beta_v = atof(DataBuffer.c_str()) ;
        beta_v = beta_v * deg   ;
        ConfigFile >> DataBuffer ;
        beta_w = atof(DataBuffer.c_str()) ;
        beta_w = beta_w * deg   ;
        G4cout << "Beta:  " << beta_u / deg <<  " " << beta_v / deg << " " << beta_w / deg << G4endl       ;
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else 
      {ReadingStatus = false; G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;}

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R && check_beta) ) 
      { 
        ReadingStatus = false; 

        ///Add The previously define telescope
        //With position method
        if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) {
          AddTelescope(   TL ,
              BL ,
              BR ,
              TR );
        }

        //with angle method
        else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) {
          AddTelescope(   R ,
              Theta ,
              Phi ,
              beta_u ,
              beta_v ,
              beta_w );
        }

        //   Reinitialisation of Check Boolean 

        check_A = false ;
        check_B = false ;
        check_C = false ;
        check_D = false ;

        check_Theta   = false ;
        check_Phi     = false ;
        check_R       = false ;
        check_beta    = false ;
        ReadingStatus = false ;

      }

    }
  }

}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void ThinSi::ConstructDetector(G4LogicalVolume* world){

  G4RotationMatrix* Det_rot  = NULL;
  G4ThreeVector     Det_pos  = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_u    = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_v    = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_w    = G4ThreeVector(0, 0, 0);

  G4int NumberOfDetector = m_DefinitionType.size();

  for (G4int i = 0 ; i < NumberOfDetector ; i++) {
    // By Point
    if (m_DefinitionType[i]) {
      // (u,v,w) unitary vector associated to telescope referencial
      // (u,v) // to silicon plan
      // w perpendicular to (u,v) plan and pointing outside
      Det_u = m_TL[i] - m_BL[i];
      Det_u = Det_u.unit();
      Det_v = m_BR[i] - m_BL[i];
      Det_v = Det_v.unit();
      Det_w = Det_u.cross(Det_v);
      Det_w = Det_w.unit();
      // Passage Matrix from Lab Referential to Telescope Referential
      // MUST2
      Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
      // translation to place Telescope
      Det_pos =  (m_TR[i]+m_TL[i]+m_BL[i]+m_BR[i])/4 ;
    }

    // By Angle
    else {
      G4double Theta = m_Theta[i];
      G4double Phi   = m_Phi[i];
      //This part because if Phi and Theta = 0 equation are false
      if (Theta == 0) Theta   = 0.0001;
      if (Theta == 2*cos(0)) Theta   = 2 * acos(0) - 0.00001;
      if (Phi   == 0) Phi = 0.0001;


      // (u,v,w) unitary vector associated to telescope referencial
      // (u,v) // to silicon plan
      // w perpendicular to (u,v) plan and pointing ThirdStage
      // Phi is angle between X axis and projection in (X,Y) plan
      // Theta is angle between  position vector and z axis
      G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad)   ;
      G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad)   ;
      G4double wZ = m_R[i] * cos(Theta / rad);
      Det_w = G4ThreeVector(wX, wY, wZ);

      // vector corresponding to the center of the module
      G4ThreeVector CT = Det_w;

      // vector parallel to one axis of silicon plane
      G4double ii = cos(Theta / rad) * cos(Phi / rad);
      G4double jj = cos(Theta / rad) * sin(Phi / rad);
      G4double kk = -sin(Theta / rad);
      G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

      Det_w = Det_w.unit();
      Det_u = Det_w.cross(Y);
      Det_v = Det_w.cross(Det_u);
      Det_v = Det_v.unit();
      Det_u = Det_u.unit();

      // Passage Matrix from Lab Referential to Telescope Referential
      // MUST2
      Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
      // Telescope is rotate of Beta angle around Det_v axis.
      Det_rot->rotate(m_beta_u[i], Det_u);
      Det_rot->rotate(m_beta_v[i], Det_v);
      Det_rot->rotate(m_beta_w[i], Det_w);
      // translation to place Telescope
      Det_pos = Det_w + CT ;
    }



    VolumeMaker(i + 1 , Det_pos , Det_rot , world);
  }

  delete Det_rot ;
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void ThinSi::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("SSSD", "TSSSDData", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void ThinSi::ReadSensitive(const G4Event* event){
  m_Event->Clear();
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////// Used to Read Event Map of detector //////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  // Si
  G4THitsMap<G4int>*     DetNbrHitMap;  
  G4THitsMap<G4int>*     StripNbrHitMap;    
  G4THitsMap<G4double>*  EnergyHitMap;
  G4THitsMap<G4double>*  TimeHitMap;

  std::map<G4int, G4int*>::iterator DetNbr_itr     ;
  std::map<G4int, G4int*>::iterator StripNbr_itr  ;
  std::map<G4int, G4double*>::iterator Energy_itr ;
  std::map<G4int, G4double*>::iterator Time_itr   ;
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////
  // Read the Scorer associate to the Silicon Strip

  //DetectorNumber   
  G4int DetNbrCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThinSi_StripScorer/DetectorNumber");
  DetNbrHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(DetNbrCollectionID));
  DetNbr_itr = DetNbrHitMap->GetMap()->begin();

  //StripNumber   
  G4int StripNbrCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThinSi_StripScorer/StripNumber");
  StripNbrHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripNbrCollectionID));

  //Energy
  G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThinSi_StripScorer/StripEnergy");
  EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID));

  //Time
  G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThinSi_StripScorer/StripTime");
  TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID));

  G4int sizeN = DetNbrHitMap      ->entries();
  G4int sizeS = StripNbrHitMap    ->entries();
  G4int sizeE = EnergyHitMap       ->entries();
  G4int sizeT = TimeHitMap         ->entries();

  // Loop on Det Number
  for (G4int l = 0 ; l < sizeN ; l++) 
  {
    G4int N     =      *(DetNbr_itr->second);
    G4int NTrackID  =   DetNbr_itr->first - N;

    if (N > 0) 
    {
      m_Event->SetEnergyDetectorNbr(N);
      m_Event->SetTimeDetectorNbr(N);

      //  Strip Number
      StripNbr_itr = StripNbrHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeS ; h++) {
        G4int STrackID  =   StripNbr_itr->first  - N ;
        G4int S         = *(StripNbr_itr->second);

        if (STrackID == NTrackID) {
          m_Event->SetEnergyStripNbr(S);
          m_Event->SetTimeStripNbr(S);
        }

        StripNbr_itr++;
      }

      //  Energy
      Energy_itr = EnergyHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeE ; h++) {
        G4int ETrackID  =   Energy_itr->first  - N;
        G4double E      = *(Energy_itr->second);

        if (ETrackID == NTrackID) {
          m_Event->SetEnergy( RandGauss::shoot(E, ResoEnergy ) );
        }

        Energy_itr++;
      }


      //  Time
      Time_itr = TimeHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeT ; h++) {
        G4int TTrackID  =   Time_itr->first   - N ;
        G4double T     = *(Time_itr->second);

        if (TTrackID == NTrackID) {
          m_Event->SetTime( RandGauss::shoot(T, ResoTime ) );
        }

        Time_itr++;
      }

    }

    DetNbr_itr++;
  }

  // clear map for next event

  DetNbrHitMap   ->clear();
  StripNbrHitMap ->clear();
  EnergyHitMap   ->clear(); 
  TimeHitMap     ->clear();    
}


void ThinSi::InitializeScorers(){

  //   Silicon Associate Scorer
  m_StripScorer = new G4MultiFunctionalDetector("ThinSi_StripScorer");

  G4VPrimitiveScorer* DetNbr   = new OBSOLETEGENERALSCORERS::PSDetectorNumber("DetectorNumber","ThinSi_", 0);
  G4VPrimitiveScorer* StripNbr = new PSStripNumber("StripNumber",0,SiliconSize, NumberOfStrip); 
  G4VPrimitiveScorer* Energy   = new OBSOLETEGENERALSCORERS::PSEnergy("StripEnergy","ThinSi_", 0);         
  G4VPrimitiveScorer* TOF      = new OBSOLETEGENERALSCORERS::PSTOF("StripTime","ThinSi_", 0);                                 


  //and register it to the multifunctionnal detector
  m_StripScorer->RegisterPrimitive(DetNbr);
  m_StripScorer->RegisterPrimitive(StripNbr);
  m_StripScorer->RegisterPrimitive(Energy);
  m_StripScorer->RegisterPrimitive(TOF);

  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer) ;
}

////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void ThinSi::InitializeMaterial(){

  G4Element* N   = new G4Element("Nitrogen" , "N"  , 7  , 14.01  * g / mole);
  G4Element* O   = new G4Element("Oxigen"   , "O"  , 8  , 16.00  * g / mole);

  G4double a, z, density;
  // Si
  a = 28.0855 * g / mole;
  density = 2.321 * g / cm3;
  m_MaterialSilicon = new G4Material("Si", z = 14., a, density);

  // Al
  density = 2.702 * g / cm3;
  a = 26.98 * g / mole;
  m_MaterialAl = new G4Material("Al", z = 13., a, density);

  //  Vacuum
  density = 0.000000001 * mg / cm3;
  m_MaterialVacuum = new G4Material("Vacuum", density, 2);
  m_MaterialVacuum->AddElement(N, .7);
  m_MaterialVacuum->AddElement(O, .3);
}


