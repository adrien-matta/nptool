//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

// C++ headers
#include <string>

// G4 headers
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"

// NPTool headers
#include "VEventGenerator.hh"
#include "DetectorConstruction.hh"

using namespace std;



class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
   PrimaryGeneratorAction(DetectorConstruction*);
   virtual ~PrimaryGeneratorAction();

public:
   void GeneratePrimaries(G4Event*);
   G4ParticleGun* GetParticleGun() {
      return m_particleGun;
   }

public:
   void ReadEventGeneratorFile(string Path);

private:
   G4ParticleGun*             m_particleGun  ;
   DetectorConstruction*      m_detector     ;
   VEventGenerator*        m_EventGenerator  ;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
