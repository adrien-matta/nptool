/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    : 02/07/13                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Fatima*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "ObsoleteGeneralScorers.hh"
#include "FatimaScorers.hh"

#include "FatimaDetector.hh"

using namespace FATIMADETECTOR;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Detector Number Scorer (deal with multiple particle hit)
FATIMAScorerLaBr3StageDetectorNumber::FATIMAScorerLaBr3StageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerLaBr3StageDetectorNumber::~FATIMAScorerLaBr3StageDetectorNumber()
{
}

G4bool FATIMAScorerLaBr3StageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   // int DetNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0); // this line does exactly the same than the line above
   int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(CrystalNbr + DetNbr + index, DetNbr);
   return TRUE;
}
void FATIMAScorerLaBr3StageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerLaBr3StageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerLaBr3StageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerLaBr3StageDetectorNumber::DrawAll()
{
}

void FATIMAScorerLaBr3StageDetectorNumber::PrintAll()
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
FATIMAScorerLaBr3StageEnergy::FATIMAScorerLaBr3StageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerLaBr3StageEnergy::~FATIMAScorerLaBr3StageEnergy()
{
}

G4bool FATIMAScorerLaBr3StageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                  // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(CrystalNbr + DetNbr + index, edep);
   return TRUE;
}

void FATIMAScorerLaBr3StageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerLaBr3StageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerLaBr3StageEnergy::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerLaBr3StageEnergy::DrawAll()
{
}

void FATIMAScorerLaBr3StageEnergy::PrintAll()
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
FATIMAScorerLaBr3StageCrystal::FATIMAScorerLaBr3StageCrystal(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerLaBr3StageCrystal::~FATIMAScorerLaBr3StageCrystal()
{
}

G4bool FATIMAScorerLaBr3StageCrystal::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   //G4int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1);
   //Adde by Anna:
  G4int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1); // daughter crystal volume
   //depth:0 is the world, 1 is the mother...
   //AC instead of GetReplicaNumber(1)
   //cout<<"****replica****"<<endl;
   //cout<<"copy nbr"<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1)<<endl;
   //cout<<"replica nbr"<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1)<<endl;
   //cout<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2)<<endl;
   //cout<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0)<<endl;
 
   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   //EvtMap->add(DetNbr + index, CrystalNbr);  // Marc
   //cout<<"index "<<index<<endl;
   //cout<<"DetNbr + index "<<DetNbr + index<<endl;   
   EvtMap->set(CrystalNbr + DetNbr + index, CrystalNbr);// modified by Anna
   return TRUE;

}

void FATIMAScorerLaBr3StageCrystal::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerLaBr3StageCrystal::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerLaBr3StageCrystal::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerLaBr3StageCrystal::DrawAll()
{
}

void FATIMAScorerLaBr3StageCrystal::PrintAll()
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
FATIMAScorerLaBr3StageTOF::FATIMAScorerLaBr3StageTOF(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerLaBr3StageTOF::~FATIMAScorerLaBr3StageTOF()
{
}

G4bool FATIMAScorerLaBr3StageTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get TOF
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(CrystalNbr + DetNbr + index, TOF);
   return TRUE;
}

void FATIMAScorerLaBr3StageTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerLaBr3StageTOF::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerLaBr3StageTOF::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerLaBr3StageTOF::DrawAll()
{
}

void FATIMAScorerLaBr3StageTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SecondStage (CsI) Detector Number Scorer (deal with multiple particle hit)
FATIMAScorerCsIStageDetectorNumber::FATIMAScorerCsIStageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerCsIStageDetectorNumber::~FATIMAScorerCsIStageDetectorNumber()
{
}

G4bool FATIMAScorerCsIStageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   // int DetNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0); // this line does exactly the same than the line above
   int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(CrystalNbr + DetNbr + index, DetNbr);
   return TRUE;
}
void FATIMAScorerCsIStageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerCsIStageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerCsIStageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerCsIStageDetectorNumber::DrawAll()
{
}

void FATIMAScorerCsIStageDetectorNumber::PrintAll()
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
FATIMAScorerCsIStageEnergy::FATIMAScorerCsIStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerCsIStageEnergy::~FATIMAScorerCsIStageEnergy()
{
}

G4bool FATIMAScorerCsIStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;      
   if (edep < 0*keV) return FALSE;      // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(CrystalNbr + DetNbr + index, edep);
   return TRUE;
}

void FATIMAScorerCsIStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerCsIStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerCsIStageEnergy::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerCsIStageEnergy::DrawAll()
{
}

void FATIMAScorerCsIStageEnergy::PrintAll()
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
// all added by Anna:

// FirstStage CrystalNbr Scorer (deal with multiple particle hit)
FATIMAScorerCsIStageCrystalNumber::FATIMAScorerCsIStageCrystalNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

FATIMAScorerCsIStageCrystalNumber::~FATIMAScorerCsIStageCrystalNumber()
{
}

G4bool FATIMAScorerCsIStageCrystalNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
   //depth:0 is the world, 1 is the mother...
   //AC instead of GetReplicaNumber(1)
   //cout<<"****replica****"<<endl;
   //cout<<"copy nbr"<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1)<<endl;
   //cout<<"replica nbr"<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1)<<endl;
   //cout<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2)<<endl;
   //cout<<aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0)<<endl;
 
   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   //cout<<"index "<<index<<endl;
   //cout<<"DetNbr + index "<<DetNbr + index<<endl;
   EvtMap->set(CrystalNbr + DetNbr + index, CrystalNbr);
   return TRUE;

}

void FATIMAScorerCsIStageCrystalNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void FATIMAScorerCsIStageCrystalNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void FATIMAScorerCsIStageCrystalNumber::Clear()
{
   EvtMap->clear();
}

void FATIMAScorerCsIStageCrystalNumber::DrawAll()
{
}

void FATIMAScorerCsIStageCrystalNumber::PrintAll()
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
