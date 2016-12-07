/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 03/09/09                                                 *
 * Last update    : 12/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the Gaspard tracker                 *
 *             The goal of this class is to be a starting point to create a  *
 *             new shape to be added to the Gaspard tracker.                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 07/09/09: Fix bug for placing module with (r,theta,phi) method.      *
 *                (N. de Sereville)                                          *
 *    + 12/10/09: Change scorer scheme (N. de Sereville)                     *
 *    + 01/10/10: Fix bug with TInteractionCoordinate map size in Read       *
 *                Sensitive (N. de Sereville)                                *
 *    + 01/07/11: Add dead layer support (N. de Sereville)                   *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// G4 Geometry headers
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"

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
#include "GaspardTrackerDummyShape.hh"
#include "ObsoleteGeneralScorers.hh"
#include "GaspardScorers.hh"
#include "RootOutput.h"
#include "NPSVDetector.hh"
#include "NPOptionManager.h"
#include "NPSDetectorFactory.hh"
// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

using namespace GPDDUMMYSHAPE;
using namespace GPDSCORERS;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerDummyShape::GaspardTrackerDummyShape()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerDummyShape::~GaspardTrackerDummyShape()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerDummyShape::AddModule(G4ThreeVector X1_Y1     ,
                                         G4ThreeVector X128_Y1   ,
                                         G4ThreeVector X1_Y128   ,
                                         G4ThreeVector X128_Y128 ,
                                         bool wFirstStage        ,
                                         bool wSecondStage       ,
                                         bool wThirdStage)
{
   m_DefinitionType.push_back(true) ;

   m_X1_Y1.push_back(X1_Y1)               ;
   m_X128_Y1.push_back(X128_Y1)           ;
   m_X1_Y128.push_back(X1_Y128)           ;
   m_X128_Y128.push_back(X128_Y128)       ;

   m_R.push_back(0)      ;
   m_Theta.push_back(0)  ;
   m_Phi.push_back(0)    ;
   m_beta_u.push_back(0) ;
   m_beta_v.push_back(0) ;
   m_beta_w.push_back(0) ;

   m_wFirstStage.push_back(wFirstStage)   ;
   m_wSecondStage.push_back(wSecondStage) ;
   m_wThirdStage.push_back(wThirdStage)   ;

//   m_wNumberStrip.push_back(wNumberStrip);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerDummyShape::AddModule(G4double R        ,
                                         G4double Theta    ,
                                         G4double Phi      ,
                                         G4double beta_u   ,
                                         G4double beta_v   ,
                                         G4double beta_w   ,
                                         bool wFirstStage  ,
                                         bool wSecondStage ,
                                         bool wThirdStage)
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0);

   m_DefinitionType.push_back(false);

   m_X1_Y1.push_back(empty)     ;
   m_X128_Y1.push_back(empty)   ;
   m_X1_Y128.push_back(empty)   ;
   m_X128_Y128.push_back(empty) ;

   m_R.push_back(R)                       ;
   m_Theta.push_back(Theta)               ;
   m_Phi.push_back(Phi)                   ;
   m_beta_u.push_back(beta_u)             ;
   m_beta_v.push_back(beta_v)             ;
   m_beta_w.push_back(beta_w)             ;

   m_wFirstStage.push_back(wFirstStage)   ;
   m_wSecondStage.push_back(wSecondStage) ;
   m_wThirdStage.push_back(wThirdStage)   ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerDummyShape::VolumeMaker(G4int DetectorNumber,
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

   /////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   G4String Name = "GPDDummyShape" + DetNumber ;

   G4Box*           solidGPDDummyShape = new G4Box(Name, 0.5*FaceFront, 0.5*FaceFront, 0.5*Length);
   G4LogicalVolume* logicGPDDummyShape = new G4LogicalVolume(solidGPDDummyShape, m_MaterialVacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicGPDDummyShape           ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     DetectorNumber);

   logicGPDDummyShape->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation) logicGPDDummyShape->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));

   //Place two marker to identify the u and v axis on silicon face:
   //marker are placed a bit before the silicon itself so they don't perturbate simulation
   //Uncomment to help debugging or if you want to understand the way the code work.
   //I should recommand to Comment it during simulation to avoid perturbation of simulation
   //Remember G4 is limitationg step on geometry constraints.
   G4ThreeVector positionMarkerU = MMCenter*0.8 + MMu*FirstStageFace/4;
   G4Box*           solidMarkerU = new G4Box("solidMarkerU", FirstStageFace/4, 1*mm, 1*mm);
   G4LogicalVolume* logicMarkerU = new G4LogicalVolume(solidMarkerU, m_MaterialVacuum, "logicMarkerU", 0, 0, 0);
   new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerU), logicMarkerU, "MarkerU", world, false, 0);

   G4VisAttributes* MarkerUVisAtt= new G4VisAttributes(G4Colour(0.,0.,0.5)); //blue
   logicMarkerU->SetVisAttributes(MarkerUVisAtt);

   G4ThreeVector positionMarkerV = MMCenter*0.8 + MMv*FirstStageFace/4;
   G4Box*           solidMarkerV = new G4Box("solidMarkerU", 1*mm, FirstStageFace/4, 1*mm);
   G4LogicalVolume* logicMarkerV = new G4LogicalVolume(solidMarkerV, m_MaterialVacuum, "logicMarkerV", 0, 0, 0);
   new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerV), logicMarkerV, "MarkerV", world, false, 0);

   G4VisAttributes* MarkerVVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.)); //green
   logicMarkerV->SetVisAttributes(MarkerVVisAtt);

   ////////////////////////////////////////////////////////////////
   ///////////////// First Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wFirstStage) {
      // Dead layers
/*      G4ThreeVector positionFirstStageDLFront = G4ThreeVector(0, 0, FirstStage_DL_Front_PosZ);
      G4ThreeVector positionFirstStageDLBack  = G4ThreeVector(0, 0, FirstStage_DL_Back_PosZ);

      G4Box*           solidFirstStageDL = new G4Box("solidFirstStageDL", 0.5*FirstStageFace, 0.5*FirstStageFace, 0.5*FirstStageDeadLayer);
      G4LogicalVolume* logicFirstStageDL = new G4LogicalVolume(solidFirstStageDL, Aluminium, "logicFirstStageDL", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionFirstStageDLFront, logicFirstStageDL, Name + "_DLFront", logicGPDDummyShape, false, 0);
      PVPBuffer = new G4PVPlacement(0, positionFirstStageDLBack,  logicFirstStageDL, Name + "_DLBack",  logicGPDDummyShape, false, 0);

      logicFirstStageDL->SetVisAttributes(G4VisAttributes::Invisible);
*/

      // Silicon detector itself
      G4ThreeVector  positionFirstStage = G4ThreeVector(0, 0, FirstStage_PosZ);

      G4Box*           solidFirstStage = new G4Box("solidFirstStage", 0.5*FirstStageFace, 0.5*FirstStageFace, 0.5*FirstStageThickness);
      G4LogicalVolume* logicFirstStage = new G4LogicalVolume(solidFirstStage, m_MaterialSilicon, "logicFirstStage", 0, 0, 0);

      new G4PVPlacement(0, 
                        positionFirstStage, 
                        logicFirstStage, 
                        Name + "_FirstStage", 
                        logicGPDDummyShape, 
                        false, 
                        DetectorNumber);

      // Set First Stage sensible
      logicFirstStage->SetSensitiveDetector(m_FirstStageScorer);

      ///Visualisation of FirstStage Strip
      G4VisAttributes* FirstStageVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));   // blue
      logicFirstStage->SetVisAttributes(FirstStageVisAtt);
   }

   ////////////////////////////////////////////////////////////////
   //////////////////// Second Stage  Construction ////////////////
   ////////////////////////////////////////////////////////////////
   if (wSecondStage) {
      // Dead layers
/*      G4ThreeVector positionSecondStageDLFront = G4ThreeVector(0, 0, SecondStage_DL_Front_PosZ);
      G4ThreeVector positionSecondStageDLBack  = G4ThreeVector(0, 0, SecondStage_DL_Back_PosZ);

      G4Box*           solidSecondStageDL = new G4Box("solidSecondStageDL", 0.5*SecondStageFace, 0.5*SecondStageFace, 0.5*SecondStageDeadLayer);
      G4LogicalVolume* logicSecondStageDL = new G4LogicalVolume(solidSecondStageDL, Aluminium, "logicSecondStageDL", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionSecondStageDLFront, logicSecondStageDL, Name + "_DLFront", logicGPDDummyShape, false, 0);
      PVPBuffer = new G4PVPlacement(0, positionSecondStageDLBack,  logicSecondStageDL, Name + "_DLBack",  logicGPDDummyShape, false, 0);

      logicSecondStageDL->SetVisAttributes(G4VisAttributes::Invisible);
*/

      // Second stage silicon detector
      G4ThreeVector  positionSecondStage = G4ThreeVector(0, 0, SecondStage_PosZ);

      G4Box*           solidSecondStage = new G4Box("solidSecondStage", 0.5*SecondStageFace, 0.5*SecondStageFace, 0.5*SecondStageThickness);
      G4LogicalVolume* logicSecondStage = new G4LogicalVolume(solidSecondStage, m_MaterialSilicon, "logicSecondStage", 0, 0, 0);

      new G4PVPlacement(0, 
                        positionSecondStage, 
                        logicSecondStage, 
                        Name + "_SecondStage", 
                        logicGPDDummyShape, 
                        false, 
                        DetectorNumber);

      // Set Second Stage sensible
      logicSecondStage->SetSensitiveDetector(m_SecondStageScorer);

      ///Visualisation of SecondStage Strip
      G4VisAttributes* SecondStageVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
      logicSecondStage->SetVisAttributes(SecondStageVisAtt);
   }

   ////////////////////////////////////////////////////////////////
   ///////////////// Third Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wThirdStage) {
      // Dead layers
/*      G4ThreeVector positionThirdStageDLFront = G4ThreeVector(0, 0, ThirdStage_DL_Front_PosZ);
      G4ThreeVector positionThirdStageDLBack  = G4ThreeVector(0, 0, ThirdStage_DL_Back_PosZ);

      G4Box*           solidThirdStageDL = new G4Box("solidThirdStageDL", 0.5*ThirdStageFace, 0.5*ThirdStageFace, 0.5*ThirdStageDeadLayer);
      G4LogicalVolume* logicThirdStageDL = new G4LogicalVolume(solidThirdStageDL, Aluminium, "logicThirdStageDL", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionThirdStageDLFront, logicThirdStageDL, Name + "_DLFront", logicGPDDummyShape, false, 0);
      PVPBuffer = new G4PVPlacement(0, positionThirdStageDLBack,  logicThirdStageDL, Name + "_DLBack",  logicGPDDummyShape, false, 0);

      logicThirdStageDL->SetVisAttributes(G4VisAttributes::Invisible);
*/

      // Third stage silicon detector
      G4ThreeVector  positionThirdStage = G4ThreeVector(0, 0, ThirdStage_PosZ);

      G4Box*           solidThirdStage = new G4Box("solidThirdStage", 0.5*ThirdStageFace, 0.5*ThirdStageFace, 0.5*ThirdStageThickness);
      G4LogicalVolume* logicThirdStage = new G4LogicalVolume(solidThirdStage, m_MaterialSilicon, "logicThirdStage", 0, 0, 0);

      new G4PVPlacement(0, 
                        positionThirdStage, 
                        logicThirdStage, 
                        Name + "_ThirdStage", 
                        logicGPDDummyShape, 
                        false, 
                        DetectorNumber);

      // Set Third Stage sensible
      logicThirdStage->SetSensitiveDetector(m_ThirdStageScorer);

      ///Visualisation of Third Stage
      G4VisAttributes* ThirdStageVisAtt = new G4VisAttributes(G4Colour(0.0, 0.9, 0.0));   // green
      logicThirdStage->SetVisAttributes(ThirdStageVisAtt);
   }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GaspardTrackerDummyShape::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GaspardTracker");
  vector<string> token_cart= {"X1_Y1","X128_Y1","X1_Y128","X128_Y128"};
  vector<string> token_sphe= {"R","THETA","PHI","BETA"};

  vector<string> token={"FIRSTSTAGE","SECONDSTAGE","THIRDSTAGE"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->GetMainValue() == "DummyShape" && blocks[i]->HasTokenList(token) ){
       
      bool first = blocks[i]->GetInt("FIRSTSTAGE");
      bool second = blocks[i]->GetInt("SECONDSTAGE");
      bool third = blocks[i]->GetInt("THIRDSTAGE");
      if(blocks[i]->HasToken("VIS"))
        m_non_sensitive_part_visiualisation =  blocks[i]->GetInt("VIS");

      if(blocks[i]->HasTokenList(token_cart)){
        // Add module
        G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y1","mm"));
        G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("X128_Y1","mm"));
        G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y128","mm"));
        G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("X128_Y128","mm"));
      
        AddModule(A,B,C,D,first,second,third);
      }
     else if(blocks[i]->HasTokenList(token_sphe)){
        // Add module
        double R = blocks[i]->GetDouble("R","mm");
        double Theta = blocks[i]->GetDouble("THETA","deg");
        double Phi = blocks[i]->GetDouble("PHI","deg");
        vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      
        AddModule(R,Theta,Phi,beta[0],beta[1],beta[2],first,second,third);
      }
    }
  } 
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void GaspardTrackerDummyShape::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
//   G4ThreeVector     MMu      = G4ThreeVector(0, 0, 0) ;
//   G4ThreeVector     MMv      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMw      = G4ThreeVector(0, 0, 0) ;
//   G4ThreeVector     MMCenter = G4ThreeVector(0, 0, 0) ;
   bool FirstStage  = true ;
   bool SecondStage = true ;
   bool ThirdStage  = true ;

   G4int NumberOfTelescope = m_DefinitionType.size() ;

   for (G4int i = 0; i < NumberOfTelescope; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         MMu = m_X128_Y1[i] - m_X1_Y1[i]; 
         MMu = MMu.unit();

         MMv = m_X1_Y128[i] - m_X1_Y1[i];
         MMv = MMv.unit();
         MMw = MMu.cross(MMv);
         MMw = MMw.unit();

         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4;

         // Passage Matrix from Lab Referential to Telescope Referential
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // translation to place Telescope
         MMpos = MMw * Length * 0.5 + MMCenter;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i] ;
         G4double Phi   = m_Phi[i]   ;

         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad);
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad);
         G4double wZ = m_R[i] * cos(Theta / rad);
         MMw = G4ThreeVector(wX, wY, wZ);

         // vector corresponding to the center of the module
         MMCenter = MMw;

         // vector parallel to one axis of silicon plane
         G4double ii = cos(Theta / rad) * cos(Phi / rad);
         G4double jj = cos(Theta / rad) * sin(Phi / rad);
         G4double kk = -sin(Theta / rad);
         G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

         MMw = MMw.unit();
         MMu = MMw.cross(Y);
         MMv = MMw.cross(MMu);
         MMv = MMv.unit();
         MMu = MMu.unit();

         // Passage Matrix from Lab Referential to Telescope Referential
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // Telescope is rotate of Beta angle around MMv axis.
         MMrot->rotate(m_beta_u[i], MMu);
         MMrot->rotate(m_beta_v[i], MMv);
         MMrot->rotate(m_beta_w[i], MMw);
         // translation to place Telescope
         MMpos = MMw * Length * 0.5 + MMCenter;
      }

      FirstStage  = m_wFirstStage[i]  ;
      SecondStage = m_wSecondStage[i] ;
      ThirdStage  = m_wThirdStage[i]  ;

      VolumeMaker(i + 1, MMpos, MMrot, FirstStage, SecondStage, ThirdStage, world);
   }

   delete MMrot ;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void GaspardTrackerDummyShape::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from NPS::VDetector to the present class
void GaspardTrackerDummyShape::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void GaspardTrackerDummyShape::ReadSensitive(const G4Event* event)
{
   //////////////////////////////////////////////////////////////////////////////////////
   //////////////////////// Used to Read Event Map of detector //////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////
   // First Stage
   std::map<G4int, G4int*>::iterator    DetectorNumber_itr;
   std::map<G4int, G4double*>::iterator Energy_itr;
   std::map<G4int, G4double*>::iterator Time_itr;
   std::map<G4int, G4int*>::iterator    X_itr;
   std::map<G4int, G4int*>::iterator    Y_itr;
   std::map<G4int, G4double*>::iterator Pos_X_itr;
   std::map<G4int, G4double*>::iterator Pos_Y_itr;
   std::map<G4int, G4double*>::iterator Pos_Z_itr;
   std::map<G4int, G4double*>::iterator Ang_Theta_itr;
   std::map<G4int, G4double*>::iterator Ang_Phi_itr;

   G4THitsMap<G4int>*    DetectorNumberHitMap;
   G4THitsMap<G4double>* EnergyHitMap;
   G4THitsMap<G4double>* TimeHitMap;
   G4THitsMap<G4int>*    XHitMap;
   G4THitsMap<G4int>*    YHitMap;
   G4THitsMap<G4double>* PosXHitMap;
   G4THitsMap<G4double>* PosYHitMap;
   G4THitsMap<G4double>* PosZHitMap;
   G4THitsMap<G4double>* AngThetaHitMap;
   G4THitsMap<G4double>* AngPhiHitMap;

   // NULL pointer are given to avoid warning at compilation
   // Second Stage
   std::map<G4int, G4double*>::iterator SecondStageEnergy_itr ;
   G4THitsMap<G4double>* SecondStageEnergyHitMap = NULL      ;
   // Third Stage
   std::map<G4int, G4double*>::iterator ThirdStageEnergy_itr  ;
   G4THitsMap<G4double>* ThirdStageEnergyHitMap = NULL    ;


   // Read the Scorer associate to the Silicon Strip
   //Detector Number
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/DetectorNumber")    ;
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))         ;
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/StripIDFront")    ;
   XHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
   X_itr = XHitMap->GetMap()->begin()                                                                    ;

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/StripIDBack");
   YHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDDummyShape/InterCoordAngPhi")    ;
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))                              ;
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()                                                                    ;


   // Read the Scorer associate to the SecondStage
   //Energy
   G4int SecondStageEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SecondStageScorerGPDDummyShape/SecondStageEnergy")   ;
   SecondStageEnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(SecondStageEnergyCollectionID))                 ;
   SecondStageEnergy_itr = SecondStageEnergyHitMap->GetMap()->begin()                                                     ;


   // Read the Scorer associate to the ThirdStage
   //Energy
   G4int ThirdStageEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThirdStageScorerGPDDummyShape/ThirdStageEnergy");
   ThirdStageEnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(ThirdStageEnergyCollectionID));
   ThirdStageEnergy_itr = ThirdStageEnergyHitMap->GetMap()->begin();


   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeE = EnergyHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = XHitMap->entries();
   G4int sizeY = YHitMap->entries();


   //G4cout << "sizeN:" << sizeN << G4endl;

   if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
      G4cout << "No match size Si Event Map: sE:"
         << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << G4endl ;
      return;
   }

   // Loop on FirstStage number
   for (G4int l = 0; l < sizeN; l++) {
      G4double N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      if (N > 0) {
         // Fill detector number
         ms_Event->SetGPDTrkFirstStageFrontEDetectorNbr(m_index["DummyShape"] + N);
         ms_Event->SetGPDTrkFirstStageFrontTDetectorNbr(m_index["DummyShape"] + N);
         ms_Event->SetGPDTrkFirstStageBackEDetectorNbr(m_index["DummyShape"] + N);
         ms_Event->SetGPDTrkFirstStageBackTDetectorNbr(m_index["DummyShape"] + N);

         // Energy
         Energy_itr = EnergyHitMap->GetMap()->begin();
         for (G4int ll = 0 ; ll < sizeE ; ll++) {
            G4int ETrackID  =   Energy_itr->first - N;
            G4double E     = *(Energy_itr->second);
            if (ETrackID == NTrackID) {
               ms_Event->SetGPDTrkFirstStageFrontEEnergy(RandGauss::shoot(E, ResoFirstStage));
               ms_Event->SetGPDTrkFirstStageBackEEnergy(RandGauss::shoot(E, ResoFirstStage));
            }
            Energy_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeT ; h++) {
            G4int TTrackID  =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               ms_Event->SetGPDTrkFirstStageFrontTTime(RandGauss::shoot(T, ResoTimeGpd));
               ms_Event->SetGPDTrkFirstStageBackTTime(RandGauss::shoot(T, ResoTimeGpd));
            }
            Time_itr++;
         }

         // Strip X
         X_itr = XHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int XTrackID  =   X_itr->first - N;
            G4int X         = *(X_itr->second);
            if (XTrackID == NTrackID) {
               ms_Event->SetGPDTrkFirstStageFrontEStripNbr(X);
               ms_Event->SetGPDTrkFirstStageFrontTStripNbr(X);
            }
            X_itr++;
         }

         // Strip Y
         Y_itr = YHitMap->GetMap()->begin()  ;
         for (G4int h = 0 ; h < sizeY ; h++) {
            G4int YTrackID  =   Y_itr->first - N;
            G4int Y         = *(Y_itr->second);
            if (YTrackID == NTrackID) {
               ms_Event->SetGPDTrkFirstStageBackEStripNbr(Y);
               ms_Event->SetGPDTrkFirstStageBackTStripNbr(Y);
            }
            Y_itr++;
         }

         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0; h < PosXHitMap->entries(); h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N    ;
            G4double PosX     = *(Pos_X_itr->second)      ;
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX) ;
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < PosYHitMap->entries(); h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N    ;
            G4double PosY     = *(Pos_Y_itr->second)      ;
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY) ;
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < PosZHitMap->entries(); h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N    ;
            G4double PosZ     = *(Pos_Z_itr->second)      ;
            if (PosZTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionZ(PosZ) ;
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0; h < AngThetaHitMap->entries(); h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N    ;
            G4double AngTheta     = *(Ang_Theta_itr->second)      ;
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta) ;
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0; h < AngPhiHitMap->entries(); h++) {
            G4int AngPhiTrackID =   Ang_Phi_itr->first - N    ;
            G4double AngPhi     = *(Ang_Phi_itr->second)      ;
            if (AngPhiTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAnglePhi(AngPhi) ;
            }
            Ang_Phi_itr++;
         }

         // Second Stage
         SecondStageEnergy_itr = SecondStageEnergyHitMap->GetMap()->begin() ;
         for (G4int h = 0 ; h < SecondStageEnergyHitMap->entries() ; h++) {
            G4int SecondStageEnergyTrackID =   SecondStageEnergy_itr->first - N;
            G4double SecondStageEnergy     = *(SecondStageEnergy_itr->second);

            if (SecondStageEnergyTrackID == NTrackID) {
               ms_Event->SetGPDTrkSecondStageEEnergy(RandGauss::shoot(SecondStageEnergy, ResoSecondStage)) ;
               ms_Event->SetGPDTrkSecondStageEPadNbr(1);
               ms_Event->SetGPDTrkSecondStageTPadNbr(1);
               ms_Event->SetGPDTrkSecondStageTTime(1);
               ms_Event->SetGPDTrkSecondStageTDetectorNbr(m_index["DummyShape"] + N);
               ms_Event->SetGPDTrkSecondStageEDetectorNbr(m_index["DummyShape"] + N);
            }
            SecondStageEnergy_itr++;
         }

         // Third Stage
         ThirdStageEnergy_itr = ThirdStageEnergyHitMap->GetMap()->begin()  ;
         for (G4int h = 0 ; h < ThirdStageEnergyHitMap->entries() ; h++) {
            G4int ThirdStageEnergyTrackID  =   ThirdStageEnergy_itr->first - N;
            G4double ThirdStageEnergy      = *(ThirdStageEnergy_itr->second);

            if (ThirdStageEnergyTrackID == NTrackID) {
               ms_Event->SetGPDTrkThirdStageEEnergy(RandGauss::shoot(ThirdStageEnergy, ResoThirdStage));
               ms_Event->SetGPDTrkThirdStageEPadNbr(1);
               ms_Event->SetGPDTrkThirdStageTPadNbr(1);
               ms_Event->SetGPDTrkThirdStageTTime(1);
               ms_Event->SetGPDTrkThirdStageTDetectorNbr(m_index["DummyShape"] + N);
               ms_Event->SetGPDTrkThirdStageEDetectorNbr(m_index["DummyShape"] + N);
            }
            ThirdStageEnergy_itr++;
         }

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
   SecondStageEnergyHitMap -> clear();
   ThirdStageEnergyHitMap  -> clear();
}



void GaspardTrackerDummyShape::InitializeScorers()
{
   bool already_exist = false;
   m_FirstStageScorer = NPS::VDetector::CheckScorer("FirstStageScorerGPDDummyShape", already_exist);
   m_SecondStageScorer = NPS::VDetector::CheckScorer("SecondStageScorerGPDDummyShape",already_exist);
   m_ThirdStageScorer = NPS::VDetector::CheckScorer("ThirdStageScorerGPDDummyShape",already_exist);
   if(already_exist) return;

   // First stage Associate Scorer
   G4VPrimitiveScorer* DetNbr                           = new OBSOLETEGENERALSCORERS::PSDetectorNumber("DetectorNumber", "GPDDummyShape", 0);
   G4VPrimitiveScorer* TOF                              = new OBSOLETEGENERALSCORERS::PSTOF("StripTime","GPDDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","GPDDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","GPDDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","GPDDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","GPDDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","GPDDummyShape", 0);
   G4VPrimitiveScorer* Energy                           = new GPDScorerFirstStageEnergy("StripEnergy", "GPDDummyShape", 0);
   G4VPrimitiveScorer* StripPositionX                   = new GPDScorerFirstStageFrontStripDummyShape("StripIDFront", 0, NumberOfStrips);
   G4VPrimitiveScorer* StripPositionY                   = new GPDScorerFirstStageBackStripDummyShape("StripIDBack", 0, NumberOfStrips);

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
     G4VPrimitiveScorer* SecondStageEnergy = new GPDScorerSecondStageEnergy("SecondStageEnergy", "GPDDummyShape", 0);
   m_SecondStageScorer->RegisterPrimitive(SecondStageEnergy);


   //  Third stage Associate Scorer 
   G4VPrimitiveScorer* ThirdStageEnergy = new GPDScorerThirdStageEnergy("ThirdStageEnergy", "GPDDummyShape", 0);
   m_ThirdStageScorer->RegisterPrimitive(ThirdStageEnergy);


   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_FirstStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_SecondStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_ThirdStageScorer);
}
