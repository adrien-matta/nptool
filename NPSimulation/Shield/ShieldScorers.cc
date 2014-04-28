/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 26/08/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Shield*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "ObsoleteGeneralScorers.hh"
#include "ShieldScorers.hh"

#include "ShieldClParis.hh"
#include "ShieldPhParis.hh"

using namespace PARISCLSHIELD;
using namespace PARISPHSHIELD;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Detector Number Scorer (deal with multiple particle hit)
SHIELDScorerCsIDetectorNumber::SHIELDScorerCsIDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

SHIELDScorerCsIDetectorNumber::~SHIELDScorerCsIDetectorNumber()
{
}

G4bool SHIELDScorerCsIDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, DetNbr);
   return TRUE;
}
void SHIELDScorerCsIDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void SHIELDScorerCsIDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void SHIELDScorerCsIDetectorNumber::Clear()
{
   EvtMap->clear();
}

void SHIELDScorerCsIDetectorNumber::DrawAll()
{
}

void SHIELDScorerCsIDetectorNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << " Detector Number : " << G4BestUnit(*(itr->second), "DetectorNumber")
      << G4endl;
   }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Energy Scorer (deal with multiple particle hit)
SHIELDScorerCsIEnergy::SHIELDScorerCsIEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

SHIELDScorerCsIEnergy::~SHIELDScorerCsIEnergy()
{
}

G4bool SHIELDScorerCsIEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                  // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void SHIELDScorerCsIEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void SHIELDScorerCsIEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void SHIELDScorerCsIEnergy::Clear()
{
   EvtMap->clear();
}

void SHIELDScorerCsIEnergy::DrawAll()
{
}

void SHIELDScorerCsIEnergy::PrintAll()
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
// FirstStage ToF Scorer (deal with multiple particle hit)
SHIELDScorerCsITOF::SHIELDScorerCsITOF(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

SHIELDScorerCsITOF::~SHIELDScorerCsITOF()
{
}

G4bool SHIELDScorerCsITOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get TOF
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, TOF);
   return TRUE;
}

void SHIELDScorerCsITOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void SHIELDScorerCsITOF::EndOfEvent(G4HCofThisEvent*)
{
}

void SHIELDScorerCsITOF::Clear()
{
   EvtMap->clear();
}

void SHIELDScorerCsITOF::DrawAll()
{
}

void SHIELDScorerCsITOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
}




