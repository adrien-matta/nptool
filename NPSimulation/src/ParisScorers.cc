/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 30/02/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Paris*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "GeneralScorers.hh"
#include "ParisScorers.hh"

#include "ParisCluster.hh"
#include "ParisPhoswich.hh"

using namespace PARISCLUSTER;
using namespace PARISPHOSWICH;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Detector Number Scorer (deal with multiple particle hit)
PARISScorerLaBr3StageDetectorNumber::PARISScorerLaBr3StageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerLaBr3StageDetectorNumber::~PARISScorerLaBr3StageDetectorNumber()
{
}

G4bool PARISScorerLaBr3StageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, DetNbr);
   return TRUE;
}
void PARISScorerLaBr3StageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerLaBr3StageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerLaBr3StageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void PARISScorerLaBr3StageDetectorNumber::DrawAll()
{
}

void PARISScorerLaBr3StageDetectorNumber::PrintAll()
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
PARISScorerLaBr3StageEnergy::PARISScorerLaBr3StageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerLaBr3StageEnergy::~PARISScorerLaBr3StageEnergy()
{
}

G4bool PARISScorerLaBr3StageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

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

void PARISScorerLaBr3StageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerLaBr3StageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerLaBr3StageEnergy::Clear()
{
   EvtMap->clear();
}

void PARISScorerLaBr3StageEnergy::DrawAll()
{
}

void PARISScorerLaBr3StageEnergy::PrintAll()
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
// FirstStage CrystalNbr Scorer (deal with multiple particle hit)
PARISScorerLaBr3StageCrystal::PARISScorerLaBr3StageCrystal(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerLaBr3StageCrystal::~PARISScorerLaBr3StageCrystal()
{
}

G4bool PARISScorerLaBr3StageCrystal::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1);
 
   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, CrystalNbr);
   return TRUE;

}

void PARISScorerLaBr3StageCrystal::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerLaBr3StageCrystal::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerLaBr3StageCrystal::Clear()
{
   EvtMap->clear();
}

void PARISScorerLaBr3StageCrystal::DrawAll()
{
}

void PARISScorerLaBr3StageCrystal::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
      std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
      for (; itr != EvtMap->GetMap()->end(); itr++) {
         G4cout << "  copy no.: " << itr->first
         << "  crystal nbr: " << G4BestUnit(*(itr->second), "Crystal")
       << G4endl;
   }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage ToF Scorer (deal with multiple particle hit)
PARISScorerLaBr3StageTOF::PARISScorerLaBr3StageTOF(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerLaBr3StageTOF::~PARISScorerLaBr3StageTOF()
{
}

G4bool PARISScorerLaBr3StageTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get TOF
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, TOF);
   return TRUE;
}

void PARISScorerLaBr3StageTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerLaBr3StageTOF::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerLaBr3StageTOF::Clear()
{
   EvtMap->clear();
}

void PARISScorerLaBr3StageTOF::DrawAll()
{
}

void PARISScorerLaBr3StageTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SecondStage (CsI) Detector Number Scorer (deal with multiple particle hit)
PARISScorerCsIStageDetectorNumber::PARISScorerCsIStageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerCsIStageDetectorNumber::~PARISScorerCsIStageDetectorNumber()
{
}

G4bool PARISScorerCsIStageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, DetNbr);
   return TRUE;
}
void PARISScorerCsIStageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerCsIStageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerCsIStageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void PARISScorerCsIStageDetectorNumber::DrawAll()
{
}

void PARISScorerCsIStageDetectorNumber::PrintAll()
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
// CsIStage Energy Scorer (deal with multiple particle hit)
PARISScorerCsIStageEnergy::PARISScorerCsIStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

PARISScorerCsIStageEnergy::~PARISScorerCsIStageEnergy()
{
}

G4bool PARISScorerCsIStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;      
   if (edep < 0*keV) return FALSE;      // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DetNbr + index, edep);
   return TRUE;
}

void PARISScorerCsIStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PARISScorerCsIStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void PARISScorerCsIStageEnergy::Clear()
{
   EvtMap->clear();
}

void PARISScorerCsIStageEnergy::DrawAll()
{
}

void PARISScorerCsIStageEnergy::PrintAll()
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


