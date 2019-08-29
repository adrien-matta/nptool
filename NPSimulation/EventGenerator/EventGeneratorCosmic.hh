#ifndef EventGeneratorCosmic_h
#define EventGeneratorCosmic_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano TRONCHIN                                        *
 * contact address: elidiano.tronchin(at)gmail.com                           *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated Cosmic Rays                    *
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
class EventGeneratorCosmic : public NPS::VEventGenerator{
public:     // Constructor and destructor
    EventGeneratorCosmic() ;
    virtual ~EventGeneratorCosmic();
    
public:     // Inherit from VEventGenerator Class
    void ReadConfiguration(NPL::InputParser)               ;
    void GenerateEvent(G4Event*) ;
    void InitializeRootOutput()                  ;
	
private:    // Source parameter from input file
    G4int event_ID;
	struct SourceParameters {
		SourceParameters()                          ;
    G4double                 m_EnergyLow        ;  // Lower limit of energy range
    G4double                 m_EnergyHigh       ;  // Upper limit of energy range
    G4double                 m_HalfOpenAngleMin ;  // Min Half open angle of the source
    G4double                 m_HalfOpenAngleMax ;  // Max Half open angle of the source
    G4double                 m_x0               ;  // Vertex Position X
    G4double                 m_y0               ;  // Vertex Position Y
    G4double                 m_z0               ;  // Vertex Position Z
    G4double                 m_SigmaX           ;
    G4double                 m_SigmaY           ;
    G4ParticleDefinition*    m_particle         ;  // Kind of particle to shoot isotropically
    vector<G4double>         m_ExcitationEnergy ;  // Excitation energy of the emitted particle
    vector<string>           m_particleName     ;
    vector<G4int>            m_Multiplicty      ;
	};
	  vector<SourceParameters> m_Parameters       ;
    ParticleStack*           m_ParticleStack    ;
    
};
#endif
