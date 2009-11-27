/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class mainly records the coordinates of interaction      *
 *             between a particle and a detector.                            *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "TInteractionCoordinates.h"

ClassImp(TInteractionCoordinates)

TInteractionCoordinates::TInteractionCoordinates()
{
   // Default constructor

   Clear();
}

TInteractionCoordinates::~TInteractionCoordinates()
{}

void TInteractionCoordinates::Clear()
{
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   fDetected_Position_X.clear();
   fDetected_Position_Y.clear();
   fDetected_Position_Z.clear();
   // Incident particle angles
   fDetected_Angle_Theta.clear();
   fDetected_Angle_Phi.clear();
}



void TInteractionCoordinates::Dump() const
{
   cout << "XXXXXXXXXXXXX Interaction coordinates XXXXXXXXXXXXXXXX" << endl;

   cout << "Interaction position : " << endl;
   cout << "\tX : " << fDetected_Position_X[0] << endl;  
   cout << "\tY : " << fDetected_Position_Y[0] << endl;  
   cout << "\tZ : " << fDetected_Position_Z[0] << endl;  
   cout << "Incident particle angles : " << endl;
   cout << "\tTheta : " << fDetected_Angle_Theta[0] << endl;
   cout << "\tPhi   : " << fDetected_Angle_Phi[0] << endl;
}
