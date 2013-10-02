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

TInitialConditions::TInitialConditions()
{
   // Default constructor

   Clear();
}

TInitialConditions::~TInitialConditions()
{}

void TInitialConditions::Clear()
{
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   fIC_Position_X.clear();
   fIC_Position_Y.clear();
   fIC_Position_Z.clear();
   // Theta and Phi angles for the emittance
   fIC_Incident_Emittance_Theta.clear();
   fIC_Incident_Emittance_Phi.clear();
   // Incident particle angles
   fIC_Incident_Angle_Theta.clear();
   fIC_Incident_Angle_Phi.clear();
   // Incident particle energy
   fIC_Incident_Energy.clear();

   // Emitted particle angles
   fIC_Emitted_Angle_ThetaCM.clear();
   fIC_Emitted_Angle_ThetaLab_IncidentFrame.clear();
   fIC_Emitted_Angle_Phi_IncidentFrame.clear();
   fIC_Emitted_Angle_ThetaLab_WorldFrame.clear();
   fIC_Emitted_Angle_Phi_WorldFrame.clear();
   // Emitted particle energy
   fIC_Emitted_Energy.clear();
}



void TInitialConditions::Dump()
{
   cout << "XXXXXXXXXXXXX Initial conditions XXXXXXXXXXXXXXXX" << endl;

   cout << "Vertex position : " << endl;
   cout << "\tX : " << fIC_Position_X[0] << endl;  
   cout << "\tY : " << fIC_Position_Y[0] << endl;  
   cout << "\tZ : " << fIC_Position_Z[0] << endl;  
   cout << "Theta and Phi angles for the emittance : " << endl;
   cout << "\tTheta : " << fIC_Incident_Emittance_Theta[0] << endl;
   cout << "\tPhi   : " << fIC_Incident_Emittance_Phi[0] << endl;
   cout << "Incident particle angles : " << endl;
   cout << "\tTheta : " << fIC_Incident_Angle_Theta[0] << endl;
   cout << "\tPhi   : " << fIC_Incident_Angle_Phi[0] << endl;
   cout << "Incident energy : " << endl;
   cout << "\tEnergy : " << fIC_Incident_Energy[0] << endl;
   cout << endl;
   cout << "Emitted particle angles : " << endl;
   cout << "\tTheta CM  : " << fIC_Emitted_Angle_ThetaCM[0] << endl;
   cout << "  Incident frame :" << endl;
   cout << "\tTheta Lab : " << fIC_Emitted_Angle_ThetaLab_IncidentFrame[0] << endl;
   cout << "\tPhi       : " << fIC_Emitted_Angle_Phi_IncidentFrame[0] << endl;
   cout << "  World frame :" << endl;
   cout << "\tTheta Lab : " << fIC_Emitted_Angle_ThetaLab_WorldFrame[0] << endl;
   cout << "\tPhi       : " << fIC_Emitted_Angle_Phi_WorldFrame[0] << endl;
   cout << "Emitted energy : " << endl;
   cout << "\tEnergy : " << fIC_Emitted_Energy[0] << endl;
}
