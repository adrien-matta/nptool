/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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

#include <iostream>
using namespace std;

#include "TInitialConditions.h"

ClassImp(TInitialConditions)

TInitialConditions::TInitialConditions(){
  // Default constructor
  Clear();
}

TInitialConditions::~TInitialConditions(){
}

void TInitialConditions::Clear(){
  // Incident beam parameter
  fIC_Incident_Particle_Name = "";
  fIC_Incident_Emittance_ThetaX = -1;
  fIC_Incident_Emittance_PhiY = -1;
  fIC_Incident_Emittance_Theta = -1;
  fIC_Incident_Emittance_Phi = -1;
  fIC_Incident_Initial_Kinetic_Energy = -1;
  fIC_Incident_Final_Kinetic_Energy = -1;
  fIC_Incident_Position_X = -1;
  fIC_Incident_Position_Y = -1;
  fIC_Incident_Position_Z = -1;
  
  // emmitted particle
  fIC_Particle_Name.clear();
  fIC_ThetaCM.clear();
  fIC_Kinetic_Energy.clear();
  fIC_Momentum_Direction_X.clear();
  fIC_Momentum_Direction_Y.clear();
  fIC_Momentum_Direction_Z.clear();
}

void TInitialConditions::Dump() const{
  cout << "--------- Initial Condition Dump ---------" << endl ;
  
  // Incident beam parameter
  cout << "\t ---- Incident Beam ---- " << endl;
  cout << "\t Particle Name:  " << fIC_Incident_Particle_Name << endl;
  cout << "\t Energy: " << fIC_Incident_Initial_Kinetic_Energy << endl;
  cout << "\t Theta_X: " << fIC_Incident_Emittance_ThetaX << endl;
  cout << "\t Phi_Y: " << fIC_Incident_Emittance_PhiY << endl;
  cout << "\t Theta: " << fIC_Incident_Emittance_Theta << endl;
  cout << "\t Phi: " << fIC_Incident_Emittance_Phi << endl;
  
  
  // Beam status at the initial interaction point
  cout << "\t ---- Interaction Point ---- " << endl;
  cout << "\t Energy: " << fIC_Incident_Final_Kinetic_Energy << endl;
  cout << "\t Position: ( "
  << fIC_Incident_Position_X << " ; "
  << fIC_Incident_Position_Y << " ; "
  << fIC_Incident_Position_Z << ")" << endl;
  
  
  // emmitted particle
  unsigned int size = fIC_Particle_Name.size();
  for(unsigned int i = 0 ; i < size; i ++){
    cout << "\t ---- Particle " << i << " ---- " << endl;
    cout << "\t Particle Name" <<   fIC_Particle_Name[i] << endl;
    cout << "\t Theta CM" <<   fIC_ThetaCM[i] << endl;
    cout << "\t Energy" <<   fIC_Kinetic_Energy[i] << endl;
    cout << "\t Momentum Direction: ( "
    << fIC_Momentum_Direction_X[i] << " ; "
    << fIC_Momentum_Direction_Y[i] << " ; "
    << fIC_Momentum_Direction_Z[i] << ")" << endl;
    
  }
  
  
}


TVector3 TInitialConditions::GetBeamDirection() const{
  return TVector3( sin(fIC_Incident_Emittance_ThetaX),
                   sin(fIC_Incident_Emittance_PhiY),
                   cos(fIC_Incident_Emittance_ThetaX) + cos(fIC_Incident_Emittance_PhiY));
}

TVector3 TInitialConditions::GetParticleDirection (const int &i) const {
  return TVector3(  fIC_Momentum_Direction_X[i],
                    fIC_Momentum_Direction_Y[i],
                    fIC_Momentum_Direction_Z[i]);
}





