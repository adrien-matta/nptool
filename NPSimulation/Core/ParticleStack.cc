/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : April 2012                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This singleton class contain all the particle to be shooted at the end of *
 * the event generation.                                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// NPS
#include"ParticleStack.hh"
#include"MaterialManager.hh"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
// NPL
#include "RootOutput.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParticleStack* ParticleStack::instance = 0 ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParticleStack* ParticleStack::getInstance(){
    if (instance == 0) instance = new ParticleStack();
    return instance ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParticleStack::ParticleStack(){
    
    m_particleGun  = new G4ParticleGun(1);
    G4ParticleDefinition* particle
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    m_particleGun->SetParticleDefinition(particle);
    m_particleGun->SetParticleEnergy(10*MeV);
    
    m_particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    m_particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    
    // Instantiate the TInitialConditions object and link it to the RootOutput tree
    m_InitialConditions = new TInitialConditions();
    RootOutput::getInstance()->GetTree()->Branch("InitialConditions","TInitialConditions",&m_InitialConditions);
    
    m_EventZero = true;
    m_First=true;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParticleStack::~ParticleStack(){
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
vector<Particle> ParticleStack::GetParticleStack(){
    return m_ParticleStack;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParticleStack::SetParticleStack(vector<Particle> particle_stack){
    m_ParticleStack = particle_stack;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParticleStack::AddParticleToStack(Particle& particle){
    
    // If the particle is the first one to be added, then the IC are cleared
    if(m_First)
        m_InitialConditions->Clear();
    
    m_ParticleStack.push_back(particle);
    
    m_First=false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParticleStack::AddBeamParticleToStack(Particle& particle){
    // If the particle is the first one to be added, then the IC are cleared
    if(m_First)
        m_InitialConditions->Clear();
    
    m_ParticleStack.push_back(particle);
    // Incident beam parameter
    m_InitialConditions-> SetIncidentParticleName   (particle.GetParticleDefinition()->GetParticleName());
    //m_InitialConditions-> SetIncidentInitialKineticEnergy  (particle. GetParticleThetaCM());
    m_InitialConditions-> SetIncidentInitialKineticEnergy  (particle. GetParticleKineticEnergy());
    
    G4ThreeVector U(1,0,0);
    G4ThreeVector V(0,1,0);
    
    m_InitialConditions-> SetIncidentEmittanceThetaX (particle.GetParticleMomentumDirection().angle(U)/deg);
    m_InitialConditions-> SetIncidentEmittancePhiY   (particle.GetParticleMomentumDirection().angle(V)/deg);
    m_InitialConditions-> SetIncidentEmittanceTheta (particle.GetParticleMomentumDirection().theta()/deg);
    m_InitialConditions-> SetIncidentEmittancePhi  (particle.GetParticleMomentumDirection().phi()/deg);
    
    // Beam status at the initial interaction point
    m_InitialConditions-> SetIncidentFinalKineticEnergy (particle. GetParticleKineticEnergy());
    m_InitialConditions-> SetIncidentPositionX     (particle. GetParticlePosition().x());
    m_InitialConditions-> SetIncidentPositionY     (particle. GetParticlePosition().y());
    m_InitialConditions-> SetIncidentPositionZ     (particle. GetParticlePosition().z());
    
    
    // The beam is not shoot most of the time, but the DEDx table are still usefull
    if( m_EventZero && particle.GetParticleDefinition()->GetPDGCharge()!=0){
        MaterialManager::getInstance()
        ->WriteDEDXTable(particle.GetParticleDefinition(),
                         1e-3*eV,particle.GetParticleKineticEnergy());
        m_EventZero = false;
    }
    
    m_First=false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Particle ParticleStack::SearchAndRemoveParticle(string name){
    
    unsigned int size = m_ParticleStack.size();
    
    for(unsigned int i = 0 ; i < size ; i++){
        string ParticleName = m_ParticleStack[i].GetParticleDefinition()->GetParticleName();
        if(ParticleName.compare(0, name.length(), name) == 0){
            Particle my_Particule = m_ParticleStack[i];
            m_ParticleStack.erase(m_ParticleStack.begin()+i);
            return my_Particule;
        }
    }
    
    return Particle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
string ParticleStack::ChangeNameToG4Standard(string OriginalName){
    string NumberOfMass ;
    string Nucleid;
    
    for (unsigned int i = 0; i < OriginalName.length(); i++) {
        ostringstream character;
        character << OriginalName[i];
        if      (character.str()=="0") NumberOfMass+="0";
        else if (character.str()=="1") NumberOfMass+="1";
        else if (character.str()=="2") NumberOfMass+="2";
        else if (character.str()=="3") NumberOfMass+="3";
        else if (character.str()=="4") NumberOfMass+="4";
        else if (character.str()=="5") NumberOfMass+="5";
        else if (character.str()=="6") NumberOfMass+="6";
        else if (character.str()=="7") NumberOfMass+="7";
        else if (character.str()=="8") NumberOfMass+="8";
        else if (character.str()=="9") NumberOfMass+="9";
        
        else if (character.str()=="A") Nucleid+="A";
        else if (character.str()=="B") Nucleid+="B";
        else if (character.str()=="C") Nucleid+="C";
        else if (character.str()=="D") Nucleid+="D";
        else if (character.str()=="E") Nucleid+="E";
        else if (character.str()=="F") Nucleid+="F";
        else if (character.str()=="G") Nucleid+="G";
        else if (character.str()=="H") Nucleid+="H";
        else if (character.str()=="I") Nucleid+="I";
        else if (character.str()=="J") Nucleid+="J";
        else if (character.str()=="K") Nucleid+="K";
        else if (character.str()=="L") Nucleid+="L";
        else if (character.str()=="M") Nucleid+="M";
        else if (character.str()=="N") Nucleid+="N";
        else if (character.str()=="O") Nucleid+="O";
        else if (character.str()=="P") Nucleid+="P";
        else if (character.str()=="Q") Nucleid+="Q";
        else if (character.str()=="R") Nucleid+="R";
        else if (character.str()=="S") Nucleid+="S";
        else if (character.str()=="T") Nucleid+="T";
        else if (character.str()=="U") Nucleid+="U";
        else if (character.str()=="V") Nucleid+="V";
        else if (character.str()=="W") Nucleid+="W";
        else if (character.str()=="X") Nucleid+="X";
        else if (character.str()=="Y") Nucleid+="Y";
        else if (character.str()=="Z") Nucleid+="Z";
        
        else if (character.str()=="a") Nucleid+="a";
        else if (character.str()=="b") Nucleid+="b";
        else if (character.str()=="c") Nucleid+="c";
        else if (character.str()=="d") Nucleid+="d";
        else if (character.str()=="e") Nucleid+="e";
        else if (character.str()=="f") Nucleid+="f";
        else if (character.str()=="g") Nucleid+="g";
        else if (character.str()=="h") Nucleid+="h";
        else if (character.str()=="i") Nucleid+="i";
        else if (character.str()=="j") Nucleid+="j";
        else if (character.str()=="k") Nucleid+="k";
        else if (character.str()=="l") Nucleid+="l";
        else if (character.str()=="m") Nucleid+="m";
        else if (character.str()=="n") Nucleid+="n";
        else if (character.str()=="o") Nucleid+="o";
        else if (character.str()=="p") Nucleid+="p";
        else if (character.str()=="q") Nucleid+="q";
        else if (character.str()=="r") Nucleid+="r";
        else if (character.str()=="s") Nucleid+="s";
        else if (character.str()=="t") Nucleid+="t";
        else if (character.str()=="u") Nucleid+="u";
        else if (character.str()=="v") Nucleid+="v";
        else if (character.str()=="w") Nucleid+="w";
        else if (character.str()=="x") Nucleid+="x";
        else if (character.str()=="y") Nucleid+="y";
        else if (character.str()=="z") Nucleid+="z";
    }
    
    // Special case for light particles
    string FinalName=Nucleid+NumberOfMass;
    if      (FinalName=="H1")       FinalName="proton";
    //  else if (FinalName=="H2")       FinalName="deuteron";
    else if (FinalName=="H3")       FinalName="triton";
    else if (FinalName=="He4")      FinalName="alpha";
    else if (FinalName=="p")        FinalName="proton";
    else if (FinalName=="d")        FinalName="deuteron";
    else if (FinalName=="t")        FinalName="triton";
    else if (FinalName=="a")        FinalName="alpha";
    else if (FinalName=="proton")   FinalName="proton";
    else if (FinalName=="deuteron") FinalName="deuteron";
    else if (FinalName=="triton")   FinalName="triton";
    else if (FinalName=="alpha")    FinalName="alpha";
    else if (FinalName=="n")        FinalName="neutron";
    else if (FinalName=="neutron")  FinalName="neutron";
    return(FinalName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParticleStack::ShootAllParticle(G4Event* anEvent){
    unsigned int size = m_ParticleStack.size();
    
    for(unsigned int i = 0 ; i < size ; i++){
        
        if(    m_ParticleStack[i].GetShootStatus()){
            // Write the DEDX table for charged particle and
            // all material used in the simulation
            if( anEvent->GetEventID()==0
               && G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()==0
               && m_ParticleStack[i].GetParticleDefinition()->GetPDGCharge()!=0){
                MaterialManager::getInstance()
                ->WriteDEDXTable(m_ParticleStack[i].GetParticleDefinition(),
                                 1e-3*eV,m_ParticleStack[i].GetParticleKineticEnergy());
            }
            
            m_particleGun->SetParticleDefinition(m_ParticleStack[i].GetParticleDefinition());
            m_particleGun->SetParticleEnergy(m_ParticleStack[i].GetParticleKineticEnergy());
            m_particleGun->SetParticleMomentumDirection(m_ParticleStack[i].GetParticleMomentumDirection());
            m_particleGun->SetParticlePosition(m_ParticleStack[i].GetParticlePosition());
            m_particleGun->GeneratePrimaryVertex(anEvent);
            //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
            m_InitialConditions-> SetParticleName       ( m_ParticleStack[i].GetParticleDefinition()->GetParticleName()) ;
            m_InitialConditions-> SetThetaCM            ( m_ParticleStack[i].GetParticleThetaCM()/deg);
            m_InitialConditions-> SetKineticEnergy      ( m_ParticleStack[i].GetParticleKineticEnergy());
            m_InitialConditions-> SetMomentumDirectionX ( m_ParticleStack[i].GetParticleMomentumDirection().x());
            m_InitialConditions-> SetMomentumDirectionY ( m_ParticleStack[i].GetParticleMomentumDirection().y());
            m_InitialConditions-> SetMomentumDirectionZ ( m_ParticleStack[i].GetParticleMomentumDirection().z());
        }
    }
    m_ParticleStack.clear();
    m_First=true;
}
