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
 * Last update    : 03/11/2009                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "EventGeneratorRadioactiveDecay.hh"

// NPS
#include "Particle.hh"

// NPL headers
#include "NPOptionManager.h"

// C++ headers
#include "cmath"

// G4 headers
#include "G4UnitsTable.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"

// CLHEP headers
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorRadioactiveDecay::EventGeneratorRadioactiveDecay()
{
    m_EnergyLow    =  0  ;
    m_EnergyHigh   =  0  ;
    m_x0           =  0  ;
    m_y0           =  0  ;
    m_z0           =  0  ;
    m_SigmaX       = 0   ;
    m_SigmaY       = 0   ;
    m_ParticleStack = ParticleStack::getInstance();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorRadioactiveDecay::~EventGeneratorRadioactiveDecay()
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorRadioactiveDecay::ReadConfiguration(NPL::InputParser parser)
{
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Radioactive");

    // if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Radioactive decay reaction found " << endl; 

    vector<string> token = {"EnergyLow","EnergyHigh"};

    for(unsigned int i = 0 ; i < blocks.size() ; i++)
    {
        if(blocks[i]->HasTokenList(token))
        {
            m_EnergyLow         =blocks[i]->GetDouble("EnergyLow","MeV");
            m_EnergyHigh        =blocks[i]->GetDouble("EnergyHigh","MeV");
            m_x0                =blocks[i]->GetDouble("x0","mm");
            m_y0                =blocks[i]->GetDouble("y0","mm");
            m_z0                =blocks[i]->GetDouble("z0","mm");
            m_HalfOpenAngleMin  =blocks[i]->GetDouble("HalfOpenAngleMin","deg");
            m_HalfOpenAngleMax  =blocks[i]->GetDouble("HalfOpenAngleMax","deg");
        }
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorRadioactiveDecay::GenerateEvent(G4Event*)
{

    G4double cos_theta_min   = cos(m_HalfOpenAngleMin);
    G4double cos_theta_max   = cos(m_HalfOpenAngleMax);
    G4double cos_theta       = cos_theta_min + (cos_theta_max - cos_theta_min) * RandFlat::shoot();
    G4double theta           = acos(cos_theta)                                                   ;
    G4double phi             = RandFlat::shoot() * 2 * pi                                        ;
    G4double particle_energy = m_EnergyLow + RandFlat::shoot() * (m_EnergyHigh - m_EnergyLow)    ;

    G4double momentum_x = sin(theta) * cos(phi)  ;
    G4double momentum_y = sin(theta) * sin(phi)  ;
    G4double momentum_z = cos(theta)             ;

    /*//Testing for generating a single electron
    Particle TestElectron(G4ParticleTable::GetParticleTable()->FindParticle("e-"),//particle
            theta,//thetacm
            particle_energy,//kinetic energy
            G4ThreeVector(momentum_x, momentum_y, momentum_z),//direction
            G4ThreeVector(0.,0.,0.),//position
            true);//shoot status

    m_ParticleStack->AddParticleToStack(TestElectron);

    Particle TestElectron2(G4ParticleTable::GetParticleTable()->FindParticle("e-"),//particle
            theta,//thetacm
            particle_energy,//kinetic energy
            G4ThreeVector(momentum_x, momentum_y, momentum_z),//direction
            G4ThreeVector(0.,0.,0.),//position
            false);//shoot status

    m_ParticleStack->AddParticleToStack(TestElectron2);
*/


    G4int Z = 56, A = 133;
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.*keV;

    Particle Ion(G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy),
            theta,
            0,
            G4ThreeVector(momentum_x, momentum_y, momentum_z),//direction
            G4ThreeVector(0.,0.,0.),//position
            true);//shoot status


    m_ParticleStack->AddParticleToStack(Ion);
}
