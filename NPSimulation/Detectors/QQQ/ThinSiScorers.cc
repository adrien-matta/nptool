/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File holding the scorer specific to the ThinSi Detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "ThinSiScorers.hh"
#include "G4UnitsTable.hh"
#include "ObsoleteGeneralScorers.hh"
#include <string>
using namespace THINSI ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Strip position Scorer
//X
PSStripNumber::PSStripNumber(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize  = StripPlaneSize                                  ;
   m_NumberOfStrip   = NumberOfStrip                                   ;
   m_StripPitch      = m_StripPlaneSize / m_NumberOfStrip   ;
}

PSStripNumber::~PSStripNumber()
{
   ;
}

G4bool PSStripNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    int DetNbr = OBSOLETEGENERALSCORERS::PickUpDetectorNumber(aStep, "ThinSi");

   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double temp = (POS(0) + m_StripPlaneSize / 2.) / m_StripPitch   ;
   G4double X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   if (X ==0 ) X =1 ;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < TriggerThreshold) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   G4int S = X ;
   EvtMap->set(index+DetNbr, S);
   return TRUE;
}

void PSStripNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSStripNumber::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSStripNumber::clear()
{
   EvtMap->clear();
}

void PSStripNumber::DrawAll()
{
   ;
}

void PSStripNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
