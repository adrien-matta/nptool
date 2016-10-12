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
    m_SigmaX        = 0;
    m_SigmaY        = 0;
    m_Multiplicty.clear();
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
    istringstream LineStream;
    
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
    bool check_Multiplicity = false;
    
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
            
            else if (DataBuffer == "SigmaX=" ) {
                ReactionFile >> DataBuffer;
                m_SigmaX = atof(DataBuffer.c_str()) * mm;
                G4cout << "SigmaX " << m_SigmaX << " mm" << G4endl;
            }
            
            else if (DataBuffer == "SigmaY=" ) {
                ReactionFile >> DataBuffer;
                m_SigmaY = atof(DataBuffer.c_str()) * mm;
                G4cout << "SigmaY " << m_SigmaY << " mm" << G4endl;
            }
            
            else if (DataBuffer == "Multiplicity=" ) {
                check_Multiplicity = true;
                LineStream.clear();
                LineStream.str(LineBuffer);
                
                getline(ReactionFile, LineBuffer);
                LineStream.clear();
                LineStream.str(LineBuffer);
                G4cout << "    Multiplicity: " ;
                while(LineStream >> DataBuffer){
                    m_Multiplicty.push_back(atof(DataBuffer.c_str()));
                    G4cout << DataBuffer << " " ;
                }
                G4cout << G4endl;
                //ReactionFile >> DataBuffer;
                //m_Multiplicty = atof(DataBuffer.c_str());
                //G4cout << "Multiplicity= " << m_Multiplicty << " " << G4endl;
            }
            
            else if (DataBuffer=="Particle=" || DataBuffer=="particle=") {
                check_particle = true ;
                LineStream.clear();
                LineStream.str(LineBuffer);
                
                getline(ReactionFile, LineBuffer);
                LineStream.clear();
                LineStream.str(LineBuffer);
                G4cout << "    Particle: " ;
                while(LineStream >> DataBuffer){
                    //m_particleName.push_back(DataBuffer);
                    G4cout << DataBuffer << " " ;
                    // Case of light particle
                    if(DataBuffer=="proton"){ m_particleName.push_back("1H")  ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="deuton"){ m_particleName.push_back("2H")  ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="triton"){ m_particleName.push_back("3H")  ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="3He" || DataBuffer=="He3") { m_particleName.push_back("3He") ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="alpha") { m_particleName.push_back("4He") ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="gamma") { m_particleName.push_back("gamma") ; check_ExcitationEnergy = true ;}
                    else if(DataBuffer=="neutron") {m_particleName.push_back("neutron") ; check_ExcitationEnergy = true ;}
                    else { check_ExcitationEnergy = true ;}
                }
                G4cout << G4endl;
            }
            
            else if (DataBuffer=="ExcitationEnergy=") {
                check_ExcitationEnergy = true ;
                ReactionFile >> DataBuffer;
                m_ExcitationEnergy = atof(DataBuffer.c_str()) * MeV;
                G4cout << "ExcitationEnergy : " << m_ExcitationEnergy << G4endl ;
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
    if(check_Multiplicity==false) {m_Multiplicty.push_back(1);check_Multiplicity=true;}
    
    if(m_Multiplicty.size()!=m_particleName.size()) {
        G4cout << "WARNING : Size of m_particleName and m_Multiplicty are different" << G4endl ;
        G4cout << "Assuming m_Multiplicty=1 for all particles" << G4endl ;
    }
    
    if(    !check_EnergyLow || !check_EnergyHigh || !check_HalfOpenAngleMin || !check_HalfOpenAngleMax || !check_x0 || !check_y0 || !check_z0 || !check_particle)
    {cout << "ERROR : Token Sequence Incomplete, Isotropic definition can not be Fonctionnal" << G4endl ; exit(1);}
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
                    m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(),m_ExcitationEnergy);
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
