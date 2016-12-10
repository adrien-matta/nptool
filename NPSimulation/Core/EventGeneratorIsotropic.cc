/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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
#include "G4IonTable.hh"
// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPS headers
#include "EventGeneratorIsotropic.hh"

// NPL headers
#include "RootOutput.h"
#include "NPNucleus.h"
#include "NPOptionManager.h"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::EventGeneratorIsotropic(){
  m_EnergyLow    =  0  ;
  m_EnergyHigh   =  0  ;
  m_x0           =  0  ;
  m_y0           =  0  ;
  m_z0           =  0  ;
  m_SigmaX       = 0;
  m_SigmaY       = 0;
  m_ParticleStack = ParticleStack::getInstance();
  m_particle = NULL;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::~EventGeneratorIsotropic(){
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Isotropic");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Isotropic reaction found " << endl; 

  vector<string> token = {"EnergyLow","EnergyHigh","HalfOpenAngleMin","HalfOpenAngleMax","x0","y0","z0","Particle"};
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      m_EnergyLow         =blocks[i]->GetDouble("EnergyLow","MeV");
      m_EnergyHigh        =blocks[i]->GetDouble("EnergyHigh","MeV");
      m_HalfOpenAngleMin  =blocks[i]->GetDouble("HalfOpenAngleMin","deg");
      m_HalfOpenAngleMax  =blocks[i]->GetDouble("HalfOpenAngleMax","deg");
      m_x0                =blocks[i]->GetDouble("x0","mm");
      m_y0                =blocks[i]->GetDouble("y0","mm");
      m_z0                =blocks[i]->GetDouble("z0","mm");
      vector<string> particleName =blocks[i]->GetVectorString("Particle");
      for(unsigned int j = 0 ; j < particleName.size() ; j++){
        if(particleName[j]=="proton"){ m_particleName.push_back("1H")  ;} 
        else if(particleName[j]=="deuton"){ m_particleName.push_back("2H")  ; }
        else if(particleName[j]=="triton"){ m_particleName.push_back("3H")  ; }
        else if(particleName[j]=="3He" || particleName[j]=="He3") { m_particleName.push_back("3He") ; }
        else if(particleName[j]=="alpha") { m_particleName.push_back("4He") ; }
        else if(particleName[j]=="gamma") { m_particleName.push_back("gamma") ;}
        else if(particleName[j]=="neutron") {m_particleName.push_back("neutron") ;}
      }

      if(blocks[i]->HasToken("ExcitationEnergy"))
        m_ExcitationEnergy =blocks[i]->GetVectorDouble("ExcitationEnergy","MeV");
        
      if(blocks[i]->HasToken("SigmaX"))
        m_SigmaX=blocks[i]->GetDouble("SigmaX","mm");
      if(blocks[i]->HasToken("SigmaY"))
        m_SigmaX=blocks[i]->GetDouble("SigmaY","mm");
      if(blocks[i]->HasToken("Multiplicity"))
        m_Multiplicty=blocks[i]->GetVectorInt("Multiplicity");
    }
    else{
      cout << "ERROR: check your input file formatting \033[0m" << endl; 
      exit(1);
    }
  }

  if(m_ExcitationEnergy.size()==0)
    m_ExcitationEnergy.resize(m_particleName.size(),0);
  if(m_Multiplicty.size()==0)
    m_Multiplicty.resize(m_particleName.size(),1);

}
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void EventGeneratorIsotropic::GenerateEvent(G4Event*){

    for(unsigned int p=0; p<m_particleName.size(); p++){
      for(int i=0; i<m_Multiplicty[p]; i++){
        m_particle=NULL;
        if(m_particle==NULL){

          if(m_particleName[p]=="gamma" || m_particleName[p]=="neutron" ||  m_particleName[p]=="opticalphoton"){
            m_particle =  G4ParticleTable::GetParticleTable()->FindParticle(m_particleName[p].c_str());
          }
          else{
            NPL::Nucleus* N = new NPL::Nucleus(m_particleName[p]);
            m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(),m_ExcitationEnergy[p]);
            delete N;
          }

        }

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

        G4double x0 = RandGauss::shoot(m_x0,m_SigmaX);
        G4double y0 = RandGauss::shoot(m_y0,m_SigmaY);

        Particle particle(m_particle, theta,particle_energy,G4ThreeVector(momentum_x, momentum_y, momentum_z),G4ThreeVector(x0, y0, m_z0));


        m_ParticleStack->AddParticleToStack(particle);
      }
    }
  }



  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void EventGeneratorIsotropic::InitializeRootOutput(){

  }
