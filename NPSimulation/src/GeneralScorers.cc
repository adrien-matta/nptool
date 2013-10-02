/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold some of the General Scorer, shared by different detector.*
 *  Those scorer could be a could basis for your own scorer                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *                                            *
 *****************************************************************************/
#include "GeneralScorers.hh"
#include "G4UnitsTable.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Energy Scorer (deal with multiple particle hit)

PSEnergy::PSEnergy(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSEnergy::~PSEnergy()
{
}

G4bool PSEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   
   G4int  index = aStep->GetTrack()->GetTrackID();
   
   EvtMap->add(index, edep);
   return TRUE;
}

void PSEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSEnergy::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSEnergy::clear()
{
   EvtMap->clear();
}

void PSEnergy::DrawAll()
{
   ;
}

void PSEnergy::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << "  energy deposit: " << G4BestUnit(*(itr->second), "Energy")
      << G4endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//TOF Scorer//
PSTOF::PSTOF(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   ;
}

PSTOF::~PSTOF()
{
   ;
}

G4bool PSTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, TOF);
   return TRUE;
}

void PSTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
         GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSTOF::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSTOF::clear()
{
   EvtMap->clear();
}

void PSTOF::DrawAll()
{
   ;
}

void PSTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Interaction coordinates X
PSInteractionCoordinatesX::PSInteractionCoordinatesX(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSInteractionCoordinatesX::~PSInteractionCoordinatesX()
{
}

G4bool PSInteractionCoordinatesX::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4double pos_x = POS.x() / mm;

   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, pos_x);
   return TRUE;
}

void PSInteractionCoordinatesX::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSInteractionCoordinatesX::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSInteractionCoordinatesX::clear()
{
   EvtMap->clear();
}

void PSInteractionCoordinatesX::DrawAll()
{
   ;
}

void PSInteractionCoordinatesX::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Interaction coordinates Y
PSInteractionCoordinatesY::PSInteractionCoordinatesY(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSInteractionCoordinatesY::~PSInteractionCoordinatesY()
{
}

G4bool PSInteractionCoordinatesY::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4double pos_y = POS.y() / mm;

   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, pos_y);
   return TRUE;
}

void PSInteractionCoordinatesY::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSInteractionCoordinatesY::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSInteractionCoordinatesY::clear()
{
   EvtMap->clear();
}

void PSInteractionCoordinatesY::DrawAll()
{
   ;
}

void PSInteractionCoordinatesY::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Interaction coordinates Z
PSInteractionCoordinatesZ::PSInteractionCoordinatesZ(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSInteractionCoordinatesZ::~PSInteractionCoordinatesZ()
{
}

G4bool PSInteractionCoordinatesZ::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4double pos_z = POS.z() / mm;

   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, pos_z);
   return TRUE;
}

void PSInteractionCoordinatesZ::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSInteractionCoordinatesZ::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSInteractionCoordinatesZ::clear()
{
   EvtMap->clear();
}

void PSInteractionCoordinatesZ::DrawAll()
{
   ;
}

void PSInteractionCoordinatesZ::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Interaction coordinates Angle Theta
PSInteractionCoordinatesAngleTheta::PSInteractionCoordinatesAngleTheta(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSInteractionCoordinatesAngleTheta::~PSInteractionCoordinatesAngleTheta()
{
}

G4bool PSInteractionCoordinatesAngleTheta::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4double angle_theta = POS.theta() / deg;

   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, angle_theta);
   return TRUE;
}

void PSInteractionCoordinatesAngleTheta::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSInteractionCoordinatesAngleTheta::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSInteractionCoordinatesAngleTheta::clear()
{
   EvtMap->clear();
}

void PSInteractionCoordinatesAngleTheta::DrawAll()
{
   ;
}

void PSInteractionCoordinatesAngleTheta::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Interaction coordinates Angle Phi
PSInteractionCoordinatesAnglePhi::PSInteractionCoordinatesAnglePhi(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSInteractionCoordinatesAnglePhi::~PSInteractionCoordinatesAnglePhi()
{
}

G4bool PSInteractionCoordinatesAnglePhi::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4double angle_phi = POS.phi() / deg;
   if (angle_phi < 0) angle_phi += 360;

   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, angle_phi);
   return TRUE;
}

void PSInteractionCoordinatesAnglePhi::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSInteractionCoordinatesAnglePhi::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSInteractionCoordinatesAnglePhi::clear()
{
   EvtMap->clear();
}

void PSInteractionCoordinatesAnglePhi::DrawAll()
{
   ;
}

void PSInteractionCoordinatesAnglePhi::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
		//Detector Number Scorer
PSDetectorNumber::PSDetectorNumber(G4String name, G4int depth, G4String VolumeName )
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = VolumeName;
}

PSDetectorNumber::~PSDetectorNumber()
{
   ;
}

G4bool PSDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   std::string name = aStep->GetTrack()->GetVolume()->GetName();
   std::string nbr ;
   size_t found;
   found=name.find(m_VolumeName);
   found = found + m_VolumeName.length();
   
   int numberOfCharacterInDetectorNumber = name.length() - (int)found  ;

	for( int i = found ; i < found + numberOfCharacterInDetectorNumber ; i++ )
			nbr += name[i] ; 
		
   G4int DetNbr = atoi( nbr.c_str() ) ;
   G4double edep = aStep->GetTotalEnergyDeposit();
   
   if (edep < 100*keV) return FALSE;
   
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(index, DetNbr);
   return TRUE;
}

void PSDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{ 
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSDetectorNumber::clear()
{
   EvtMap->clear();
}

void PSDetectorNumber::DrawAll()
{
   ;
}

void PSDetectorNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << "  energy deposit: " << G4BestUnit(*(itr->second), "Energy")
      << G4endl;
   }
}
