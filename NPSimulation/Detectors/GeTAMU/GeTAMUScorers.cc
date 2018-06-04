/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

#include <algorithm>
#include "GeTAMUScorers.hh"
#include "G4UnitsTable.hh"
using namespace std;
using namespace CLHEP;
using namespace GETAMUSCORERS;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_GeTAMU::PS_GeTAMU(G4String name,G4int Level,G4int depth)
  :G4VPrimitiveScorer(name, depth),HCID(-1){
	m_Position = G4ThreeVector(-1000,-1000,-1000);
  m_LocalPosition = G4ThreeVector(-1000,-1000,-1000);
	m_CloverNumber  = -1;
	m_CrystalNumber = -1;
	m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_GeTAMU::~PS_GeTAMU(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_GeTAMU::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  // contain Energy Time, DetNbr, StripFront and StripBack
  G4double* Infos = new G4double[12];
  Infos[0] = aStep->GetTotalEnergyDeposit();
  Infos[1] = aStep->GetPreStepPoint()->GetGlobalTime();

  m_CrystalNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  m_Position  = aStep->GetPreStepPoint()->GetPosition();
  //from local position one should be able to calculate the segments
  m_LocalPosition = aStep->GetPreStepPoint()->GetTouchableHandle()
                         ->GetHistory()->GetTopTransform().TransformPoint(m_Position);

  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  Infos[2] = m_Position.x();
  Infos[3] = m_Position.y();
  Infos[4] = m_Position.z();
  Infos[5] = m_Position.theta();
  Infos[6] = m_Position.phi();

  //This will be used in the future to calculate the segments
  Infos[9] = m_LocalPosition.x();
  Infos[10] = m_LocalPosition.y();
  Infos[11] = m_LocalPosition.z();

	// Find Clover with Closest central phi angle
	G4double phiDeg = Infos[6]*180/CLHEP::pi;
	G4double distance[4] = { fabs(phiDeg - 45) , fabs(phiDeg - 135) , fabs(phiDeg - -135) , fabs(phiDeg - -45) };
	m_CloverNumber = std::min_element(distance, distance+4) - distance + 1;
	
	Infos[7] = m_CloverNumber;
  Infos[8] = m_CrystalNumber;

	m_Index = m_CloverNumber * 1 + m_CrystalNumber * 1e3;

  // Sum up all hits in the SAME CORE AND CLOVER
  map<G4int, G4double**>::iterator it;
  it= EvtMap->GetMap()->find(m_Index);
  if(it!=EvtMap->GetMap()->end()){
    G4double* dummy = *(it->second);
    Infos[0]+=dummy[0];
  }
	
  EvtMap->set(m_Index, Infos);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_GeTAMU::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_GeTAMU::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_GeTAMU::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }

  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_GeTAMU::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_GeTAMU::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
