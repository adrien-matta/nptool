#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "math.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandGeneral.h"

// Event Generator Class
#include "EventGeneratorTransfert.hh"
#include "EventGeneratorTransfertToResonance.hh"
#include "EventGeneratorIsotropic.hh"
#include "EventGeneratorBeam.hh"

#include <cstdlib>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
   delete m_particleGun;
   delete m_EventGenerator;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
      : m_detector(det)
{
   m_EventGenerator = NULL;

   m_particleGun  = new G4ParticleGun(1);
   G4ParticleDefinition* particle
   = G4ParticleTable::GetParticleTable()->FindParticle("proton");
   m_particleGun->SetParticleDefinition(particle);
   m_particleGun->SetParticleEnergy(10*MeV);

   m_particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
   m_particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
   if (m_EventGenerator != NULL)m_EventGenerator->GenerateEvent(anEvent, m_particleGun) ;
   else {
      G4cout << "No Event Generator!" << G4endl ;
      return ;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ReadEventGeneratorFile(string Path)
{
   // added by Nicolas [07/05/09]
   string GlobalPath = getenv("NPTOOL");
   Path = GlobalPath + "/Inputs/EventGenerator/" + Path;
   string LineBuffer;
   ifstream EventGeneratorFile;
   EventGeneratorFile.open(Path.c_str());

   bool check_TransfertToResonance = false   ;
   bool check_Isotropic        = false ;
   bool check_Transfert        = false ;
   bool check_Beam                = false ;

   if (EventGeneratorFile.is_open()) cout << " Event Generator file " << Path << " loading " << endl  ;
   else                      {
      cout << " Error, Event Generator file " << Path << " found" << endl ;
      return;
   }

   while (!EventGeneratorFile.eof()) {
      //Pick-up next line
      getline(EventGeneratorFile, LineBuffer);

      //Search for comment Symbol %
      if (LineBuffer.compare(0, 1, "%") == 0) {   /*do nothing*/
         ;
      }


      //Search for Isotropic source
      else if (LineBuffer.compare(0, 9, "Isotropic") == 0  && !check_Isotropic) {
         check_Isotropic = true                       ;
         VEventGenerator* myEventGenerator = new EventGeneratorIsotropic()   ;
         EventGeneratorFile.close()                   ;
         myEventGenerator->ReadConfiguration(Path)       ;
         EventGeneratorFile.open(Path.c_str())           ;
         myEventGenerator->InitializeRootOutput()           ;
         m_EventGenerator = myEventGenerator             ;
      }

      //Search for Beam
      else if (LineBuffer.compare(0, 4, "Beam") == 0  && !check_Beam) {
         check_Beam = true                                           ;
         VEventGenerator* myEventGenerator = new EventGeneratorBeam()                    ;
         EventGeneratorFile.close()                                  ;
         myEventGenerator->ReadConfiguration(Path)                      ;
         EventGeneratorFile.open(Path.c_str())                          ;
         myEventGenerator->InitializeRootOutput()           ;
         myEventGenerator->SetTargetThickness(m_detector->GetTargetThickness())  ;
         myEventGenerator->SetTargetRadius(m_detector->GetTargetRadius())     ;
         myEventGenerator->SetTargetCoordinate(m_detector->GetTargetX() ,
               m_detector->GetTargetY()   ,
               m_detector->GetTargetZ())  ;
         m_EventGenerator = myEventGenerator                            ;
      }


      //Search for Transfert To Resonance
      else if (LineBuffer.compare(0, 9, "Transfert") == 0 && !check_Transfert && LineBuffer.compare(0, 11, "TransfertTo") != 0) {
         check_Transfert = true                                      ;
         VEventGenerator* myEventGenerator = new EventGeneratorTransfert()                  ;
         EventGeneratorFile.close()                                  ;
         myEventGenerator->ReadConfiguration(Path)                      ;
         EventGeneratorFile.open(Path.c_str())                          ;
         myEventGenerator->InitializeRootOutput()           ;
         myEventGenerator->SetTargetThickness(m_detector->GetTargetThickness())  ;
         myEventGenerator->SetTargetRadius(m_detector->GetTargetRadius())     ;
         myEventGenerator->SetTargetCoordinate( m_detector->GetTargetX() ,
               									m_detector->GetTargetY()   ,
               									m_detector->GetTargetZ())  ;
         m_EventGenerator = myEventGenerator                            ;
      }

      //Search for Transfert To Resonance
      else if (LineBuffer.compare(0, 21, "TransfertToResonance") == 0 && !check_TransfertToResonance) {
         check_TransfertToResonance = true                              ;
         VEventGenerator* myEventGenerator = new EventGeneratorTransfertToResonance()       ;
         EventGeneratorFile.close()                                  ;
         myEventGenerator->ReadConfiguration(Path)                      ;
         EventGeneratorFile.open(Path.c_str())                          ;
         myEventGenerator->InitializeRootOutput()           ;
         myEventGenerator->SetTargetThickness(m_detector->GetTargetThickness())  ;
         myEventGenerator->SetTargetRadius(m_detector->GetTargetRadius())     ;
         myEventGenerator->SetTargetCoordinate(m_detector->GetTargetX() ,
               m_detector->GetTargetY()   ,
               m_detector->GetTargetZ())  ;
         m_EventGenerator = myEventGenerator                            ;
      }
   }
   EventGeneratorFile.close();
}

