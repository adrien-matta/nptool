/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated Isotropic ion Source           *
 *  Very usefull to figure out Geometric Efficacity of experimental Set-Up   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++
#include<limits>

// G4 headers
#include "G4ParticleTable.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPS headers
#include "EventGeneratorIsotropic.hh"


// NPl headers
#include "RootOutput.h"
#include "NPNucleus.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::EventGeneratorIsotropic(){
  m_EnergyLow    =  0  ;
  m_EnergyHigh   =  0  ;
  m_x0           =  0  ;
  m_y0           =  0  ;
  m_z0           =  0  ;
  m_particle     =  NULL;
  m_ParticleStack = ParticleStack::getInstance();
  m_ExcitationEnergy = 0 ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::~EventGeneratorIsotropic(){
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::ReadConfiguration(string Path,int){
  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;
  
  bool ReadingStatus = false ;
  bool check_EnergyLow = false ;
  bool check_EnergyHigh = false ;
  bool check_HalfOpenAngleMin = false ;
  bool check_HalfOpenAngleMax = false ;
  bool check_x0 = false ;
  bool check_y0 = false ;
  bool check_z0 = false ;
  bool check_particle = false ;
  bool check_ExcitationEnergy = false ;
  
  ////////Reaction Setting needs///////
  string particle   ;
  //////////////////////////////////////////////////////////////////////////////////////////
  ifstream ReactionFile;
  ReactionFile.open(Path.c_str());
  
  if (ReactionFile.is_open()) {}
  else {
    return;
  }
  
  while (!ReactionFile.eof()) {
    //Pick-up next line
    getline(ReactionFile, LineBuffer);
    
    if (LineBuffer.compare(0, 9, "Isotropic") == 0) {
      G4cout << "///////////////////////////////////////////////////" << G4endl ;
      G4cout << "Isotropic Source Found" << G4endl ;
      ReadingStatus = true;}
    
    
    while (ReadingStatus)
      {
      ReactionFile >> DataBuffer;
      
      //Search for comment Symbol %
      if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      else if (DataBuffer == "EnergyLow=") {
        check_EnergyLow = true ;
        ReactionFile >> DataBuffer;
        m_EnergyLow = atof(DataBuffer.c_str()) * MeV;
        G4cout << "Minimum energy " << m_EnergyLow / MeV << " MeV" << G4endl;
      }
      
      else if (DataBuffer == "EnergyHigh=") {
        check_EnergyHigh = true ;
        ReactionFile >> DataBuffer;
        m_EnergyHigh = atof(DataBuffer.c_str()) * MeV;
        G4cout << "Maximum energy " << m_EnergyHigh / MeV << " MeV" << G4endl;
      }
      
      else if (DataBuffer == "HalfOpenAngleMin=") {
        check_HalfOpenAngleMin = true ;
        ReactionFile >> DataBuffer;
        m_HalfOpenAngleMin = atof(DataBuffer.c_str()) * deg;
        G4cout << "HalfOpenAngleMin " << m_HalfOpenAngleMin / deg << " degree" << G4endl;
      }
      
      else if (DataBuffer == "HalfOpenAngleMax=") {
        check_HalfOpenAngleMax = true ;
        ReactionFile >> DataBuffer;
        m_HalfOpenAngleMax = atof(DataBuffer.c_str()) * deg;
        G4cout << "HalfOpenAngleMax " << m_HalfOpenAngleMax / deg << " degree" << G4endl;
      }
      
      else if (DataBuffer == "x0=") {
        check_x0 = true ;
        ReactionFile >> DataBuffer;
        m_x0 = atof(DataBuffer.c_str()) * mm;
        G4cout << "x0 " << m_x0 << " mm" << G4endl;
      }
      
      else if (DataBuffer == "y0=") {
        check_y0 = true ;
        ReactionFile >> DataBuffer;
        m_y0 = atof(DataBuffer.c_str()) * mm;
        G4cout << "y0 " << m_y0 << " mm" << G4endl;
      }
      
      else if (DataBuffer == "z0=" ) {
        check_z0 = true ;
        ReactionFile >> DataBuffer;
        m_z0 = atof(DataBuffer.c_str()) * mm;
        G4cout << "z0 " << m_z0 << " mm" << G4endl;
      }
      
      else if (DataBuffer=="Particle=") {
        check_particle = true ;
        ReactionFile >> m_particleName;
        G4cout << "Particle : " << m_particleName << endl ;
        
      }
      
      else if (DataBuffer=="ExcitationEnergy=") {
        check_ExcitationEnergy = true ;
        ReactionFile >> DataBuffer;
        m_ExcitationEnergy = atof(DataBuffer.c_str()) * MeV;
        G4cout << "ExcitationEnergy : " << m_ExcitationEnergy << endl ;
      }
      
      //   If no isotropic Token and no comment, toggle out
      else
        {ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
      
      ///////////////////////////////////////////////////
      //   If all Token found toggle out
      if(    check_EnergyLow && check_EnergyHigh && check_HalfOpenAngleMin && check_HalfOpenAngleMax && check_x0 && check_y0 && check_z0 && check_particle && check_ExcitationEnergy)
        ReadingStatus = false ;
      
      }
    
  }
  
  if(    !check_EnergyLow || !check_EnergyHigh || !check_HalfOpenAngleMin || !check_HalfOpenAngleMax || !check_x0 || !check_y0 || !check_z0 || !check_particle )
    {cout << "WARNING : Token Sequence Incomplete, Isotropic definition could not be Fonctionnal" << endl ;}
  

  
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::GenerateEvent(G4Event*){
  
  if(m_particle==NULL){
    if(m_particleName!="gamma"){
      NPL::Nucleus* N = new NPL::Nucleus(m_particleName);
      m_particle = G4ParticleTable::GetParticleTable()->GetIon(N->GetZ(), N->GetA(),m_ExcitationEnergy);
      delete N;
    }
    else{
      m_particle =  G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    }
    
  }
  
  // Clear TInitialConditions
  G4double cos_theta_min   = cos(m_HalfOpenAngleMin);
  G4double cos_theta_max   = cos(m_HalfOpenAngleMax);
  G4double cos_theta       = cos_theta_min + (cos_theta_max - cos_theta_min) * RandFlat::shoot();
  G4double theta           = acos(cos_theta)                                                   ;
  G4double phi             = RandFlat::shoot() * 2 * pi                                        ;
  G4double particle_energy = m_EnergyLow + RandFlat::shoot() * (m_EnergyHigh - m_EnergyLow)    ;
  
  // Direction of particle, energy and laboratory angle
  G4double momentum_x = sin(theta) * cos(phi)  ;
  G4double momentum_y = sin(theta) * sin(phi)  ;
  G4double momentum_z = cos(theta)             ;
  
  Particle particle(m_particle, theta,particle_energy,G4ThreeVector(momentum_x, momentum_y, momentum_z),G4ThreeVector(m_x0, m_y0, m_z0));
  
  m_ParticleStack->AddParticleToStack(particle);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::InitializeRootOutput(){
  
}
