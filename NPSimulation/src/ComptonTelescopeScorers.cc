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
#include "G4VProcess.hh"

// NPTool headers
#include "GeneralScorers.hh"

#include "ComptonTelescopeScorers.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Added by Adrien MATTA:
// Those Scorer use TrackID as map index. This way ones can rebuild energy deposit,
// time of flight or position,... particle by particle for each event. Because standard
// scorer provide by G4 don't work this way but using a global ID for each event you should
// not use those scorer with some G4 provided ones or being very carefull doing so.


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Tower Number Scorer (deal with multiple particle hit)
ComptonTelescopeScorerProcess::ComptonTelescopeScorerProcess(G4String name, G4String volumeName, G4String processName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName  = volumeName;
   m_ProcessName = processName;
}

ComptonTelescopeScorerProcess::~ComptonTelescopeScorerProcess()
{
}

G4bool ComptonTelescopeScorerProcess::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   G4bool status = false;

   // Get track ID
   G4Track *track          = aStep->GetTrack();
   G4int    trackID        = track->GetTrackID();
   if (trackID > 1) {    // secondary particle
      // get some relevant track informations
//      G4int    parentID       = track->GetParentID();
      G4int    stepNumber     = track->GetCurrentStepNumber();
      G4String parentProcName = track->GetCreatorProcess()->GetProcessName();
      G4String particleName   = track->GetDefinition()->GetParticleName();
      // select secondary electrons created by compton effect
      if (particleName   == "e-"          &&    // secondary is an electron
          parentProcName == m_ProcessName &&    // electron has been created through compton effect
          stepNumber     == 1                   // first step of track 
         ) {
         // electron kinetic energy
         G4double ElectronKineticEnergy = aStep->GetPreStepPoint()->GetKineticEnergy() / MeV;
         // id of dsssd & tower
         // dsssd id
         G4int dsssdID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);
         // tower id
         G4int towerID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
         // build index for map
         G4int index = trackID + pow(100,1)*dsssdID + pow(100,2)*towerID;
         // fill map
         EvtMap->set(index, ElectronKineticEnergy);

         // debug outputs
//         G4cout << "////////// Compton electron found.............." << G4endl;
//         G4cout << "\tCopyNumbers()\t" << dsssdID << "\t" << towerID << G4endl;
//         G4cout << "\tElectron energy\t" << ElectronKineticEnergy << G4endl;

         status = true;
      }
   }

   return status;
}

void ComptonTelescopeScorerProcess::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerProcess::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerProcess::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerProcess::DrawAll()
{
}

void ComptonTelescopeScorerProcess::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << " Tower Number : " << G4BestUnit(*(itr->second), "TowerNumber")
      << G4endl;
   }
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Tower Number Scorer (deal with multiple particle hit)
ComptonTelescopeScorerTrackerTowerNumber::ComptonTelescopeScorerTrackerTowerNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerTrackerTowerNumber::~ComptonTelescopeScorerTrackerTowerNumber()
{
}

G4bool ComptonTelescopeScorerTrackerTowerNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   // int DetNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0); // this line does exactly the same than the line above
   int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DSSSDNbr + DetNbr + index, DetNbr);
   return TRUE;
}

void ComptonTelescopeScorerTrackerTowerNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerTrackerTowerNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerTrackerTowerNumber::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerTrackerTowerNumber::DrawAll()
{
}

void ComptonTelescopeScorerTrackerTowerNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << " Tower Number : " << G4BestUnit(*(itr->second), "TowerNumber")
      << G4endl;
   }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage Energy Scorer (deal with multiple particle hit)
ComptonTelescopeScorerTrackerEnergy::ComptonTelescopeScorerTrackerEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerTrackerEnergy::~ComptonTelescopeScorerTrackerEnergy()
{
}

G4bool ComptonTelescopeScorerTrackerEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                  // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DSSSDNbr + DetNbr + index, edep);
   return TRUE;
}

void ComptonTelescopeScorerTrackerEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerTrackerEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerTrackerEnergy::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerTrackerEnergy::DrawAll()
{
}

void ComptonTelescopeScorerTrackerEnergy::PrintAll()
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
ComptonTelescopeScorerTrackerDSSSDNumber::ComptonTelescopeScorerTrackerDSSSDNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerTrackerDSSSDNumber::~ComptonTelescopeScorerTrackerDSSSDNumber()
{
}

G4bool ComptonTelescopeScorerTrackerDSSSDNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   //G4int CrystalNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetReplicaNumber(1);
   //Adde by Anna:
  G4int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1); // daughter crystal volume
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
   EvtMap->set(DSSSDNbr + DetNbr + index, DSSSDNbr);// modified by Anna
   return TRUE;

}

void ComptonTelescopeScorerTrackerDSSSDNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerTrackerDSSSDNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerTrackerDSSSDNumber::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerTrackerDSSSDNumber::DrawAll()
{
}

void ComptonTelescopeScorerTrackerDSSSDNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
      std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
      for (; itr != EvtMap->GetMap()->end(); itr++) {
         G4cout << "  copy no.: " << itr->first
         << "  crystal nbr: " << G4BestUnit(*(itr->second), "DSSSD")
       << G4endl;
   }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// FirstStage ToF Scorer (deal with multiple particle hit)
ComptonTelescopeScorerTrackerTOF::ComptonTelescopeScorerTrackerTOF(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerTrackerTOF::~ComptonTelescopeScorerTrackerTOF()
{
}

G4bool ComptonTelescopeScorerTrackerTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get TOF
   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();

   G4double edep = aStep->GetTotalEnergyDeposit();
   //   if (edep < 100*keV) return FALSE;
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DSSSDNbr + DetNbr + index, TOF);
   return TRUE;
}

void ComptonTelescopeScorerTrackerTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerTrackerTOF::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerTrackerTOF::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerTrackerTOF::DrawAll()
{
}

void ComptonTelescopeScorerTrackerTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SecondStage (CsI) Tower Number Scorer (deal with multiple particle hit)
ComptonTelescopeScorerCalorimeterTowerNumber::ComptonTelescopeScorerCalorimeterTowerNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerCalorimeterTowerNumber::~ComptonTelescopeScorerCalorimeterTowerNumber()
{
}

G4bool ComptonTelescopeScorerCalorimeterTowerNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   // int DetNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0); // this line does exactly the same than the line above
   int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 0*keV) return FALSE;                   // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DSSSDNbr + DetNbr + index, DetNbr);
   return TRUE;
}

void ComptonTelescopeScorerCalorimeterTowerNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerCalorimeterTowerNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerCalorimeterTowerNumber::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerCalorimeterTowerNumber::DrawAll()
{
}

void ComptonTelescopeScorerCalorimeterTowerNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << " Tower Number : " << G4BestUnit(*(itr->second), "DetectorNumber")
      << G4endl;
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Calorimeter Energy Scorer (deal with multiple particle hit)
ComptonTelescopeScorerCalorimeterEnergy::ComptonTelescopeScorerCalorimeterEnergy(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerCalorimeterEnergy::~ComptonTelescopeScorerCalorimeterEnergy()
{
}

G4bool ComptonTelescopeScorerCalorimeterEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);
   int DSSSDNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);

   // get energy
   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double edep = aStep->GetTotalEnergyDeposit();
   //if (edep < 100*keV) return FALSE;      
   if (edep < 0*keV) return FALSE;      // = HERE IS THE DIFFERENCE WITH GENERALSCORER 
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->add(DSSSDNbr + DetNbr + index, edep);
   return TRUE;
}

void ComptonTelescopeScorerCalorimeterEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerCalorimeterEnergy::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerCalorimeterEnergy::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerCalorimeterEnergy::DrawAll()
{
}

void ComptonTelescopeScorerCalorimeterEnergy::PrintAll()
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

// FirstStage DetectorNbr Scorer (deal with multiple particle hit)
ComptonTelescopeScorerCalorimeterDetectorNumber::ComptonTelescopeScorerCalorimeterDetectorNumber(G4String name, G4String volumeName, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_VolumeName = volumeName;
}

ComptonTelescopeScorerCalorimeterDetectorNumber::~ComptonTelescopeScorerCalorimeterDetectorNumber()
{
}

G4bool ComptonTelescopeScorerCalorimeterDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   // get detector number
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, m_VolumeName);

   // get energy
   G4int DetectorNbr  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
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
   EvtMap->set(DetectorNbr + DetNbr + index, DetectorNbr);
   return TRUE;

}

void ComptonTelescopeScorerCalorimeterDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void ComptonTelescopeScorerCalorimeterDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
}

void ComptonTelescopeScorerCalorimeterDetectorNumber::Clear()
{
   EvtMap->clear();
}

void ComptonTelescopeScorerCalorimeterDetectorNumber::DrawAll()
{
}

void ComptonTelescopeScorerCalorimeterDetectorNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
      std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
      for (; itr != EvtMap->GetMap()->end(); itr++) {
         G4cout << "  copy no.: " << itr->first
         << "  detector nbr: " << G4BestUnit(*(itr->second), "Detector")
       << G4endl;
   }
}
