/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 12/02/2010                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This file describes the specific scorers associated to the EDEN        *
 *    neutron detector                                                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "G4UnitsTable.hh"
#include <string>
#include "EDENScorers.hh"
#include "GeneralScorers.hh"
using namespace EDENSCORERS;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ChargeScorer
EDENScorerCharge::EDENScorerCharge(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}



EDENScorerCharge::~EDENScorerCharge()
{
}



G4bool EDENScorerCharge::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   int DetNbr = GENERALSCORERS::PickUpDetectorNumber(aStep, "EDEN");

   G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
   POS = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(POS);

   G4int  index   = aStep->GetTrack()->GetTrackID();
   G4String pname = aStep->GetTrack()->GetDefinition()->GetParticleName();
   G4double edep  = aStep->GetTotalEnergyDeposit();
   G4cout << pname << "   " << DetNbr << "  " << index << "  " << edep << G4endl;

   G4double Light = edep;
   if (pname == "e-")          Light = LightOutput(edep, 0);
   else if (pname == "proton") Light = LightOutput(edep, 1);
   else if (pname == "carbon") Light = LightOutput(edep, 2);
   else G4cout << "Unknown type of particle" << G4endl;

   EvtMap->add(index+DetNbr, Light);
   return TRUE;
}



void EDENScorerCharge::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}



void EDENScorerCharge::EndOfEvent(G4HCofThisEvent*)
{
}



void EDENScorerCharge::clear()
{
   EvtMap->clear();
}



void EDENScorerCharge::DrawAll()
{
}



void EDENScorerCharge::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}


G4double EDENScorerCharge::LightOutput(G4double E, G4int pType)
{
   G4double a1[3] = {1, 0.83, 0.017};
   G4double a2[3] = {0, 2.82, 0.000};
   G4double a3[3] = {0, 0.25, 0.000};
   G4double a4[3] = {0, 0.93, 0.000};
   
   return a1[pType]*E - a2[pType]* (1 - exp(-a3[pType]*pow(E,a4[pType])));
}
