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



void TInteractionCoordinates::Dump()
{
   cout << "XXXXXXXXXXXXX Initial conditions XXXXXXXXXXXXXXXX" << endl;

   cout << "Vertex position : " << endl;
   cout << "\tX : " << fDetected_Position_X[0] << endl;  
   cout << "\tY : " << fDetected_Position_Y[0] << endl;  
   cout << "\tZ : " << fDetected_Position_Z[0] << endl;  
   cout << "Incident particle angles : " << endl;
   cout << "\tTheta : " << fDetected_Angle_Theta[0] << endl;
   cout << "\tPhi   : " << fDetected_Angle_Phi[0] << endl;
}
