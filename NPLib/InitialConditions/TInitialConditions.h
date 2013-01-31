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
  double fIC_Incident_Emittance_ThetaX;
  double fIC_Incident_Emittance_PhiY;
  double fIC_Incident_Emittance_Theta;
  double fIC_Incident_Emittance_Phi;
  double fIC_Incident_Initial_Kinetic_Energy;
  double fIC_Incident_Final_Kinetic_Energy;
  double fIC_Incident_Position_X;
  double fIC_Incident_Position_Y;
  double fIC_Incident_Position_Z;
  
  // emmitted particle
  vector<string> fIC_Particle_Name;
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
  void SetIncidentParticleName   (string Incident_Particle_Name)   {fIC_Incident_Particle_Name = Incident_Particle_Name;}
  void SetIncidentInitialKineticEnergy  (double Incident_Initial_Kinetic_Energy)
    {fIC_Incident_Initial_Kinetic_Energy  = Incident_Initial_Kinetic_Energy;}
  void SetIncidentFinalKineticEnergy  (double Incident_Final_Kinetic_Energy)
    {fIC_Incident_Final_Kinetic_Energy  = Incident_Final_Kinetic_Energy;}

  void SetIncidentEmittanceTheta (double Incident_Emittance_Theta) {fIC_Incident_Emittance_Theta = Incident_Emittance_Theta;}
  void SetIncidentEmittancePhi   (double Incident_Emittance_Phi)   {fIC_Incident_Emittance_Phi   = Incident_Emittance_Phi;}
  void SetIncidentEmittanceThetaX (double Incident_Emittance_ThetaX) {fIC_Incident_Emittance_ThetaX = Incident_Emittance_ThetaX;}
  void SetIncidentEmittancePhiY   (double Incident_Emittance_PhiY)   {fIC_Incident_Emittance_PhiY   = Incident_Emittance_PhiY;}
  
  // Beam status at the initial interaction point
  void SetIncidentPositionX     (double Incident_Position_X)      {fIC_Incident_Position_X     = Incident_Position_X;}
  void SetIncidentPositionY     (double Incident_Position_Y)      {fIC_Incident_Position_Y     = Incident_Position_Y;}
  void SetIncidentPositionZ     (double Incident_Position_Z)      {fIC_Incident_Position_Z     = Incident_Position_Z;}
  
  // emmitted particle
  void SetParticleName       (string Particle_Name)         {fIC_Particle_Name.push_back(Particle_Name);}
  void SetThetaCM            (double ThetaCM)               {fIC_ThetaCM.push_back(ThetaCM);}
  void SetKineticEnergy      (double Kinetic_Energy)        {fIC_Kinetic_Energy.push_back(Kinetic_Energy);}
  void SetMomentumDirectionX (double Momentum_Direction_X)  {fIC_Momentum_Direction_X.push_back(Momentum_Direction_X);}
  void SetMomentumDirectionY (double Momentum_Direction_Y)  {fIC_Momentum_Direction_Y.push_back(Momentum_Direction_Y);}
  void SetMomentumDirectionZ (double Momentum_Direction_Z)  {fIC_Momentum_Direction_Z.push_back(Momentum_Direction_Z);}
  
  /////////////////////           GETTERS           ////////////////////////
  // Incident beam parameter
  string GetIncidentParticleName   () const  {return fIC_Incident_Particle_Name   ;}
  double GetIncidentInitialKineticEnergy  () const  {return fIC_Incident_Initial_Kinetic_Energy  ;}
  double GetIncidentFinalKineticEnergy () const {return fIC_Incident_Final_Kinetic_Energy ;}
  double GetIncidentEmittanceTheta () const  {return fIC_Incident_Emittance_Theta ;}
  double GetIncidentEmittancePhi   () const  {return fIC_Incident_Emittance_Phi   ;}
  
  // Beam status at the initial Incident point
  double GetIncidentPositionX     () const {return fIC_Incident_Position_X     ;}
  double GetIncidentPositionY     () const {return fIC_Incident_Position_Y     ;}
  double GetIncidentPositionZ     () const {return fIC_Incident_Position_Z     ;}
  
  // emmitted particle
  string GetParticleName        (int i) const {return fIC_Particle_Name[i];}
  double GetThetaCM             (int i) const {return fIC_ThetaCM[i];}
  double GetKineticEnergy       (int i) const {return fIC_Kinetic_Energy[i];}
  double GetMomentumDirectionX  (int i) const {return fIC_Momentum_Direction_X[i];}
  double GetMomentumDirectionY  (int i) const {return fIC_Momentum_Direction_Y[i];}
  double GetMomentumDirectionZ  (int i) const {return fIC_Momentum_Direction_Z[i];}
  
  unsigned int GetEmittedMult() const {return fIC_Particle_Name.size();}
  
  ClassDef(TInitialConditions, 1) // InitialConditions structure
};

#endif
