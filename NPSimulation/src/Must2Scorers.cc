/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : october 2010                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the MUST2 Detector                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "Must2Scorers.hh"
#include "G4UnitsTable.hh"
#include "GeneralScorers.hh"
#include <string>
using namespace MUST2 ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Strip position Scorer
//X
PSStripNumberX::PSStripNumberX(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize = StripPlaneSize ;
   m_NumberOfStrip  = NumberOfStrip  ;
	 m_StripPitch			= m_StripPlaneSize / m_NumberOfStrip;
}

PSStripNumberX::~PSStripNumberX()
{
   ;
}

G4bool PSStripNumberX::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	 int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "MUST2Telescope");

   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double temp = (POS(1) + m_StripPlaneSize / 2.) / m_StripPitch   ;
   G4int X = int(temp) + 1 ;
   //Rare case where particle is close to edge of silicon plan
   if (X == m_NumberOfStrip+1) X = m_NumberOfStrip;
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index+DetNbr, X);
   return TRUE;
}

void PSStripNumberX::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSStripNumberX::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSStripNumberX::clear()
{
   EvtMap->clear();
}

void PSStripNumberX::DrawAll()
{
   ;
}

void PSStripNumberX::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Y
PSStripNumberY::PSStripNumberY(G4String name, G4int depth, G4double StripPlaneSize, G4int NumberOfStrip)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   m_StripPlaneSize =   StripPlaneSize ;
   m_NumberOfStrip    = NumberOfStrip  ;
	 m_StripPitch			= m_StripPlaneSize / m_NumberOfStrip;
}

PSStripNumberY::~PSStripNumberY()
{
   ;
}

G4bool PSStripNumberY::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	 int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "MUST2Telescope");


   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4double temp = (POS(0) + m_StripPlaneSize / 2.) / m_StripPitch   ;
   G4int temp2 = temp ;
   G4int Y = temp2 + 1                    ;
   //Rare case where particle is close to edge of silicon plan
   if (Y == m_NumberOfStrip+1) Y = m_NumberOfStrip;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index =  aStep->GetTrack()->GetTrackID();
   EvtMap->set(index+DetNbr, Y);
   return TRUE;
}

void PSStripNumberY::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSStripNumberY::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSStripNumberY::clear()
{
   EvtMap->clear();
}

void PSStripNumberY::DrawAll()
{
   ;
}

void PSStripNumberY::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}







//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//CsI Cristal / SiLi Pad Number Scorer
//
PSPadOrCristalNumber::PSPadOrCristalNumber(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{}

PSPadOrCristalNumber::~PSPadOrCristalNumber()
{}

G4bool PSPadOrCristalNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
		std::string name = aStep->GetTrack()->GetVolume()->GetName();
		std::string nbr ;

		G4int temp1,temp2 ;
		G4int VolumeNumber;
		nbr = name[name.length()-1]	;
		temp1 = atoi( nbr.c_str() )	;
		
		nbr = name[name.length()-2]	;
		temp2 = atoi( nbr.c_str() )	;
		
		nbr.clear();
		
		if(temp2!=0) { nbr+= name[name.length()-2]	; nbr+= name[name.length()-1]	; VolumeNumber = atoi( nbr.c_str() )	;}
		
		else 				 { nbr= name[name.length()-1]	; VolumeNumber = atoi( nbr.c_str() )	;}

	 	G4int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "MUST2Telescope");

		G4double edep = aStep->GetTotalEnergyDeposit();
		if (edep < 100*keV) return FALSE;
		G4int  index =  aStep->GetTrack()->GetTrackID();
		EvtMap->set(index+DetNbr, VolumeNumber);
		return TRUE;
}

void PSPadOrCristalNumber::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSPadOrCristalNumber::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSPadOrCristalNumber::clear()
{
   EvtMap->clear();
}

void PSPadOrCristalNumber::DrawAll()
{
   ;
}

void PSPadOrCristalNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}















