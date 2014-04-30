#ifndef EventGeneratorNuclearDeexcitation_H
#define EventGeneratorNuclearDeexcitation_H
/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * NPTool Author: Adrien MATTA        contact address: matta@ipno.in2p3.fr   *
 * Class Author: Lee Evitts           contact address: evitts@triumf.ca      *
 *                                                                           *
 * Creation Date  : April 2014                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Read in a decay scheme for nuclear de-excitation (involving gamma rays,   *
 * ICC electrons and IPF e-/e+ pairs                                         *  
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// STL
#include <string>
#include <iostream>
using namespace std;

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"
#include "ParticleStack.hh"

// ROOT
#include "TH1F.h"
#include "TH2F.h"

class EventGeneratorNuclearDeexcitation : public VEventGenerator{
public: // Constructor and destructor
  EventGeneratorNuclearDeexcitation();
  ~EventGeneratorNuclearDeexcitation();
  
public: // Inherit from VEventGenerator class
  void ReadConfiguration(string, int);
  void GenerateEvent(G4Event*);

private: // Transition and level properties
  string                  fNucleiName;
  double                  fBindingEnergy;
  vector<int>             mLevelID;
  vector<double>          mLevelProb;
  vector<double>          mLevelEnergy;
  vector<int>             mTransID;
  vector<double>          mTransEnergy;
  vector<TH2F*>           mCrossSectionLeptonHist2D;
  vector<double>          mTransProb;
  vector<double>          mTransICC;
  vector<string>          mTransField;
  vector<int>             mTransOrder;
  vector<double>          mTransIPFC;
  vector<int>             mTransFID;
  
private: // Particle properties that are passed to the stack
  G4ParticleDefinition*   fParticleDefinition;
  G4ThreeVector           fParticleDirection;
  G4double                fEmissionTheta;
  G4ThreeVector           fPosition;
  G4double                fParticleEnergy;
  
  ParticleStack*          fParticleStack;

private: // Flags used such that only one particle is emitted per event
  G4bool                  fInCascade;
  G4int                   fChosenLevel;

private: // Store the IPF e- data, emitted next event
  G4bool                  fEmitSecondIPF;
  G4ParticleDefinition*   fElectronDefinition;
  double                  fElectronTheta;
  double                  fElectronEnergy;
  G4ThreeVector           fElectronDirection;
  double                  fElectronPhi;
  
private:
  void AddTransition(int CurrentLevelID, double CurrentLevelProb, double CurrentLevelEnergy,
                     int TransitionID, double TransitionProbability, double ICC, string FieldType,
                     int PolarityOrder, double IPFC, int FinalLevelID) {
                     
    mLevelID.push_back(CurrentLevelID);
    mLevelProb.push_back(CurrentLevelProb);
    mLevelEnergy.push_back(CurrentLevelEnergy);
    mTransID.push_back(TransitionID);
    mTransEnergy.push_back(0);
    mTransProb.push_back(TransitionProbability);
    mTransICC.push_back(ICC);
    mTransField.push_back(FieldType);
    mTransOrder.push_back(PolarityOrder);
    mTransIPFC.push_back(IPFC);
    mTransFID.push_back(FinalLevelID);
    
  }

  TH2F* BuildCrossSection(double transition_energy, string field_type, double polarity_order);
  double GetBornCrossSection(string field_type, double transition_energy_ratio, int multipolarity, double tot_energy_p, double tot_energy_m, double angle_positron_electron);


};
#endif
