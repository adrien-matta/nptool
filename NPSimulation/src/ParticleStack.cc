/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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

// G4 headers
#include "G4ParticleTable.hh"

// NPL
#include "RootOutput.h"

ParticleStack* ParticleStack::instance = 0 ;

ParticleStack* ParticleStack::getInstance()
{
    if (instance == 0) instance = new ParticleStack();
    return instance ;
}

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
    RootOutput::getInstance()->GetTree()->Branch("InitialConditions","TInitialConditions",&m_InitialConditions);}

ParticleStack::~ParticleStack(){
    
}

vector<Particle> ParticleStack::GetParticleStack(){
    return m_ParticleStack;
}

void ParticleStack::SetParticleStack(vector<Particle> particle_stack){
    m_ParticleStack = particle_stack;
}

void ParticleStack::AddParticleToStack(Particle particle){
    m_ParticleStack.push_back(particle);
    m_InitialConditions->SetICPositionX(particle.GetParticlePosition().x());
    m_InitialConditions->SetICPositionY(particle.GetParticlePosition().y());
    m_InitialConditions->SetICPositionZ(particle.GetParticlePosition().z());
    m_InitialConditions->SetICEmittedAngleThetaLabWorldFrame(particle.GetParticleMomentumDirection().theta());
}

Particle ParticleStack::SearchAndRemoveParticle(string name){
 
    for(unsigned int i = 0 ; i < m_ParticleStack.size() ; i++){
        string ParticleName = m_ParticleStack[i].GetParticleDefinition()->GetParticleName();
        if(ParticleName.compare(0, name.length(), name) == 0) 
            {
                Particle my_Particule = m_ParticleStack[i];
                m_ParticleStack.erase(m_ParticleStack.begin()+i);
                return my_Particule;
            }
    }
    
    return Particle();
}

string ParticleStack::ChangeNameToG4Standard(string OriginalName){
    string NumberOfMass ;
    string Nucleid;
    
    for (unsigned int i = 0; i < OriginalName.length(); i++) {
        ostringstream character;
        character << OriginalName[i];
        if (character.str()=="0") NumberOfMass+="0";
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
         if (FinalName=="H1")       FinalName="proton";
    else if (FinalName=="H2")       FinalName="deuteron";
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

void ParticleStack::ShootAllParticle(G4Event* anEvent){
    for(unsigned int i = 0 ; i < m_ParticleStack.size() ; i++){
        
        if(m_ParticleStack[i].GetShootStatus()){
            m_particleGun->SetParticleDefinition(m_ParticleStack[i].GetParticleDefinition());
            m_particleGun->SetParticleEnergy(m_ParticleStack[i].GetParticleKineticEnergy());
            m_particleGun->SetParticleMomentumDirection(m_ParticleStack[i].GetParticleMomentumDirection());
            m_particleGun->SetParticlePosition(m_ParticleStack[i].GetParticlePosition());
            m_particleGun->GeneratePrimaryVertex(anEvent);
        }
    }
    m_ParticleStack.clear();
    m_InitialConditions->Clear();
}