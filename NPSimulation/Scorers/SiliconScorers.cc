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
 *  File old the scorer specific to the Sharc Detector                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new type of scorer is aim to become the standard for DSSD,SSSD and   *
 * PAD detector (any Silicon Detector)                                       *
 * Addition for pixel silicon detecters e.g. SAGE & SPEDE by Daniel Cox      *
 *****************************************************************************/
#include "SiliconScorers.hh"
#include "G4UnitsTable.hh"
using namespace SILICONSCORERS ;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Images::PS_Silicon_Images(G4String name, string imageFront,string imageBack,double scalingFront, double scalingBack, double centerOffsetX,double centerOffsetY,unsigned int ignoreValue, G4int depth)  :G4VPrimitiveScorer(name, depth),HCID(-1){
    m_ImageFront = new NPL::Image(imageFront,scalingFront,scalingFront);
    m_ImageBack  = new NPL::Image(imageBack,scalingBack,scalingBack);
    m_ScalingFront = scalingFront;
    m_ScalingBack  = scalingBack;
    m_CenterOffsetX = centerOffsetX;
    m_CenterOffsetY = centerOffsetY;
    m_IgnoreValue = ignoreValue;
    m_Level = depth;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Images::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, PixelFront and PixelBack
  t_Energy = aStep->GetTotalEnergyDeposit();
  t_Time = aStep->GetPreStepPoint()->GetGlobalTime();

  t_DetectorNbr = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  t_Position  = aStep->GetPreStepPoint()->GetPosition();
  
  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  // N.B: Theta and phi are calculated with respect to (0,0,0)
  t_X = t_Position.x();
  t_Y = t_Position.y();
  t_Z = t_Position.z();
  t_Theta = t_Position.theta();
  t_Phi = t_Position.phi();
  // transforming the position to the local volume
  t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);
  t_PixelFront = m_ImageFront->GetPixelAtCoordinate(t_Position.x(),t_Position.y());
  t_PixelBack = m_ImageBack->GetPixelAtCoordinate(t_Position.x(),t_Position.y());

  // If front and back are in inactive part of the wafer,
  // nothing is added to the map
  if(t_PixelFront == m_IgnoreValue && t_PixelBack == m_IgnoreValue)
    return FALSE;

  t_Index =  aStep->GetTrack()->GetTrackID() + t_DetectorNbr * 1e3 ;
  // Check if the particle has interact before, if yes, add up the energies.
  map<unsigned int, double>::iterator it;
  it= m_Energy.find(t_Index);
  if(it!=m_Energy.end()){
    double dummy = it->second;
    t_Energy+=dummy;
  }

  m_Energy[t_Index] = t_Energy; 
  m_Time[t_Index] = t_Time;
  m_DetectorNbr[t_Index] = t_DetectorNbr;
  m_PixelFront[t_Index] = t_PixelFront;
  m_PixelBack[t_Index] = t_PixelBack;
  m_X[t_Index] = t_X;
  m_Y[t_Index] = t_Y;
  m_Z[t_Index] = t_Z;
  m_Theta[t_Index] = t_Theta;
  m_Phi[t_Index] = t_Phi;
  

  return TRUE;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Images::Initialize(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Images::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Images::clear(){
    m_Energy.clear();
    m_Time.clear();
    m_DetectorNbr.clear();
    m_PixelFront.clear();
    m_PixelBack.clear();
    m_X.clear();
    m_Y.clear();
    m_Z.clear();
    m_Theta.clear();
    m_Phi.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<unsigned int> PS_Silicon_Images::GetIndexes(){
    vector<unsigned int > indexes;
    map<unsigned int , double>::iterator it;
    for(it=m_Energy.begin(); it!=m_Energy.end();it++)
        indexes.push_back(it->first);
    
    return indexes;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
unsigned int PS_Silicon_Images::GetPixelFront(unsigned int index){
    return  m_PixelFront[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
unsigned int PS_Silicon_Images::GetPixelBack(unsigned int index){
    return m_PixelBack[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Images::GetARGBFront(unsigned int index,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b){
    unsigned int Info =m_PixelFront[index];
    a = (Info>>24)&0xff;
    r = (Info>>16)&0xff;
    g = (Info>>8)&0xff;
    b = (Info>>0)&0xff;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Images::GetARGBBack(unsigned int index,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b){
    unsigned int Info =m_PixelBack[index];
    
    a = (Info>>24)&0xff;
    r = (Info>>16)&0xff;
    g = (Info>>8)&0xff;
    b = (Info>>0)&0xff;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetEnergy(unsigned int index){
    return m_Energy[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetTime(unsigned int index){
    return m_Time[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
unsigned int PS_Silicon_Images::GetDetectorNbr(unsigned int index){
    return m_DetectorNbr[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetX(unsigned int index){
    return m_X[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetY(unsigned int index){
    return m_Y[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetZ(unsigned int index){
    return m_Z[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetTheta(unsigned int index){
    return m_Theta[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double PS_Silicon_Images::GetPhi(unsigned int index){
    return m_Phi[index];
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Rectangle::PS_Silicon_Rectangle(G4String name,G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4int depth,G4String axis)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    m_StripPlaneLength = StripPlaneLength;
    m_StripPlaneWidth = StripPlaneWidth;
    m_NumberOfStripLength = NumberOfStripLength;
    m_NumberOfStripWidth = NumberOfStripWidth;
    m_StripPitchLength = m_StripPlaneLength / m_NumberOfStripLength;
    m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
    m_Level = Level;
    m_Axis=axis;
    
    m_Position = G4ThreeVector(-1000,-1000,-1000);
    m_DetectorNumber = -1;
    m_StripLengthNumber = -1;
    m_StripWidthNumber = -1;
    m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Rectangle::~PS_Silicon_Rectangle(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Rectangle::ProcessHits(G4Step* aStep, G4TouchableHistory*){
    
    // contain Energy Time, DetNbr, StripFront and StripBack
    G4double* Infos = new G4double[10];
    Infos[0]  = aStep->GetTotalEnergyDeposit();
    Infos[1]  = aStep->GetPreStepPoint()->GetGlobalTime();
    
    m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
    m_Position  = aStep->GetPreStepPoint()->GetPosition();
    
    // Interaction coordinates (used to fill the InteractionCoordinates branch)
    Infos[2] = m_Position.x();
    Infos[3] = m_Position.y();
    Infos[4] = m_Position.z();
    Infos[5] = m_Position.theta();
    Infos[6] = m_Position.phi();
    
    m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
    
    if(m_Axis=="xy"){
        m_StripLengthNumber = (int)((m_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
        m_StripWidthNumber = (int)((m_Position.y() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
    }
    else if(m_Axis=="yz"){
        m_StripLengthNumber = (int)((m_Position.y() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
        m_StripWidthNumber = (int)((m_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
    }
    else if(m_Axis=="xz"){
        m_StripLengthNumber = (int)((m_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
        m_StripWidthNumber = (int)((m_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
    }
    
    //Rare case where particle is close to edge of silicon plan
    if (m_StripLengthNumber > m_NumberOfStripLength) m_StripLengthNumber = m_NumberOfStripLength;
    if (m_StripWidthNumber > m_NumberOfStripWidth) m_StripWidthNumber = m_NumberOfStripWidth;
    
    Infos[7] = m_DetectorNumber;
    Infos[8] = m_StripLengthNumber;
    Infos[9] = m_StripWidthNumber;
    
    m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripLengthNumber * 1e6 + m_StripWidthNumber * 1e9;
    
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
void PS_Silicon_Rectangle::Initialize(G4HCofThisEvent* HCE){
    EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
    if (HCID < 0) {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::clear(){
    std::map<G4int, G4double**>::iterator    MapIterator;
    for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
        delete *(MapIterator->second);
    }

    EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Rectangle::PrintAll(){
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
    G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
    G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Annular::PS_Silicon_Annular(G4String name,G4int Level, G4double StripPlaneInnerRadius, G4double StripPlaneOuterRadius, G4double PhiStart,G4double PhiStop, G4int NumberOfStripRing,G4int NumberOfStripSector,G4int NumberOfQuadrant,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    
    m_StripPlaneInnerRadius = StripPlaneInnerRadius;
    m_StripPlaneOuterRadius = StripPlaneOuterRadius;
    m_PhiStart = PhiStart;
    m_PhiStop = PhiStop;
    m_NumberOfStripRing = NumberOfStripRing;
    m_NumberOfStripSector = NumberOfStripSector;
    m_NumberOfStripQuadrant  = NumberOfQuadrant;
    m_StripPitchRing =  (m_StripPlaneOuterRadius-m_StripPlaneInnerRadius)/m_NumberOfStripRing;
    m_StripPitchSector = (m_PhiStop-m_PhiStart)/m_NumberOfStripSector;
    m_StripPitchQuadrant = (m_PhiStop-m_PhiStart)/m_NumberOfStripQuadrant;
    m_Level = Level;
    
    m_Position = G4ThreeVector(-1000,-1000,-1000);
    m_uz = G4ThreeVector(0,0,1);
    m_DetectorNumber = -1;
    m_StripRingNumber  = -1;
    m_StripSectorNumber = -1;
    m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Annular::~PS_Silicon_Annular(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Annular::ProcessHits(G4Step* aStep, G4TouchableHistory*){
    // contain Energy Time, DetNbr, StripFront and StripBack
    G4double* Infos = new G4double[11];
    Infos[0] = aStep->GetTotalEnergyDeposit();
    
    Infos[1] = aStep->GetPreStepPoint()->GetGlobalTime();
    
    m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
    m_Position = aStep->GetPreStepPoint()->GetPosition();
    
    // Interaction coordinates (used to fill the InteractionCoordinates branch)
    Infos[2] = m_Position.x();
    Infos[3] = m_Position.y();
    Infos[4] = m_Position.z();
    Infos[5] = m_Position.theta();
    Infos[6] = m_Position.phi();
    
    m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
    m_StripRingNumber = (int) ((m_Position.rho() - m_StripPlaneInnerRadius) / m_StripPitchRing ) + 1 ;
    
    double phi = m_Position.phi();

    m_StripSectorNumber   = (int) ((phi - m_PhiStart)  / m_StripPitchSector ) + 1 ;
    m_StripQuadrantNumber = (int) ((phi - m_PhiStart)  /m_StripPitchQuadrant) + 1 ;
    
    //Rare case where particle is close to edge of silicon plan
    if (m_StripRingNumber > m_NumberOfStripRing) m_StripRingNumber = m_NumberOfStripRing;
    if (m_StripSectorNumber > m_NumberOfStripSector) m_StripSectorNumber = m_NumberOfStripSector;
    if (m_StripQuadrantNumber > m_NumberOfStripQuadrant) m_StripQuadrantNumber = m_NumberOfStripQuadrant;
    
    Infos[7] = m_DetectorNumber;
    Infos[8] = m_StripRingNumber;
    Infos[9] = m_StripSectorNumber;
    Infos[10] = m_StripQuadrantNumber;
    
    m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripRingNumber * 1e6 + m_NumberOfStripSector * 1e9;
    
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
void PS_Silicon_Annular::Initialize(G4HCofThisEvent* HCE){
    EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
    if (HCID < 0) {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::clear(){
    std::map<G4int, G4double**>::iterator    MapIterator;
    for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
        delete *(MapIterator->second);
    }
    
    EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Annular::PrintAll(){
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
    G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
    G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Resistive::PS_Silicon_Resistive(G4String name,G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripWidth,G4int depth)
:G4VPrimitiveScorer(name, depth),HCID(-1){
    m_StripPlaneLength = StripPlaneLength;
    m_StripPlaneWidth = StripPlaneWidth;
    m_NumberOfStripWidth = NumberOfStripWidth;
    m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
    m_Level = Level;
    
    m_Position = G4ThreeVector(-1000,-1000,-1000);
    m_DetectorNumber = -1;
    m_StripWidthNumber = -1;
    m_Index = -1 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Resistive::~PS_Silicon_Resistive(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Resistive::ProcessHits(G4Step* aStep, G4TouchableHistory*){
    
    // contain Energy Total, E1, E2, Time, DetNbr,  and StripWidth
    G4double* EnergyAndTime = new G4double[10];
    
    EnergyAndTime[2] = aStep->GetPreStepPoint()->GetGlobalTime();
    
    m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
    m_Position  = aStep->GetPreStepPoint()->GetPosition();
    
    // Interaction coordinates (used to fill the InteractionCoordinates branch)
    EnergyAndTime[5] = m_Position.x();
    EnergyAndTime[6] = m_Position.y();
    EnergyAndTime[7] = m_Position.z();
    EnergyAndTime[8] = m_Position.theta();
    EnergyAndTime[9] = m_Position.phi();
    
    m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);
    
    m_StripWidthNumber = (int)((m_Position.x() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
    // m_StripWidthNumber = m_NumberOfStripWidth - m_StripWidthNumber + 1 ;
    
    // The energy is divided in two depending on the position
    // position along the resistive strip
    double P = (m_Position.z())/(0.5*m_StripPlaneLength);
    // Upstream Energy
    EnergyAndTime[0] = aStep->GetTotalEnergyDeposit()*(1+P)*0.5;
    
    // Downstream Energy
    EnergyAndTime[1] = aStep->GetTotalEnergyDeposit()-EnergyAndTime[0];
    
    EnergyAndTime[3] = m_DetectorNumber;
    EnergyAndTime[4] = m_StripWidthNumber;
    
    //Rare case where particle is close to edge of silicon plan
    if (m_StripWidthNumber > m_NumberOfStripWidth) m_StripWidthNumber = m_NumberOfStripWidth;
    
    m_Index =  aStep->GetTrack()->GetTrackID() + m_DetectorNumber * 1e3 + m_StripWidthNumber * 1e6;
    
    // Check if the particle has interact before, if yes, add up the energies.
    map<G4int, G4double**>::iterator it;
    it= EvtMap->GetMap()->find(m_Index);
    if(it!=EvtMap->GetMap()->end()){
        G4double* dummy = *(it->second);
        EnergyAndTime[0]+=dummy[0];
        EnergyAndTime[1]+=dummy[1];
    }
    EvtMap->set(m_Index, EnergyAndTime);
    return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::Initialize(G4HCofThisEvent* HCE){
    EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
    if (HCID < 0) {
        HCID = GetCollectionID(0);
    }
    HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::clear(){
    std::map<G4int, G4double**>::iterator    MapIterator;
    for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
        delete *(MapIterator->second);
    }
    
    EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::DrawAll(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Resistive::PrintAll(){
    G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
    G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
    G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Pixel::PS_Silicon_Pixel(G4String name,
		G4int Level,
		vector<G4double>* PixelInnerRadius,
		vector<G4double>* PixelOuterRadius,
		vector<G4double>* PixelPhiStart,
		vector<G4double>* PixelPhiStop,
		G4int depth)

	:G4VPrimitiveScorer(name, depth),HCID(-1){

		m_PixelInnerRadius = *PixelInnerRadius;
		m_PixelOuterRadius = *PixelOuterRadius;
		m_PixelPhiStart = *PixelPhiStart;
		m_PixelPhiStop = *PixelPhiStop;

		m_Level = Level;

		m_Position = G4ThreeVector(-1000,-1000,-1000);
		m_uz = G4ThreeVector(0,0,1);
		m_DetectorNumber = -1;
		m_PixelNumber  = -1;
		m_Index = -1 ;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Silicon_Pixel::~PS_Silicon_Pixel(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Silicon_Pixel::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  // contain Energy, time, detector number, pixel number, and interaction coordinates
  G4double* EnergyAndTime = new G4double[9];
  
  EnergyAndTime[0] = aStep->GetTotalEnergyDeposit();//Energy
  EnergyAndTime[1] = aStep->GetPreStepPoint()->GetGlobalTime();//Time

  m_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  m_Position = aStep->GetPreStepPoint()->GetPosition();

  // Interaction coordinates (used to fill the InteractionCoordinates branch)
  EnergyAndTime[2] = m_Position.x();
  EnergyAndTime[3] = m_Position.y();
  EnergyAndTime[4] = m_Position.z();
  EnergyAndTime[5] = m_Position.theta();
  EnergyAndTime[6] = m_Position.phi();

  EnergyAndTime[7] = m_DetectorNumber;//Detector number 

  //Translate to detector reference frame from global frame
  m_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(m_Position);

	//Find interaction radius
	double intX = abs(m_Position.x());	
	double intY = abs(m_Position.y());	
	double intRadius = sqrt(intX*intX+intY*intY);
	
	//Find interaction angle and convert to degrees
	double intPhi = atan2(m_Position.x(),m_Position.y());
	intPhi+=1*M_PI;
	intPhi*=180;
	intPhi/=M_PI;

	//TODO expand this to include inter-pixel dead layer

	if ( intRadius < m_PixelInnerRadius[0] )//sanity check
	{
		//cout << "Interaction point has smaller radius than defined detector\n";
		return TRUE;
	}
	else if (intRadius > m_PixelOuterRadius[int(m_PixelOuterRadius.size())-1] )//sanity check
	{
		//cout << "Interaction point has larger radius than defined detector\n";
		return TRUE;
	}
	else
	{
		//Find which pixel it is in
		for (int i = 0; i < int(m_PixelInnerRadius.size()); i++)
		{
			if (intRadius 	> m_PixelInnerRadius[i] &&
				intRadius 	< m_PixelOuterRadius[i] &&
				intPhi		> m_PixelPhiStart[i]	&&
				intPhi		< m_PixelPhiStop[i])
			{
				EnergyAndTime[8] = i;//Set pixel number
				break;
			}
		}
	}

//TODO why isn't the pixel number being passed back properly?!
  // Check if the particle has interact before, if yes, add up the energies.
  map<G4int, G4double**>::iterator it;
  it= EvtMap->GetMap()->find(m_Index);
  if(it!=EvtMap->GetMap()->end()){
    G4double* dummy = *(it->second);
    EnergyAndTime[0]+=dummy[0];
    EnergyAndTime[1]+=dummy[1];
	EnergyAndTime[8] = dummy[8];//Make sure first interaction point is the reported pixel
  }
  EvtMap->set(m_Index, EnergyAndTime);
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Pixel::Initialize(G4HCofThisEvent* HCE){
  EvtMap = new NPS::HitsMap<G4double*>(GetMultiFunctionalDetector()->GetName(), GetName());
  if (HCID < 0) {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Pixel::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Pixel::clear(){
  std::map<G4int, G4double**>::iterator    MapIterator;
  for (MapIterator = EvtMap->GetMap()->begin() ; MapIterator != EvtMap->GetMap()->end() ; MapIterator++){
    delete *(MapIterator->second);
  }

  EvtMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Pixel::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Silicon_Pixel::PrintAll(){
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl ;
  G4cout << " PrimitiveScorer " << GetName() << G4endl               ;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl     ;
}
