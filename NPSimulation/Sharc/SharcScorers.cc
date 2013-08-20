 /*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : February 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the Sharc Detector                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new type of scorer is aim to become the standard for DSSD,SSSD and   *
 * PAD detector (any thing with a rectangular shape)                         *
 *****************************************************************************/
#include "SharcScorers.hh"
#include "G4UnitsTable.hh"
using namespace SHARC ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Rectangle::PS_Silicon_Rectangle(G4String name, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
  m_StripPlaneLength = StripPlaneLength;
  m_StripPlaneWidth = StripPlaneWidth;
  m_NumberOfStripLength = NumberOfStripLength;
  m_NumberOfStripWidth = NumberOfStripWidth;
  m_StripPitchLength = m_StripPlaneLength / m_NumberOfStripLength;
  m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
  
  m_Position = G4ThreeVector(-1000,-1000,-1000);
  m_DetectorNumber = -1;
  m_StripLengthNumber = -1;
  m_StripWidthNumber = -1;
  m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Rectangle::~PS_Silicon_Rectangle(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Rectangle::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, StripFront and StripBack
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
  
  m_StripLengthNumber = (int)((m_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
  m_StripWidthNumber = (int)((m_Position.y() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;

  m_StripLengthNumber = m_NumberOfStripLength - m_StripLengthNumber + 1 ;
  m_StripWidthNumber = m_NumberOfStripWidth - m_StripWidthNumber + 1 ;
  
  
  EnergyAndTime[2] = m_DetectorNumber;
  EnergyAndTime[3] = m_StripLengthNumber;
  EnergyAndTime[4] = m_StripWidthNumber;
  
    
  //Rare case where particle is close to edge of silicon plan
  if (m_StripLengthNumber > m_NumberOfStripLength) m_StripLengthNumber = m_StripLengthNumber;
  if (m_StripWidthNumber > m_NumberOfStripWidth) m_StripWidthNumber = m_StripWidthNumber;
  
  m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripLengthNumber * 1e6 + m_StripWidthNumber * 1e9;
  
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
void PS_Silicon_Rectangle::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new G4THitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }
  
  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::DrawAll(){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Annular::PS_Silicon_Annular(G4String name, G4double StripPlaneInnerRadius, G4double StripPlaneOuterRadius, G4double PhiStart,G4double PhiStop, G4int NumberOfStripRadial,G4int NumberOfStripTheta,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
  
  m_StripPlaneInnerRadius = StripPlaneInnerRadius;
  m_StripPlaneOuterRadius = StripPlaneOuterRadius;
  m_PhiStart = PhiStart;
  m_PhiStop = PhiStop;
  m_NumberOfStripRadial = NumberOfStripRadial;
  m_NumberOfStripTheta = NumberOfStripTheta;
  m_StripPitchRadial = (m_PhiStop-m_PhiStart)/m_NumberOfStripTheta;
  m_StripPitchTheta = (m_StripPlaneOuterRadius-m_StripPlaneInnerRadius)/m_NumberOfStripRadial;
  
  m_Position = G4ThreeVector(-1000,-1000,-1000);
  m_uz = G4ThreeVector(0,0,1);
  m_DetectorNumber = -1;
  m_StripRadialNumber  = -1;
  m_StripThetaNumber = -1;
  m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Annular::~PS_Silicon_Annular(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Annular::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  // contain Energy Time, DetNbr, StripFront and StripBack
  G4double* EnergyAndTime = new G4double[10];
  EnergyAndTime[0] = aStep->GetTotalEnergyDeposit();
  
  EnergyAndTime[1] = aStep->GetPreStepPoint()->GetGlobalTime();
  
  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);
  m_Position = aStep->GetPreStepPoint()->GetPosition();
 
  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  EnergyAndTime[5] = m_Position.x();
  EnergyAndTime[6] = m_Position.y();
  EnergyAndTime[7] = m_Position.z();
  EnergyAndTime[8] = m_Position.theta();
  EnergyAndTime[9] = m_Position.phi();
    
  m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
  m_StripRadialNumber = (int) ((m_StripPlaneOuterRadius - m_Position.rho()) / m_StripPitchTheta ) + 1 ;

  m_StripThetaNumber =  (int) ((m_Position.phi() - m_PhiStart*0.5) / m_StripPitchRadial ) + 1 ;
  
  EnergyAndTime[2] = m_DetectorNumber;
  EnergyAndTime[3] = m_StripThetaNumber;
  EnergyAndTime[4] = m_StripRadialNumber;
  
  
  //Rare case where particle is close to edge of silicon plan
  if (m_StripRadialNumber > m_NumberOfStripRadial) m_StripRadialNumber = m_NumberOfStripRadial;
  if (m_StripThetaNumber > m_NumberOfStripTheta) m_StripThetaNumber = m_NumberOfStripTheta;
  
  m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripRadialNumber * 1e6 + m_NumberOfStripTheta * 1e9;
  
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
void PS_Silicon_Annular::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new G4THitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }
  
  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::DrawAll(){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}