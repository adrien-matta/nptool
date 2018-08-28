/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : June 2018                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for TPC Drift Electron collector                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new type of scorer to count drift electron in the cathode            *
 *                                                                           *
 *****************************************************************************/
#include "TPCScorers.hh"
#include "G4UnitsTable.hh"
using namespace TPCSCORERS ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCCathode::PS_TPCCathode(G4String name,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    m_Index = -1 ;
    m_Level = depth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCCathode::~PS_TPCCathode(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_TPCCathode::ProcessHits(G4Step* aStep, G4TouchableHistory*){
    
    // contain Energy Time, DetNbr, StripFront and StripBack
    G4double* Infos = new G4double[10];
    Infos[0] = 0;
    Infos[1] = aStep->GetPreStepPoint()->GetProperTime();
    
    m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
    m_Position  = aStep->GetPreStepPoint()->GetPosition();
    
    // Interaction coordinates (used to fill the InteractionCoordinates branch)
    Infos[2] = m_Position.x();
    Infos[3] = m_Position.y();
    Infos[4] = m_Position.z();
    Infos[5] = m_Position.theta();
    Infos[6] = m_Position.phi();
    Infos[7] = m_DetectorNumber;
    
    
    G4String PID = aStep->GetTrack()->GetDefinition()->GetParticleName();
    
    m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
    
    
    Infos[8] = (int)((m_Position.z()+128)/2);
    Infos[9] = (int)((m_Position.x()+128)/2);
    
    m_Index = Infos[8] + 128*Infos[9] ;
    
    
    if(PID=="driftelectron"){
        Infos[0] = aStep->GetTrack()->GetWeight();
    }
    
    // Check if the particle has interact before, if yes, add up the number of electron.
    map<G4int, G4double**>::iterator it;
    it= EvtMap->GetMap()->find(m_Index);
    if(it!=EvtMap->GetMap()->end()){
        G4double* dummy = *(it->second);
        Infos[0]+=dummy[0];
        Infos[1]=dummy[1];
    }
    
    EvtMap->set(m_Index, Infos);
    return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::Initialize(G4HCofThisEvent* HCE){
    EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
    if (HCID < 0) {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::clear(){
    std::map<G4int, G4double**>::iterator    MapIterator;
    for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
        delete *(MapIterator->second);
    }
    
    EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::PrintAll(){
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
    G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
    G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
