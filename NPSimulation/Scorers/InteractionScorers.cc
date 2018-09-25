/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : February 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer to record Hit energy,time and position               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "InteractionScorers.hh"
#include "G4UnitsTable.hh"
using namespace InteractionScorers ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<InteractionData>::iterator InteractionDataVector::find(const unsigned int& index){
 for(vector<InteractionData>::iterator it= m_Data.begin()  ; it !=m_Data.end() ; it++){
    if((*it).GetIndex()==index)
      return it;
  }
  return m_Data.end();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Interactions::PS_Interactions(G4String name,TInteractionCoordinates* Inter, int depth)  :G4VPrimitiveScorer(name, depth){
    m_Level = depth;
    m_InterractionCoordinates=Inter;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Interactions::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  static G4StepPoint* point;
  point = aStep->GetPreStepPoint();
  t_Energy = aStep->GetTotalEnergyDeposit();
  t_Time = point->GetGlobalTime();
  t_Position  = point->GetPosition();
  t_Index = aStep->GetTrack()->GetTrackID(); 
  vector<InteractionData>::iterator it;
  it = m_DataVector.find(t_Index); 
  if(it!=m_DataVector.end())
    it->Add(t_Energy);
  else
    m_DataVector.Set(t_Index,t_Energy,t_Time,t_Position.x(),t_Position.y(),t_Position.z(),t_Position.theta(),t_Position.phi());


  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Interactions::Initialize(G4HCofThisEvent*){
 // Clear is called by EventAction

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Interactions::EndOfEvent(G4HCofThisEvent*){
  unsigned int size = m_DataVector.size();

  for(unsigned int i = 0 ; i < size ; i++)
     m_InterractionCoordinates->SetInteraction(m_DataVector[i]->GetEnergy(),m_DataVector[i]->GetTime(),m_DataVector[i]->GetPositionX(),m_DataVector[i]->GetPositionY(),m_DataVector[i]->GetPositionZ(),m_DataVector[i]->GetTheta()/deg,m_DataVector[i]->GetPhi()/deg);

  clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Interactions::clear(){
  m_DataVector.clear();
}
