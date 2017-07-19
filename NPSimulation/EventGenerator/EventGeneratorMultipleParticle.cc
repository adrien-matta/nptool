/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouac@nscl.msu.edu       *
 *                                                                           *
 * Creation Date  : April 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated MultipleParticle ion Source           *
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
#include "EventGeneratorMultipleParticle.hh"

// NPl headers
#include "RootOutput.h"
#include "NPNucleus.h"
#include "NPOptionManager.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorMultipleParticle::EventGeneratorMultipleParticle(){
    // NPTool path
    string GlobalPath = getenv("NPTOOL");
    string StandardPath = GlobalPath + "/Inputs/EventGenerator/";
    
    m_x0            = 0;
    m_y0            = 0;
    m_z0            = 0;
    m_SigmaX        = 0;
    m_SigmaY        = 0;
    m_Events        = 0;
    m_File          = StandardPath;
    m_particleName.clear();
    m_ParticleStack = ParticleStack::getInstance();
    m_Event.clear();
    m_Multiplicity.clear();
    m_Energy.clear();
    m_Theta.clear();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorMultipleParticle::~EventGeneratorMultipleParticle(){
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorMultipleParticle::ReadConfiguration(NPL::InputParser parser){
    
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("MultipleParticle");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "\033[1;35m//// MultipleParticle token found " << endl;
    
    vector<string> token = {"x0", "y0", "z0", "SigmaX", "SigmaY","Events","EventFile"};
    
    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        blocks[i]->Dump();
        if(blocks[i]->HasTokenList(token)){
            m_x0        = blocks[i]->GetDouble("x0","mm");
            m_y0        = blocks[i]->GetDouble("y0","mm");
            m_z0        = blocks[i]->GetDouble("z0","mm");
            m_SigmaX    = blocks[i]->GetDouble("SigmaX","mm");
            m_SigmaY    = blocks[i]->GetDouble("SigmaY","mm");
            m_Events    = blocks[i]->GetInt("Events");
            m_FileName  = blocks[i]->GetString("EventFile");
            m_File += m_FileName;
        }
        
        else{
            cout << "ERROR: check your input file formatting \033[0m" << endl;
            exit(1);
        }
        
        
        
        ifstream evtfile;
        evtfile.open(m_File.c_str());
        string sMult, sParticle;
        int iMult;
        double dEnergy, dAngle;
        vector<string> vParticle;
        vector<double> vEnergy, vTheta;
        if(evtfile.is_open()){
            cout << "Event file found: " << m_FileName <<  " \033[0m" << endl;
            while(!evtfile.eof()){
                evtfile >> sMult >> iMult;
                m_Multiplicity.push_back(iMult);
                //cout << sMult << " " << iMult << endl;
                for(int k=0; k<iMult; k++){
                    evtfile >> sParticle >> dEnergy >> dAngle;
                    if(sParticle=="proton"){vParticle.push_back("1H");}
                    else if(sParticle=="deuteron" || sParticle=="deuton"){vParticle.push_back("2H");}
                    else if(sParticle=="triton"){vParticle.push_back("3H");}
                    else if(sParticle=="3He" || sParticle=="He3"){vParticle.push_back("3He");}
                    else if(sParticle=="alpha"){vParticle.push_back("4He");}
                    else if(sParticle=="gamma") { vParticle.push_back("gamma") ;}
                    else if(sParticle=="neutron") {vParticle.push_back("neutron") ;}
                    else vParticle.push_back(sParticle);
                    
                    vEnergy.push_back(dEnergy);
                    vTheta.push_back(dAngle);
                }
                m_particleName.push_back(vParticle);
                m_Energy.push_back(vEnergy);
                m_Theta.push_back(vTheta);
            }
        }
        else{
            cout << "ERROR: Event File for transport model not found \033[0m" << endl;
            exit(1);
        }
        
        evtfile.close();
        if(m_Multiplicity.size() != m_Events){
            cout << "\033[1;35m WARNING: Number of Events: " << m_Events << " different from what found in " << m_FileName << ": " << m_Multiplicity.size() << endl;
        }
    }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorMultipleParticle::GenerateEvent(G4Event* evt){
    
    unsigned int evtID = evt->GetEventID();
    cout << " - evtID= " << evtID << endl;

    
    if(evtID>m_Multiplicity.size()-1){
        int k = evtID/m_Multiplicity.size();
        if(k>0)evtID = evtID-k*m_Multiplicity.size();
        cout << " - k= " << k << endl;
        cout << " - new evtID= " << evtID << endl;
    }
    
    cout << " - Mult= " << m_Multiplicity[evtID] << endl;
    for(int i=0; i<m_Multiplicity[evtID]; i++){
        cout << "Particle Name: " << m_particleName[evtID][i] << endl;
        cout << "Energy= " << m_Energy[evtID][i] << endl;
        cout << "Theta= " << m_Theta[evtID][i] << endl;
        m_particle=NULL;
        if(m_particle==NULL){
            if(m_particleName[evtID][i]=="gamma" || m_particleName[evtID][i]=="neutron" ||  m_particleName[evtID][i]=="opticalphoton"){
                m_particle =  G4ParticleTable::GetParticleTable()->FindParticle(m_particleName[evtID][i].c_str());
            }
            else{
                NPL::Nucleus* N = new NPL::Nucleus(m_particleName[evtID][i]);
                m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(), 0);
                delete N;
            }
        }
        
        Double_t theta = m_Theta[evtID][i];
        Double_t particle_energy= m_Energy[evtID][i];
        theta = theta*deg;
        particle_energy = particle_energy / MeV;
        G4double phi             = RandFlat::shoot() * 2 * pi;
        
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



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorMultipleParticle::InitializeRootOutput(){
  
}
