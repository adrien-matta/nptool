#ifndef VEventGenerator_h
#define VEventGenerator_h

// C++ header
#include <string>

// G4 header defining G$ types
#include "globals.hh"

// G4 headers
#include "G4ParticleGun.hh"
#include "G4Event.hh"

using namespace std;



class VEventGenerator
{
public:
   VEventGenerator();
   virtual ~VEventGenerator();

public:
   virtual void ReadConfiguration(string) {};
   virtual void GenerateEvent(G4Event*, G4ParticleGun*) {};
   virtual void InitializeRootOutput() {};

   // Used in some case to generate event inside the target
   virtual void SetTargetThickness(G4double) {};
   virtual void SetTargetRadius(G4double) {};
   virtual void SetTargetCoordinate(G4double, G4double, G4double) {};
};


#endif
