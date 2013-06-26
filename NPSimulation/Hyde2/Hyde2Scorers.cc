/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 11/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Hyde2Tracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "Hyde2Scorers.hh"

#include "Hyde2TrackerSquare1.hh"
#include "Hyde2TrackerTrapezoid1.hh"
#include "Hyde2TrackerTrapezoid2.hh"
using namespace HYD2SQUARE1;
using namespace HYD2TRAP1;
using namespace HYD2TRAP2;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerFirstStageEnergy::HYD2ScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerFirstStageEnergy::~HYD2ScorerFirstStageEnergy()
{
}

G4bool HYD2ScorerFirstStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerFirstStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageEnergy::DrawAll()
{
}

void HYD2ScorerFirstStageEnergy::PrintAll()
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
// SecondStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerSecondStageEnergy::HYD2ScorerSecondStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerSecondStageEnergy::~HYD2ScorerSecondStageEnergy()
{
}

G4bool HYD2ScorerSecondStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerSecondStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerSecondStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerSecondStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerSecondStageEnergy::DrawAll()
{
}

void HYD2ScorerSecondStageEnergy::PrintAll()
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
// ThirdStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerThirdStageEnergy::HYD2ScorerThirdStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerThirdStageEnergy::~HYD2ScorerThirdStageEnergy()
{
}

G4bool HYD2ScorerThirdStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerThirdStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerThirdStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerThirdStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerThirdStageEnergy::DrawAll()
{
}

void HYD2ScorerThirdStageEnergy::PrintAll()
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
// FourthStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerFourthStageEnergy::HYD2ScorerFourthStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerFourthStageEnergy::~HYD2ScorerFourthStageEnergy()
{
}

G4bool HYD2ScorerFourthStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerFourthStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFourthStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFourthStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFourthStageEnergy::DrawAll()
{
}

void HYD2ScorerFourthStageEnergy::PrintAll()
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
// FifthStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerFifthStageEnergy::HYD2ScorerFifthStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerFifthStageEnergy::~HYD2ScorerFifthStageEnergy()
{
}

G4bool HYD2ScorerFifthStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerFifthStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFifthStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFifthStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFifthStageEnergy::DrawAll()
{
}

void HYD2ScorerFifthStageEnergy::PrintAll()
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
// SixthStage Energy Scorer (deal with multiple particle hit)
HYD2ScorerSixthStageEnergy::HYD2ScorerSixthStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HYD2ScorerSixthStageEnergy::~HYD2ScorerSixthStageEnergy()
{
}

G4bool HYD2ScorerSixthStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HYD2ScorerSixthStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerSixthStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerSixthStageEnergy::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerSixthStageEnergy::DrawAll()
{
}

void HYD2ScorerSixthStageEnergy::PrintAll()
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
// FirstStage Front Strip position Scorer for Square1 geometry
HYD2ScorerFirstStageFrontStripSquare1::HYD2ScorerFirstStageFrontStripSquare1(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HYD2ScorerFirstStageFrontStripSquare1::~HYD2ScorerFirstStageFrontStripSquare1()
{
}

G4bool HYD2ScorerFirstStageFrontStripSquare1::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Square1");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2SQUARE1::FirstStageFace / m_NumberOfStrip;

   G4double temp = (POS(0) + HYD2SQUARE1::FirstStageFace / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   // if (X == 129) X = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (X == 221) X = 220;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HYD2ScorerFirstStageFrontStripSquare1::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageFrontStripSquare1::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageFrontStripSquare1::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageFrontStripSquare1::DrawAll()
{
}

void HYD2ScorerFirstStageFrontStripSquare1::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Square1 geometry
HYD2ScorerFirstStageBackStripSquare1::HYD2ScorerFirstStageBackStripSquare1(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HYD2ScorerFirstStageBackStripSquare1::~HYD2ScorerFirstStageBackStripSquare1()
{
}

G4bool HYD2ScorerFirstStageBackStripSquare1::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Square1");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2SQUARE1::FirstStageFace / m_NumberOfStrip;

   G4double temp = (POS(1) + HYD2SQUARE1::FirstStageFace / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   // if (Y == 129) Y = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (Y == 221) Y = 220;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HYD2ScorerFirstStageBackStripSquare1::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageBackStripSquare1::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageBackStripSquare1::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageBackStripSquare1::DrawAll()
{
}

void HYD2ScorerFirstStageBackStripSquare1::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Trapezoid1 geometry
HYD2ScorerFirstStageFrontStripTrapezoid1::HYD2ScorerFirstStageFrontStripTrapezoid1(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HYD2ScorerFirstStageFrontStripTrapezoid1::~HYD2ScorerFirstStageFrontStripTrapezoid1()
{
}

G4bool HYD2ScorerFirstStageFrontStripTrapezoid1::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Trapezoid1");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2TRAP1::FirstStageBaseLarge / m_NumberOfStrip;

   G4double temp = (POS(0) + HYD2TRAP1::FirstStageBaseLarge / 2.) / StripPitch   ;
   G4double X = int(temp) + 1;
   //Rare case where particle is close to edge of silicon plan
   // if (X == 129) X = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (X == 215) X = 214;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HYD2ScorerFirstStageFrontStripTrapezoid1::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageFrontStripTrapezoid1::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageFrontStripTrapezoid1::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageFrontStripTrapezoid1::DrawAll()
{
}

void HYD2ScorerFirstStageFrontStripTrapezoid1::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Trapezoid1 geometry
HYD2ScorerFirstStageBackStripTrapezoid1::HYD2ScorerFirstStageBackStripTrapezoid1(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HYD2ScorerFirstStageBackStripTrapezoid1::~HYD2ScorerFirstStageBackStripTrapezoid1()
{
}

G4bool HYD2ScorerFirstStageBackStripTrapezoid1::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Trapezoid1");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2TRAP1::FirstStageHeight / m_NumberOfStrip;

   G4double temp = (POS(1) + HYD2TRAP1::FirstStageHeight / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   // if (Y == 129) Y = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (Y == 221) Y = 220;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HYD2ScorerFirstStageBackStripTrapezoid1::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageBackStripTrapezoid1::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageBackStripTrapezoid1::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageBackStripTrapezoid1::DrawAll()
{
}

void HYD2ScorerFirstStageBackStripTrapezoid1::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Trapezoid2 geometry
HYD2ScorerFirstStageFrontStripTrapezoid2::HYD2ScorerFirstStageFrontStripTrapezoid2(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HYD2ScorerFirstStageFrontStripTrapezoid2::~HYD2ScorerFirstStageFrontStripTrapezoid2()
{
}

G4bool HYD2ScorerFirstStageFrontStripTrapezoid2::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Trapezoid2");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2TRAP2::FirstStageBaseLarge / m_NumberOfStrip;

   G4double temp = (POS(0) + HYD2TRAP2::FirstStageBaseLarge / 2.) / StripPitch   ;
   G4double X = int(temp) + 1;
   //Rare case where particle is close to edge of silicon plan
   // if (X == 129) X = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (X == 130) X = 129;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HYD2ScorerFirstStageFrontStripTrapezoid2::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageFrontStripTrapezoid2::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageFrontStripTrapezoid2::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageFrontStripTrapezoid2::DrawAll()
{
}

void HYD2ScorerFirstStageFrontStripTrapezoid2::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Trapezoid2 geometry
HYD2ScorerFirstStageBackStripTrapezoid2::HYD2ScorerFirstStageBackStripTrapezoid2(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HYD2ScorerFirstStageBackStripTrapezoid2::~HYD2ScorerFirstStageBackStripTrapezoid2()
{
}

G4bool HYD2ScorerFirstStageBackStripTrapezoid2::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HYD2Trapezoid2");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HYD2TRAP2::FirstStageHeight / m_NumberOfStrip;

   G4double temp = (POS(1) + HYD2TRAP2::FirstStageHeight / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   // if (Y == 129) Y = 128;  Wed Feb  6 19:16:08 CET 2013 angelmsb
   if (Y == 127) Y = 126;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HYD2ScorerFirstStageBackStripTrapezoid2::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HYD2ScorerFirstStageBackStripTrapezoid2::EndOfEvent(G4HCofThisEvent*)
{
}

void HYD2ScorerFirstStageBackStripTrapezoid2::Clear()
{
   EvtMap->clear();
}

void HYD2ScorerFirstStageBackStripTrapezoid2::DrawAll()
{
}

void HYD2ScorerFirstStageBackStripTrapezoid2::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

