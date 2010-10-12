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
 *             GaspardTracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "GaspardScorers.hh"

#include "GaspardTrackerDummyShape.hh"
#include "GaspardTrackerSquare.hh"
#include "GaspardTrackerTrapezoid.hh"
#include "GaspardTrackerAnnular.hh"
using namespace GPDSQUARE;
using namespace GPDTRAP;
using namespace GPDANNULAR;
using namespace GPDDUMMYSHAPE;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Energy Scorer (deal with multiple particle hit)
GPDScorerFirstStageEnergy::GPDScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

GPDScorerFirstStageEnergy::~GPDScorerFirstStageEnergy()
{
}

G4bool GPDScorerFirstStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void GPDScorerFirstStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageEnergy::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageEnergy::DrawAll()
{
}

void GPDScorerFirstStageEnergy::PrintAll()
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
GPDScorerSecondStageEnergy::GPDScorerSecondStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

GPDScorerSecondStageEnergy::~GPDScorerSecondStageEnergy()
{
}

G4bool GPDScorerSecondStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void GPDScorerSecondStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerSecondStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerSecondStageEnergy::Clear()
{
   EvtMap->clear();
}

void GPDScorerSecondStageEnergy::DrawAll()
{
}

void GPDScorerSecondStageEnergy::PrintAll()
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
GPDScorerThirdStageEnergy::GPDScorerThirdStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

GPDScorerThirdStageEnergy::~GPDScorerThirdStageEnergy()
{
}

G4bool GPDScorerThirdStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void GPDScorerThirdStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerThirdStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerThirdStageEnergy::Clear()
{
   EvtMap->clear();
}

void GPDScorerThirdStageEnergy::DrawAll()
{
}

void GPDScorerThirdStageEnergy::PrintAll()
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
// FirstStage Front Strip position Scorer for DummyShape geometry
GPDScorerFirstStageFrontStripDummyShape::GPDScorerFirstStageFrontStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageFrontStripDummyShape::~GPDScorerFirstStageFrontStripDummyShape()
{
}

G4bool GPDScorerFirstStageFrontStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDDummyShape");

   // get front strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDDUMMYSHAPE::FirstStageFace / m_NumberOfStrip;

   G4double temp = (POS(0) + GPDDUMMYSHAPE::FirstStageFace / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;

   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void GPDScorerFirstStageFrontStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageFrontStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageFrontStripDummyShape::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageFrontStripDummyShape::DrawAll()
{
}

void GPDScorerFirstStageFrontStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for DummyShape geometry
GPDScorerFirstStageBackStripDummyShape::GPDScorerFirstStageBackStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageBackStripDummyShape::~GPDScorerFirstStageBackStripDummyShape()
{
}

G4bool GPDScorerFirstStageBackStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDDummyShape");

   // get back strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDDUMMYSHAPE::FirstStageFace / m_NumberOfStrip;

   G4double temp = (POS(1) + GPDDUMMYSHAPE::FirstStageFace / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void GPDScorerFirstStageBackStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageBackStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageBackStripDummyShape::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageBackStripDummyShape::DrawAll()
{
}

void GPDScorerFirstStageBackStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Square geometry
GPDScorerFirstStageFrontStripSquare::GPDScorerFirstStageFrontStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageFrontStripSquare::~GPDScorerFirstStageFrontStripSquare()
{
}

G4bool GPDScorerFirstStageFrontStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDSquare");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(0) + GPDSQUARE::SiliconFace / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == 129) X = 128;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void GPDScorerFirstStageFrontStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageFrontStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageFrontStripSquare::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageFrontStripSquare::DrawAll()
{
}

void GPDScorerFirstStageFrontStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Square geometry
GPDScorerFirstStageBackStripSquare::GPDScorerFirstStageBackStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageBackStripSquare::~GPDScorerFirstStageBackStripSquare()
{
}

G4bool GPDScorerFirstStageBackStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDSquare");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(1) + GPDSQUARE::SiliconFace / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == 129) Y = 128;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void GPDScorerFirstStageBackStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageBackStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageBackStripSquare::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageBackStripSquare::DrawAll()
{
}

void GPDScorerFirstStageBackStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Trapezoid geometry
GPDScorerFirstStageFrontStripTrapezoid::GPDScorerFirstStageFrontStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

GPDScorerFirstStageFrontStripTrapezoid::~GPDScorerFirstStageFrontStripTrapezoid()
{
}

G4bool GPDScorerFirstStageFrontStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDTrapezoid");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDTRAP::FirstStageBaseLarge / m_NumberOfStrip;

   G4double temp = (POS(0) + GPDTRAP::FirstStageBaseLarge / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == 129) X = 128;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void GPDScorerFirstStageFrontStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageFrontStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageFrontStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageFrontStripTrapezoid::DrawAll()
{
}

void GPDScorerFirstStageFrontStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Trapezoid geometry
GPDScorerFirstStageBackStripTrapezoid::GPDScorerFirstStageBackStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

GPDScorerFirstStageBackStripTrapezoid::~GPDScorerFirstStageBackStripTrapezoid()
{
}

G4bool GPDScorerFirstStageBackStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDTrapezoid");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = GPDTRAP::FirstStageHeight / m_NumberOfStrip;

   G4double temp = (POS(1) + GPDTRAP::FirstStageHeight / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == 129) Y = 128;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void GPDScorerFirstStageBackStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageBackStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageBackStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageBackStripTrapezoid::DrawAll()
{
}

void GPDScorerFirstStageBackStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Annular geometry
GPDScorerFirstStageFrontStripAnnular::GPDScorerFirstStageFrontStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageFrontStripAnnular::~GPDScorerFirstStageFrontStripAnnular()
{
}

G4bool GPDScorerFirstStageFrontStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDAnnular");

   // Hit position in the world frame
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   G4cout << "world frame hit position" << G4endl;
   G4cout << POS.x() << "   " << POS.y() << "   " << POS.z() << G4endl;

   // Hit position in the detector frame
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   // Radial position in the Oxy plane
   G4double r = sqrt(pow(POS(0), 2) + pow(POS(1), 2));

   // Phi angle
   G4double phi = atan2(POS(1), POS(0)) / deg;
   if (phi < 0) phi += 360;

   // Phi quadrant
   G4double PhiWidth = 360. / GPDANNULAR::NbThetaQuadrant;
   G4double PhiQuadrantNumber = floor(phi / PhiWidth);

   // Theta strip pitch
   // Interstrip should be taken into account here. To be done
   G4double ThetaStripPitch = (GPDANNULAR::FirstStageRmax - GPDANNULAR::FirstStageRmin) / GPDANNULAR::NbThetaStrips;
   G4double dummy = (r - GPDANNULAR::FirstStageRmin);
   if (dummy < 0 && fabs(dummy) < 1e-6) dummy *= -1;
   G4double ThetaStripNumber = floor(dummy / ThetaStripPitch);
   ThetaStripNumber += PhiQuadrantNumber * GPDANNULAR::NbThetaStrips;

      G4cout << "POS: " << POS << G4endl;
      G4cout << "r, phi " << r << "  " << phi << G4endl;
      G4cout << "PhiWidth, PhiQuadrantNumber " << PhiWidth << "  " << PhiQuadrantNumber << G4endl;
      G4cout << "ThetaStripPitch, ThetaStripNumber, dummy " << ThetaStripPitch << "  " << ThetaStripNumber << "  " << dummy << G4endl;

   if (ThetaStripNumber < 1e-6) {
    /*  G4cout << "POS: " << POS << G4endl;
      G4cout << "r, phi " << r << "  " << phi << G4endl;
      G4cout << "PhiWidth, PhiQuadrantNumber " << PhiWidth << "  " << PhiQuadrantNumber << G4endl;
      G4cout << "ThetaStripPitch, ThetaStripNumber, dummy " << ThetaStripPitch << "  " << ThetaStripNumber << "  " << dummy << G4endl;*/
   }

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, ThetaStripNumber);

   return TRUE;
}

void GPDScorerFirstStageFrontStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageFrontStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageFrontStripAnnular::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageFrontStripAnnular::DrawAll()
{
}

void GPDScorerFirstStageFrontStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Annular geometry
GPDScorerFirstStageBackStripAnnular::GPDScorerFirstStageBackStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

GPDScorerFirstStageBackStripAnnular::~GPDScorerFirstStageBackStripAnnular()
{
}

G4bool GPDScorerFirstStageBackStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "GPDAnnular");

   // Hit position in the world frame
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();

   // Hit position in the detector frame
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   // Phi angle
   G4double phi = atan2(POS(1), POS(0)) / deg;
   if (phi < 0) phi += 360;

   // Phi strip number
   // Interstrip should be taken into account here. To be done
   G4double PhiWidth = 360. / (GPDANNULAR::NbPhiStrips * GPDANNULAR::NbThetaQuadrant);
   G4double PhiStripNumber = floor(phi / PhiWidth);

//   G4cout << POS << G4endl;
//   G4cout << "phi " << phi << " PhiWidth  " << PhiWidth << "  PhiStripNumber " << PhiStripNumber << G4endl;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, PhiStripNumber);

   return TRUE;
}

void GPDScorerFirstStageBackStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void GPDScorerFirstStageBackStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void GPDScorerFirstStageBackStripAnnular::Clear()
{
   EvtMap->clear();
}

void GPDScorerFirstStageBackStripAnnular::DrawAll()
{
}

void GPDScorerFirstStageBackStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
