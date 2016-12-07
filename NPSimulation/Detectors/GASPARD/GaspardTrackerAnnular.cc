/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 15/07/09                                                 *
 * Last update    : 12/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a module of annular shape for the Gaspard tracker      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 12/10/09: Change scorer scheme (N. de Sereville)                     *
 *    + 01/10/10: Fix bug with TInteractionCoordinate map size in Read       *
 *                Sensitive (N. de Sereville)                                *
 *                                                                           *
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

// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "GaspardTrackerAnnular.hh"
#include "ObsoleteGeneralScorers.hh"
//#include "GaspardScorers.hh"
#include "SiliconScorers.hh"
#include "TGaspardTrackerData.h"
#include "RootOutput.h"
#include "NPSVDetector.hh"
// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

using namespace GPDANNULAR;
//using namespace GPDSCORERS;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerAnnular::GaspardTrackerAnnular()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerAnnular::~GaspardTrackerAnnular()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerAnnular::AddModule(G4double PosZ,
                                      G4double Rmin,
                                      G4double Rmax,
                                      bool wFirstStage,
                                      bool wSecondStage,
                                      bool wThirdStage)
{
   m_PosZ.push_back(PosZ);
   m_Rmin.push_back(Rmin);
   m_Rmax.push_back(Rmax);

   m_wFirstStage.push_back(wFirstStage)   ;
   m_wSecondStage.push_back(wSecondStage) ;
   m_wThirdStage.push_back(wThirdStage)   ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerAnnular::VolumeMaker(G4int DetectorNumber,
                                        G4ThreeVector MMpos,
                                        G4RotationMatrix* MMrot,
                                        bool wFirstStage,
                                        bool wSecondStage,
                                        bool wThirdStage,
                                        G4LogicalVolume* world)
{
   G4double NbrTelescopes = DetectorNumber  ;
   G4String DetNumber                   ;
   ostringstream Number                      ;
   Number << NbrTelescopes                   ;
   DetNumber = Number.str()             ;

   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   // Name of the module
   G4String Name = "GPDAnnular" + DetNumber;

   // Definition of the volume containing the sensitive detector
   G4Tubs* solidMM = new G4Tubs(Name, 
                                Rmin,
                                Rmax,
                                Length/2,
                                0*deg, 
                                360*deg);

//   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, Iron, Name, 0, 0, 0);
   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, m_MaterialVacuum, Name, 0, 0, 0);
   
   new G4PVPlacement(G4Transform3D(*MMrot, MMpos), logicMM, Name, world, false, DetectorNumber);

   logicMM->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation){
     G4VisAttributes VisAtt(G4Colour(0.90, 0.90, 0.90));
     VisAtt.SetForceWireframe(true);
     logicMM->SetVisAttributes(VisAtt);
   }
   // Definition of a vaccuum volume
   G4ThreeVector positionVacBox = G4ThreeVector(0, 0, VacBox_PosZ);

   G4Tubs* solidVacBox = new G4Tubs("solidVacBox", 
                                    FirstStageRmin,
                                    FirstStageRmax,
                                    VacBoxThickness/2,
                                    0*deg, 
                                    360*deg); 

   G4LogicalVolume* logicVacBox = new G4LogicalVolume(solidVacBox, m_MaterialVacuum, "logicVacBox", 0, 0, 0);

   G4PVPlacement(0, positionVacBox, logicVacBox, "G" + DetNumber + "VacBox", logicMM, false, DetectorNumber);

   logicVacBox->SetVisAttributes(G4VisAttributes::Invisible);

   ////////////////////////////////////////////////////////////////
   /////////////////// First Stage Construction////////////////////
   ////////////////////////////////////////////////////////////////
   if (wFirstStage) {
      // Aluminium dead layers
      G4ThreeVector positionAluStripFront = G4ThreeVector(0, 0, AluStripFront_PosZ);
      G4ThreeVector positionAluStripBack  = G4ThreeVector(0, 0, AluStripBack_PosZ);

      G4Tubs* solidAluStrip = new G4Tubs("AluBox", 
                                         FirstStageRmin,
                                         FirstStageRmax,
                                         AluStripThickness/2, 
                                         0*deg, 
                                         360*deg); 

//      G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, Aluminium, "logicAluStrip", 0, 0, 0);
      G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, m_MaterialVacuum, "logicAluStrip", 0, 0, 0);

      new G4PVPlacement(0, positionAluStripFront, logicAluStrip, "G" + DetNumber + "AluStripFront", logicMM, false, DetectorNumber);
      new G4PVPlacement(0, positionAluStripBack,  logicAluStrip, "G" + DetNumber + "AluStripBack",  logicMM, false, DetectorNumber);

      logicAluStrip->SetVisAttributes(G4VisAttributes::Invisible);

      // Silicon detector itself
      G4ThreeVector  positionSilicon = G4ThreeVector(0, 0, Silicon_PosZ);

      G4Tubs* solidSilicon = new G4Tubs("solidSilicon", 
                                         FirstStageRmin,
                                         FirstStageRmax,
                                         FirstStageThickness/2, 
                                         0*deg, 
                                         360*deg); 
      G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon, m_MaterialSilicon, "logicSilicon", 0, 0, 0);

      new G4PVPlacement(0, positionSilicon, logicSilicon, Name + "_Silicon", logicMM, false, DetectorNumber);

      // Set First Stage sensible
      logicSilicon->SetSensitiveDetector(m_FirstStageScorer);

      ///Visualisation of Silicon Strip
      G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9)) ;   // blue
      logicSilicon->SetVisAttributes(SiliconVisAtt)                        ;
   }

   ////////////////////////////////////////////////////////////////
   //////////////// Second Stage  Construction ////////////////////
   ////////////////////////////////////////////////////////////////
   if (wSecondStage) {
      // Second stage silicon detector
      G4ThreeVector  positionSecondStage = G4ThreeVector(0, 0, SecondStage_PosZ);

      G4Tubs* solidSecondStage = new G4Tubs("solidSecondStage", 
                                            FirstStageRmin,
                                            FirstStageRmax,
                                            SecondStageThickness/2, 
                                            0*deg, 
                                            360*deg); 

      G4LogicalVolume* logicSecondStage = new G4LogicalVolume(solidSecondStage, m_MaterialSilicon, "logicSecondStage", 0, 0, 0);

      new G4PVPlacement(0, positionSecondStage, logicSecondStage, Name + "_SecondStage", logicMM, false, 0);

      ///Visualisation of Second Stage
      G4VisAttributes* SecondStageVisAtt = new G4VisAttributes(G4Colour(0.9, 0, 0));   // red
      logicSecondStage->SetVisAttributes(SecondStageVisAtt);

      // Set Second Stage sensible
      logicSecondStage->SetSensitiveDetector(m_SecondStageScorer);
   }

   ////////////////////////////////////////////////////////////////
   ///////////////// Third Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wThirdStage) {
      // Third stage silicon detector
      G4ThreeVector  positionThirdStage = G4ThreeVector(0, 0, ThirdStage_PosZ);

      G4Tubs* solidThirdStage = new G4Tubs("solidThirdStage", 
                                            FirstStageRmin,
                                            FirstStageRmax,
                                            ThirdStageThickness/2, 
                                            0*deg, 
                                            360*deg); 

      G4LogicalVolume* logicThirdStage = new G4LogicalVolume(solidThirdStage, m_MaterialSilicon, "logicThirdStage", 0, 0, 0);

      new G4PVPlacement(0, positionThirdStage, logicThirdStage, Name + "_ThirdStage", logicMM, false, 0);

      ///Visualisation of Third Stage
      G4VisAttributes* ThirdStageVisAtt = new G4VisAttributes(G4Colour(0.0, 0.9, 0.)) ;   // green
      logicThirdStage->SetVisAttributes(ThirdStageVisAtt)                        ;

      // Set Third Stage sensible
      logicThirdStage->SetSensitiveDetector(m_ThirdStageScorer);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GaspardTrackerAnnular::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GaspardTracker");
  vector<string> tokenp= {"Z","RMIN","RMAX"};

  vector<string> token={"FIRSTSTAGE","SECONDSTAGE","THIRDSTAGE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->GetMainValue() == "Annular" && blocks[i]->HasTokenList(token) ){
      bool first = blocks[i]->GetInt("FIRSTSTAGE");
      bool second = blocks[i]->GetInt("SECONDSTAGE");
      bool third = blocks[i]->GetInt("THIRDSTAGE");
        m_non_sensitive_part_visiualisation =  blocks[i]->GetInt("VIS");
      if(blocks[i]->HasTokenList(tokenp)){
        // Add module
        double Z    = blocks[i]->GetDouble("Z","mm");
        double Rmin = blocks[i]->GetDouble("Rmin","mm");
        double Rmax = blocks[i]->GetDouble("Rmax","mm");
        AddModule(Z,Rmin,Rmax,first,second,third);
      }
      else{
      cout << "ERROR: Check input formatting for Gaspard" << endl;
      exit(1);
      }
    }
  } 
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void GaspardTrackerAnnular::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
   G4ThreeVector MMpos = G4ThreeVector(0, 0, 0) ;

   bool FirstStage  = true ;
   bool SecondStage = true ;
   bool ThirdStage  = true ;

   G4int NumberOfModule = m_PosZ.size() ;

   for (G4int i = 0; i < NumberOfModule; i++) {
      // translation to position the module
      // test if module is in the forward or backward hemisphere
      (m_PosZ[i] < 0) ? m_PosZ[i] -= 0.5*Length : m_PosZ[i] += 0.5*Length;
      MMpos = G4ThreeVector(0, 0, m_PosZ[i]);

      // Passage Matrix from Lab Referential to Module Referential
      // Identity matrix by default
      MMrot = new G4RotationMatrix();
      if (MMpos.z() < 0) MMrot->rotateX(180*deg);

      // Presence of 1st, 2nd and 3rd stage
      FirstStage  = m_wFirstStage[i]  ;
      SecondStage = m_wSecondStage[i] ;
      ThirdStage  = m_wThirdStage[i]  ;

      // Build geometry and declare scorers
      VolumeMaker(i + 1, MMpos, MMrot, FirstStage, SecondStage, ThirdStage , world);
   }

   delete MMrot ;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void GaspardTrackerAnnular::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from NPS::VDetector to the present class
void GaspardTrackerAnnular::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void GaspardTrackerAnnular::ReadSensitive(const G4Event* event)
{
   //////////////
   // First stage
   G4THitsMap<G4double*>* GPD1HitMap;
   std::map<G4int, G4double**>::iterator GPD1_itr;

   G4int GPD1CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDAnnular/GPDAnnularFirstStage");
   GPD1HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD1CollectionID));

   // Loop on the GPD map
   for (GPD1_itr = GPD1HitMap->GetMap()->begin(); GPD1_itr != GPD1HitMap->GetMap()->end(); GPD1_itr++) {
      G4double* Info = *(GPD1_itr->second);

      double Energy = Info[0];
      if (Energy > EnergyThreshold) {
         double Time       = Info[1];
         int DetNbr        = (int) Info[7];
         int StripFront    = (int) Info[8];
         int StripBack     = (int) Info[9];
         
         // detector number
         ms_Event->SetGPDTrkFirstStageFrontEDetectorNbr(m_index["Annular"] + DetNbr);
         ms_Event->SetGPDTrkFirstStageFrontTDetectorNbr(m_index["Annular"] + DetNbr);
         ms_Event->SetGPDTrkFirstStageBackEDetectorNbr(m_index["Annular"] + DetNbr);
         ms_Event->SetGPDTrkFirstStageBackTDetectorNbr(m_index["Annular"] + DetNbr);

         // energy
         ms_Event->SetGPDTrkFirstStageFrontEEnergy(RandGauss::shoot(Energy, ResoFirstStage));
         ms_Event->SetGPDTrkFirstStageBackEEnergy(RandGauss::shoot(Energy, ResoFirstStage));

         // time
         Time = RandGauss::shoot(Time, ResoTimePPAC);
         ms_Event->SetGPDTrkFirstStageFrontTTime(RandGauss::shoot(Time, ResoTimeGpd));
         ms_Event->SetGPDTrkFirstStageBackTTime(RandGauss::shoot(Time, ResoTimeGpd));

         // strips X and Y
         ms_Event->SetGPDTrkFirstStageFrontEStripNbr(StripFront);
         ms_Event->SetGPDTrkFirstStageFrontTStripNbr(StripFront);
         ms_Event->SetGPDTrkFirstStageBackEStripNbr(StripBack);
         ms_Event->SetGPDTrkFirstStageBackTStripNbr(StripBack);

         // Interaction Coordinates
         ms_InterCoord->SetDetectedPositionX(Info[2]);
         ms_InterCoord->SetDetectedPositionY(Info[3]);
         ms_InterCoord->SetDetectedPositionZ(Info[4]);
         ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg);
         ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg);

      }
   }
   // clear map for next event
   GPD1HitMap->clear();


   //////////////
   // Second stage
   G4THitsMap<G4double*>* GPD2HitMap;
   std::map<G4int, G4double**>::iterator GPD2_itr;

   G4int GPD2CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SecondStageScorerGPDAnnular/GPDAnnularSecondStage");
   GPD2HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD2CollectionID));

   // Loop on the GPD map
   for (GPD2_itr = GPD2HitMap->GetMap()->begin(); GPD2_itr != GPD2HitMap->GetMap()->end(); GPD2_itr++) {
      G4double* Info = *(GPD2_itr->second);

      double Energy = Info[0];
      if (Energy > EnergyThreshold) {
         double Time       = Info[1];
         int DetNbr        = (int) Info[7];
         int StripFront    = (int) Info[8];

         // detector number
         ms_Event->SetGPDTrkSecondStageEDetectorNbr(m_index["Annular"] + DetNbr);
         ms_Event->SetGPDTrkSecondStageTDetectorNbr(m_index["Annular"] + DetNbr);

         // energy
         ms_Event->SetGPDTrkSecondStageEEnergy(RandGauss::shoot(Energy, ResoSecondStage));

         // time
         Time = RandGauss::shoot(Time, ResoTimePPAC);
         ms_Event->SetGPDTrkSecondStageTTime(RandGauss::shoot(Time, ResoTimeGpd));

         // strips X and Y
         ms_Event->SetGPDTrkSecondStageEPadNbr(StripFront);
         ms_Event->SetGPDTrkSecondStageTPadNbr(StripFront);
      }
   }
   // clear map for next event
   GPD2HitMap->clear();


   //////////////
   // Third stage
   G4THitsMap<G4double*>* GPD3HitMap;
   std::map<G4int, G4double**>::iterator GPD3_itr;

   G4int GPD3CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThirdStageScorerGPDAnnular/GPDAnnularThirdStage");
   GPD3HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD3CollectionID));

   // Loop on the GPD map
   for (GPD3_itr = GPD3HitMap->GetMap()->begin(); GPD3_itr != GPD3HitMap->GetMap()->end(); GPD3_itr++) {
      G4double* Info = *(GPD3_itr->second);

      double Energy = Info[0];
      if (Energy > EnergyThreshold) {
         double Time       = Info[1];
         int DetNbr        = (int) Info[7];
         int StripFront    = (int) Info[8];

         // detector number
         ms_Event->SetGPDTrkThirdStageEDetectorNbr(m_index["Annular"] + DetNbr);
         ms_Event->SetGPDTrkThirdStageTDetectorNbr(m_index["Annular"] + DetNbr);

         // energy
         ms_Event->SetGPDTrkThirdStageEEnergy(RandGauss::shoot(Energy, ResoThirdStage));

         // time
         Time = RandGauss::shoot(Time, ResoTimePPAC);
         ms_Event->SetGPDTrkThirdStageTTime(RandGauss::shoot(Time, ResoTimeGpd));

         // strips X and Y
         ms_Event->SetGPDTrkThirdStageEPadNbr(StripFront);
         ms_Event->SetGPDTrkThirdStageTPadNbr(StripFront);
      }
   }
   // clear map for next event
   GPD3HitMap->clear();
}



void GaspardTrackerAnnular::InitializeScorers()
{
   // check whether scorers are already defined
   bool already_exist  = false;
   m_FirstStageScorer  = NPS::VDetector::CheckScorer("FirstStageScorerGPDAnnular",  already_exist);
   m_SecondStageScorer = NPS::VDetector::CheckScorer("SecondStageScorerGPDAnnular", already_exist);
   m_ThirdStageScorer  = NPS::VDetector::CheckScorer("ThirdStageScorerGPDAnnular",  already_exist);
   if (already_exist) return;


   // First stage scorer
   G4VPrimitiveScorer* GPDScorerFirstStage =
      new SILICONSCORERS::PS_Silicon_Annular("GPDAnnularFirstStage",
                                             0,
                                             FirstStageRmin,
                                             FirstStageRmax,
                                             360*deg,
                                             0*deg,
                                             NbThetaStrips,
                                             NbPhiStrips,
                                             1);

   // Second stage scorer
   G4VPrimitiveScorer* GPDScorerSecondStage =
      new SILICONSCORERS::PS_Silicon_Annular("GPDAnnularSecondStage",
                                             0,
                                             FirstStageRmin,
                                             FirstStageRmax,
                                             0*deg,
                                             360*deg,
                                             1,
                                             1,
                                             1);

   // Third stage scorer
   G4VPrimitiveScorer* GPDScorerThirdStage =
      new SILICONSCORERS::PS_Silicon_Annular("GPDAnnularThirdStage",
                                             0,
                                             FirstStageRmin,
                                             FirstStageRmax,
                                             0*deg,
                                             360*deg,
                                             1,
                                             1,
                                             1);

   // register scorers to the multifunctionnal detector
   m_FirstStageScorer  ->RegisterPrimitive(GPDScorerFirstStage);
   m_SecondStageScorer ->RegisterPrimitive(GPDScorerSecondStage);
   m_ThirdStageScorer  ->RegisterPrimitive(GPDScorerThirdStage);



/*
   // First stage Associate Scorer
   G4VPrimitiveScorer* DetNbr                           = new OBSOLETEGENERALSCORERS::PSDetectorNumber("DetectorNumber", "GPDAnnular", 0);
   G4VPrimitiveScorer* TOF                              = new OBSOLETEGENERALSCORERS::PSTOF("StripTime","GPDAnnular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","GPDAnnular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","GPDAnnular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","GPDAnnular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","GPDAnnular", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","GPDAnnular", 0);
   G4VPrimitiveScorer* Energy                           = new GPDScorerFirstStageEnergy("StripEnergy", "GPDAnnular", 0);
   G4VPrimitiveScorer* StripPositionX                   = new GPDScorerFirstStageFrontStripAnnular("StripNumberX", 0, FirstStageRmax, 128);
   G4VPrimitiveScorer* StripPositionY                   = new GPDScorerFirstStageBackStripAnnular("StripNumberY", 0, FirstStageRmax, 128);

   //and register it to the multifunctionnal detector
   m_FirstStageScorer->RegisterPrimitive(DetNbr);
   m_FirstStageScorer->RegisterPrimitive(Energy);
   m_FirstStageScorer->RegisterPrimitive(TOF);
   m_FirstStageScorer->RegisterPrimitive(StripPositionX);
   m_FirstStageScorer->RegisterPrimitive(StripPositionY);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesX);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesY);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesZ);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi);

   // Second stage Associate Scorer
   G4VPrimitiveScorer* SecondStageEnergy = new GPDScorerSecondStageEnergy("SecondStageEnergy", "GPDAnnular", 0);
   m_SecondStageScorer->RegisterPrimitive(SecondStageEnergy);

   //  Third stage Associate Scorer 
   G4VPrimitiveScorer* ThirdStageEnergy = new GPDScorerThirdStageEnergy("ThirdStageEnergy", "GPDAnnular", 0);
   m_ThirdStageScorer->RegisterPrimitive(ThirdStageEnergy);
*/
   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_FirstStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_SecondStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_ThirdStageScorer);
}
