#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class manages the Event Generators used in NPSimulation             *
 *  Every time you add a new kind of event generator, you need to add a few  *
 *  line in the ReadConfiguration method to detect the associate Token       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>

// G4 headers
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"

// NPTool headers
#include "VEventGenerator.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorActionMessenger.hh"
using namespace std;
using namespace CLHEP;

typedef void(NPS::VEventGenerator::*VEventGenerator_FuncPtr)(G4Event*);


class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);
    virtual ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

  public:
    void ReadEventGeneratorFile(string Path);
    void ClearEventGenerator();


  private:// VEventGenerator Function Pointer
    VEventGenerator_FuncPtr m_GenerateEvent;

  public:
    void SetTarget();

  private:
    DetectorConstruction* m_detector;
    vector<NPS::VEventGenerator*> m_EventGenerator;
    PrimaryGeneratorActionMessenger* m_Messenger;
    
};

#endif
