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
  m_ParticleStack = ParticleStack::getInstance();
    event_ID=0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::~EventGeneratorIsotropic(){
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::SourceParameters::SourceParameters(){
  m_EnergyLow    =  0  ;
  m_EnergyHigh   =  0  ;
  m_x0           =  0  ;
  m_y0           =  0  ;
  m_z0           =  0  ;
  m_SigmaX       =  0  ;
  m_SigmaY       =  0  ;
  m_particle     = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Isotropic");
	m_Parameters.reserve(blocks.size());
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Isotropic reaction found " << endl; 
	
  vector<string> token = {"EnergyLow","EnergyHigh","HalfOpenAngleMin","HalfOpenAngleMax","x0","y0","z0","Particle"};
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
			m_Parameters.push_back(SourceParameters());
			const vector<SourceParameters>::reverse_iterator it = m_Parameters.rbegin();
			
      it->m_EnergyLow         =blocks[i]->GetDouble("EnergyLow","MeV");
      it->m_EnergyHigh        =blocks[i]->GetDouble("EnergyHigh","MeV");
      it->m_HalfOpenAngleMin  =blocks[i]->GetDouble("HalfOpenAngleMin","deg");
      it->m_HalfOpenAngleMax  =blocks[i]->GetDouble("HalfOpenAngleMax","deg");
      it->m_x0                =blocks[i]->GetDouble("x0","mm");
      it->m_y0                =blocks[i]->GetDouble("y0","mm");
      it->m_z0                =blocks[i]->GetDouble("z0","mm");
      vector<string> particleName =blocks[i]->GetVectorString("Particle");
      for(unsigned int j = 0 ; j < particleName.size() ; j++){
        if(particleName[j]=="proton"){ it->m_particleName.push_back("1H")  ;} 
        else if(particleName[j]=="deuton"){ it->m_particleName.push_back("2H")  ; }
        else if(particleName[j]=="triton"){ it->m_particleName.push_back("3H")  ; }
        else if(particleName[j]=="3He" || particleName[j]=="He3") { it->m_particleName.push_back("3He") ; }
        else if(particleName[j]=="alpha") { it->m_particleName.push_back("4He") ; }
        else if(particleName[j]=="gamma") { it->m_particleName.push_back("gamma") ;}
        else if(particleName[j]=="neutron") {it->m_particleName.push_back("neutron") ;}
        else it->m_particleName.push_back(particleName[j]);
      }

      if(blocks[i]->HasToken("ExcitationEnergy"))
        it->m_ExcitationEnergy =blocks[i]->GetVectorDouble("ExcitationEnergy","MeV");
        
      if(blocks[i]->HasToken("SigmaX"))
        it->m_SigmaX=blocks[i]->GetDouble("SigmaX","mm");
      if(blocks[i]->HasToken("SigmaY"))
        it->m_SigmaY=blocks[i]->GetDouble("SigmaY","mm");
      if(blocks[i]->HasToken("Multiplicity"))
        it->m_Multiplicty=blocks[i]->GetVectorInt("Multiplicity");
    }
    else{
      cout << "ERROR: check your input file formatting \033[0m" << endl; 
      exit(1);
    }
	}

	for(auto& par : m_Parameters) {
		if(par.m_ExcitationEnergy.size()==0)
			par.m_ExcitationEnergy.resize(par.m_particleName.size(),0);
		if(par.m_Multiplicty.size()==0)
			par.m_Multiplicty.resize(par.m_particleName.size(),1);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::GenerateEvent(G4Event*){

	for(auto& par : m_Parameters) {
    for(unsigned int p=0; p<par.m_particleName.size(); p++){
      for(int i=0; i<par.m_Multiplicty[p]; i++){
        par.m_particle=NULL;
        if(par.m_particle==NULL){

          if(par.m_particleName[p]=="gamma" || par.m_particleName[p]=="neutron" ||  par.m_particleName[p]=="opticalphoton"){
            par.m_particle =  G4ParticleTable::GetParticleTable()->FindParticle(par.m_particleName[p].c_str());
          }
          else{
            NPL::Nucleus* N = new NPL::Nucleus(par.m_particleName[p]);
            par.m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(),par.m_ExcitationEnergy[p]);
            delete N;
          }

        }

        G4double cos_theta_min   = cos(par.m_HalfOpenAngleMin);
        G4double cos_theta_max   = cos(par.m_HalfOpenAngleMax);
        G4double cos_theta       = cos_theta_min + (cos_theta_max - cos_theta_min) * RandFlat::shoot();
        G4double theta           = acos(cos_theta)                                                   ;
        G4double phi             = RandFlat::shoot() * 2 * pi                                        ;
        G4double particle_energy = par.m_EnergyLow + RandFlat::shoot() * (par.m_EnergyHigh - par.m_EnergyLow)    ;
          
          cout << "Event ID= " << event_ID << " / theta= " << theta*180/3.1415 << " / energy= " << particle_energy << endl;
          event_ID++;
          

        // Direction of particle, energy and laboratory angle
        G4double momentum_x = sin(theta) * cos(phi)  ;
        G4double momentum_y = sin(theta) * sin(phi)  ;
        G4double momentum_z = cos(theta)             ;

        G4double x0 = RandGauss::shoot(par.m_x0,par.m_SigmaX);
        G4double y0 = RandGauss::shoot(par.m_y0,par.m_SigmaY);

        Particle particle(par.m_particle, theta,particle_energy,G4ThreeVector(momentum_x, momentum_y, momentum_z),G4ThreeVector(x0, y0, par.m_z0));


        m_ParticleStack->AddParticleToStack(particle);
      }
    }
  }
	
}


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void EventGeneratorIsotropic::InitializeRootOutput(){

  }
