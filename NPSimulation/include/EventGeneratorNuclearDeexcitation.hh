#ifndef EventGeneratorNuclearDeexcitation_H
#define EventGeneratorNuclearDeexcitation_H

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

private:
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
  
  G4ParticleDefinition*   fParticleDefinition;
  G4ThreeVector           fParticleDirection;
  G4double                fEmissionTheta;
  G4ThreeVector           fPosition;
  G4double                fParticleEnergy;
  
  ParticleStack*          fParticleStack;
  
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
