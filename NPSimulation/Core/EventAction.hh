#ifndef EventAction_h
#define EventAction_h 1
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
 *  A quite Standard Geant4 EventAction class.                               *
 *  Call the Fill method of the output tree.                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// G4 header defining G4 types
#include "globals.hh"

// G4 header
#include "G4UserEventAction.hh"

// NPTool header
#include "DetectorConstruction.hh"

// STL
#include<time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class EventAction : public G4UserEventAction{
  public:
    EventAction();
    virtual ~EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    void SetDetector(DetectorConstruction* detector);
    void ProgressDisplay();
    void SetRunLength(int);
  private: // Progress Display
    clock_t begin;
    clock_t end;
    unsigned int treated;
    unsigned int inter;
    unsigned int total;
    double mean_rate;
    int displayed;

  private:
    DetectorConstruction*  m_detector;
    static EventAction* m_EventAction;

  public:
    static EventAction* GetInstance();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
