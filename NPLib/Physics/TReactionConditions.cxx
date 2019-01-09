/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : 17/07/18                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class records all the information concerning the beam    *
 *             rwaction generators, e.g. vertex of interaction, angles of    *
 *             emitted particles...                                          *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "TReactionConditions.h"

ClassImp(TReactionConditions)
////////////////////////////////////////////////////////////////////////////////
TReactionConditions::TReactionConditions(){
}
////////////////////////////////////////////////////////////////////////////////
TReactionConditions::~TReactionConditions(){
}
////////////////////////////////////////////////////////////////////////////////
void TReactionConditions::Clear(){
    // Beam beam parameter
    fRC_Beam_Particle_Name="";
    fRC_Beam_Emittance_ThetaX = -1;
    fRC_Beam_Emittance_PhiY = -1;
    fRC_Beam_Emittance_Theta = -1;
    fRC_Beam_Emittance_Phi = -1;
    fRC_Beam_Reaction_Energy = -1;

    fRC_Vertex_Position_X = -1;
    fRC_Vertex_Position_Y = -1;
    fRC_Vertex_Position_Z = -1;
    
    fRC_ThetaCM = -1;
    
    // emmitted particles
    fRC_Particle_Name.clear();
    fRC_Theta.clear();
    fRC_Phi.clear();
    fRC_LAB_Theta.clear();
    fRC_LAB_Phi.clear();
    fRC_Kinetic_Energy.clear();
    fRC_Momentum_Direction_X.clear();
    fRC_Momentum_Direction_Y.clear();
    fRC_Momentum_Direction_Z.clear();
}
////////////////////////////////////////////////////////////////////////////////
void TReactionConditions::Dump() const{
    cout << "--------- Initial Condition Dump ---------" << endl ;
    
    // Beam beam parameter
    cout << "\t ---- Beam ---- " << endl;
    cout << "\t Particle Name:  " << fRC_Beam_Particle_Name << endl;
    cout << "\t Reaction Energy: " << fRC_Beam_Reaction_Energy << endl;
    cout << "\t Theta_X: " << fRC_Beam_Emittance_ThetaX << endl;
    cout << "\t Phi_Y: " << fRC_Beam_Emittance_PhiY << endl;
    cout << "\t Theta: " << fRC_Beam_Emittance_Theta << endl;
    cout << "\t Phi: " << fRC_Beam_Emittance_Phi << endl;
    
    
    // Beam status at the initial interaction point
    cout << "\t ---- Interaction Point ---- " << endl;
    cout << "\t Energy: " << fRC_Beam_Reaction_Energy << endl;
    cout << "\t ThetaCM: " << fRC_ThetaCM << endl;
    cout << "\t Position: ( "
    << fRC_Vertex_Position_X << " ; "
    << fRC_Vertex_Position_Y << " ; "
    << fRC_Vertex_Position_Z << ")" << endl;
    
    // emmitted particle
    unsigned int size = fRC_Particle_Name.size();
    for(unsigned int i = 0 ; i < size; i ++){
        cout << "\t ---- Particle " << i << " ---- " << endl;
        cout << "\t Particle Name: " <<   fRC_Particle_Name[i] << endl;
        cout << "\t Kinetic Energy: " <<   fRC_Kinetic_Energy[i] << endl;
        cout << "\t Momentum Direction: ( "
        << fRC_Momentum_Direction_X[i] << " ; "
        << fRC_Momentum_Direction_Y[i] << " ; "
        << fRC_Momentum_Direction_Z[i] << ")" << endl;
    }

   
}
////////////////////////////////////////////////////////////////////////////////
TVector3 TReactionConditions::GetBeamDirection() const{
    return TVector3( sin(fRC_Beam_Emittance_Theta*deg)*cos(fRC_Beam_Emittance_Phi*deg),
                    sin(fRC_Beam_Emittance_Theta*deg)*sin(fRC_Beam_Emittance_Phi*deg),
                    cos(fRC_Beam_Emittance_Theta*deg));
}
////////////////////////////////////////////////////////////////////////////////
TVector3 TReactionConditions::GetParticleDirection (const int &i) const {
    return TVector3(  fRC_Momentum_Direction_X[i],
                    fRC_Momentum_Direction_Y[i],
                    fRC_Momentum_Direction_Z[i]);
}

