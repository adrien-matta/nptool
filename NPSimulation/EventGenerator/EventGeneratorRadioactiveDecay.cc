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
#include <string>

// G4 headers
#include "G4UnitsTable.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4UIManager.hh"

// CLHEP headers
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorRadioactiveDecay::EventGeneratorRadioactiveDecay()
{
    m_EnergyLow    = 0;
    m_EnergyHigh   = 0;
    m_x0           = 0;
    m_y0           = 0;
    m_z0           = 0;
    m_SigmaX       = 0;
    m_SigmaY       = 0;
    m_Z            = 0;
    m_A            = 0;
    m_ParticleStack = ParticleStack::getInstance();

    /* initialize random seed: */
    srand (time(NULL));
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

    vector<string> token = {"Z","A","EnergyLow","EnergyHigh","x0","y0","z0","HalfOpenAngleMin","HalfOpenAngleMax"};

    for(unsigned int i = 0 ; i < blocks.size() ; i++)
    {
        if(blocks[i]->HasTokenList(token))
        {
            m_Z                 =blocks[i]->GetInt("Z");
            m_A                 =blocks[i]->GetInt("A");
            m_EnergyLow         =blocks[i]->GetDouble("EnergyLow","MeV");
            m_EnergyHigh        =blocks[i]->GetDouble("EnergyHigh","MeV");
            m_x0                =blocks[i]->GetDouble("x0","mm");
            m_y0                =blocks[i]->GetDouble("y0","mm");
            m_z0                =blocks[i]->GetDouble("z0","mm");
            m_HalfOpenAngleMin  =blocks[i]->GetDouble("HalfOpenAngleMin","deg");
            m_HalfOpenAngleMax  =blocks[i]->GetDouble("HalfOpenAngleMax","deg");
        }
	  if(blocks[i]->HasToken("SigmaX"))
		  m_SigmaX=blocks[i]->GetDouble("SigmaX","mm");
	  if(blocks[i]->HasToken("SigmaY"))
		  m_SigmaX=blocks[i]->GetDouble("SigmaY","mm");
	  if(blocks[i]->HasToken("ExcitationEnergy"))
		  m_ExcitationEnergy=blocks[i]->GetDouble("ExcitationEnergy","keV");
      if(blocks[i]->HasToken("InitialPopulation"))
      {
          G4String initialPopulationFileName = blocks [i]->GetString("InitialPopulation");

          ifstream filename (initialPopulationFileName);
          G4double level, population;
          while (filename >> level >> population)
          {
             m_InitalLevel.push_back(level);
             m_InitalPopulation.push_back(population);
            
             cout << "Level " << level << " Population " << population << "\n";
          }
      }
	  if(blocks[i]->HasToken("PhotonEvaporation"))
      {
          G4String command = "/grdm/setPhotoEvaporationFile ";
          G4String fileName = blocks[i]->GetString("PhotonEvaporation");
          m_PhotonEvaporation=command+
                  to_string(m_Z)+" "+
                  to_string(m_A)+" "+
                  fileName;
          cout << "Reading file " << fileName << endl;
      }
	  if(blocks[i]->HasToken("RadioactiveDecay"))
      {
          G4String command = "/grdm/setRadioactiveDecayFile ";
          G4String fileName = blocks[i]->GetString("RadioactiveDecay");
          m_RadioactiveDecay=command+
                  to_string(m_Z)+" "+
                  to_string(m_A)+" "+
                  fileName;
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

    G4double x0 = RandGauss::shoot(m_x0,m_SigmaX);
    G4double y0 = RandGauss::shoot(m_y0,m_SigmaY);
    G4double z0 = m_z0;

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


    G4int Z = m_Z, A = m_A;
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.;
    G4double populationSum = 0;
    
    if (m_ExcitationEnergy != 0) excitEnergy = m_ExcitationEnergy;
    else if (m_InitalLevel.size() > 0)
    {
        //G4double startLevel = rand() % 1;
        float startLevel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        for (unsigned int i = 0; i < m_InitalLevel.size(); i++)
        {
            populationSum += m_InitalPopulation[i];
            //cout << populationSum << " " << startLevel << "\n";
            if (populationSum > startLevel)
            {
                excitEnergy = m_InitalLevel[i] * keV;
                break;
            }
        } 
    }

    //cout << "chosen start " << excitEnergy << endl;


    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);

    G4UImanager* UI = G4UImanager::GetUIpointer();
    if (m_PhotonEvaporation) UI->ApplyCommand(m_PhotonEvaporation);
    if (m_RadioactiveDecay)  UI->ApplyCommand(m_RadioactiveDecay);

    Particle Ion(ion,
            theta,
            0,
            G4ThreeVector(momentum_x, momentum_y, momentum_z),//direction
            G4ThreeVector(x0,y0,z0),//position
            true);//shoot status


    m_ParticleStack->AddParticleToStack(Ion);
}
