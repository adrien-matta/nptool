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
 *             ANU*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "ANUScorers.hh"

#include "ANUDetDummyShape.hh"
//#include "ANUDetSquare.hh"
//#include "ANUDetTrapezoid.hh"
//#include "ANUDetAnnular.hh"
//using namespace ANUSQUARE;
//using namespace ANUTRAP;
//using namespace ANUANNULAR;
using namespace ANUDUMMYSHAPE;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Energy Scorer (deal with multiple particle hit)
ANUScorerSiLiEnergy::ANUScorerSiLiEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ANUScorerSiLiEnergy::~ANUScorerSiLiEnergy()
{
}

G4bool ANUScorerSiLiEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void ANUScorerSiLiEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiEnergy::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiEnergy::DrawAll()
{
}

void ANUScorerSiLiEnergy::PrintAll()
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
// SiLi Front Strip position Scorer for DummyShape geometry
ANUScorerSiLiFrontStripDummyShape::ANUScorerSiLiFrontStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiFrontStripDummyShape::~ANUScorerSiLiFrontStripDummyShape()
{
}

G4bool ANUScorerSiLiFrontStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUDummyShape");

   // get front strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUDUMMYSHAPE::SiLiFaceWidth / m_NumberOfStrip;  // longitudinal strip

   G4double temp = (POS(0) + ANUDUMMYSHAPE::SiLiFaceWidth / 2.) / StripPitch   ;
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

void ANUScorerSiLiFrontStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiFrontStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiFrontStripDummyShape::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiFrontStripDummyShape::DrawAll()
{
}

void ANUScorerSiLiFrontStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Back Strip position Scorer for DummyShape geometry
ANUScorerSiLiBackStripDummyShape::ANUScorerSiLiBackStripDummyShape(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiBackStripDummyShape::~ANUScorerSiLiBackStripDummyShape()
{
}

G4bool ANUScorerSiLiBackStripDummyShape::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUDummyShape");

   // get back strip number
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUDUMMYSHAPE::SiLiFaceLength / m_NumberOfStrip;    // transversal strip

   G4double temp = (POS(1) + ANUDUMMYSHAPE::SiLiFaceLength / 2.) / StripPitch   ;
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

void ANUScorerSiLiBackStripDummyShape::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiBackStripDummyShape::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiBackStripDummyShape::Clear()   

{
   EvtMap->clear();
}

void ANUScorerSiLiBackStripDummyShape::DrawAll()
{
}

void ANUScorerSiLiBackStripDummyShape::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


/*
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Front Strip position Scorer for Square geometry
ANUScorerSiLiFrontStripSquare::ANUScorerSiLiFrontStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiFrontStripSquare::~ANUScorerSiLiFrontStripSquare()
{
}

G4bool ANUScorerSiLiFrontStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUSquare");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(0) + ANUSQUARE::SiliconFace / 2.) / StripPitch   ;
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

void ANUScorerSiLiFrontStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiFrontStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiFrontStripSquare::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiFrontStripSquare::DrawAll()
{
}

void ANUScorerSiLiFrontStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Back Strip position Scorer for Square geometry
ANUScorerSiLiBackStripSquare::ANUScorerSiLiBackStripSquare(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiBackStripSquare::~ANUScorerSiLiBackStripSquare()
{
}

G4bool ANUScorerSiLiBackStripSquare::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUSquare");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUSQUARE::SiliconFace / m_NumberOfStrip;

   G4double temp = (POS(1) + ANUSQUARE::SiliconFace / 2.) / StripPitch   ;
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

void ANUScorerSiLiBackStripSquare::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiBackStripSquare::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiBackStripSquare::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiBackStripSquare::DrawAll()
{
}

void ANUScorerSiLiBackStripSquare::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Front Strip position Scorer for Trapezoid geometry
ANUScorerSiLiFrontStripTrapezoid::ANUScorerSiLiFrontStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

ANUScorerSiLiFrontStripTrapezoid::~ANUScorerSiLiFrontStripTrapezoid()
{
}

G4bool ANUScorerSiLiFrontStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUTrapezoid");

   // get front strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUTRAP::SiLiBaseLarge / m_NumberOfStrip;

   G4double temp = (POS(0) + ANUTRAP::SiLiBaseLarge / 2.) / StripPitch   ;
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

void ANUScorerSiLiFrontStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiFrontStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiFrontStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiFrontStripTrapezoid::DrawAll()
{
}

void ANUScorerSiLiFrontStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Back Strip position Scorer for Trapezoid geometry
ANUScorerSiLiBackStripTrapezoid::ANUScorerSiLiBackStripTrapezoid(G4String name, G4int depth, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_NumberOfStrip = NumberOfStrip;
}

ANUScorerSiLiBackStripTrapezoid::~ANUScorerSiLiBackStripTrapezoid()
{
}

G4bool ANUScorerSiLiBackStripTrapezoid::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUTrapezoid");

   // get back strip
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double StripPitch = ANUTRAP::SiLiHeight / m_NumberOfStrip;

   G4double temp = (POS(1) + ANUTRAP::SiLiHeight / 2.) / StripPitch   ;
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

void ANUScorerSiLiBackStripTrapezoid::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiBackStripTrapezoid::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiBackStripTrapezoid::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiBackStripTrapezoid::DrawAll()
{
}

void ANUScorerSiLiBackStripTrapezoid::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Front Strip position Scorer for Annular geometry
ANUScorerSiLiFrontStripAnnular::ANUScorerSiLiFrontStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiFrontStripAnnular::~ANUScorerSiLiFrontStripAnnular()
{
}

G4bool ANUScorerSiLiFrontStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUAnnular");

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

void ANUScorerSiLiFrontStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiFrontStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiFrontStripAnnular::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiFrontStripAnnular::DrawAll()
{
}

void ANUScorerSiLiFrontStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SiLi Back Strip position Scorer for Annular geometry
ANUScorerSiLiBackStripAnnular::ANUScorerSiLiBackStripAnnular(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
}

ANUScorerSiLiBackStripAnnular::~ANUScorerSiLiBackStripAnnular()
{
}

G4bool ANUScorerSiLiBackStripAnnular::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "ANUAnnular");

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

void ANUScorerSiLiBackStripAnnular::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ANUScorerSiLiBackStripAnnular::EndOfEvent(G4HCofThisEvent*)
{
}

void ANUScorerSiLiBackStripAnnular::Clear()
{
   EvtMap->clear();
}

void ANUScorerSiLiBackStripAnnular::DrawAll()
{
}

void ANUcorerSiLiBackStripAnnular::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
*/
