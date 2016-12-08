#ifndef EventGeneratorpBUU_h
#define EventGeneratorpBUU_h
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
 *  This event Generator is used to simulated ion Source from pBUU simulation         *
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

class EventGeneratorpBUU : public NPS::VEventGenerator{
public:     // Constructor and destructor
    EventGeneratorpBUU();
    virtual ~EventGeneratorpBUU();
  
public:     // Inherit from VEventGenerator Class
    void ReadConfiguration(NPL::InputParser)              ;
    void GenerateEvent(G4Event*) ;
    void InitializeRootOutput()                  ;
  
private:    // Source parameter from input file
    G4double               m_x0               ;  // Vertex Position X
    G4double               m_y0               ;  // Vertex Position Y
    G4double               m_z0               ;  // Vertex Position Z
    G4double               m_SigmaX           ;
    G4double               m_SigmaY           ;
    G4ParticleDefinition*  m_particle         ;  // Kind of particle to shoot 
    G4double               m_ExcitationEnergy ;  // Excitation energy of the emitted particle
    string                 m_particleName     ;
    ParticleStack*         m_ParticleStack    ;
    
    //TH1F* fAngleHist;
    //TH1F* fEnergyHist;
    TH2F* fAngleEnergyHist;
  
};
#endif
