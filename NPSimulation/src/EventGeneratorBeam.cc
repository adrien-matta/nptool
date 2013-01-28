/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  This event Generator is used to simulated Radioactive Ion beam           *
 *  Emmitance, Energy spread and dispersion are taken into account           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++
#include <limits>

// G4 header
#include "G4ParticleTable.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPSimulation header
#include "EventGeneratorBeam.hh"
#include "Particle.hh"
// NPL header
#include "RootOutput.h"

using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorBeam::EventGeneratorBeam(){
  m_ParticleStack  = ParticleStack::getInstance();
  m_InitConditions = new TInitialConditions();
  m_Target         = NULL    ;
  m_particle       = NULL    ;
  m_Beam           = new NPL::Beam();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorBeam::~EventGeneratorBeam(){
  delete m_InitConditions;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void   EventGeneratorBeam::SetTarget(Target* Target){
  if(Target!=0){
    m_Target = Target;
  }
  
  // Set the target parameter for the internal event generator of m_Beam
  m_Beam->SetTargetSize(m_Target->GetTargetRadius());
  m_Beam->SetTargetAngle(m_Target->GetTargetAngle());
  m_Beam->SetTargetThickness(m_Target->GetTargetThickness());
  m_Beam->SetTargetZ(m_Target->GetTargetZ());
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::ReadConfiguration(string Path,int dump){
  dump=0;
  m_Beam->ReadConfigurationFile(Path);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::GenerateEvent(G4Event* anEvent){
  //--------------write the DeDx Table -------------------
  if( anEvent->GetEventID()==0){
    // Define the particle to be shoot
    m_particle = G4ParticleTable::GetParticleTable()->GetIon(m_Beam->GetNucleus()->GetZ(), m_Beam->GetNucleus()->GetA() , 0.);

    if(m_Target!=0 ){
      m_Target->WriteDEDXTable(m_particle ,0, m_Beam->GetEnergy()+4*m_Beam->GetSigmaEnergy());
    }
  }
  
  m_InitConditions->Clear();
  
  ///////////////////////////////////////////////////////////////////////
  ///// Calculate the incident beam direction as well as the vertex /////
  ///// of interaction in target and Energy Loss of the beam within /////
  ///// the target.                                                 /////
  ///////////////////////////////////////////////////////////////////////
  G4ThreeVector InterCoord;
  
  G4double Beam_theta, Beam_phi, FinalBeamEnergy, InitialBeamEnergy, x0, y0, z0, Beam_thetaX, Beam_phiY;

  m_Beam->GenerateRandomEvent(InitialBeamEnergy, x0, y0, z0, Beam_thetaX, Beam_phiY);
  G4double Xdir = sin(Beam_thetaX);
  G4double Ydir = sin(Beam_phiY);
  G4double Zdir = cos(Beam_thetaX) + cos(Beam_phiY);
  G4ThreeVector BeamDir = G4ThreeVector(Xdir,Ydir,Zdir)   ;
  
  Beam_theta = BeamDir.theta()    ;
  Beam_phi   = BeamDir.phi()      ;
  FinalBeamEnergy = m_Target->SlowDownBeam(m_particle, InitialBeamEnergy,z0,Beam_theta);
  ///////////////////////////////////////////////////////
  ///// Add the Beam particle to the particle Stack /////
  ///////////////////////////////////////////////////////
  
  Particle BeamParticle( m_particle,
                         FinalBeamEnergy,
                         BeamDir.unit(),
                         G4ThreeVector(x0,y0,z0),
                         1);
  m_ParticleStack->AddParticleToStack(BeamParticle);

  }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorBeam::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......