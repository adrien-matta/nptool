 /*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: morfouac@nscl.msu.edu     *
 *                                                                           *
 * Creation Date  : January 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for Drift Electron collector                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new type of scorer to count drift electron in the cathode            *
 *                                                                           *
 *****************************************************************************/
#include "DriftElectronScorers.hh"
#include "G4UnitsTable.hh"
using namespace DRIFTELECTRONSCORERS ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_DECathode::PS_DECathode(G4String name,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    m_Index = -1 ;
    m_Level = depth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_DECathode::~PS_DECathode(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_DECathode::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, StripFront and StripBack
  G4double* Infos = new G4double[9];
  Infos[0] = 0;
  Infos[1] = aStep->GetPreStepPoint()->GetProperTime();

  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  m_Position  = aStep->GetPreStepPoint()->GetPosition();

  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  Infos[2] = m_Position.x();
  Infos[3] = m_Position.y();
  Infos[4] = m_Position.z();
  Infos[5] = m_Position.theta();
  Infos[6] = m_Position.phi();
  Infos[7] = m_DetectorNumber;

  m_Index = m_DetectorNumber * 1e3 ;
  G4String PID = aStep->GetTrack()->GetDefinition()->GetParticleName();

    if(PID=="driftelectron"){
        Infos[0] = aStep->GetTrack()->GetWeight();
    }

    // Check if the particle has interact before, if yes, add up the number of electron.
    map<G4int, G4double**>::iterator it;
    it= EvtMap->GetMap()->find(m_Index);
    if(it!=EvtMap->GetMap()->end()){
      G4double* dummy = *(it->second);
      Infos[0]+=dummy[0];
      Infos[1]=dummy[1];
    }

  EvtMap->set(m_Index, Infos);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DECathode::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DECathode::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DECathode::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }

  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DECathode::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DECathode::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_DEDigitizer::PS_DEDigitizer(G4String name,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    m_Index = -1 ;
    m_Level = depth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_DEDigitizer::~PS_DEDigitizer(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_DEDigitizer::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, StripFront and StripBack
  G4double* Infos = new G4double[9];
  Infos[0] = 0;
  Infos[1] = aStep->GetPreStepPoint()->GetProperTime();

  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  m_Position  = aStep->GetPreStepPoint()->GetPosition();

  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  Infos[2] = m_Position.x();
  Infos[3] = m_Position.y();
  Infos[4] = m_Position.z();
  Infos[5] = m_Position.theta();
  Infos[6] = m_Position.phi();
  Infos[7] = m_DetectorNumber;

  m_Index =  aStep->GetTrack()->GetTrackID()  +  m_DetectorNumber*1e6 ;
  G4String PID = aStep->GetTrack()->GetDefinition()->GetParticleName();

    if(PID=="driftelectron"){
        Infos[0] = aStep->GetTrack()->GetWeight();
    }

    // Check if the particle has interact before, if yes, add up the number of electron.
    map<G4int, G4double**>::iterator it;
    it= EvtMap->GetMap()->find(m_Index);
    if(it!=EvtMap->GetMap()->end()){
      G4double* dummy = *(it->second);
      Infos[0]+=dummy[0];
      Infos[1]=dummy[1];
    }

  EvtMap->set(m_Index, Infos);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DEDigitizer::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DEDigitizer::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DEDigitizer::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }

  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DEDigitizer::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_DEDigitizer::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
