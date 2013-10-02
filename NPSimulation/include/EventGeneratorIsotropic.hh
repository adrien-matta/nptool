#ifndef EventGeneratorIsotropic_h
#define EventGeneratorIsotropic_h

// C++ header
#include <string>

// G4 headers
#include "G4Event.hh"
#include "G4ParticleGun.hh"

// NPTool headers
#include "VEventGenerator.hh"
#include "TInitialConditions.h"

using namespace std;


class EventGeneratorIsotropic : public VEventGenerator
{
public:     // Constructor and destructor
   EventGeneratorIsotropic() ;
   virtual ~EventGeneratorIsotropic();

public:     // Inherit from VEventGenerator Class
   void ReadConfiguration(string)               ;
   void GenerateEvent(G4Event*, G4ParticleGun*) ;
   void InitializeRootOutput()                  ;

private:    // Source parameter from input file
   G4double               m_EnergyLow        ;  // Lower limit of energy range
   G4double               m_EnergyHigh       ;  // Upper limit of energy range
   G4double               m_HalfOpenAngleMin ;  // Min Half open angle of the source
   G4double               m_HalfOpenAngleMax ;  // Max Half open angle of the source
   G4double               m_x0               ;  // Vertex Position X
   G4double               m_y0               ;  // Vertex Position Y
   G4double               m_z0               ;  // Vertex Position Z
   G4ParticleDefinition*  m_particle         ;  // Kind of particle to shoot isotropically

private:    // Output ROOT class storing characteristics of source
   TInitialConditions* m_InitConditions;
};
#endif
