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
 *****************************************************************************/
#include "DSSDScorers.hh"
#include "G4UnitsTable.hh"
using namespace DSSDScorers ;
vector<DSSDData>::iterator DSSDDataVector::find(const unsigned int& index){
  for(vector<DSSDData>::iterator it= m_Data.begin()  ; it !=m_Data.end() ; it++){
    if((*it).GetIndex()==index)
      return it;
  }
  return m_Data.end();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Images::PS_Images(G4String name, string imageFront,string imageBack,double scalingFront, double scalingBack, double centerOffsetX,double centerOffsetY,unsigned int ignoreValue, G4int depth)  :G4VPrimitiveScorer(name, depth){
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
G4bool PS_Images::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, PixelFront and PixelBack
  t_Energy = aStep->GetTotalEnergyDeposit();
  t_Time = aStep->GetPreStepPoint()->GetGlobalTime();

  t_DetectorNbr = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  t_Position  = aStep->GetPreStepPoint()->GetPosition();

  // transforming the position to the local volume
  t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);
  t_PixelFront = m_ImageFront->GetPixelAtCoordinate(t_Position.x(),t_Position.y());
  t_PixelBack = m_ImageBack->GetPixelAtCoordinate(t_Position.x(),t_Position.y());

  // If front and back are in inactive part of the wafer,
  // nothing is added to the unordered_map
  if(t_PixelFront == m_IgnoreValue && t_PixelBack == m_IgnoreValue)
    return FALSE;


  // Check if the particle has interact before, if yes, add up the energies.
   vector<DSSDData>::iterator it;
  
  it= m_HitFront.find(DSSDData::CalculateIndex(t_PixelFront,t_DetectorNbr));
  if(it!=m_HitFront.end()){
    it->Add(t_Energy);
  }

  else{
    m_HitFront.Set(t_Energy,t_Time,t_PixelFront,t_DetectorNbr);
  }

  // Check if the particle has interact before, if yes, add up the energies.
  it= m_HitBack.find(DSSDData::CalculateIndex(t_PixelBack,t_DetectorNbr));
  if(it!=m_HitBack.end()){
    it->Add(t_Energy);
  }

  else{
    m_HitBack.Set(t_Energy,t_Time,t_PixelBack,t_DetectorNbr);
  }

  return TRUE;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Images::Initialize(G4HCofThisEvent*){
  clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Images::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Images::clear(){
  m_HitFront.clear();
  m_HitBack.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Images::GetARGBFront(unsigned int& i,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b){
  unsigned int Info = m_HitFront[i]->GetStrip();
  a = (Info>>24)&0xff;
  r = (Info>>16)&0xff;
  g = (Info>>8)&0xff;
  b = (Info>>0)&0xff;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Images::GetARGBBack(unsigned int& i,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b){
  unsigned int Info = m_HitBack[i]->GetStrip();
  a = (Info>>24)&0xff;
  r = (Info>>16)&0xff;
  g = (Info>>8)&0xff;
  b = (Info>>0)&0xff;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Rectangle::PS_Rectangle(G4String name,G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4int depth,G4String axis)
  :G4VPrimitiveScorer(name, depth){
    m_StripPlaneLength = StripPlaneLength;
    m_StripPlaneWidth = StripPlaneWidth;
    m_NumberOfStripLength = NumberOfStripLength;
    m_NumberOfStripWidth = NumberOfStripWidth;
    m_StripPitchLength = m_StripPlaneLength / m_NumberOfStripLength;
    m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
    m_Level = Level;
    if(axis=="xy")
      m_Axis=ps_xy;
    else if(axis=="yz")
      m_Axis=ps_yz;
    else if(axis=="xz")
      m_Axis=ps_xz;


  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Rectangle::~PS_Rectangle(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Rectangle::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Time, DetNbr, StripFront and StripBack
  t_Energy  = aStep->GetTotalEnergyDeposit();
  t_Time  = aStep->GetPreStepPoint()->GetGlobalTime();

  t_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  t_Position  = aStep->GetPreStepPoint()->GetPosition();

  t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);

  if(m_Axis==ps_xy){
    t_StripLengthNumber = (int)((t_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    t_StripWidthNumber = (int)((t_Position.y() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }
  else if(m_Axis==ps_yz){
    t_StripLengthNumber = (int)((t_Position.y() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    t_StripWidthNumber = (int)((t_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }
  else if(m_Axis==ps_xz){
    t_StripLengthNumber = (int)((t_Position.x() + m_StripPlaneLength / 2.) / m_StripPitchLength ) + 1  ;
    t_StripWidthNumber = (int)((t_Position.z() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;
  }

  //Rare case where particle is close to edge of silicon plan
  if (t_StripLengthNumber > m_NumberOfStripLength) t_StripLengthNumber = m_NumberOfStripLength;
  if (t_StripWidthNumber > m_NumberOfStripWidth) t_StripWidthNumber = m_NumberOfStripWidth;

  // Check if the particle has interact before, if yes, add up the energies.
  vector<DSSDData>::iterator it;
  // Length
  it = m_HitLength.find(DSSDData::CalculateIndex(t_StripLengthNumber,t_DetectorNumber));
  if(it!=m_HitLength.end()){
    it->Add(t_Energy);
  }
  else
    m_HitLength.Set(t_Energy,t_Time,t_StripLengthNumber,t_DetectorNumber);
  // Width
  it = m_HitWidth.find(DSSDData::CalculateIndex(t_StripWidthNumber,t_DetectorNumber));
  if(it!=m_HitWidth.end()){
    it->Add(t_Energy);
  }
  else
    m_HitWidth.Set(t_Energy,t_Time,t_StripWidthNumber,t_DetectorNumber);


return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Rectangle::Initialize(G4HCofThisEvent*){
  clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Rectangle::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Rectangle::clear(){
  m_HitLength.clear();
  m_HitWidth.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Rectangle::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Rectangle::PrintAll(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Annular::PS_Annular(G4String name,G4int Level, G4double StripPlaneInnerRadius, G4double StripPlaneOuterRadius, G4double PhiStart,G4double PhiStop, G4int NumberOfStripRing,G4int NumberOfStripSector,G4int NumberOfQuadrant,G4int depth)
  :G4VPrimitiveScorer(name, depth){

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

    m_uz = G4ThreeVector(0,0,1);
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Annular::~PS_Annular(){
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Annular::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  t_Energy = aStep->GetTotalEnergyDeposit();

  t_Time = aStep->GetPreStepPoint()->GetGlobalTime();

  t_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  t_Position = aStep->GetPreStepPoint()->GetPosition();

  //Transform into local coordinates
  t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);
  t_StripRingNumber = (int) ((t_Position.rho() - m_StripPlaneInnerRadius) / m_StripPitchRing ) + 1 ;

  // phi() from G4-CLHEP method return azimuth between [-pi;pi]
  // we need it in [0;2pi] to calculate sector nbr in [1,NSectors]
  // only add 360 if the value is negative
  double phi = (t_Position.phi()<0)?  t_Position.phi()+2*pi : t_Position.phi() ;

  // factor out the extra 360 degrees before strip/quad calculation
  t_StripSectorNumber   = (int) ( fmod((phi - m_PhiStart),2*pi)  / m_StripPitchSector  ) + 1 ;
  t_StripQuadrantNumber = (int) ( fmod((phi - m_PhiStart),2*pi)  / m_StripPitchQuadrant) + 1 ;

  //Rare case where particle is close to edge of silicon plan
  if (t_StripRingNumber > m_NumberOfStripRing) t_StripRingNumber = m_NumberOfStripRing;
  if (t_StripSectorNumber > m_NumberOfStripSector) t_StripSectorNumber = m_NumberOfStripSector;
  if (t_StripQuadrantNumber > m_NumberOfStripQuadrant) t_StripQuadrantNumber = m_NumberOfStripQuadrant;

  vector<DSSDData>::iterator it;
  // Ring
  it = m_HitRing.find(DSSDData::CalculateIndex(t_StripRingNumber,t_DetectorNumber));
  if(it!=m_HitRing.end()){
    it->Add(t_Energy);
  }
  else
    m_HitRing.Set(t_Energy,t_Time,t_StripRingNumber,t_DetectorNumber);

  //Sector
  it = m_HitSector.find(DSSDData::CalculateIndex(t_StripSectorNumber,t_DetectorNumber));
  if(it!=m_HitSector.end()){
    it->Add(t_Energy);
  }
  else
    m_HitSector.Set(t_Energy,t_Time,t_StripSectorNumber,t_DetectorNumber);

  //Quadrant
  it = m_HitQuadrant.find(DSSDData::CalculateIndex(t_StripQuadrantNumber,t_DetectorNumber));
  if(it!=m_HitQuadrant.end()){
    it->Add(t_Energy);
  }
  else
    m_HitQuadrant.Set(t_Energy,t_Time,t_StripQuadrantNumber,t_DetectorNumber);

  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Annular::Initialize(G4HCofThisEvent*){
  clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Annular::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Annular::clear(){
  m_HitRing.clear();
  m_HitSector.clear();
  m_HitQuadrant.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Annular::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Annular::PrintAll(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Resistive::PS_Resistive(G4String name,G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripWidth,G4int depth)
  :G4VPrimitiveScorer(name, depth){
    m_StripPlaneLength = StripPlaneLength;
    m_StripPlaneWidth = StripPlaneWidth;
    m_NumberOfStripWidth = NumberOfStripWidth;
    m_StripPitchWidth = m_StripPlaneWidth / m_NumberOfStripWidth;
    m_Level = Level;

    t_Position = G4ThreeVector(-1000,-1000,-1000);
    t_DetectorNumber = -1;
    t_StripWidthNumber = -1;
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PS_Resistive::~PS_Resistive(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PS_Resistive::ProcessHits(G4Step* aStep, G4TouchableHistory*){

  // contain Energy Total, E1, E2, Time, DetNbr,  and StripWidth
  t_Energy = aStep->GetTotalEnergyDeposit(); 
  t_Time = aStep->GetPreStepPoint()->GetGlobalTime();
  
  t_DetectorNumber = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(m_Level);
  t_Position = aStep->GetPreStepPoint()->GetPosition();
  t_Position = aStep->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(t_Position);

  t_StripWidthNumber = (int)((t_Position.x() + m_StripPlaneWidth / 2.) / m_StripPitchWidth ) + 1  ;

  // The energy is divided in two depending on the position
  // position along the resistive strip
  double P = (t_Position.z())/(0.5*m_StripPlaneLength);
  // Energy
  t_EnergyUp = aStep->GetTotalEnergyDeposit()*(1+P)*0.5; 
  t_EnergyDown = t_Energy-t_EnergyUp; 

  //Rare case where particle is close to edge of silicon plan
  if (t_StripWidthNumber > m_NumberOfStripWidth) t_StripWidthNumber = m_NumberOfStripWidth;


  // Up
  vector<DSSDData>::iterator it;
  it = m_HitUp.find(DSSDData::CalculateIndex(t_DetectorNumber,t_StripWidthNumber));
  if(it!=m_HitUp.end())
    it->Add(t_EnergyUp);
  else
    m_HitUp.Set(t_EnergyUp,t_Time,t_DetectorNumber,t_StripWidthNumber);
    
  // Down
  it = m_HitDown.find(DSSDData::CalculateIndex(t_DetectorNumber,t_StripWidthNumber));
  if(it!=m_HitDown.end())
    it->Add(t_EnergyDown);
  else
    m_HitDown.Set(t_EnergyDown,t_Time,t_DetectorNumber,t_StripWidthNumber);
  
   // Back
  it = m_HitBack.find(DSSDData::CalculateIndex(t_DetectorNumber,t_StripWidthNumber));
  if(it!=m_HitBack.end())
    it->Add(t_Energy);
  else
    m_HitBack.Set(t_Energy,t_Time,t_DetectorNumber,1);
  
  
  
  return TRUE;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Resistive::Initialize(G4HCofThisEvent* ){
  clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Resistive::EndOfEvent(G4HCofThisEvent*){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Resistive::clear(){
  m_HitUp.clear();
  m_HitDown.clear();
  m_HitBack.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Resistive::DrawAll(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PS_Resistive::PrintAll(){
}
