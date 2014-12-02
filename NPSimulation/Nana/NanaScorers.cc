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
 *             Nana*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4UnitsTable.hh"

// NPTool headers
#include "ObsoleteGeneralScorers.hh"
#include "NanaScorers.hh"

#include "NanaDetector.hh"

using namespace NANADETECTOR;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Detector Number Scorer (deal with multiple particle hit)
NANAScorerLaBr3StageDetectorNumber::NANAScorerLaBr3StageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerLaBr3StageDetectorNumber::~NANAScorerLaBr3StageDetectorNumber()
{
}

G4bool NANAScorerLaBr3StageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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
void NANAScorerLaBr3StageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerLaBr3StageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerLaBr3StageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void NANAScorerLaBr3StageDetectorNumber::DrawAll()
{
}

void NANAScorerLaBr3StageDetectorNumber::PrintAll()
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
NANAScorerLaBr3StageEnergy::NANAScorerLaBr3StageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerLaBr3StageEnergy::~NANAScorerLaBr3StageEnergy()
{
}

G4bool NANAScorerLaBr3StageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void NANAScorerLaBr3StageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerLaBr3StageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerLaBr3StageEnergy::Clear()
{
   EvtMap->clear();
}

void NANAScorerLaBr3StageEnergy::DrawAll()
{
}

void NANAScorerLaBr3StageEnergy::PrintAll()
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
NANAScorerLaBr3StageCrystal::NANAScorerLaBr3StageCrystal(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerLaBr3StageCrystal::~NANAScorerLaBr3StageCrystal()
{
}

G4bool NANAScorerLaBr3StageCrystal::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void NANAScorerLaBr3StageCrystal::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerLaBr3StageCrystal::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerLaBr3StageCrystal::Clear()
{
   EvtMap->clear();
}

void NANAScorerLaBr3StageCrystal::DrawAll()
{
}

void NANAScorerLaBr3StageCrystal::PrintAll()
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
NANAScorerLaBr3StageTOF::NANAScorerLaBr3StageTOF(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerLaBr3StageTOF::~NANAScorerLaBr3StageTOF()
{
}

G4bool NANAScorerLaBr3StageTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void NANAScorerLaBr3StageTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerLaBr3StageTOF::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerLaBr3StageTOF::Clear()
{
   EvtMap->clear();
}

void NANAScorerLaBr3StageTOF::DrawAll()
{
}

void NANAScorerLaBr3StageTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SecondStage (CsI) Detector Number Scorer (deal with multiple particle hit)
NANAScorerCsIStageDetectorNumber::NANAScorerCsIStageDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerCsIStageDetectorNumber::~NANAScorerCsIStageDetectorNumber()
{
}

G4bool NANAScorerCsIStageDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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
void NANAScorerCsIStageDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerCsIStageDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerCsIStageDetectorNumber::Clear()
{
   EvtMap->clear();
}

void NANAScorerCsIStageDetectorNumber::DrawAll()
{
}

void NANAScorerCsIStageDetectorNumber::PrintAll()
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
NANAScorerCsIStageEnergy::NANAScorerCsIStageEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerCsIStageEnergy::~NANAScorerCsIStageEnergy()
{
}

G4bool NANAScorerCsIStageEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void NANAScorerCsIStageEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerCsIStageEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerCsIStageEnergy::Clear()
{
   EvtMap->clear();
}

void NANAScorerCsIStageEnergy::DrawAll()
{
}

void NANAScorerCsIStageEnergy::PrintAll()
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
NANAScorerCsIStageCrystalNumber::NANAScorerCsIStageCrystalNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

NANAScorerCsIStageCrystalNumber::~NANAScorerCsIStageCrystalNumber()
{
}

G4bool NANAScorerCsIStageCrystalNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
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

void NANAScorerCsIStageCrystalNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void NANAScorerCsIStageCrystalNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void NANAScorerCsIStageCrystalNumber::Clear()
{
   EvtMap->clear();
}

void NANAScorerCsIStageCrystalNumber::DrawAll()
{
}

void NANAScorerCsIStageCrystalNumber::PrintAll()
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
