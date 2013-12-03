 /****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : December 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Generic scorer for Resistive Strip Silicon detector                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Energy Sharing and Balistic deficit effect.                              *
 *                                                                           *
 *****************************************************************************/
#include "ResistiveStripScorers.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Resistive::PS_Silicon_Resistive(G4String name, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripWidth,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
  m_StripPlaneLength = StripPlaneLength;
  m_StripPlaneWidth = StripPlaneWidth;
  m_NumberOfStripWidth = NumberOfStripWidth;
  m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
  
  m_Position = G4ThreeVector(-1000,-1000,-1000);
  m_DetectorNumber = -1;
  m_StripWidthNumber = -1;
  m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Resistive::~PS_Silicon_Resistive(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Resistive::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Total, E1, E2, Time, DetNbr,  and StripWidth
  G4double* EnergyAndTime = new G4double[10];
  EnergyAndTime[0] = aStep->GetTotalEnergyDeposit();
  
  EnergyAndTime[1] = aStep->GetPreStepPoint()->GetGlobalTime();
  
  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);
  m_Position  = aStep->GetPreStepPoint()->GetPosition();
  
  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  EnergyAndTime[5] = m_Position.x();
  EnergyAndTime[6] = m_Position.y();
  EnergyAndTime[7] = m_Position.z();
  EnergyAndTime[8] = m_Position.theta();
  EnergyAndTime[9] = m_Position.phi();
  
  m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
  
  m_StripWidthNumber = (int)((m_Position.y() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  m_StripWidthNumber = m_NumberOfStripWidth - m_StripWidthNumber + 1 ;
  
  EnergyAndTime[2] = m_DetectorNumber;
  EnergyAndTime[4] = m_StripWidthNumber;
    
  //Rare case where particle is close to edge of silicon plan
  if (m_StripWidthNumber > m_NumberOfStripWidth) m_StripWidthNumber = m_NumberOfStripWidth;
  
  m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripWidthNumber * 1e6;
  
  // Check if the particle has interact before, if yes, add up the energies.
    map<G4int, G4double**>::iterator it;
    it= EvtMap->GetMap()->find(m_Index);
    if(it!=EvtMap->GetMap()->end()){
      G4double* dummy = *(it->second);
      EnergyAndTime[0]+=dummy[0];
    }
  
  EvtMap->set(m_Index, EnergyAndTime);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new G4THitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }
  
  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::DrawAll(){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

