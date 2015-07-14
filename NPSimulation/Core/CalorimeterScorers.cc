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
 * PAD detector (any Silicon Detector)                                       *
 *****************************************************************************/
#include "CalorimeterScorers.hh"
#include "G4UnitsTable.hh"
using namespace CALORIMETERSCORERS ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Calorimeter::PS_Calorimeter(G4String name, vector<G4int> NestingLevel,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
  m_NestingLevel = NestingLevel;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Calorimeter::~PS_Calorimeter(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Calorimeter::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy, Time + as many copy number as nested volume
  unsigned int mysize = m_NestingLevel.size(); 
  G4double* Infos = new G4double[2+mysize];
  Infos[0] = aStep->GetTotalEnergyDeposit();
  Infos[1] = aStep->GetPreStepPoint()->GetGlobalTime();
  for(unsigned int i = 0 ; i < mysize ; i++){ 
    Infos[i+2] = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_NestingLevel[i]);
  }

  m_Index = 0 ;
  G4int multiplier = 1;
   for(unsigned int i = 0 ; i < mysize ; i++){ 
    m_Index+= aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_NestingLevel[i])*multiplier;
    multiplier*=10; 
  }
  
  // Check if the particle has interact before, if yes, add up the energies.
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
void PS_Calorimeter::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new G4THitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Calorimeter::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Calorimeter::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }
  
  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Calorimeter::DrawAll(){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Calorimeter::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}

