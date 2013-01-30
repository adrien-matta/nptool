/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    : 04/09/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class records all the information concerning the event   *
 *             generators, e.g. vertex of interaction, angles of emitted     *
 *             particles...                                                  *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 04/09/09: Add private members for emittance  (N. de Sereville)       *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef __INITIALCONDITIONS__
#define __INITIALCONDITIONS__

// STL Header
#include <vector>
#include <string>

using namespace std ;

// Root Header
#include "TObject.h"

class TInitialConditions : public TObject{
private:
  
  // Incident beam parameter
  string fIC_Incident_Particle_Name;
  double fIC_Incident_Emittance_Phi;
  double fIC_Incident_Emittance_Theta;
  double fIC_Incident_Kinetic_Energy;
  
  // Beam status at the initial interaction point
  double fIC_Interaction_Kinetic_Energy;
  double fIC_Interaction_Position_X;
  double fIC_Interaction_Position_Y;
  double fIC_Interaction_Position_Z;
  
  // emmitted particle
  vector<string> fIC_Particle_Name;
  vector<string> fIC_Process_Name;
  vector<double> fIC_ThetaCM;
  vector<double> fIC_Kinetic_Energy;
  vector<double> fIC_Momentum_Direction_X;
  vector<double> fIC_Momentum_Direction_Y;
  vector<double> fIC_Momentum_Direction_Z;
  
public:
  TInitialConditions();
  virtual ~TInitialConditions();
  
  void  Clear();
  void  Clear(const Option_t*) {};
  void  Dump() const;
  
  /////////////////////           GetTERS           ////////////////////////
  // Incident beam parameter
  void SetIncidentParticleName   (string Incident_Particle_Name)   {fIC_Incident_Particle_Name   = Incident_Particle_Name;}
  void SetIncidentKineticEnergy  (double Incident_Kinetic_Energy)  {fIC_Incident_Kinetic_Energy  = Incident_Kinetic_Energy;}
  void SetIncidentEmittanceTheta (double Incident_Emittance_Theta) {fIC_Incident_Emittance_Theta = Incident_Emittance_Theta;}
  void SetIncidentEmittancePhi   (double Incident_Emittance_Phi)   {fIC_Incident_Emittance_Phi   = Incident_Emittance_Phi;}
  
  // Beam status at the initial interaction point
  void SetInteractionKineticEnergy (double Interaction_Kinetic_Energy)  {fIC_Interaction_Kinetic_Energy = Interaction_Kinetic_Energy;}
  void SetInteractionPositionX     (double Interaction_Position_X)      {fIC_Interaction_Position_X     = Interaction_Position_X;}
  void SetInteractionPositionY     (double Interaction_Position_Y)      {fIC_Interaction_Position_Y     = Interaction_Position_Y;}
  void SetInteractionPositionZ     (double Interaction_Position_Z)      {fIC_Interaction_Position_Z     = Interaction_Position_Z;}
  
  // emmitted particle
  void SetParticleName       (string Particle_Name)         {fIC_Particle_Name.push_back(Particle_Name);}
  void SetProcessName        (string Process_Name)          {fIC_Process_Name.push_back(Process_Name);}
  void SetThetaCM            (double ThetaCM)               {fIC_ThetaCM.push_back(ThetaCM);}
  void SetKineticEnergy      (double Kinetic_Energy)        {fIC_Kinetic_Energy.push_back(Kinetic_Energy);}
  void SetMomentumDirectionX (double Momentum_Direction_X)  {fIC_Momentum_Direction_X.push_back(Momentum_Direction_X);}
  void SetMomentumDirectionY (double Momentum_Direction_Y)  {fIC_Momentum_Direction_Y.push_back(Momentum_Direction_Y);}
  void SetMomentumDirectionZ (double Momentum_Direction_Z)  {fIC_Momentum_Direction_Z.push_back(Momentum_Direction_Z);}
  /////////////////////           GETTERS           ////////////////////////
  // Incident beam parameter
  string GetIncidentParticleName   () const  {return fIC_Incident_Particle_Name   ;}
  double GetIncidentKineticEnergy  () const  {return fIC_Incident_Kinetic_Energy  ;}
  double GetIncidentEmittanceTheta () const  {return fIC_Incident_Emittance_Theta ;}
  double GetIncidentEmittancePhi   () const  {return fIC_Incident_Emittance_Phi   ;}
  
  // Beam status at the initial interaction point
  double GetInteractionKineticEnergy () const {return fIC_Interaction_Kinetic_Energy ;}
  double GetInteractionPositionX     () const {return fIC_Interaction_Position_X     ;}
  double GetInteractionPositionY     () const {return fIC_Interaction_Position_Y     ;}
  double GetInteractionPositionZ     () const {return fIC_Interaction_Position_Z     ;}
  
  // emmitted particle
  string GetParticleName        (int i) const {return fIC_Particle_Name[i];}
  string GetProcessName         (int i) const {return fIC_Process_Name[i];}
  double GetThetaCM             (int i) const {return fIC_ThetaCM[i];}
  double GetKineticEnergy       (int i) const {return fIC_Kinetic_Energy[i];}
  double GetMomentumDirectionX  (int i) const {return fIC_Momentum_Direction_X[i];}
  double GetMomentumDirectionY  (int i) const {return fIC_Momentum_Direction_Y[i];}
  double GetMomentumDirectionZ  (int i) const {return fIC_Momentum_Direction_Z[i];}
  
  unsigned int GetMult() const {return fIC_Particle_Name.size();}
  
  ClassDef(TInitialConditions, 1) // InitialConditions structure
};

#endif
