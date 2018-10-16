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
#include <cstdlib>

#include "TROOT.h"    // to use gROOT point
#include "TMath.h"
#include "TRandom.h"
#include "TFormula.h"
#include "TF1.h"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPS headers
#include "EventGeneratorCosmic.hh"

// NPL headers
#include "RootOutput.h"
#include "NPNucleus.h"
#include "NPOptionManager.h"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorCosmic::EventGeneratorCosmic(){
  m_ParticleStack = ParticleStack::getInstance();
    event_ID=0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorCosmic::~EventGeneratorCosmic(){
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorCosmic::SourceParameters::SourceParameters(){
  m_EnergyLow    =  0  ;
  m_EnergyHigh   =  0  ;
  m_x0           =  0 ;
  m_y0           =  2200  ;
  m_z0           =  0  ;
  m_SigmaX       =  0  ;
  m_SigmaY       =  0  ;
  m_particle     = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorCosmic::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Cosmic");
	m_Parameters.reserve(blocks.size());
 if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Cosmic reaction found " << endl; 
	
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
        else if(particleName[j]=="mu+") { it->m_particleName.push_back("mu+") ;}
        else if(particleName[j]=="mu-") { it->m_particleName.push_back("mu-") ;}
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


  G4double CosmicAngle;
  G4double H = 1500*mm;
  G4double R = 500*mm; 
  G4double x0 =0;
  G4double m_y0 = 0;
  G4double z0 = 0;
  G4double momentum_x = 0;
  G4double momentum_z = 0;
  G4double randomize1=0, randomize2=0 ;
  G4double momentum_y = 0;
  G4double angle = 0;
  TF1* cosSq2= new TF1("cosSq2", "TMath::Power(cos(x),2)", 0, (TMath::Pi())/2);

void EventGeneratorCosmic::GenerateEvent(G4Event*){

	for(auto& par : m_Parameters) {
    for(unsigned int p=0; p<par.m_particleName.size(); p++){
      for(int i=0; i<par.m_Multiplicty[p]; i++){
        par.m_particle=NULL;
        if(par.m_particle==NULL){

          if(par.m_particleName[p]=="gamma" || par.m_particleName[p]=="neutron" ||  par.m_particleName[p]=="opticalphoton"  ||  par.m_particleName[p]=="mu+" ||  par.m_particleName[p]=="mu-"){
            par.m_particle =  G4ParticleTable::GetParticleTable()->FindParticle(par.m_particleName[p].c_str());
          }
          else{
            NPL::Nucleus* N = new NPL::Nucleus(par.m_particleName[p]);
            par.m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(),par.m_ExcitationEnergy[p]);
            delete N;
          }

        }
  
        
        G4double particle_energy = par.m_EnergyLow ;//+ RandFlat::shoot() * (par.m_EnergyHigh - par.m_EnergyLow)    ;
          event_ID++;
          

        // Direction of particle, energy and laboratory angle

        

        angle = RandFlat::shoot()*2*pi;
        CosmicAngle = cosSq2->GetRandom();
        randomize1 = RandFlat::shoot()    ;
        randomize2 = RandFlat::shoot()    ;

        /* //Putting a cylinder
        if(randomize>0){          //top
        momentum_x = cos(angle)*sin(CosmicAngle);
        momentum_z = sin(angle)*sin(CosmicAngle);
          
        x0 = cos(angle2*2)*R*(randomize*2);
        z0 = sin(angle2*2)*R*(randomize*2);
        par.m_y0 = H/2;
        } else {                  //lateral surface
          
          x0 = cos(angle)*R;
          z0 = sin(angle)*R;
          par.m_y0 = (.5-RandFlat::shoot())*H;   ///!!!!!!!

          momentum_x = -cos(angle+angle2)*sin(CosmicAngle);
          momentum_z = -sin(angle+angle2)*sin(CosmicAngle);
        }
        */

        // Begin Constrain to pass in a square with L = 3* R

        x0 = R*(randomize1-0.5)*3;
        z0 = R*(randomize2-0.5)*3;
    
        momentum_y = cos(CosmicAngle);
        momentum_x = cos(angle)*sin(CosmicAngle);
        momentum_z = sin(angle)*sin(CosmicAngle);

        x0 = x0-momentum_x*( H/2 / momentum_y);// *( H/2 / momentum_y) this is to have the origin always with par.m_y0 = H/2;
        z0 = z0-momentum_z*( H/2 / momentum_y);
        par.m_y0 = H/2; // momentum_y*( H/2 / momentum_y);

        // End Constrain to pass in a square with L = 3* R
        
        momentum_y = -momentum_y;



        Particle particle(par.m_particle, 0,particle_energy,G4ThreeVector(momentum_x, momentum_y, momentum_z),G4ThreeVector(x0, par.m_y0, z0));


        m_ParticleStack->AddParticleToStack(particle);

      }
    }
  }
	
}


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  void EventGeneratorCosmic::InitializeRootOutput(){

  }
