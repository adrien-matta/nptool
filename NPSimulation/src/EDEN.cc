/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : February 2010                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class describes the EDEN neutron detector                        *
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
#include "G4Tubs.hh"

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

// NPTool header
#include "EDEN.hh"
#include "GeneralScorers.hh"
#include "EDENScorers.hh"
#include "RootOutput.h"
using namespace GENERALSCORERS;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace EDENDETECTOR;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// EDEN Specific Method
EDEN::EDEN()
{
   InitializeMaterial();
   m_Event = new TEdenSimuData();
}



EDEN::~EDEN()
{
   delete m_MaterialEDEN_liqSci;
   delete m_Event;
   delete m_EDENScorer;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EDEN::AddEDENModule(G4double R,
			 G4double Theta,
			 G4double Phi) 
{
   m_R.push_back(R);
   m_Theta.push_back(Theta);
   m_Phi.push_back(Phi);
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void EDEN::ReadConfiguration(string Path)
{
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer, DataBuffer;

   G4double R = 0, Theta = 0, Phi = 0;
   G4String Scintillator;

   bool check_Theta   = false;
   bool check_Phi     = false;
   bool check_R       = false;
   bool ReadingStatus = false;


   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);

      // If line is a Start Up EDEN bloc, Reading toggle to true
      if (LineBuffer.compare(0, 4, "EDEN") == 0) {
         G4cout << "///" << G4endl;
         G4cout << "EDEN Module found: " << G4endl;
         ReadingStatus = true;
      }
      // Else don't toggle to Reading Block Status
      else ReadingStatus = false;

      // Reading Block
      while (ReadingStatus) {
         // Pickup Next Word 
         ConfigFile >> DataBuffer;

         // Comment Line 
         if (DataBuffer.compare(0, 1, "%") == 0) {
            ConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }

         // Finding another telescope (safety), toggle out
         else if (DataBuffer.compare(0, 4, "EDEN") == 0) {
            cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl;
            ReadingStatus = false;
         }

         // Angle method
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
            cout << "R:  " << R/mm << endl;
         }

         ///////////////////////////////////////////////////
         // If no Detector Token and no comment, toggle out
         else {
            ReadingStatus = false; 
            G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl;
         }

         /////////////////////////////////////////////////
         // If All necessary information there, toggle out
         if (check_Theta && check_Phi && check_R) { 
            AddEDENModule(R,
		          Theta,
		          Phi); 

            // Reinitialisation of Check Boolean 
            check_Theta   = false;
            check_Phi     = false;
            check_R       = false;
            ReadingStatus = false;
            cout << "///"<< endl;
      	 }
      }
   }
}



// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void EDEN::ConstructDetector(G4LogicalVolume* world)
{
   G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0);

   for (unsigned short i = 0; i < m_R.size(); i++) {
      G4double wX = m_R[i] * sin(m_Theta[i]) * cos(m_Phi[i]);
      G4double wY = m_R[i] * sin(m_Theta[i]) * sin(m_Phi[i]);
      G4double wZ = m_R[i] * cos(m_Theta[i]);
      Det_pos = G4ThreeVector(wX, wY, wZ);

      VolumeMaker(Det_pos , i+1, world) ;
   }
}



void EDEN::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world)
{
   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////		
   G4PVPlacement* PVPBuffer;

   // Name of the module
   std::ostringstream DetectorNumber;
   DetectorNumber << DetNumber;
   G4String Name = "EDEN" + DetectorNumber.str();

   // Definition of the volume containing the sensitive detector
   G4Tubs* solidEDEN = new G4Tubs(Name, 0, 10*cm, 2.5*cm, 0*deg, 360*deg);

   G4LogicalVolume* logicEDEN = new G4LogicalVolume(solidEDEN, m_MaterialEDEN_liqSci, Name + "_Scintillator", 0, 0, 0);
   logicEDEN->SetSensitiveDetector(m_EDENScorer);

   G4VisAttributes* EDENVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
   logicEDEN->SetVisAttributes(EDENVisAtt);

   PVPBuffer = new G4PVPlacement(0,
				 Det_pos,
		               	 logicEDEN,
		               	 Name  + "_Scintillator",
		               	 world,
		               	 false,
		               	 0);
}



// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void EDEN::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("EDEN", "TEdenSimuData", &m_Event) ;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void EDEN::ReadSensitive(const G4Event* event)
{
   // Clear the data of the TEdenSimuData object
   m_Event->Clear();

   //////////////////////////////////////////////////////////////////////////////////////
   //////////////////////// Used to Read Event Map of detector //////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////
   std::map<G4int, G4int*>::iterator    DetectorNumber_itr;
   std::map<G4int, G4double*>::iterator Charge_itr;
   std::map<G4int, G4double*>::iterator Time_itr;
   std::map<G4int, G4double*>::iterator Pos_X_itr;
   std::map<G4int, G4double*>::iterator Pos_Y_itr;
   std::map<G4int, G4double*>::iterator Pos_Z_itr;
   std::map<G4int, G4double*>::iterator Ang_Theta_itr;
   std::map<G4int, G4double*>::iterator Ang_Phi_itr;

   G4THitsMap<G4int>*    DetectorNumberHitMap;
   G4THitsMap<G4double>* ChargeHitMap;
   G4THitsMap<G4double>* TimeHitMap;
   G4THitsMap<G4double>* PosXHitMap;
   G4THitsMap<G4double>* PosYHitMap;
   G4THitsMap<G4double>* PosZHitMap;
   G4THitsMap<G4double>* AngThetaHitMap;
   G4THitsMap<G4double>* AngPhiHitMap;

   // Read the Scorer associate to the Silicon Strip
   //Detector Number
   G4int DetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/DetectorNumber");
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(DetCollectionID));
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin();

   // Charge
   G4int ChargeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/Charge");
   ChargeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(ChargeCollectionID));
   Charge_itr = ChargeHitMap->GetMap()->begin();

   //Time of Flight
   G4int TimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/Time");
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(TimeCollectionID));
   Time_itr = TimeHitMap->GetMap()->begin();

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/InterCoordX");
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID));
   Pos_X_itr = PosXHitMap->GetMap()->begin();

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/InterCoordY");
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID));
   Pos_Y_itr = PosYHitMap->GetMap()->begin();

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/InterCoordZ");
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID));
   Pos_Z_itr = PosXHitMap->GetMap()->begin();

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/InterCoordAngTheta");
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID));
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("EDENScorer/InterCoordAngPhi");
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID));
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();

   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeQ = ChargeHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = PosXHitMap->entries();

   if (sizeN != sizeQ || sizeQ != sizeT) {
      G4cout << "No match size EDEN Event Map: sQ:" << sizeQ << " sT:" << sizeT << endl;
      return;
   }


   // Loop on detector number
   for (G4int l = 0; l < sizeN; l++) {
      G4double N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      if (N > 0) {
         // Fill detector number
         m_Event->SetEdenSimuDetectorNumber(N);

         // Energy
         for (G4int l = 0; l < sizeQ; l++) {
            G4int QTrackID =   Charge_itr->first - N;
            G4double Q     = *(Charge_itr->second);
            if (QTrackID == NTrackID) {
               m_Event->SetEdenSimuCharge(RandGauss::shoot(Q, 1));
            }
            Charge_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeT; h++) {
            G4int TTrackID =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               m_Event->SetEdenSimuTime(RandGauss::shoot(T, 1));
            }
            Time_itr++;
         }

         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeX; h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N;
            G4double PosX     = *(Pos_X_itr->second);
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX);
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeX; h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N;
            G4double PosY     = *(Pos_Y_itr->second);
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY);
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeX; h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N;
            G4double PosZ     = *(Pos_Z_itr->second);
            if (PosZTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionZ(PosZ);
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeX; h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N;
            G4double AngTheta     = *(Ang_Theta_itr->second);
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta);
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0; h < sizeX; h++) {
            G4int AngPhiTrackID =   Ang_Phi_itr->first - N;
            G4double AngPhi     = *(Ang_Phi_itr->second);
            if (AngPhiTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAnglePhi(AngPhi);
            }
            Ang_Phi_itr++;
         }

         DetectorNumber_itr++;
      }

      // clear map for next event
      DetectorNumberHitMap    -> clear();
//      ChargeHitMap            -> clear();
      TimeHitMap              -> clear();
      PosXHitMap              -> clear();
      PosYHitMap              -> clear();
      PosZHitMap              -> clear();
      AngThetaHitMap          -> clear();
      AngPhiHitMap            -> clear();
   }
}



////////////////////////////////////////////////////////////////
void EDEN::InitializeMaterial()
{
   ////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol;
   G4double density = 0, a = 0, z = 0;
   G4int ncomponents = 0, natoms = 0;

   // for EDEN
   G4Element* H = new G4Element("Hydrogen", symbol = "H", z = 1, a = 1.01   * g / mole);
   G4Element* C = new G4Element("Carbon"  , symbol = "C", z = 6, a = 12.011 * g / mole);

   ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   // EDEN NE213
   density = 0.87 * g / cm3;
   m_MaterialEDEN_liqSci = new G4Material("EDEN_liqSci", density, ncomponents = 2);
   m_MaterialEDEN_liqSci->AddElement(H, natoms = 10);
   m_MaterialEDEN_liqSci->AddElement(C, natoms = 8);
}



////////////////////////////////////////////////////////////////	
void EDEN::InitializeScorers() 
{ 
   // EDEN associated scorer
   m_EDENScorer                                         = new G4MultiFunctionalDetector("EDENScorer");
   G4VPrimitiveScorer* DetNbr                           = new GENERALSCORERS::PSDetectorNumber("DetectorNumber", "EDEN", 0);
   G4VPrimitiveScorer* TOF                              = new GENERALSCORERS::PSTOF("Time", "EDEN", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new GENERALSCORERS::PSInteractionCoordinatesX("InterCoordX", "EDEN", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new GENERALSCORERS::PSInteractionCoordinatesY("InterCoordY", "EDEN", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new GENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ", "EDEN", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new GENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta", "EDEN", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new GENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi", "EDEN", 0);
   G4VPrimitiveScorer* Charge                           = new EDENSCORERS::EDENScorerCharge("Charge", 0);

   //and register it to the multifunctionnal detector
   m_EDENScorer->RegisterPrimitive(DetNbr);
   m_EDENScorer->RegisterPrimitive(Charge);
   m_EDENScorer->RegisterPrimitive(TOF);
   m_EDENScorer->RegisterPrimitive(InteractionCoordinatesX);
   m_EDENScorer->RegisterPrimitive(InteractionCoordinatesY);
   m_EDENScorer->RegisterPrimitive(InteractionCoordinatesZ);
   m_EDENScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta);
   m_EDENScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi);

   // Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_EDENScorer);
}
