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

// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "AnnularS1.hh"
#include "GeneralScorers.hh"
#include "AnnularS1Scorers.hh"
#include "TS1Data.h"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace ANNULARS1;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::AnnularS1()
{
   m_Event      = new TS1Data();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::~AnnularS1()
{
   delete m_Event;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularS1::AddModule(G4double PosZ,
                          G4double Rmin,
                          G4double Rmax)
{
   m_PosZ.push_back(PosZ);
   m_Rmin.push_back(Rmin);
   m_Rmax.push_back(Rmax);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularS1::VolumeMaker(G4int TelescopeNumber   ,
                            G4ThreeVector MMpos     ,
                            G4RotationMatrix* MMrot ,
                            G4LogicalVolume* world)
{
   G4double NbrTelescopes = TelescopeNumber  ;
   G4String DetectorNumber                   ;
   ostringstream Number                      ;
   Number << NbrTelescopes                   ;
   DetectorNumber = Number.str()             ;

   ////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol                      ;
   G4double density = 0. , a = 0, z = 0 ;
   G4int ncomponents = 0, natoms = 0    ;

   G4Element* H   = new G4Element("Hydrogen" , symbol = "H"  , z = 1  , a = 1.01   * g / mole);
   G4Element* C   = new G4Element("Carbon"   , symbol = "C"  , z = 6  , a = 12.011 * g / mole);
   G4Element* N   = new G4Element("Nitrogen" , symbol = "N"  , z = 7  , a = 14.01  * g / mole);
   G4Element* O   = new G4Element("Oxigen"   , symbol = "O"  , z = 8  , a = 16.00  * g / mole);
   G4Element* I   = new G4Element("Iode"     , symbol = "I"  , z = 53 , a = 126.9  * g / mole);
   G4Element* Cs  = new G4Element("Cesium"   , symbol = "Cs" , z = 55 , a = 132.9  * g / mole);

   G4Element* Co  = new G4Element("Cobalt"  , symbol = "Co" , z = 27 , a = 58.933 * g / mole);
   G4Element* Cr  = new G4Element("Cromium"  , symbol = "Cr" , z = 24 , a = 51.996 * g / mole);
   G4Element* Ni  = new G4Element("Nickel"   , symbol = "Ni" , z = 28 , a = 58.69  * g / mole);
   G4Element* Fe  = new G4Element("Iron"     , symbol = "Fe" , z = 26 , a = 55.847 * g / mole);
   G4Element* W   = new G4Element("Tungsten" , symbol = "W"  , z = 74 , a = 183.5  * g / mole);

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

   // Iron
   density = 7.874 * g / cm3;
   a = 55.847 * g / mole;
   G4Material* Iron = new G4Material("Iron", z = 26., a, density);

   // CsI
   density = 4.51 * g / cm3;
   G4Material* CsI = new G4Material("CsI", density, ncomponents = 2);
   CsI->AddElement(Cs , natoms = 1);
   CsI->AddElement(I  , natoms = 1);

   //  Vacuum
   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, ncomponents = 2);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);

   //  Mylar
   density = 1.397 * g / cm3;
   G4Material* Myl = new G4Material("Mylar", density, ncomponents = 3);
   Myl->AddElement(C, natoms = 10);
   Myl->AddElement(H, natoms = 8);
   Myl->AddElement(O, natoms = 4);

   // Havar
   G4Material* Harvar = new G4Material("Havar", 8.3*g / cm3, 5);
   Harvar->AddElement(Co , 42);
   Harvar->AddElement(Cr , 20);
   Harvar->AddElement(Ni , 13);
   Harvar->AddElement(Fe , 19);
   Harvar->AddElement(W  ,  1);

   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   // Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
   // If don't you will have a Warning unused variable 'myPVP'
   G4PVPlacement* PVPBuffer ;

   // Name of the module
   G4String Name = "AnnularS1" + DetectorNumber;

   // Definition of the volume containing the sensitive detector
   G4Tubs* solidMM = new G4Tubs(Name, 
                                Rmin,
                                Rmax,
                                Length/2,
                                0*deg, 
                                360*deg);

//   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, Iron, Name, 0, 0, 0);
   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, Vacuum, Name, 0, 0, 0);

   PVPBuffer     = new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicMM                      ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     0);

   logicMM->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation) logicMM->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));


   // Aluminium dead layers
   G4ThreeVector positionAluStripFront = G4ThreeVector(0, 0, AluStripFront_PosZ);
   G4ThreeVector positionAluStripBack  = G4ThreeVector(0, 0, AluStripBack_PosZ);

   G4Tubs* solidAluStrip = new G4Tubs("AluBox", 
                                      FirstStageRmin,
                                      FirstStageRmax,
                                      AluStripThickness/2, 
                                      0*deg, 
                                      360*deg); 

//   G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, Aluminium, "logicAluStrip", 0, 0, 0);
   G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, Vacuum, "logicAluStrip", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, positionAluStripFront, logicAluStrip, "G" + DetectorNumber + "AluStripFront", logicMM, false, 0);
   PVPBuffer = new G4PVPlacement(0, positionAluStripBack,  logicAluStrip, "G" + DetectorNumber + "AluStripBack",  logicMM, false, 0);

   logicAluStrip->SetVisAttributes(G4VisAttributes::Invisible);

   // Silicon detector itself
   G4ThreeVector  positionSilicon = G4ThreeVector(0, 0, Silicon_PosZ);

   G4Tubs* solidSilicon = new G4Tubs("solidSilicon", 
                                     FirstStageRmin,
                                     FirstStageRmax,
                                     FirstStageThickness/2, 
                                     0*deg, 
                                     360*deg); 

   G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon, Silicon, "logicSilicon", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, positionSilicon, logicSilicon, "G" + DetectorNumber + "Silicon", logicMM, false, 0);

   // Set Silicon strip sensible
   // instantiate a new scorer
   G4MultiFunctionalDetector* StripScorer = new G4MultiFunctionalDetector("ScorerAnnularS1_" + DetectorNumber);
   // attach it to the Silicon plate
   logicSilicon->SetSensitiveDetector(StripScorer);
   // and declare it to the SDManager
   G4SDManager::GetSDMpointer()->AddNewDetector(StripScorer);

   // instantiate primitive scorer
   G4VPrimitiveScorer* energy       ;
   G4VPrimitiveScorer* TOF          ;
   G4VPrimitiveScorer* ThetaStripPosition  ;
   G4VPrimitiveScorer* PhiStripPosition  ;
   G4VPrimitiveScorer* InteractionCoordinatesX  ;
   G4VPrimitiveScorer* InteractionCoordinatesY  ;
   G4VPrimitiveScorer* InteractionCoordinatesZ  ;
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta  ;
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi  ;

   // create primitive scorer
   energy         = new AnnularS1ScorerEnergy("StripEnergy", 0)                          ;
   TOF            = new AnnularS1ScorerTime("StripTime", 0)                               ;
   ThetaStripPosition = new AnnularS1ScorerThetaStripNumber("ThetaStripNumber", 0, FirstStageRmax, 128) ;
   PhiStripPosition = new AnnularS1ScorerPhiStripNumber("PhiStripNumber", 0, FirstStageRmax, 128) ;
   InteractionCoordinatesX = new PSInteractionCoordinatesX("InterCoordX", 0) ;
   InteractionCoordinatesY = new PSInteractionCoordinatesY("InterCoordY", 0) ;
   InteractionCoordinatesZ = new PSInteractionCoordinatesZ("InterCoordZ", 0) ;
   InteractionCoordinatesAngleTheta = new PSInteractionCoordinatesAngleTheta("InterCoordAngTheta", 0) ;
   InteractionCoordinatesAnglePhi   = new PSInteractionCoordinatesAnglePhi("InterCoordAngPhi", 0) ;

   // and register it to the multifunctionnal detector
   StripScorer->RegisterPrimitive(energy)         ;
   StripScorer->RegisterPrimitive(TOF)            ;
   StripScorer->RegisterPrimitive(ThetaStripPosition) ;
   StripScorer->RegisterPrimitive(PhiStripPosition) ;
   StripScorer->RegisterPrimitive(InteractionCoordinatesX) ;
   StripScorer->RegisterPrimitive(InteractionCoordinatesY) ;
   StripScorer->RegisterPrimitive(InteractionCoordinatesZ) ;
   StripScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta) ;
   StripScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi) ;

   ///Visualisation of Silicon Strip
//   G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
   G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9)) ;
   logicSilicon->SetVisAttributes(SiliconVisAtt)                        ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AnnularS1::ReadConfiguration(string Path)
{
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   G4double Z = 0, Rmin = 0, Rmax = 0;

   bool check_Z    = false ;
   bool check_Rmin = false ;
   bool check_Rmax = false ;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 9, "AnnularS1") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Annular element found: " << G4endl   ;

         ConfigFile >> DataBuffer;
         //Position method
         if (DataBuffer.compare(0, 2, "Z=") == 0) {
            check_Z = true;
            ConfigFile >> DataBuffer ;
            Z = atof(DataBuffer.c_str()) ;
            Z = Z * mm;
            cout << "Z:  " << Z / mm << endl;
         }

         ConfigFile >> DataBuffer;
         //Position method
         if (DataBuffer.compare(0, 5, "RMIN=") == 0) {
            check_Rmin = true;
            ConfigFile >> DataBuffer ;
            Rmin = atof(DataBuffer.c_str()) ;
            Rmin = Rmin * mm;
            cout << "Rmin:  " << Rmin / mm << endl;
         }

         ConfigFile >> DataBuffer;
         //Position method
         if (DataBuffer.compare(0, 5, "RMAX=") == 0) {
            check_Rmax = true;
            ConfigFile >> DataBuffer ;
            Rmax = atof(DataBuffer.c_str()) ;
            Rmax = Rmax * mm;
            cout << "Rmax:  " << Rmax / mm << endl;
         }

         ConfigFile >> DataBuffer;
         if (DataBuffer.compare(0, 4, "VIS=") == 0) {
            ConfigFile >> DataBuffer;
            if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
         }

         //Add The previously define telescope
         //With position method
         if (check_Z && check_Rmin && check_Rmax) {
            AddModule(Z,
                      Rmin,
                      Rmax);
         }
         else {
            G4cout << "Wrong Token, AnnularS1 Annular Element not added" << G4endl;
         }
      }
   }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void AnnularS1::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
//   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
   MMpos = G4ThreeVector(0, 0, 0) ;

   G4int NumberOfModule = m_PosZ.size() ;

   for (G4int i = 0; i < NumberOfModule; i++) {
      // translation to position the module
      MMpos = G4ThreeVector(0, 0, m_PosZ[i]);

      // Passage Matrix from Lab Referential to Module Referential
      // Identity matrix by default
      MMrot = new G4RotationMatrix();
      if (MMpos.z() < 0) MMrot->rotateX(180*deg);

      // Build geometry and declare scorers
      VolumeMaker(i + 1, MMpos, MMrot, world);
   }

   delete MMrot ;
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
   bool checkSi = false      ;
   G4String DetectorNumber ;

   // Clear ROOT objects
   m_Event->Clear();

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Used to Read Event Map of detector //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

   // First Stage
   std::map<G4int, G4double*>::iterator Energy_itr     ;
   std::map<G4int, G4double*>::iterator Time_itr    ;
   std::map<G4int, G4double*>::iterator X_itr       ;
   std::map<G4int, G4double*>::iterator Y_itr       ;
   std::map<G4int, G4double*>::iterator Pos_X_itr       ;
   std::map<G4int, G4double*>::iterator Pos_Y_itr       ;
   std::map<G4int, G4double*>::iterator Pos_Z_itr       ;
   std::map<G4int, G4double*>::iterator Ang_Theta_itr       ;
   std::map<G4int, G4double*>::iterator Ang_Phi_itr       ;

   G4THitsMap<G4double>* EnergyHitMap              ;
   G4THitsMap<G4double>* TimeHitMap             ;
   G4THitsMap<G4double>* XHitMap                ;
   G4THitsMap<G4double>* YHitMap                ;
   G4THitsMap<G4double>* PosXHitMap                ;
   G4THitsMap<G4double>* PosYHitMap                ;
   G4THitsMap<G4double>* PosZHitMap                ;
   G4THitsMap<G4double>* AngThetaHitMap                ;
   G4THitsMap<G4double>* AngPhiHitMap                ;

   // NULL pointer are given to avoid warning at compilation


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
   G4int HitNumber = 0;
   G4int NumberOfModule = m_PosZ.size() ;

   for (G4int i = 0 ; i < NumberOfModule ; i++) {
      checkSi = false;

      G4int k = i + 1;
      ostringstream buffer;
      buffer << k;
      DetectorNumber = buffer.str();

      // Read the Scorer associated to the first Stage
      //Energy
      G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/StripEnergy")   ;
      EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
      Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

      //Time of Flight
      G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/StripTime")    ;
      TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
      Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

      //Strip Number X
      G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/ThetaStripNumber")    ;
      XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
      X_itr = XHitMap->GetMap()->begin()                                                                    ;

      //Strip Number Y
      G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/PhiStripNumber")    ;
      YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
      Y_itr = YHitMap->GetMap()->begin()                                                                    ;

      //Interaction Coordinate X
      G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/InterCoordX")    ;
      PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
      Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

      //Interaction Coordinate Y
      G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/InterCoordY")    ;
      PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
      Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

      //Interaction Coordinate Z
      G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/InterCoordZ")    ;
      PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
      Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

      //Interaction Coordinate Angle Theta
      G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/InterCoordAngTheta")    ;
      AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
      Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

      //Interaction Coordinate Angle Phi
      G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerAnnularS1_" + DetectorNumber + "/InterCoordAngPhi")    ;
      AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))                              ;
      Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()                                                                    ;

      // Check the size of different map
      G4int sizeE = EnergyHitMap->entries();
      G4int sizeT = TimeHitMap->entries();
      G4int sizeX = XHitMap->entries();
      G4int sizeY = YHitMap->entries();

      if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
         G4cout << "No match size Si Event Map: sE:"
         << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << endl ;
         return;
      }

      // Loop on Strip energy
      for (G4int l = 0 ; l < sizeE ; l++) {
         G4int ETrackID  =   Energy_itr->first     ;
         G4double E     = *(Energy_itr->second)    ;

         if (E > 0) {
            checkSi = true   ;
            m_Event->SetS1ThetaEEnergy(RandGauss::shoot(E, ResoFirstStage))    ;
            m_Event->SetS1PhiEEnergy(RandGauss::shoot(E, ResoFirstStage))    ;

            //  Time
            Time_itr = TimeHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeT ; h++) {
               G4int TTrackID  =   Time_itr->first       ;
               G4double T     = *(Time_itr->second)      ;

               if (TTrackID == ETrackID) {
                  T = RandGauss::shoot(T, ResoTimePPAC)   ;
                  m_Event->SetS1ThetaTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
                  m_Event->SetS1PhiTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
               }
               Time_itr++;
            }

            // Theta
            X_itr = XHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int XTrackID  =   X_itr->first     ;
               G4double X     = *(X_itr->second)      ;
               if (XTrackID == ETrackID) {
                  m_Event->SetS1ThetaEDetectorNbr(k) ;
                  m_Event->SetS1ThetaTDetectorNbr(k) ;
                  m_Event->SetS1ThetaEStripNbr(X)   ;
                  m_Event->SetS1ThetaTStripNbr(X)   ;
               }

               X_itr++;
            }

            // Phi
            Y_itr = YHitMap->GetMap()->begin()  ;
            for (G4int h = 0 ; h < sizeY ; h++) {
               G4int YTrackID  =   Y_itr->first    ;
               G4double Y     = *(Y_itr->second)      ;
               if (YTrackID == ETrackID) {
                  m_Event->SetS1PhiEDetectorNbr(k) ;
                  m_Event->SetS1PhiTDetectorNbr(k) ;
                  m_Event->SetS1PhiEStripNbr(Y)   ;
                  m_Event->SetS1PhiTStripNbr(Y)   ;
               }

               Y_itr++;
            }

            // Pos X
            Pos_X_itr = PosXHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosXTrackID =   Pos_X_itr->first     ;
               G4double PosX     = *(Pos_X_itr->second)      ;
               if (PosXTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionX(PosX) ;
               }
               Pos_X_itr++;
            }

            // Pos Y
            Pos_Y_itr = PosYHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosYTrackID =   Pos_Y_itr->first     ;
               G4double PosY     = *(Pos_Y_itr->second)      ;
               if (PosYTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionY(PosY) ;
               }
               Pos_Y_itr++;
            }

            // Pos Z
            Pos_Z_itr = PosZHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosZTrackID =   Pos_Z_itr->first     ;
               G4double PosZ     = *(Pos_Z_itr->second)      ;
               if (PosZTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionZ(PosZ) ;
               }
               Pos_Z_itr++;
            }

            // Angle Theta
            Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int AngThetaTrackID =   Ang_Theta_itr->first     ;
               G4double AngTheta     = *(Ang_Theta_itr->second)      ;
               if (AngThetaTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedAngleTheta(AngTheta) ;
               }
               Ang_Theta_itr++;
            }

            // Angle Phi
            Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int AngPhiTrackID =   Ang_Phi_itr->first     ;
               G4double AngPhi     = *(Ang_Phi_itr->second)      ;
               if (AngPhiTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedAnglePhi(AngPhi) ;
               }
               Ang_Phi_itr++;
            }


         }

         Energy_itr++;
         if (checkSi) HitNumber++ ;
      }

      // clear map for next event
      EnergyHitMap   ->clear();
      TimeHitMap     ->clear();
      XHitMap        ->clear();
      YHitMap        ->clear();
      PosXHitMap     ->clear();
      PosYHitMap     ->clear();
      PosZHitMap     ->clear();
      AngThetaHitMap ->clear();
      AngPhiHitMap   ->clear();
   }
}
