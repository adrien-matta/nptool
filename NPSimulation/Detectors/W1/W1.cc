/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 12/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define the W1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
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
#include "G4Material.hh"
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
#include "ObsoleteGeneralScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "W1.hh"
#include "W1Scorers.hh"
#include "TW1Data.h"
#include "RootOutput.h"
#include "NPOptionManager.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace W1SQUARE;



W1::W1()
   : m_Event(new TW1Data)
{
   InitializeMaterials();
}



W1::~W1()
{
   delete m_Event;
}



void W1::AddDetector(G4ThreeVector X1_Y1, G4ThreeVector X16_Y1,
                     G4ThreeVector X1_Y16, G4ThreeVector X16_Y16)
{
   m_DefinitionType.push_back(true);

   m_X1_Y1.push_back(X1_Y1);
   m_X16_Y1.push_back(X16_Y1);
   m_X1_Y16.push_back(X1_Y16);
   m_X16_Y16.push_back(X16_Y16);

   m_R.push_back(0);
   m_Theta.push_back(0);
   m_Phi.push_back(0);
   m_beta_u.push_back(0);
   m_beta_v.push_back(0);
   m_beta_w.push_back(0);
}



void W1::AddDetector(G4double R, G4double Theta, G4double Phi,
                     G4double beta_u, G4double beta_v, G4double beta_w)
{
  G4ThreeVector empty = G4ThreeVector(0, 0, 0);

  m_DefinitionType.push_back(false);

  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_beta_u.push_back(beta_u);
  m_beta_v.push_back(beta_v);
  m_beta_w.push_back(beta_w);

  m_X1_Y1.push_back(empty);
  m_X16_Y1.push_back(empty);
  m_X1_Y16.push_back(empty);
  m_X16_Y16.push_back(empty);

}



void W1::VolumeMaker(G4int             DetecNumber,
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
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   G4String Name = "W1Square" + DetectorNumber;

   // Definition of the volume containing the sensitive detector
   G4Box*           solidW1 = new G4Box(Name, 0.5*FaceFront, 0.5*FaceFront, 0.5*Length);
   G4LogicalVolume* logicW1 = new G4LogicalVolume(solidW1, m_MaterialVacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*rotation, position), logicW1, Name, world, false, 0);

   logicW1->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation) logicW1->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));

   // Aluminium dead layers
   G4ThreeVector positionAluStripFront = G4ThreeVector(0, 0, AluStripFront_PosZ);
   G4ThreeVector positionAluStripBack  = G4ThreeVector(0, 0, AluStripBack_PosZ);

   G4Box*           solidAluStrip = new G4Box("AluBox", 0.5*SiliconFace, 0.5*SiliconFace, 0.5*AluStripThickness);
//   G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, m_MaterialAluminium, "logicAluStrip", 0, 0, 0);
   G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, m_MaterialVacuum, "logicAluStrip", 0, 0, 0);

   new G4PVPlacement(0, positionAluStripFront, logicAluStrip, Name + "_AluStripFront", logicW1, false, 0);
   new G4PVPlacement(0, positionAluStripBack,  logicAluStrip, Name + "_AluStripBack",  logicW1, false, 0);

   logicAluStrip->SetVisAttributes(G4VisAttributes::Invisible);

   // Silicon detector itself
   G4ThreeVector  positionSilicon = G4ThreeVector(0, 0, Silicon_PosZ);

   G4Box*           solidSilicon = new G4Box("solidSilicon", 0.5*SiliconFace, 0.5*SiliconFace, 0.5*SiliconThickness);
   G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon, m_MaterialSilicon, "logicSilicon", 0, 0, 0);

   new G4PVPlacement(0, positionSilicon, logicSilicon, Name + "_Silicon", logicW1, false, 0);

   // Set Silicon strip sensible
   logicSilicon->SetSensitiveDetector(m_Scorer);

   ///Visualisation of Silicon Strip
   G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
   logicSilicon->SetVisAttributes(SiliconVisAtt);
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void W1::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("W1");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 
  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    // Cartesian Case
    vector<string> cart = {"X1_Y1","X1_Y16","X16_Y1","X16_Y16","VIS"};
    // Spherical Case
    vector<string> sphe= {"R","THETA","PHI","BETA","VIS"};

    if(blocks[i]->HasTokenList(cart)){
      cout << endl << "////  W1 " << i+1 << endl;
      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y1","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("X16_Y1","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y16","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("X16_Y16","mm"));
      if(blocks[i]->GetInt("VIS"))
        m_non_sensitive_part_visiualisation = true;
      AddDetector(A,B,C,D) ;
    }

    else if(blocks[i]->HasTokenList(sphe)){
      cout << endl << "////  W1 " << i+1 << endl;
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi= blocks[i]->GetDouble("PHI","deg");
      double R = blocks[i]->GetDouble("R","mm");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      if(blocks[i]->GetInt("VIS"))
        m_non_sensitive_part_visiualisation = true;

      AddDetector(Theta,Phi,R,beta[0],beta[1],beta[2]);

    }

    else{
      cout << "ERROR: Missing token for W1 blocks, check your input file" << endl;
      exit(1);
    }
  }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void W1::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* W1rot    = NULL;
   G4ThreeVector     W1pos    = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     W1u      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     W1v      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     W1w      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     W1Center = G4ThreeVector(0, 0, 0) ;
   
   G4int NumberOfDetector = m_DefinitionType.size() ;
   for (G4int i = 0; i < NumberOfDetector; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         W1u = m_X16_Y1[i] - m_X1_Y1[i];
         W1u = W1u.unit();

         W1v = m_X1_Y16[i] - m_X1_Y1[i];
         W1v = W1v.unit();

         W1w = W1u.cross(W1v);
         W1w = W1w.unit();

         W1Center = (m_X1_Y1[i] + m_X1_Y16[i] + m_X16_Y1[i] + m_X16_Y16[i]) / 4;

         // Passage Matrix from Lab Referential to Telescope Referential
         W1rot = new G4RotationMatrix(W1u, W1v, W1w);
         // translation to place Telescope
         W1pos = W1w * Length * 0.5 + W1Center;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i];
         G4double Phi   = m_Phi[i];

         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad);
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad);
         G4double wZ = m_R[i] * cos(Theta / rad);
         W1w = G4ThreeVector(wX, wY, wZ);

         // vector corresponding to the center of the module
         W1Center = W1w;

         // vector parallel to one axis of silicon plane
         G4double ii = cos(Theta / rad) * cos(Phi / rad);
         G4double jj = cos(Theta / rad) * sin(Phi / rad);
         G4double kk = -sin(Theta / rad);
         G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

         W1w = W1w.unit();
         W1u = W1w.cross(Y);
         W1v = W1w.cross(W1u);
         W1v = W1v.unit();
         W1u = W1u.unit();

         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         W1rot = new G4RotationMatrix(W1u, W1v, W1w);
         // Telescope is rotate of Beta angle around W1v axis.
         W1rot->rotate(m_beta_u[i], W1u);
         W1rot->rotate(m_beta_v[i], W1v);
         W1rot->rotate(m_beta_w[i], W1w);
         // translation to place Telescope
         W1pos = W1w * Length * 0.5 + W1Center;
      }

      VolumeMaker(i + 1, W1pos, W1rot, world);
   }

   delete W1rot;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void W1::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
    if(!pTree->FindBranch("W1")){
    pTree->Branch("W1", "TW1Data", &m_Event);
   }
   pTree->SetBranchAddress("W1", &m_Event);
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void W1::ReadSensitive(const G4Event* event)
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
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/DetectorNumber");
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID));
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin();

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/FrontStripNumber");
   XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID));
   X_itr = XHitMap->GetMap()->begin();

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/BackStripNumber");
   YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ScorerW1/InterCoordAngPhi");
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
      << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << G4endl ;
      return;
   }

   // Loop on W1 number 
   for (G4int l = 0; l < sizeN; l++) {
      G4double N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      if (N > 0) {
         // Fill detector number
         m_Event->SetFrontEDetectorNbr(N);
         m_Event->SetFrontTDetectorNbr(N);
         m_Event->SetBackEDetectorNbr(N);
         m_Event->SetBackTDetectorNbr(N);

         //  Energy
         Energy_itr = EnergyHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeE ; h++) {
            G4int ETrackID =   Energy_itr->first - N;
            G4double E     = *(Energy_itr->second);
            if (ETrackID == NTrackID) {
               m_Event->SetFrontEEnergy(RandGauss::shoot(E, EnergyResolution));
               m_Event->SetBackEEnergy(RandGauss::shoot(E, EnergyResolution));
            }
            Energy_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeT ; h++) {
            G4int TTrackID =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               m_Event->SetFrontTTime(RandGauss::shoot(T, TimeResolution));
               m_Event->SetBackTTime(RandGauss::shoot(T, TimeResolution));
            }
            Time_itr++;
         }

         // strip front 
         X_itr = XHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int XTrackID  =   X_itr->first - N;
            G4double X     = *(X_itr->second);
            if (XTrackID == NTrackID) {
               m_Event->SetFrontEStripNbr(X)   ;
               m_Event->SetFrontTStripNbr(X)   ;
            }
            X_itr++;
         }

         // strip back
         Y_itr = YHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeY ; h++) {
            G4int YTrackID  =   Y_itr->first - N;
            G4double Y     = *(Y_itr->second);
            if (YTrackID == NTrackID) {
               m_Event->SetBackEStripNbr(Y);
               m_Event->SetBackTStripNbr(Y);
            }
            Y_itr++;
         }

         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0; h < PosXHitMap->entries(); h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N;
            G4double PosX     = *(Pos_X_itr->second);
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX);
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0; h < PosYHitMap->entries(); h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N;
            G4double PosY     = *(Pos_Y_itr->second);
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY);
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0; h < PosZHitMap->entries(); h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N;
            G4double PosZ     = *(Pos_Z_itr->second);
            if (PosZTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionZ(PosZ);
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0; h < AngThetaHitMap->entries(); h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N;
            G4double AngTheta     = *(Ang_Theta_itr->second);
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta);
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0; h < AngPhiHitMap->entries(); h++) {
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



void W1::InitializeMaterials(){           
   m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
   m_MaterialAluminium =MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
   m_MaterialIron = MaterialManager::getInstance()->GetMaterialFromLibrary("Fe");
   m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}



void W1::InitializeScorers(){
  bool already_exist = false;
   // Associate Scorer
   m_Scorer = CheckScorer("ScorerW1",already_exist);
   if(already_exist) return;

   G4VPrimitiveScorer* DetNbr                           = new OBSOLETEGENERALSCORERS::PSDetectorNumber("DetectorNumber", "W1Square", 0);
   G4VPrimitiveScorer* Energy                           = new OBSOLETEGENERALSCORERS::PSEnergy("StripEnergy", "W1Square", 0);
   G4VPrimitiveScorer* TOF                              = new OBSOLETEGENERALSCORERS::PSTOF("StripTime", "W1Square", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","W1Square", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","W1Square", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","W1Square", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","W1Square", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","W1Square", 0);
   G4VPrimitiveScorer* ThetaStripPosition               = new W1ScorerFrontStripNumber("FrontStripNumber", 0, NbStrips);
   G4VPrimitiveScorer* PhiStripPosition                 = new W1ScorerBackStripNumber("BackStripNumber", 0, NbStrips);

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
 ////////////////////////////////////////////////////////////////////////////////
 //            Construct Method to be pass to the DetectorFactory              //
 ////////////////////////////////////////////////////////////////////////////////
 NPS::VDetector* W1::Construct(){
  return  (NPS::VDetector*) new W1();
 }

 ////////////////////////////////////////////////////////////////////////////////
 //            Registering the construct method to the factory                 //
 ////////////////////////////////////////////////////////////////////////////////
 extern"C" {
 class proxy_nps_w1{
   public:
    proxy_nps_w1(){
      NPS::DetectorFactory::getInstance()->AddToken("W1","W1");
      NPS::DetectorFactory::getInstance()->AddDetector("W1",W1::Construct);
    }
};

 proxy_nps_w1 p_nps_w1;
 }
