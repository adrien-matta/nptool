/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold some of the General Scorer, shared by different detector.*
 *  Those scorer could be a could basis for your own scorer                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using*
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "MDMScorer.hh"
#include "G4UnitsTable.hh"
using namespace CLHEP;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//   The following function is used in many scorer. following the Detector Volume Nomenclature
//   DetectorNameX_SubPart_SubPart
//   where X stand for the detector number.

namespace {
  int PickUpDetectorNumber(G4Step* aStep, std::string DetName)
  {
    std::string name = aStep->GetTrack()->GetVolume()->GetName();
    std::string nbr;
    size_t start, end;
    
    start = name.find(DetName) + DetName.length();
    end   = name.find("_");
    
    int numberOfCharacterInDetectorNumber = (int)end - (int)start;
    
    for (unsigned int i = start; i < start + numberOfCharacterInDetectorNumber; i++)
      nbr += name[i];
    
    return atoi(nbr.c_str());
  } }


MDMScorer::MDMScorer(G4String name, G4String VolumeName, G4int depth)
  : G4VPrimitiveScorer(name, depth), HCID(-1)
{
  m_VolumeName = VolumeName;
}

MDMScorer::~MDMScorer()
{
}

G4bool MDMScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  int DetNumber = PickUpDetectorNumber(aStep, m_VolumeName) ;
  G4int  index  = aStep->GetTrack()->GetTrackID();
    
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double M = aStep->GetPreStepPoint()->GetMass();
	// This gives the correct charge for a fully stripped ion (proton number)
  G4double Q = aStep->GetTrack()->GetDynamicParticle()->GetParticleDefinition()->GetPDGCharge();
	// THIS gives something strange (non-integers that change every step - some sort of stripping?)
	//aStep->GetPreStepPoint()->GetCharge();
  G4ThreeVector POS  = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector MOM  = aStep->GetPreStepPoint()->GetMomentumDirection();
  
  MDMScorer::Infos info;
  info.Edep   = edep/MeV;
  info.Mass   = M;
  info.Charge = Q;
  info.Pos    = POS;
  info.Mom    = MOM;
  // NOTE: add() calls += operator
  EvtMap->add(index+DetNumber, info);
  return TRUE;
}

void MDMScorer::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new NPS::HitsMap<MDMScorer::Infos>
    (GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void MDMScorer::EndOfEvent(G4HCofThisEvent*)
{
  ;
}

void MDMScorer::clear()
{
  EvtMap->clear();
}

void MDMScorer::DrawAll()
{
  ;
}

void MDMScorer::PrintAll()
{
  // G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  // G4cout << " PrimitiveScorer " << GetName() << G4endl;
  // G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  // std::map<G4int, G4double*>::iterator itr = EvtMap->GetMap()->begin();
  // for (; itr != EvtMap->GetMap()->end(); itr++) {
  //     G4cout << "  copy no.: " << itr->first
  //     << "  energy deposit: " << G4BestUnit(*(itr->second), "Energy")
  //     << G4endl;
  // }
}
