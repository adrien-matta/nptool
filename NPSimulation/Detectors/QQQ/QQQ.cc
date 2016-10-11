/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
#include "G4Tubs.hh"
#include "G4Trap.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPTool header
#include "QQQ.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
using namespace QQQ_LOCAL;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// QQQ Specific Method
QQQ::QQQ(){
  InitializeMaterial();
  m_Event = new TQQQData();
  m_StripScorer=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
QQQ::~QQQ(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void QQQ::AddTelescope( G4ThreeVector TL,
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void QQQ::AddTelescope( G4double R,
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
void QQQ::VolumeMaker(  G4int             DetNumber ,
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
  G4String Name = "QQQ" + DetectorNumber;

//  G4Box* solidQQQ = new G4Box(Name+"Solid", 0.5*DetectorSize, 0.5*DetectorSize, 0.5*FrameThickness);
  G4Tubs* solidQQQ = new G4Tubs(Name+"Solid", FrameRMin, FrameRMax, 0.5*FrameThickness, FrameStartPhi, FrameDeltaPhi);

  G4LogicalVolume* logicQQQ =
    new G4LogicalVolume(solidQQQ, m_MaterialVacuum, Name+"_logic", 0, 0);

  new G4PVPlacement(   G4Transform3D(*Det_rot, Det_pos) ,
      logicQQQ         ,
      Name                ,
      world               ,
      false               ,
      DetNumber);

  logicQQQ->SetVisAttributes(G4VisAttributes::Invisible);
  
  // Frame is made of 4 thick box (2 Horizontal and 2 Vertical)
  //G4Box* solidFrameHorizontal = new G4Box(Name + "_Frame", 0.5*SiliconSize, 0.5*(DetectorSize - SiliconSize) / 2, 0.5*FrameThickness*mm)   ;
  //G4Box* solidFrameVertical  = new G4Box(Name + "_Frame", 0.5*(DetectorSize - SiliconSize) / 2, 0.5*DetectorSize, 0.5*FrameThickness*mm)   ;

  //G4LogicalVolume* logicFrameHorizontal = new G4LogicalVolume(solidFrameHorizontal, m_MaterialAl, Name, 0, 0);
  //G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.0, 0.5, 1));
  //logicFrameHorizontal->SetVisAttributes(VisAtt1);


  //G4LogicalVolume* logicFrameVertical = new G4LogicalVolume(solidFrameVertical, m_MaterialAl, Name, 0, 0);
  //logicFrameVertical->SetVisAttributes(VisAtt1); 

  //G4ThreeVector FrameTopPosition    = G4ThreeVector(0 ,  0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ; 
  //G4ThreeVector FrameBottomPosition = G4ThreeVector(0 , -0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ;
  //G4ThreeVector FrameLeftPosition   = G4ThreeVector(0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;
  //G4ThreeVector FrameRightPosition  = G4ThreeVector(-0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;


  //new G4PVPlacement(   0,
  //    FrameTopPosition,
  //    logicFrameHorizontal,
  //    Name + "_Frame",
  //    logicQQQ,
  //    false,
  //    DetNumber);

  //new G4PVPlacement(   0,
  //    FrameBottomPosition,
  //    logicFrameHorizontal,
  //    Name + "_Frame",
  //    logicQQQ,
  //    false,
  //    DetNumber);

  //new G4PVPlacement(   0,
  //    FrameLeftPosition,
  //   logicFrameVertical,
  //    Name + "_Frame",
  //    logicQQQ,
  //    false,
  //    DetNumber);

  //new G4PVPlacement(   0,
  //    FrameRightPosition,
  //    logicFrameVertical,
  //    Name + "_Frame",
  //    logicQQQ,
  //    false,
  //    DetNumber);


  G4ThreeVector posAluFront  = G4ThreeVector(0 ,  0  ,  AluStripFront_PosZ)  ;
  G4ThreeVector posSi1        = G4ThreeVector(0 ,  0  ,  0                 )  ;
  G4ThreeVector posSi2        = G4ThreeVector(0 ,  0  ,  0.5*SiliconThickness1 + AluThickness + DetGap +  0.5*SiliconThickness2                )  ;
  G4ThreeVector posAluBack   = G4ThreeVector(0 ,  0  ,  AluStripBack_PosZ )  ;

  G4Tubs*   solidAlu  =
    new G4Tubs("QQQAlu", DetectorRMin, DetectorRMax, 0.5*AluThickness, DetStartPhi, DetDeltaPhi) ;

  G4LogicalVolume* logicAlu  =
    new G4LogicalVolume(solidAlu, m_MaterialAl, "logicAlu", 0, 0, 0)    ;


  new G4PVPlacement(0  ,  posAluFront ,  logicAlu ,  Name + "_AluFront"   ,  logicQQQ ,  true, DetNumber)  ;
  new G4PVPlacement(0  ,  posAluBack  ,  logicAlu ,  Name + "_AluBack"    ,  logicQQQ ,  true, DetNumber)  ;


// delta E detector

  G4Tubs*   solidSi1  =
    new G4Tubs("QQQ1", DetectorRMin, DetectorRMax, 0.5*SiliconThickness1, DetStartPhi,DetDeltaPhi);

  G4LogicalVolume* logicSi1  =
    new G4LogicalVolume(solidSi1, m_MaterialSilicon, "logicSi1", 0, 0, 0);

  //logicAlu->SetVisAttributes(G4VisAttributes::Invisible);  

  new G4PVPlacement(0, posSi1, logicSi1, Name + "_Si", logicQQQ, true, DetNumber);


  logicSi1->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 0.0, 0.0)));  // red

// E detector:

  G4Tubs*   solidSi2  =
    new G4Tubs("QQQ2", DetectorRMin, DetectorRMax, 0.5*SiliconThickness2, DetStartPhi,DetDeltaPhi);

  G4LogicalVolume* logicSi2  =
    new G4LogicalVolume(solidSi2, m_MaterialSilicon, "logicSi2", 0, 0, 0);

  //logicAlu->SetVisAttributes(G4VisAttributes::Invisible);  

  new G4PVPlacement(0, posSi2, logicSi2, Name + "_Si", logicQQQ, true, DetNumber);


  logicSi2->SetVisAttributes(G4VisAttributes(G4Colour(1.0, 0.0, 0.0)));  // red


  //attach it to the Silicon plate
  logicSi1 ->SetSensitiveDetector(m_StripScorer);
  logicSi2 ->SetSensitiveDetector(m_StripScorer);
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void QQQ::ReadConfiguration(string Path){
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

    //   If line is a Start Up QQQ bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 3, "QQQ") == 0 && LineBuffer.compare(0, 4, "QQQA") != 0) 
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
      else if (DataBuffer=="QQQ") {
        G4cout << "WARNING: Another Telescope is found before standard sequence of Token, Error may occured in detector definition" << G4endl ;
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
        G4cout << "Top Left position : (" << TLX << ";" << TLY << ";" << TLZ << ")" << G4endl;
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
        G4cout << "Top Right position : (" << BLX << ";" << BLY << ";" << BLZ << ")" << G4endl;
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
        G4cout << "Bottom Right position : (" << BRX << ";" << BRY << ";" << BRZ << ")" << G4endl;
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
        G4cout << "Theta:  " << Theta / deg << G4endl;
      }

      else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str()) ;
        Phi = Phi * deg;
        G4cout << "Phi:  " << Phi / deg << G4endl;
      }

      else if (DataBuffer.compare(0, 2, "R=") == 0) {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str()) ;
        R = R * mm;
        G4cout << "R:  " << R*mm << G4endl;
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void QQQ::ConstructDetector(G4LogicalVolume* world){

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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void QQQ::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("QQQ", "TQQQData", &m_Event) ;
  pTree->SetBranchAddress("QQQ", &m_Event) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void QQQ::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  G4THitsMap<G4double*>* StripHitMap;
  std::map<G4int, G4double**>::iterator Strip_itr;
  
  G4int StripCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("QQQ_Scorer/StripScorer");
  StripHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(StripCollectionID));
  
  // Loop on the Strip map
  for (Strip_itr = StripHitMap->GetMap()->begin() ; Strip_itr != StripHitMap->GetMap()->end() ; Strip_itr++){
    
    G4double* Info = *(Strip_itr->second);
    
    double Energy = Info[0];
    
    if(Energy>EnergyThreshold){
      double Time       = Info[1];
      int DetNbr        = (int) Info[7];
      int StripFront    = (int) Info[8];

      m_Event->SetEnergyDetectorNbr(DetNbr);
      m_Event->SetEnergyStripNbr(StripFront);
      m_Event->SetEnergy(RandGauss::shoot(Energy, ResoEnergy));
    
      m_Event->SetTimeDetectorNbr(DetNbr);
      m_Event->SetTimeStripNbr(StripFront);
      m_Event->SetTime(RandGauss::shoot(Time, ResoTime));
      
    }
  }
  // clear map for next event
  StripHitMap->clear();
 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void QQQ::InitializeScorers(){
  bool already_exist = false;
  //   Silicon Associate Scorer
  m_StripScorer = CheckScorer("QQQ_Scorer",already_exist);
  if(already_exist) return;

  G4VPrimitiveScorer* StripScorer =
    new  SILICONSCORERS::PS_Silicon_Rectangle("StripScorer",0,
        DetectorSize,
        DetectorSize,
        NumberOfStrip,
        1);

  m_StripScorer->RegisterPrimitive(StripScorer);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void QQQ::InitializeMaterial(){
  m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
  m_MaterialAl = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* QQQ::Construct(){
  return  (NPS::VDetector*) new QQQ();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy{
    public:
      proxy(){
        NPS::DetectorFactory::getInstance()->AddToken("QQQ","QQQ");
        NPS::DetectorFactory::getInstance()->AddDetector("QQQ",QQQ::Construct);
      }
  };

  proxy p;
}
