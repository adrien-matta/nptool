/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
using namespace TPCScorers;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<CathodeData>::iterator CathodeDataVector::find(const unsigned int& index){
  for(vector<CathodeData>::iterator it= m_Data.begin()  ; it !=m_Data.end() ; it++){
    if((*it).GetIndex()==index)
      return it;
  }
  return m_Data.end();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCCathode::PS_TPCCathode(G4String name,G4int depth)
:G4VPrimitiveScorer(name, depth){
    m_Level = depth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCCathode::~PS_TPCCathode(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_TPCCathode::ProcessHits(G4Step* aStep, G4TouchableHistory*){
 
 
    G4String PID = aStep->GetTrack()->GetDefinition()->GetParticleName();
    
    if(PID!="driftelectron"){
      return false;
    }
 
    // contain Energy Time, DetNbr, StripFront and StripBack
    t_Charge = aStep->GetTrack()->GetWeight();
    t_Time = aStep->GetPreStepPoint()->GetProperTime();
    
    t_Detector = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
    t_Position = aStep->GetPreStepPoint()->GetPosition();
    t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);
    
    t_PadX = (int)((t_Position.z()+128)/2);
    t_PadY = (int)((t_Position.x()+128)/2);
    
    vector<CathodeData>::iterator it;
    it = m_Data.find(CathodeData::CalculateIndex(t_Detector,t_PadX,t_PadY));
    if(it!= m_Data.end())
      it->Add(t_Charge); 
      
    else
      m_Data.Set(t_Charge,t_Time,t_Detector,t_PadX,t_PadY);

    return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::Initialize(G4HCofThisEvent*){
  m_Data.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::clear(){
   m_Data.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCCathode::PrintAll(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
