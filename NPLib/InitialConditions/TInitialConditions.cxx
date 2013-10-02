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
