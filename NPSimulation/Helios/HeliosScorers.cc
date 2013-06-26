/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Helios*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "HeliosScorers.hh"

#include "HeliosDetDummyShape.hh"
//#include "HeliosDetSquare.hh"
//#include "HeliosDetTrapezoid.hh"
//#include "HeliosDetAnnular.hh"
//using namespace HELIOSSQUARE;
//using namespace HELIOSTRAP;
//using namespace HELIOSANNULAR;
using namespace HELIOSDUMMYSHAPE;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Energy Scorer (deal with multiple particle hit)
HeliosScorerFirstStageEnergy::HeliosScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

HeliosScorerFirstStageEnergy::~HeliosScorerFirstStageEnergy()
{
}

G4bool HeliosScorerFirstStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void HeliosScorerFirstStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageEnergy::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageEnergy::DrawAll()
{
}

void HeliosScorerFirstStageEnergy::PrintAll()
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
HeliosScorerFirstStageFrontStripDummyShape::HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageFrontStripDummyShape::~HeliosScorerFirstStageFrontStripDummyShape()
{
}

G4bool HeliosScorerFirstStageFrontStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosDummyShape");

   // get front strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HELIOSDUMMYSHAPE::FirstStageFaceWidth / m_NumberOfStrip;  // longitudinal strip

   G4double temp = (POS(0) + HELIOSDUMMYSHAPE::FirstStageFaceWidth / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;

   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HeliosScorerFirstStageFrontStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageFrontStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageFrontStripDummyShape::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageFrontStripDummyShape::DrawAll()
{
}

void HeliosScorerFirstStageFrontStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for DummyShape geometry
HeliosScorerFirstStageBackStripDummyShape::HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageBackStripDummyShape::~HeliosScorerFirstStageBackStripDummyShape()
{
}

G4bool HeliosScorerFirstStageBackStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosDummyShape");

   // get back strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HELIOSDUMMYSHAPE::FirstStageFaceLength / m_NumberOfStrip;    // transversal strip

   G4double temp = (POS(1) + HELIOSDUMMYSHAPE::FirstStageFaceLength / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HeliosScorerFirstStageBackStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageBackStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageBackStripDummyShape::Clear()   

{
   EvtMap->clear();
}

void HeliosScorerFirstStageBackStripDummyShape::DrawAll()
{
}

void HeliosScorerFirstStageBackStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Square geometry
HeliosScorerFirstStageFrontStripSquare::HeliosScorerFirstStageFrontStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageFrontStripSquare::~HeliosScorerFirstStageFrontStripSquare()
{
}

G4bool HeliosScorerFirstStageFrontStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosSquare");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HeliosSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(0) + HeliosSQUARE::SiliconFace / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == 129) X = 128;
   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HeliosScorerFirstStageFrontStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageFrontStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageFrontStripSquare::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageFrontStripSquare::DrawAll()
{
}

void HeliosScorerFirstStageFrontStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Square geometry
HeliosScorerFirstStageBackStripSquare::HeliosScorerFirstStageBackStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageBackStripSquare::~HeliosScorerFirstStageBackStripSquare()
{
}

G4bool HeliosScorerFirstStageBackStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosSquare");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HeliosSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(1) + HeliosSQUARE::SiliconFace / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == 129) Y = 128;

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HeliosScorerFirstStageBackStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageBackStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageBackStripSquare::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageBackStripSquare::DrawAll()
{
}

void HeliosScorerFirstStageBackStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Trapezoid geometry
HeliosScorerFirstStageFrontStripTrapezoid::HeliosScorerFirstStageFrontStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HeliosScorerFirstStageFrontStripTrapezoid::~HeliosScorerFirstStageFrontStripTrapezoid()
{
}

G4bool HeliosScorerFirstStageFrontStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosTrapezoid");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HeliosTRAP::FirstStageBaseLarge / m_NumberOfStrip;

   G4double temp = (POS(0) + HeliosTRAP::FirstStageBaseLarge / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == 129) X = 128;
   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HeliosScorerFirstStageFrontStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageFrontStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageFrontStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageFrontStripTrapezoid::DrawAll()
{
}

void HeliosScorerFirstStageFrontStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Trapezoid geometry
HeliosScorerFirstStageBackStripTrapezoid::HeliosScorerFirstStageBackStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

HeliosScorerFirstStageBackStripTrapezoid::~HeliosScorerFirstStageBackStripTrapezoid()
{
}

G4bool HeliosScorerFirstStageBackStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosTrapezoid");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = HeliosTRAP::FirstStageHeight / m_NumberOfStrip;

   G4double temp = (POS(1) + HeliosTRAP::FirstStageHeight / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == 129) Y = 128;

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HeliosScorerFirstStageBackStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageBackStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageBackStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageBackStripTrapezoid::DrawAll()
{
}

void HeliosScorerFirstStageBackStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Front Strip position Scorer for Annular geometry
HeliosScorerFirstStageFrontStripAnnular::HeliosScorerFirstStageFrontStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageFrontStripAnnular::~HeliosScorerFirstStageFrontStripAnnular()
{
}

G4bool HeliosScorerFirstStageFrontStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosAnnular");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = m_StripPlaneSize / m_NumberOfStrip;

   G4double temp = (POS(0) + m_StripPlaneSize / 2.) / StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == 129) X = 128;
   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, X);
   return TRUE;
}

void HeliosScorerFirstStageFrontStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageFrontStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageFrontStripAnnular::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageFrontStripAnnular::DrawAll()
{
}

void HeliosScorerFirstStageFrontStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Back Strip position Scorer for Annular geometry
HeliosScorerFirstStageBackStripAnnular::HeliosScorerFirstStageBackStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

HeliosScorerFirstStageBackStripAnnular::~HeliosScorerFirstStageBackStripAnnular()
{
}

G4bool HeliosScorerFirstStageBackStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "HeliosAnnular");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = m_StripPlaneSize / m_NumberOfStrip;

   G4double temp = (POS(1) + m_StripPlaneSize / 2.) / StripPitch   ;
   G4int temp2 = temp ;
   G4double Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == 129) Y = 128;

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;
   if (edep < 100*eV) return FALSE;  // modified by mMarc
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, Y);
   return TRUE;
}

void HeliosScorerFirstStageBackStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void HeliosScorerFirstStageBackStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void HeliosScorerFirstStageBackStripAnnular::Clear()
{
   EvtMap->clear();
}

void HeliosScorerFirstStageBackStripAnnular::DrawAll()
{
}

void HelioScorerFirstStageBackStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
*/
