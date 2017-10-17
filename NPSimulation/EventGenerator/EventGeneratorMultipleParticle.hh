#ifndef EventGeneratorMultipleParticle_h
#define EventGeneratorMultipleParticle_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouac@nscl.msu.edu       *
 *                                                                           *
 * Creation Date  : April 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated data from transport model outputs         *
 *  Very usefull to figure out Geometric Efficacity of experimental Set-Up   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>
using namespace std;

using namespace CLHEP;

// G4 headers
#include "G4Event.hh"

// NPS headers
#include "VEventGenerator.hh"
#include "ParticleStack.hh"
#include "NPInputParser.h"
// ROOT Headers
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

class EventGeneratorMultipleParticle : public NPS::VEventGenerator{
public:     // Constructor and destructor
    EventGeneratorMultipleParticle();
    virtual ~EventGeneratorMultipleParticle();
  
public:     // Inherit from VEventGenerator Class
    void ReadConfiguration(NPL::InputParser)              ;
    void GenerateEvent(G4Event*) ;
    void InitializeRootOutput()                  ;
  
private:    // Source parameter from input file
    G4double                m_x0               ;  // Vertex Position X
    G4double                m_y0               ;  // Vertex Position Y
    G4double                m_z0               ;  // Vertex Position Z
    G4double                m_SigmaX           ;
    G4double                m_SigmaY           ;
    unsigned int            m_Events;
    string                  m_FileName;
    string                  m_File;
    G4ParticleDefinition*   m_particle         ;  // Kind of particle to shoot
    vector<int>             m_Event;
    vector<int>             m_Multiplicity;
    vector<vector<string>>  m_particleName     ;
    vector<vector<double>>  m_Energy;
    vector<vector<double>>  m_Theta;
    ParticleStack*          m_ParticleStack    ;

  
};
#endif
