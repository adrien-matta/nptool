/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
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

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool headers
#include "EventGeneratorIsotropic.hh"
#include "RootOutput.h"

using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::EventGeneratorIsotropic()
{
   m_InitConditions = new TInitialConditions();

   m_EnergyLow    =  0  ;
   m_EnergyHigh   =  0  ;
   m_x0           =  0  ;
   m_y0           =  0  ;
   m_z0           =  0  ;
   m_particle     =  G4ParticleTable::GetParticleTable()->FindParticle("geantino");
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorIsotropic::~EventGeneratorIsotropic()
{
   delete m_InitConditions;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::ReadConfiguration(string Path)
{
   ////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

   bool ReadingStatus = false ;
   bool check_EnergyLow = false ;
   bool check_EnergyHigh = false ;
   bool check_HalfOpenAngleMin = false ;
   bool check_HalfOpenAngleMax = false ;
   bool check_x0 = false ;
   bool check_y0 = false ;
   bool check_z0 = false ;
   bool check_particle = false ;
   
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
            G4cout << "Isotropic Source Found" << G4endl ;
            ReadingStatus = true;}
            
            
       while (ReadingStatus)
          {
              ReactionFile >> DataBuffer;
          
              //Search for comment Symbol %
              if (DataBuffer.compare(0, 1, "%") == 0) {   ReactionFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

               else if (DataBuffer.compare(0, 10, "EnergyLow=") == 0) {
                  check_EnergyLow = true ;
                  ReactionFile >> DataBuffer;
                  m_EnergyLow = atof(DataBuffer.c_str()) * MeV;
                  G4cout << "Minimum energy " << m_EnergyLow / MeV << " MeV" << G4endl;
               }

               else if (DataBuffer.compare(0, 11, "EnergyHigh=") == 0) {
                  check_EnergyHigh = true ;
                  ReactionFile >> DataBuffer;
                  m_EnergyHigh = atof(DataBuffer.c_str()) * MeV;
                  G4cout << "Maximum energy " << m_EnergyHigh / MeV << " MeV" << G4endl;
               }

               else if (DataBuffer.compare(0, 17, "HalfOpenAngleMin=") == 0) {
                  check_HalfOpenAngleMin = true ;
                  ReactionFile >> DataBuffer;
                  m_HalfOpenAngleMin = atof(DataBuffer.c_str()) * deg;
                  G4cout << "HalfOpenAngleMin " << m_HalfOpenAngleMin / deg << " degree" << G4endl;
               }

               else if (DataBuffer.compare(0, 17, "HalfOpenAngleMax=") == 0) {
                  check_HalfOpenAngleMax = true ;
                  ReactionFile >> DataBuffer;
                  m_HalfOpenAngleMax = atof(DataBuffer.c_str()) * deg;
                  G4cout << "HalfOpenAngleMax " << m_HalfOpenAngleMax / deg << " degree" << G4endl;
               }

               else if (DataBuffer.compare(0, 3, "x0=") == 0) {
                  check_x0 = true ;
                  ReactionFile >> DataBuffer;
                  m_x0 = atof(DataBuffer.c_str()) * mm;
                  G4cout << "x0 " << m_x0 << " mm" << G4endl;
               }

               else if (DataBuffer.compare(0, 3, "y0=") == 0) {
                  check_y0 = true ;
                  ReactionFile >> DataBuffer;
                  m_y0 = atof(DataBuffer.c_str()) * mm;
                  G4cout << "y0 " << m_y0 << " mm" << G4endl;
               }

               else if (DataBuffer.compare(0, 3, "z0=") == 0) {
                  check_z0 = true ;
                  ReactionFile >> DataBuffer;
                  m_y0 = atof(DataBuffer.c_str()) * mm;
                  G4cout << "y0 " << m_y0 << " mm" << G4endl;
               }

               else if (DataBuffer.compare(0, 9, "particle=") == 0) {
                  check_particle = true ;
                  ReactionFile >> DataBuffer;
                  particle = DataBuffer;
                  G4cout << "particle  " << particle << G4endl;
                  m_particle = G4ParticleTable::GetParticleTable()->FindParticle(particle);
               }
              
               //   If no isotropic Token and no comment, toggle out
               else 
                  {ReadingStatus = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
                  
               ///////////////////////////////////////////////////
            //   If all Token found toggle out
               if(    check_EnergyLow && check_EnergyHigh && check_HalfOpenAngleMin && check_HalfOpenAngleMax && check_x0 && check_y0 && check_z0 && check_particle )
                  ReadingStatus = false ;   
               
          }
       
      }
   
   if(    !check_EnergyLow || !check_EnergyHigh || !check_HalfOpenAngleMin || !check_HalfOpenAngleMax || !check_x0 || !check_y0 || !check_z0 || !check_particle )
          {cout << "WARNING : Token Sequence Incomplete, Isotropic definition could not be Fonctionnal" << endl ;}
         
  cout << "///////////////////////////////////////////////////" << endl << endl ;
   
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::GenerateEvent(G4Event* anEvent, G4ParticleGun* particleGun)
{
   // Clear TInitialConditions
   m_InitConditions->Clear();

   particleGun->SetParticleDefinition(m_particle);

   G4double cos_theta_min   = cos(m_HalfOpenAngleMin);
   G4double cos_theta_max   = cos(m_HalfOpenAngleMax);
   G4double cos_theta       = cos_theta_min + (cos_theta_max - cos_theta_min) * RandFlat::shoot();
   G4double theta           = acos(cos_theta)                                                   ;
   //   G4double phi             = 3*pi/2; //RandFlat::shoot() * 2 * pi                                        ;
   G4double phi             = RandFlat::shoot() * 2 * pi                                        ;
   G4double particle_energy = m_EnergyLow + RandFlat::shoot() * (m_EnergyHigh - m_EnergyLow)    ;

   // Direction of particle, energy and laboratory angle
   G4double momentum_x = sin(theta) * cos(phi)  ;
   G4double momentum_y = sin(theta) * sin(phi)  ;
   G4double momentum_z = cos(theta)             ;

   // Set the gun to shoot
   particleGun->SetParticleMomentumDirection(G4ThreeVector(momentum_x, momentum_y, momentum_z))   ;
   particleGun->SetParticleEnergy(particle_energy)                                                ;
   particleGun->SetParticlePosition(G4ThreeVector(m_x0, m_y0, m_z0))                              ;

   // Fill TInitialConditions class
   // Interaction vertex
   m_InitConditions->SetICPositionX(m_x0 / mm);
   m_InitConditions->SetICPositionY(m_y0 / mm);
   m_InitConditions->SetICPositionZ(m_z0 / mm);
   // Incident "particles"
   // Everything is zero for a source
   m_InitConditions->SetICIncidentEmittanceTheta(0);
   m_InitConditions->SetICIncidentEmittancePhi(0);
   m_InitConditions->SetICIncidentAngleTheta(0);
   m_InitConditions->SetICIncidentAnglePhi(0);
   m_InitConditions->SetICIncidentEnergy(0);
   // Emitted particle angles
   m_InitConditions->SetICEmittedAngleThetaCM(theta / deg);
   m_InitConditions->SetICEmittedAngleThetaLabIncidentFrame(theta / deg);
   m_InitConditions->SetICEmittedAnglePhiIncidentFrame(phi / deg);
   m_InitConditions->SetICEmittedAngleThetaLabWorldFrame(theta / deg);
   m_InitConditions->SetICEmittedAnglePhiWorldFrame(phi / deg);
   // Emitted particle energy
   m_InitConditions->SetICEmittedEnergy(particle_energy / MeV);


   //Shoot particle
   particleGun->GeneratePrimaryVertex(anEvent)                                                    ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorIsotropic::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("InitialConditions", "TInitialConditions", &m_InitConditions);
}
