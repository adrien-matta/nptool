/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 * Modified for ANU by: Lee Evitts  contact address: evitts@triumf.ca        *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Modified Date  : 07/04/14                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the ANU detector                    *
 *             Taken from the HELIOS detector and modified
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: 
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// G4 Geometry headers
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

// G4 various headers
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"

// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "ANUDetDummyShape.hh"
#include "GeneralScorers.hh"
#include "ANUScorers.hh"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace ANUDUMMYSHAPE;


// --------------------
// Constructor
ANUDetDummyShape::ANUDetDummyShape()
{
   ms_InterCoord = 0;
}
// --------------------


// --------------------
// Deconstructor
ANUDetDummyShape::~ANUDetDummyShape() {
}
// --------------------


// --------------------
// Virtual Method of VDetector class:
// Read stream of Configfile to pick-up parameters of detector element
// Called from [DetectorConstruction::ReadDetectorConfiguration] Method
void ANUDetDummyShape::ReadConfiguration(string Path)
{
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer, DataBuffer; 

  G4double Z, T, R1, R2, PC, PD;

  bool ReadingStatus = false;

  bool check_Z = false;
  bool check_T = false;
  bool check_R1 = false;
  bool check_R2 = false;
  bool check_PC = false;
  bool check_PD = false;
   
  bool checkVis = false;

  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);
      
    if (LineBuffer.compare(0, 13, "ANUDummyShape") == 0) {
      G4cout << "DummyShape element found: " << G4endl   ;
      ReadingStatus = true ;
    }
         
    while (ReadingStatus) {
      ConfigFile >> DataBuffer;
      // Ignore Comments
      if (DataBuffer.compare(0, 1, "%") == 0) {/* _ */;}
	
      // Read in parameters
      // TODO: Can this be refactored?
      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true;
        ConfigFile >> DataBuffer ;
        Z = atof(DataBuffer.c_str()) ;
        Z = Z * mm;
        G4cout << "Z:  " << Z / mm << G4endl;
      } else if (DataBuffer.compare(0, 2, "T=") == 0) {
        check_T = true;
        ConfigFile >> DataBuffer ;
        T = atof(DataBuffer.c_str()) ;
        T = T * mm;
        G4cout << "T:  " << T / mm << G4endl;
      }else if (DataBuffer.compare(0, 3, "R1=") == 0) {
        check_R1 = true;
        ConfigFile >> DataBuffer ;
        R1 = atof(DataBuffer.c_str()) ;
        R1 = R1 * mm;
        G4cout << "R1:  " << R1 / mm << G4endl;
      } else if (DataBuffer.compare(0, 3, "R2=") == 0) {
        check_R2 = true;
        ConfigFile >> DataBuffer ;
        R2 = atof(DataBuffer.c_str()) ;
        R2 = R2 * mm;
        G4cout << "R2:  " << R2 / mm << G4endl;
      } else if (DataBuffer.compare(0, 3, "PC=") == 0) {
        check_PC = true;
        ConfigFile >> DataBuffer ;
        PC = atof(DataBuffer.c_str()) ;
        PC = PC * deg;
        G4cout << "PC:  " << PC / deg << G4endl;
      } else if (DataBuffer.compare(0, 3, "PD=") == 0) {
        check_PD = true;
        ConfigFile >> DataBuffer ;
        PD = atof(DataBuffer.c_str()) ;
        PD = PD * deg;
        G4cout << "PD:  " << PD / deg << G4endl;
      } else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
        checkVis = true ;
        ConfigFile >> DataBuffer;
        if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
      } else G4cout << "WARNING: Wrong Token, ANUDummyShape: DummyShape Element not added" << G4endl;

      // Add the recently defined element
      if (check_Z && check_T && check_R1 && check_R2 && check_PC && check_PD && checkVis) {
        ReadingStatus =     false;
        check_Z =           false;
        check_T =           false;
        check_R1   =        false;
        check_R2     =      false;
        check_PC   =        false;
        check_PD     =      false;
        checkVis =          false;
		     
		    // Add element to array
        m_Z.push_back(Z);
        m_T.push_back(T);
        m_R1.push_back(R1);
        m_R2.push_back(R2);
        m_PC.push_back(PC);
        m_PD.push_back(PD);
      }
    }
  }
}
// --------------------


// --------------------
// Construct detector, called in [DetecorConstruction::AddDetector]
void ANUDetDummyShape::ConstructDetector(G4LogicalVolume* world)
{
  
  G4int NbrOfDetectors = m_Z.size();

  // Define Materials
  G4double a = 28.0855 * g / mole;
  G4double density = 2.321 * g / cm3;
  G4Material* Si = new G4Material("Si", 14., a, density);
  
  a = 26.9815 * g / mole;
  density = 2.7 * g / cm3;
  G4Material* Al = new G4Material("Al", 13., a, density);
  
  // Declare Variables/Arrays
  G4RotationMatrix* rotate = new G4RotationMatrix(0.,0.,0.);
  G4ThreeVector locate = G4ThreeVector(0.,0.,0.);
  G4SubtractionSolid* ANUHolderSubtraction[NbrOfDetectors];
  
  // Construct shape of detector holder
  G4double ANUHolder_InnerRadius = 2.*mm;
  G4double ANUHolder_OuterRadius = 15.*mm;
  G4double ANUHolder_HalfThickness = 2.*mm;
  G4double ANUHolder_TargetDistance = 352.*mm;
  G4Cons* ANUHolder = new G4Cons("ANUHolder", ANUHolder_InnerRadius, ANUHolder_OuterRadius, 
          ANUHolder_InnerRadius, ANUHolder_OuterRadius, ANUHolder_HalfThickness, 0., 2.*M_PI*rad);

  // Build detector elements ...
  for (G4int i = 0; i < NbrOfDetectors; i++) {
    G4double Z = m_Z[i];
    G4double T = m_T[i];
    //  ... make flush:
    if (Z >= 0.0) Z += T/2.0; else Z -= T/2.0;
    G4double PC = m_PC[i] / rad;
    G4double PD = m_PD[i] / rad;

    // ... give the element a name
    G4String DetectorNumber;
    ostringstream Number;
    Number << static_cast<double>(i);
    DetectorNumber = Number.str();

    // ... define volume
    G4String Name = "ANUDummyShape" + DetectorNumber ;

    // ... build logical volume (& combine them, to be used in building the absorber)
    G4Cons* ANUDetectorShape = new G4Cons(Name, 0, m_R2[i], 0, m_R2[i], 0.5*T, PC-PD, 2.*PD);    
    G4LogicalVolume* ANUDetectorLogical = new G4LogicalVolume(ANUDetectorShape, Si, Name, 0, 0, 0);
    
    // ... place volume
    locate.setX(m_R1[i] * cos(PC));
    locate.setY(m_R1[i] * sin(PC));
    locate.setZ(0);
    
    // ... need to recreate the detector shape with a different thickness (for subtraction purposes)
    G4Cons* ANUDetectorShapeThick = new G4Cons("ANUDetectorShapeThick", 0, m_R2[i], 0, m_R2[i], 
                                                2.*ANUHolder_HalfThickness, PC-PD, 2.*PD);    
    if (i == 0)
      ANUHolderSubtraction[i] = new G4SubtractionSolid("ANUHolderSubtraction", ANUHolder, ANUDetectorShapeThick, rotate, locate);
    else
      ANUHolderSubtraction[i] = new G4SubtractionSolid("ANUHolderSubtraction", ANUHolderSubtraction[i-1], ANUDetectorShapeThick, rotate, locate);
      
    locate.setZ(Z);
    
    
    
    new G4PVPlacement(rotate, locate, ANUDetectorLogical, Name, world, false, 0);
    ANUDetectorLogical->SetVisAttributes(G4VisAttributes(G4Colour(1, 0., 0.0)));  // red
  }
  
  // Build the detector holder   
  G4LogicalVolume* ANUHolderLogic = new G4LogicalVolume(ANUHolderSubtraction[NbrOfDetectors-1], Al, "ANUHolder", 0, 0, 0);
  locate.setX(0.);
  locate.setY(0.);
  locate.setZ(ANUHolder_TargetDistance);
  new G4PVPlacement(rotate, locate, ANUHolderLogic, "ANUHolder", world, false, 0);
  
}
// --------------------


// --------------------
// Connect the class to the output TTree of the simulation
void ANUDetDummyShape::InitializeRootOutput() {
}
// --------------------


// --------------------
// Set the TInteractionCoordinates object from VDetector to the present class
void ANUDetDummyShape::SetInterCoordPointer(TInteractionCoordinates* interCoord){
   ms_InterCoord = interCoord;
}
// --------------------


// --------------------
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void ANUDetDummyShape::ReadSensitive(const G4Event* event)
{
   //////////////////////////////////////////////////////////////////////////////////////
   //////////////////////// Used to Read Event Map of detector //////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////
   // First Stage
   std::map<G4int, G4int*>::iterator    DetectorNumber_itr;
   std::map<G4int, G4double*>::iterator Energy_itr;
   std::map<G4int, G4double*>::iterator Time_itr;
   std::map<G4int, G4double*>::iterator X_itr;
   std::map<G4int, G4double*>::iterator Y_itr;
   std::map<G4int, G4double*>::iterator Pos_X_itr;
   std::map<G4int, G4double*>::iterator Pos_Y_itr;
   std::map<G4int, G4double*>::iterator Pos_Z_itr;
   std::map<G4int, G4double*>::iterator Ang_Theta_itr;
   std::map<G4int, G4double*>::iterator Ang_Phi_itr;

   G4THitsMap<G4int>*    DetectorNumberHitMap;
   G4THitsMap<G4double>* EnergyHitMap;
   G4THitsMap<G4double>* TimeHitMap;
   G4THitsMap<G4double>* XHitMap;
   G4THitsMap<G4double>* YHitMap;
   G4THitsMap<G4double>* PosXHitMap;
   G4THitsMap<G4double>* PosYHitMap;
   G4THitsMap<G4double>* PosZHitMap;
   G4THitsMap<G4double>* AngThetaHitMap;
   G4THitsMap<G4double>* AngPhiHitMap;



   // Read the Scorer associate to the Silicon Strip
   //Detector Number
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/DetectorNumber")    ;
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))         ;
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

  /*
   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/StripIDFront")    ;
   XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
   X_itr = XHitMap->GetMap()->begin()                                                                    ;

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/StripIDBack");
   YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;
  */
  
   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorerANUDummyShape/InterCoordAngPhi")    ;
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))                              ;
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()                                                                    ;




   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeE = EnergyHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = XHitMap->entries();
   G4int sizeY = YHitMap->entries();


   //G4cout << "sizeN:" << sizeN << G4endl;


   if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
     //G4cout << "sizeE:" << sizeE << G4endl;
     //G4cout << "sizeT:" << sizeT << G4endl;
     //G4cout << "sizeX:" << sizeX << G4endl;
     //G4cout << "sizeY:" << sizeY << G4endl;
     //G4cout << "No match size Si Event Map: sE:"
     // << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << endl ;
      return;
   }

   // Loop on SiLi number
   for (G4int l = 0; l < sizeN; l++) {
      G4int N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      //G4cout <<"N:" <<N << G4endl;
      //G4cout <<"DetectorNumber_itr->first:" << DetectorNumber_itr->first <<  G4endl;
      //G4cout <<"NTrackID:" <<NTrackID << G4endl;


      if (N > 0) {
         // Fill detector number
         ms_Event->SetANUSiLiEDetectorNbr(m_index["DummyShape"] + N);
         ms_Event->SetANUSiLiTDetectorNbr(m_index["DummyShape"] + N);

         // Energy
         for (G4int l = 0 ; l < sizeE ; l++) {
            G4int ETrackID  =   Energy_itr->first - N;
            G4double E     = *(Energy_itr->second);
            if (ETrackID == NTrackID) {
               ms_Event->SetANUSiLiEEnergy(RandGauss::shoot(E, ResoSiLi));
              }
            Energy_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeT ; h++) {
            G4int TTrackID  =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               T = RandGauss::shoot(T, ResoTimeGpd);
               ms_Event->SetANUSiLiTTime(RandGauss::shoot(T, ResoTimeGpd));
           }
            Time_itr++;
         }

         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N    ;
            G4double PosX     = *(Pos_X_itr->second)      ;
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX) ;
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N    ;
            G4double PosY     = *(Pos_Y_itr->second)      ;
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY) ;
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N    ;
            G4double PosZ     = *(Pos_Z_itr->second)      ;
            if (PosZTrackID == NTrackID) {
	      ms_InterCoord->SetDetectedPositionZ(RandGauss::shoot(PosZ, ResoSiLi)) ; // for ANU !!!!
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N    ;
            G4double AngTheta     = *(Ang_Theta_itr->second)      ;
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta) ;
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngPhiTrackID =   Ang_Phi_itr->first - N    ;
            G4double AngPhi     = *(Ang_Phi_itr->second)      ;
            if (AngPhiTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAnglePhi(AngPhi) ;
            }
            Ang_Phi_itr++;
         }


         DetectorNumber_itr++;
      }

      // clear map for next event
      DetectorNumberHitMap    -> clear();
      EnergyHitMap            -> clear();
      TimeHitMap              -> clear();
      XHitMap                 -> clear();
      YHitMap                 -> clear();
      PosXHitMap              -> clear();
      PosYHitMap              -> clear();
      PosZHitMap              -> clear();
      AngThetaHitMap          -> clear();
      AngPhiHitMap            -> clear();
   }
}



void ANUDetDummyShape::InitializeScorers()
{
   // First stage Associate Scorer
   m_SiLiScorer                                   = new G4MultiFunctionalDetector("SiLiScorerANUDummyShape");
   G4VPrimitiveScorer* DetNbr                           = new GENERALSCORERS::PSDetectorNumber("DetectorNumber", "ANUDummyShape", 0);
   G4VPrimitiveScorer* TOF                              = new GENERALSCORERS::PSTOF("StripTime","ANUDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new GENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","ANUDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new GENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","ANUDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new GENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","ANUDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new GENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","ANUDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new GENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","ANUDummyShape", 0);
   G4VPrimitiveScorer* Energy                           = new ANUScorerSiLiEnergy("StripEnergy", "ANUDummyShape", 0);
   //G4VPrimitiveScorer* StripPositionX                   = new ANUScorerSiLiFrontStripDummyShape("StripIDFront", 0, NumberOfStrips);
   //G4VPrimitiveScorer* StripPositionY                   = new ANUScorerSiLiBackStripDummyShape("StripIDBack", 0, NumberOfStrips);

   //and register it to the multifunctionnal detector
   m_SiLiScorer->RegisterPrimitive(DetNbr);
   m_SiLiScorer->RegisterPrimitive(Energy);
   m_SiLiScorer->RegisterPrimitive(TOF);
   //m_SiLiScorer->RegisterPrimitive(StripPositionX);
   //m_SiLiScorer->RegisterPrimitive(StripPositionY);
   m_SiLiScorer->RegisterPrimitive(InteractionCoordinatesX);
   m_SiLiScorer->RegisterPrimitive(InteractionCoordinatesY);
   m_SiLiScorer->RegisterPrimitive(InteractionCoordinatesZ);
   m_SiLiScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta);
   m_SiLiScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi);




   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_SiLiScorer);
}
