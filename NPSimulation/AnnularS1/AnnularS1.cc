/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 16/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define the S1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  + 11/10/09: Change scorer philosophy, one scorer for the detector number *
 *              added (N. de Sereville)                                      *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// G4 Geometry headers
#include "G4Box.hh"
#include "G4Tubs.hh"

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
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "AnnularS1.hh"
#include "AnnularS1Scorers.hh"
#include "TS1Data.h"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

using namespace ANNULARS1;
using namespace S1SCORERS;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::AnnularS1(){
   m_Event = new TS1Data();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::~AnnularS1(){
   delete m_Event;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularS1::AddModule(G4double PosZ){
   m_PosZ.push_back(PosZ);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularS1::VolumeMaker(G4int             DetecNumber,
                            G4ThreeVector     position,
                            G4RotationMatrix* rotation,
                            G4LogicalVolume*  world)
{
   G4double NbrTelescopes = DetecNumber;
   G4String DetectorNumber;
   ostringstream Number;
   Number << NbrTelescopes;
   DetectorNumber = Number.str();

   ////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol                      ;
   G4double density = 0. , a = 0, z = 0 ;
   G4int ncomponents = 0, natoms = 0    ;

      ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   // Si
   a = 28.0855 * g / mole;
   density = 2.321 * g / cm3;
   G4Material* Silicon = new G4Material("Si", z = 14., a, density);

   // Al
   density = 2.702 * g / cm3;
   a = 26.98 * g / mole;
   G4Material* Aluminium = new G4Material("Aluminium", z = 13., a, density);

   //  Vacuum
   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, ncomponents = 2);
   G4Element* N = new G4Element("Nitrogen" , symbol = "N"  , z = 7  , a = 14.01  * g / mole);
   G4Element* O = new G4Element("Oxigen"   , symbol = "O"  , z = 8  , a = 16.00  * g / mole);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);
   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   // Name of the module
   G4String Name = "S1Annular" + DetectorNumber;

   // Building the PCB
   // The PCB is a simple extruded volume from 8reference point
   vector<G4TwoVector> polygon;
   for(unsigned int i = 0 ; i < 8 ; i++){
    G4TwoVector Point(PCBPointsX[i],PCBPointsY[i]);
    polygon.push_back(Point);
   }

  // Mast volume containing all the detector
  G4ExtrudedSolid* solidAnnularS1 = new G4ExtrudedSolid(Name,
                                                       polygon,
                                                       PCBThickness*0.5,
                                                       G4TwoVector(0,0),1,
                                                       G4TwoVector(0,0),1);

   // Definition of the volume containing the sensitive detector
   G4LogicalVolume* logicAnnularS1 = new G4LogicalVolume(solidAnnularS1, Vacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*rotation, position),
                                     logicAnnularS1,
                                     Name,
                                     world,
                                     false,
                                     0);
   
   logicAnnularS1->SetVisAttributes(G4VisAttributes::Invisible);

   // PCB Base
   G4ExtrudedSolid* solidPCBBase = new G4ExtrudedSolid("PCBBase",
                                                       polygon,
                                                       PCBThickness*0.5,
                                                       G4TwoVector(0,0),1,
                                                       G4TwoVector(0,0),1);   

   // Wafer Shape to be substracted to the PCB
   G4Tubs* solidWaferShapeBase = new G4Tubs("WaferShape", 
                                            WaferInnerRadius,
                                            WaferOutterRadius,
                                            PCBThickness,
                                            0*deg, 
                                            360*deg); 
G4RotationMatrix* norotation = new  G4RotationMatrix();                        

  G4RotationMatrix* cutrotation = new  G4RotationMatrix(G4ThreeVector(0,0,1),45*deg);                        
  G4ThreeVector cutposition1(80*mm+WaferRCut,0,0); cutposition1.setPhi(45*deg);
  G4Transform3D transform1(*cutrotation,cutposition1);

  G4Box* solidCutout = new G4Box("cuttout",80*mm,80*mm,80*mm); 

  G4SubtractionSolid* solidWaferShape1 = new G4SubtractionSolid("WaferShape1",
                                                                 solidWaferShapeBase,
                                                                 solidCutout,
                                                                 transform1);

  
  G4ThreeVector cutposition2(-80*mm-WaferRCut,0,0); cutposition2.setPhi(-135*deg);
  G4Transform3D transform2(*cutrotation,cutposition2);
  G4SubtractionSolid* solidWaferShape = new G4SubtractionSolid("WaferShape",
                                                                 solidWaferShape1,
                                                                 solidCutout,
                                                                 transform2);


   // PCB final
   G4SubtractionSolid* solidPCB1 = new G4SubtractionSolid("AnnularS1_PCB1",
                                                                 solidPCBBase,
                                                                 solidWaferShape);
  
   G4Tubs* solidTub = 
    new G4Tubs("central_hole",0,PCBInnerRadius,PCBThickness,0,360*deg); 
   G4SubtractionSolid* solidPCB = new G4SubtractionSolid("AnnularS1_PCB",
                                                                 solidPCB1,
                                                                 solidTub);


    G4LogicalVolume* logicPCB = new G4LogicalVolume(solidPCB, Vacuum, "AnnularS1_PCB", 0, 0, 0);

    new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
                                     logicPCB,
                                     "AnnularS1_PCB",
                                     logicAnnularS1,
                                     false,
                                     0);
   
   G4VisAttributes* PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
   logicPCB->SetVisAttributes(PCBVisAtt);


   // Wafer itself
   G4Tubs* solidWaferBase = new G4Tubs("Wafer", 
                                            WaferInnerRadius,
                                            WaferOutterRadius,
                                            WaferThickness,
                                            0*deg, 
                                            360*deg); 
  
  G4SubtractionSolid* solidWafer1 = new G4SubtractionSolid("Wafer1",
                                                            solidWaferBase,
                                                            solidCutout,
                                                            transform1);

  G4SubtractionSolid* solidWafer = new G4SubtractionSolid("Wafer",
                                                           solidWafer1,
                                                           solidCutout,
                                                           transform2);

  G4LogicalVolume* logicWafer = new G4LogicalVolume(solidWafer, Silicon, "AnnularS1_Wafer", 0, 0, 0);
  new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
                                     logicWafer,
                                     "AnnularS1_Wafer",
                                     logicAnnularS1,
                                     false,
                                     0);

     G4VisAttributes* SiVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
     logicWafer->SetVisAttributes(SiVisAtt); 
   
   // Active Wafer
   G4Tubs* solidActiveWaferBase = new G4Tubs("ActiveWafer", 
                                            ActiveWaferInnerRadius,
                                            ActiveWaferOutterRadius,
                                            WaferThickness,
                                            0*deg, 
                                            360*deg); 
  
  G4SubtractionSolid* solidActiveWafer1 = new G4SubtractionSolid("ActiveWafer1",
                                                            solidActiveWaferBase,
                                                            solidCutout,
                                                            transform1);

  G4SubtractionSolid* solidActiveWafer = new G4SubtractionSolid("ActiveWafer",
                                                           solidActiveWafer1,
                                                           solidCutout,
                                                           transform2);

  G4LogicalVolume* logicActiveWafer = new G4LogicalVolume(solidActiveWafer, Silicon, "AnnularS1_ActiveWafer", 0, 0, 0);
  new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
                                     logicActiveWafer,
                                     "AnnularS1_ActiveWafer",
                                     logicWafer,
                                     false,
                                     0);
  logicActiveWafer->SetVisAttributes(SiVisAtt);
  
  // Set Silicon strip sensible
   logicActiveWafer->SetSensitiveDetector(m_Scorer);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AnnularS1::ReadConfiguration(string Path){
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer, DataBuffer;

   G4double Z = 0;
   bool check_Z       = false;
   bool check_VIS     = false;
   bool ReadingStatus = false;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);

      if (LineBuffer.compare(0, 9, "AnnularS1") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Annular element found: " << G4endl   ;
         ReadingStatus = true ;
      }
      else ReadingStatus = false ;

      while (ReadingStatus) {
         ConfigFile >> DataBuffer;

         // Search for comment Symbol %
    if (DataBuffer.compare(0, 1, "%") == 0) {
            ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
         }

         // Position method
         else if (DataBuffer.compare(0, 2, "Z=") == 0) {
       check_Z = true;
       ConfigFile >> DataBuffer ;
       Z = atof(DataBuffer.c_str()) ;
       Z = Z * mm;
       cout << "Z:  " << Z / mm << endl;
    }
         else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
            check_VIS = true;
       ConfigFile >> DataBuffer;
       if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
         }
         else {
            ///////////////////////////////////////////////////
            // If no Detector Token and no comment, toggle out
            ReadingStatus = false;
            G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl;
         }

         // Add The previously define module
         if (check_Z && check_VIS) {
            AddModule(Z);
            check_Z       = false;
            check_VIS     = false;
            ReadingStatus = false;
            cout << "///"<< endl;
         }
      }
   }
}



// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void AnnularS1::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* rotation = NULL;
   G4ThreeVector     position = G4ThreeVector(0, 0, 0);

   G4int NumberOfModule = m_PosZ.size() ;

   for (G4int i = 0; i < NumberOfModule; i++) {
      // translation to position the module
      position = G4ThreeVector(0, 0, m_PosZ[i]);

      // Passage Matrix from Lab Referential to Module Referential
      // Identity matrix by default
      rotation = new G4RotationMatrix();
      if (position.z() < 0) rotation->rotateX(180*deg);

      // Build geometry and declare scorers
      VolumeMaker(i + 1, position, rotation, world);
   }

   delete rotation ;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void AnnularS1::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("AnnularS1", "TS1Data", &m_Event);
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void AnnularS1::ReadSensitive(const G4Event* event)
{
   // Clear ROOT objects
   m_Event->Clear();

   //////////////////////////////////////////////////////////////////////////////////
   /////////////// Variables  Used to Read Event Map of detector ////////////////////
   //////////////////////////////////////////////////////////////////////////////////
   // First Stage
   std::map<G4int, G4int*>::iterator DetectorNumber_itr;
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

   /////////////////////////////////////////////////////////////////////////////////
   ////////////////// Read scorers  ////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////
   // Read the Scorer associated to the first Stage
   //Detector Number
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/DetectorNumber");
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID));
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin();

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/ThetaStripNumber")    ;
   XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
   X_itr = XHitMap->GetMap()->begin()                                                                    ;

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/PhiStripNumber")    ;
   YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1/InterCoordAngPhi");
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID));
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();

   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeE = EnergyHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = XHitMap->entries();
   G4int sizeY = YHitMap->entries();

   if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
      G4cout << "No match size Si Event Map: sE:"
      << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << endl ;
      return;
   }

   // Loop on AnnularS1 number 
   for (G4int l = 0; l < sizeN; l++) {
      G4double N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      if (N > 0) {
         // Fill detector number
         m_Event->SetS1ThetaEDetectorNbr(N);
         m_Event->SetS1ThetaTDetectorNbr(N);
         m_Event->SetS1PhiEDetectorNbr(N);
         m_Event->SetS1PhiTDetectorNbr(N);

         //  Energy
         Energy_itr = EnergyHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeE ; h++) {
            G4int ETrackID =   Energy_itr->first - N;
            G4double E     = *(Energy_itr->second);
            if (ETrackID == NTrackID) {
               m_Event->SetS1ThetaEEnergy(RandGauss::shoot(E, ResoFirstStage));
               m_Event->SetS1PhiEEnergy(RandGauss::shoot(E, ResoFirstStage));
            }
            Energy_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeT ; h++) {
            G4int TTrackID =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               T = RandGauss::shoot(T, ResoTimePPAC)   ;
               m_Event->SetS1ThetaTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
               m_Event->SetS1PhiTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
            }
            Time_itr++;
         }

         // Theta
         X_itr = XHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int XTrackID  =   X_itr->first - N;
            G4double X     = *(X_itr->second);
            if (XTrackID == NTrackID) {
               m_Event->SetS1ThetaEStripNbr(X)   ;
               m_Event->SetS1ThetaTStripNbr(X)   ;
            }
            X_itr++;
         }

         // Phi
         Y_itr = YHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeY ; h++) {
            G4int YTrackID  =   Y_itr->first - N;
            G4double Y     = *(Y_itr->second);
            if (YTrackID == NTrackID) {
               m_Event->SetS1PhiEStripNbr(Y);
               m_Event->SetS1PhiTStripNbr(Y);
            }
            Y_itr++;
         }

         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N;
            G4double PosX     = *(Pos_X_itr->second);
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX);
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N;
            G4double PosY     = *(Pos_Y_itr->second);
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY);
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N;
            G4double PosZ     = *(Pos_Z_itr->second);
            if (PosZTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionZ(PosZ);
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N;
            G4double AngTheta     = *(Ang_Theta_itr->second);
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta);
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngPhiTrackID =   Ang_Phi_itr->first - N;
            G4double AngPhi     = *(Ang_Phi_itr->second);
            if (AngPhiTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAnglePhi(AngPhi);
            }
            Ang_Phi_itr++;
         }

      } // end if number of detector > 0

      DetectorNumber_itr++;
   } // end loop on detector multiplicity

   // clear map for next event
   DetectorNumberHitMap -> clear();
   EnergyHitMap         -> clear();
   TimeHitMap           -> clear();
   XHitMap              -> clear();
   YHitMap              -> clear();
   PosXHitMap           -> clear();
   PosYHitMap           -> clear();
   PosZHitMap           -> clear();
   AngThetaHitMap       -> clear();
   AngPhiHitMap         -> clear();
}



void AnnularS1::InitializeScorers()
{
   // Associate Scorer
   m_Scorer = new G4MultiFunctionalDetector("ScorerAnnularS1");
   G4VPrimitiveScorer* DetNbr                           = new GENERALSCORERS::PSDetectorNumber("DetectorNumber", "S1Annular", 0);
   G4VPrimitiveScorer* Energy                           = new GENERALSCORERS::PSEnergy("StripEnergy", "S1Annular", 0);
   G4VPrimitiveScorer* TOF                              = new GENERALSCORERS::PSTOF("StripTime", "S1Annular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new GENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","S1Annular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new GENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","S1Annular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new GENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","S1Annular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new GENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","S1Annular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new GENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","S1Annular", 0);
   G4VPrimitiveScorer* ThetaStripPosition               = new AnnularS1ScorerThetaStripNumber("ThetaStripNumber", 0);
   G4VPrimitiveScorer* PhiStripPosition                 = new AnnularS1ScorerPhiStripNumber("PhiStripNumber", 0);

   //and register it to the multifunctionnal detector
   m_Scorer->RegisterPrimitive(DetNbr);
   m_Scorer->RegisterPrimitive(Energy);
   m_Scorer->RegisterPrimitive(TOF);
   m_Scorer->RegisterPrimitive(ThetaStripPosition);
   m_Scorer->RegisterPrimitive(PhiStripPosition);
   m_Scorer->RegisterPrimitive(InteractionCoordinatesX);
   m_Scorer->RegisterPrimitive(InteractionCoordinatesY);
   m_Scorer->RegisterPrimitive(InteractionCoordinatesZ);
   m_Scorer->RegisterPrimitive(InteractionCoordinatesAngleTheta);
   m_Scorer->RegisterPrimitive(InteractionCoordinatesAnglePhi);

   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_Scorer);
}
