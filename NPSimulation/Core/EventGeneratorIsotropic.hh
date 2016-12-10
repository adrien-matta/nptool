#ifndef EventGeneratorIsotropic_h
#define EventGeneratorIsotropic_h
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
// C++ header
#include <string>
#include <cmath>
using namespace std;

using namespace CLHEP;

// G4 headers
#include "G4Event.hh"

// NPS headers
#include "VEventGenerator.hh"
#include "ParticleStack.hh"
#include "NPInputParser.h"
class EventGeneratorIsotropic : public NPS::VEventGenerator{
public:     // Constructor and destructor
    EventGeneratorIsotropic() ;
    virtual ~EventGeneratorIsotropic();
    
public:     // Inherit from VEventGenerator Class
    void ReadConfiguration(NPL::InputParser)               ;
    void GenerateEvent(G4Event*) ;
    void InitializeRootOutput()                  ;
    
private:    // Source parameter from input file
    G4double               m_EnergyLow        ;  // Lower limit of energy range
    G4double               m_EnergyHigh       ;  // Upper limit of energy range
    G4double               m_HalfOpenAngleMin ;  // Min Half open angle of the source
    G4double               m_HalfOpenAngleMax ;  // Max Half open angle of the source
    G4double               m_x0               ;  // Vertex Position X
    G4double               m_y0               ;  // Vertex Position Y
    G4double               m_z0               ;  // Vertex Position Z
    G4double               m_SigmaX           ;
    G4double               m_SigmaY           ;
    G4ParticleDefinition*  m_particle         ;  // Kind of particle to shoot isotropically
    vector<G4double>       m_ExcitationEnergy ;  // Excitation energy of the emitted particle
    vector<string>         m_particleName     ;
    ParticleStack*         m_ParticleStack    ;
    vector<G4int>          m_Multiplicty;
    
};
#endif
