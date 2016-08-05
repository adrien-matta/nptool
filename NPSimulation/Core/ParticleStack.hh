#ifndef __PARTICLESTACK__
#define __PARTICLESTACK__
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : April 2012                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This singleton class contain all the particle to be shooted at the end of *
 * the event generation.                                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// NPS
#include "Particle.hh"

// NPL
#include "TInitialConditions.h"

// STL
#include <vector>
#include <string>
using namespace std;
using namespace CLHEP;

// G4
#include "G4ParticleGun.hh"
#include "G4Event.hh"

class ParticleStack{
  public:
    // Designed to be a singleton (i.e. only one instance
    // can exist). A member function called Instance is defined, which allows
    // the user to get a pointer to the existing instance or to create it if
    // it does not yet exist:
    // (see the constructor for an explanation of the arguments)
    static ParticleStack* getInstance();

    // The analysis class instance can be deleted by calling the Destroy
    // method (NOTE: The class destructor is protected, and can thus not be
    // called directly):
    static void Destroy();

  protected:
    // Constructor (protected)
    ParticleStack();

    // Destructor (protected)
    ~ParticleStack();

    // Prevent copying
    ParticleStack(const ParticleStack& only);
    const ParticleStack& operator=(const ParticleStack& only);

  private:
    // The static instance of the ParticleStack class:
    static ParticleStack* instance;    
    // The particle gun
    G4ParticleGun* m_particleGun;
    // Host the Initial conditions TObject
    TInitialConditions* m_InitialConditions;
    // Needed to now if the IC has to be cleared or not
    // true if the particle to be added is the first one
    bool m_First;

    // USed to know if it the very first event generated, so we can write DEDX 
    // table for the beam
    bool m_EventZero;

  private: // Private Member
    vector<Particle> m_ParticleStack;

  public: // Getter and Setter
    vector<Particle> GetParticleStack();
    void             SetParticleStack(vector<Particle>);
    G4ParticleGun*    GetParticleGun() {return m_particleGun;};

  public: // Particle management and shooting method
    // EventGenerator use this method to add particle in the stack
    void AddParticleToStack(Particle&);

    // Add the Particle to the stack and fill Initial Condition Beam field 
    void AddBeamParticleToStack(Particle&);

    // Search for a specific particle in the stack
    Particle SearchAndRemoveParticle(string);

    // Transform the particle name to G4 standard: i.e: 10He -> He10
    string ChangeNameToG4Standard(string);

    // Shoot everything in the stack at the end of the event
    void ShootAllParticle(G4Event* anEvent);
};
#endif
