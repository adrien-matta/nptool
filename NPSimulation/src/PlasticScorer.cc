/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : September 2009                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class old some of the Platic Scorer, shared by different detector.  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *****************************************************************************/
#include "PlasticScorers.hh"
#include "G4UnitsTable.hh"
using namespace PLASTIC;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Energy Scorer (deal with multiple particle hit)

PSEnergy::PSEnergy(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSEnergy::~PSEnergy()
{
}

G4bool PSEnergy::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   std::string name = aStep->GetTrack()->GetVolume()->GetName();
   std::string nbr ;
   size_t found;
   found=name.find("Plastic");
   found = found + 7;
   
   int numberOfCharacterInDetectorNumber = name.length() - (int)found  ;

	for( int i = found ; i < found + numberOfCharacterInDetectorNumber ; i++ )
			nbr += name[i] ; 
		
   G4int DetNbr = atoi( nbr.c_str() ) ;

   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   
   G4int  index = aStep->GetTrack()->GetTrackID();
   
   EvtMap->add(DetNbr+index, edep);
   return TRUE;
}

void PSEnergy::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSEnergy::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSEnergy::clear()
{
   EvtMap->clear();
}

void PSEnergy::DrawAll()
{
   ;
}

void PSEnergy::PrintAll()
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
		//Detector Number Scorer
PSDetectorNumber::PSDetectorNumber(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
}

PSDetectorNumber::~PSDetectorNumber()
{
   ;
}

G4bool PSDetectorNumber::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   std::string name = aStep->GetTrack()->GetVolume()->GetName();
   std::string nbr ;
   size_t found;
   found=name.find("Plastic");
   found = found + 7;
   
   int numberOfCharacterInDetectorNumber = name.length() - (int)found  ;

	for( int i = found ; i < found + numberOfCharacterInDetectorNumber ; i++ )
			nbr += name[i] ; 
		
   G4int DetNbr = atoi( nbr.c_str() ) ;
   G4double edep = aStep->GetTotalEnergyDeposit();
   
   if (edep < 100*keV) return FALSE;
   
   G4int  index = aStep->GetTrack()->GetTrackID();
   
   EvtMap->set(DetNbr + index, DetNbr);
   return TRUE;
}

void PSDetectorNumber::Initialize(G4HCofThisEvent* HCE)
{ 
   EvtMap = new G4THitsMap<G4int>(GetMultiFunctionalDetector()->GetName(), GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSDetectorNumber::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSDetectorNumber::clear()
{
   EvtMap->clear();
}

void PSDetectorNumber::DrawAll()
{
   ;
}

void PSDetectorNumber::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
   G4cout << " PrimitiveScorer " << GetName() << G4endl;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl;
   std::map<G4int, G4int*>::iterator itr = EvtMap->GetMap()->begin();
   for (; itr != EvtMap->GetMap()->end(); itr++) {
      G4cout << "  copy no.: " << itr->first
      << "  energy deposit: " << G4BestUnit(*(itr->second), "Energy")
      << G4endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//TOF Scorer//
PSTOF::PSTOF(G4String name, G4int depth)
      : G4VPrimitiveScorer(name, depth), HCID(-1)
{
   ;
}

PSTOF::~PSTOF()
{
   ;
}

G4bool PSTOF::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
   std::string name = aStep->GetTrack()->GetVolume()->GetName();
   std::string nbr ;
   size_t found;
   found=name.find("Plastic");
   found = found + 7;
   
   int numberOfCharacterInDetectorNumber = name.length() - (int)found  ;

	for( int i = found ; i < found + numberOfCharacterInDetectorNumber ; i++ )
			nbr += name[i] ; 
		
   G4int DetNbr = atoi( nbr.c_str() ) ;


   G4double TOF  = aStep->GetPreStepPoint()->GetGlobalTime();
   G4double edep = aStep->GetTotalEnergyDeposit();
   if (edep < 100*keV) return FALSE;
   G4int  index = aStep->GetTrack()->GetTrackID();
   EvtMap->set(DetNbr + index, TOF);
   return TRUE;
}

void PSTOF::Initialize(G4HCofThisEvent* HCE)
{
   EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
         GetName());
   if (HCID < 0) {
      HCID = GetCollectionID(0);
   }
   HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void PSTOF::EndOfEvent(G4HCofThisEvent*)
{
   ;
}

void PSTOF::clear()
{
   EvtMap->clear();
}

void PSTOF::DrawAll()
{
   ;
}

void PSTOF::PrintAll()
{
   G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
   G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
   G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
