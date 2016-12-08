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
 *  This event Generator is used to simulated pBUU ion Source           *
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
#include "EventGeneratorpBUU.hh"

// NPl headers
#include "RootOutput.h"
#include "NPNucleus.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorpBUU::EventGeneratorpBUU(){
    m_x0            =  0  ;
    m_y0            =  0  ;
    m_z0            =  0  ;
    m_SigmaX        = 0;
    m_SigmaY        = 0;
    m_particle      =  NULL;
    m_ParticleStack = ParticleStack::getInstance();
    m_ExcitationEnergy = 0 ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorpBUU::~EventGeneratorpBUU(){
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorpBUU::ReadConfiguration(NPL::InputParser ){
    return;
    string Path="";
    ////////General Reading needs////////
    string LineBuffer;
    string DataBuffer;
  
    bool ReadingStatus = false ;
    bool check_AngleEnergyHistPath = false;
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
    
        if (LineBuffer.compare(0, 4, "pBUU") == 0) {
            G4cout << "///////////////////////////////////////////////////" << G4endl ;
            G4cout << "pBUU Source Found" << G4endl ;
            ReadingStatus = true;
        }
      
    
    
        while (ReadingStatus){
            ReactionFile >> DataBuffer;
            //Search for comment Symbol %
            if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
 
            //Angle is X-axis and Energy is Y-axis
            else if  (DataBuffer== "AngleEnergyHistPath=") {
                check_AngleEnergyHistPath = true ;
                TString FileName,HistName;
                ReactionFile >> FileName >> HistName;
                G4cout << "Reading Energy-Angle Distribution file: " << FileName << G4endl;
              
                string GlobalPath = getenv("NPTOOL");
                TString StandardPath = GlobalPath + "/Inputs/EventGenerator/" + FileName;
          
                TFile *f1 = new TFile(StandardPath);
                fAngleEnergyHist = (TH2F*) f1->FindObjectAny(HistName);
                if(!fAngleEnergyHist){
                    G4cout << "Error: Histogramm " << HistName << " not found in file " << FileName << G4endl;
                    cout << "Error: Histogramm " << HistName << " not found in file " << FileName << endl;
                    exit(1);
                }
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
     
            else if (DataBuffer=="Particle=" || DataBuffer=="particle=") {
                check_particle = true ;
                ReactionFile >> m_particleName;
                G4cout << "Particle : " << m_particleName << G4endl ;
        
                // Case of light particle
                if(m_particleName=="proton"){ m_particleName="1H"  ; check_ExcitationEnergy = true ;}
                else if(m_particleName=="deuton"){ m_particleName="2H"  ; check_ExcitationEnergy = true ;}
                else if(m_particleName=="triton"){ m_particleName="3H"  ; check_ExcitationEnergy = true ;}
                else if(m_particleName=="alpha") { m_particleName="4He" ; check_ExcitationEnergy = true ;}
                else if(m_particleName=="gamma") { check_ExcitationEnergy = true ;}
                else if(m_particleName=="neutron") { check_ExcitationEnergy = true ;}
                else { check_ExcitationEnergy = true ;}
            }
      
            else if (DataBuffer=="ExcitationEnergy=") {
                check_ExcitationEnergy = true ;
                ReactionFile >> DataBuffer;
                m_ExcitationEnergy = atof(DataBuffer.c_str()) * MeV;
                G4cout << "ExcitationEnergy : " << m_ExcitationEnergy << G4endl ;
            }
      
            //   If no pBUU Token and no comment, toggle out
            else
            {ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
      
            ///////////////////////////////////////////////////
            //   If all Token found toggle out
            if(    check_AngleEnergyHistPath && check_x0 && check_y0 && check_z0 && check_particle && check_ExcitationEnergy)
                ReadingStatus = false ;
            }
        }
  
        if(    !check_AngleEnergyHistPath || !check_x0 || !check_y0 || !check_z0 || !check_particle )
            {G4cout << "ERROR : Token Sequence Incomplete, pBUU definition can not be Fonctionnal" << G4endl ; exit(1);}
    }



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorpBUU::GenerateEvent(G4Event*){
  
    if(m_particle==NULL){
        if(m_particleName=="gamma" || m_particleName=="neutron" ||  m_particleName=="opticalphoton"){
            m_particle =  G4ParticleTable::GetParticleTable()->FindParticle(m_particleName.c_str());
        }
        else{
            NPL::Nucleus* N = new NPL::Nucleus(m_particleName);
            m_particle = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(N->GetZ(), N->GetA(),m_ExcitationEnergy);
            delete N;
        }
    }
    
    Double_t theta;
    Double_t particle_energy;
    fAngleEnergyHist->GetRandom2(theta,particle_energy);
    theta = theta*deg;
    particle_energy = particle_energy / MeV;
    //G4double theta = fAngleHist->GetRandom()*deg;
    //G4double particle_energy = fEnergyHist->GetRandom() / MeV;
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



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorpBUU::InitializeRootOutput(){
  
}
