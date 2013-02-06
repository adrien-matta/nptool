/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
:G4VPrimitiveScorer(name, depth), HCID(-1){
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Rectangle::~PS_Silicon_Rectangle(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Rectangle::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);
  m_Position  = aStep->GetPreStepPoint()->GetPosition();
  m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);
  
  m_StripLengthNumber = (int)((m_Position(1) + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
  m_StripWidthNumber = (int)((m_Position(2) + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;

  //Rare case where particle is close to edge of silicon plan
  if (m_StripLengthNumber == m_NumberOfStripLength+1) m_StripLengthNumber = m_StripLengthNumber;
  if (m_StripWidthNumber == m_NumberOfStripWidth+1) m_StripWidthhNumber = m_StripWidthNumber;
  
  G4double[2] EnergyAndTime;
  
  EnergyAndTime[0] = aStep->GetTotalEnergyDeposit();
  EnergyAndTime[1] = aStep->GetPreStepPoint()->GetGlobalTime();;
  
  if (edep < TriggerThreshold) return FALSE;
  G4int  index =  aStep->GetTrack()->GetTrackID();
  EvtMap->set(index+DetNbr, X);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
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
