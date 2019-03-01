/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : february 2019                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for MINOS-TPC Drift Electron collector                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Count drift electrons per pads in MINOS                                    *
 *                                                                           *
 *****************************************************************************/
#include "CylinderTPCScorers.hh"
#include "G4UnitsTable.hh"
using namespace CylinderTPCScorers;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<AnodeData>::iterator AnodeDataVector::find(const unsigned int& Pad){
  for(vector<AnodeData>::iterator it= m_Data.begin()  ; it !=m_Data.end() ; it++){
    if((*it).GetPad()==Pad)
      return it;
  }
  return m_Data.end();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCAnode::PS_TPCAnode(G4String name,vector<G4int> NestingLevel,G4int depth)
:G4VPrimitiveScorer(name, depth){
    m_NestingLevel = NestingLevel;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_TPCAnode::~PS_TPCAnode(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_TPCAnode::ProcessHits(G4Step* aStep, G4TouchableHistory*){

    /* unsigned int mysize = m_NestingLevel.size(); */
    /* for (unsigned int i =0; i < mysize ; i++){ */
    /*     t_Level.push_back(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_NestingLevel[i])); */
    /* } */

    G4String PID = aStep->GetTrack()->GetDefinition()->GetParticleName();
    
    if(PID!="driftelectron"){
      return false;
    }
 
    G4int Pad = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0);


    // contain Enegy Time, DetNbr, StripFront and StripBack
    t_Charge = aStep->GetTrack()->GetWeight();
    t_DriftTime = aStep->GetPreStepPoint()->GetProperTime();
   
    // Convert Pad number to X,Y position 

        //For the case of replica pads
    G4int Ring = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);   
    int PadsPerRing[18]={144,152,156,164,172,176,184,192,196,204,212,216,224,228,236,244,248,256};  
    
    G4double R = (50.+ Ring*2.2)*mm;
    G4double dPhi= (2.*M_PI/PadsPerRing[Ring]);
    G4double Phi = Pad*dPhi;
    for(int i = 0; i < Ring; i++){
        Pad += PadsPerRing[i];
    }
    t_Pad = Pad;

        //For the case of parameterized pads
    /*
    t_Pad = Pad;
    G4int Ring = 0;
    if  (Pad<144){
        Ring = 0;}
    else if (144<=Pad && Pad<296){
        Ring = 1;}
    else if (296<=Pad && Pad<452){
        Ring = 2;}
    else if (452<=Pad && Pad<616){
        Ring = 3;}
    else if (616<=Pad && Pad<788){
        Ring = 4;}
    else if (788<=Pad && Pad<964){
        Ring = 5;}
   else if (964<=Pad && Pad<1148){
        Ring = 6;}
    else if (1148<=Pad && Pad<1340){
        Ring = 7;}
    else if (1340<=Pad && Pad<1536){
        Ring = 8;}
    else if (1536<=Pad && Pad<1740){
        Ring = 9;}
    else if (1740<=Pad && Pad<1952){
        Ring = 10;}
    else if (1952<=Pad && Pad<2168){
        Ring = 11;}
    else if (2168<=Pad && Pad<2392){
        Ring = 12;}
    else if (2392<=Pad && Pad<2620){
        Ring = 13;}
    else if (2620<=Pad && Pad<2856){
        Ring = 14;}
    else if (2856<=Pad && Pad<3100){
        Ring = 15;}
    else if (3100<=Pad && Pad<3348){
        Ring = 16;}
    else if (3348<=Pad && Pad<3604){
        Ring = 17;}

    G4double R = (50.+ Ring*2.2)*mm;
    G4double dPhi= (2*M_PI/PadsPerRing[Ring]);
    for(int i = 0; i < Ring; i++) {
        Pad -= PadsPerRing[Ring];
    }
    G4double Phi = Pad*dPhi;
    */
    

    t_X = R*cos(Phi);
    t_Y = R*sin(Phi);

    vector<AnodeData>::iterator it;
    it = m_Data.find(t_Pad);
    if(it!= m_Data.end())
      it->Add(t_Charge); 
    else
      m_Data.Set(t_Charge,t_DriftTime,t_Pad,t_X,t_Y);

    return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCAnode::Initialize(G4HCofThisEvent*){
  m_Data.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCAnode::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCAnode::clear(){
   m_Data.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCAnode::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_TPCAnode::PrintAll(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
