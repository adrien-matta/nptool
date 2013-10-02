#ifndef EventGeneratorBeam_h
#define EventGeneratorBeam_h

// C++ header
#include <string>

// G4 header
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"

// NPTool header
#include "VEventGenerator.hh"

using namespace std  ;



class EventGeneratorBeam : public VEventGenerator
{
public:     // Constructor and destructor
   EventGeneratorBeam()   ;
   virtual ~EventGeneratorBeam()   ;

public:     // Inherit from VEventGenerator Class
   void  ReadConfiguration(string)           ;
   void  GenerateEvent(G4Event*, G4ParticleGun*) ;
   void  InitializeRootOutput()  ;
   void  SetTargetThickness(double TargetThickness)            {
      m_TargetThickness = TargetThickness     ;
   }
   void  SetTargetRadius(double TargetRadius)               {
      m_TargetRadius    = TargetRadius        ;
   }
   void  SetTargetCoordinate(G4double X, G4double Y, G4double Z)  {
      m_TargetX = X ;
      m_TargetY = Y ;
      m_TargetZ = Z ;
   }

private: // Source parameter
   G4ParticleDefinition*   m_particle        ;  // Kind of particle to shoot
   G4double             m_BeamEnergy      ;
   G4double             m_BeamEnergySpread   ;
   G4double             m_BeamFWHMX       ;
   G4double             m_BeamFWHMY       ;
   G4double             m_BeamEmmitanceTheta     ;
   G4double				m_BeamEmmitancePhi		;

private: // Initial Value
   G4double             m_InitialBeamX    ;
   G4double             m_InitialBeamY    ;
   G4double             m_InitialBeamTheta   ;

private: // Target Value
   G4double             m_TargetThickness ;
   G4double             m_TargetRadius    ;
   G4double             m_TargetX         ;
   G4double          m_TargetY         ;
   G4double          m_TargetZ         ;
};
#endif
