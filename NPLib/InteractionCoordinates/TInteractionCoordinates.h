#ifndef __INTERACTIONCOORDINATES__
#define __INTERACTIONCOORDINATES__

#include <vector>
#include "TObject.h"

using namespace std ;


class TInteractionCoordinates : public TObject
{
private:
   // Detected particle properties (before interactions in the target)
   // Vertex of interaction
   vector<Double_t>	fDetected_Position_X;
   vector<Double_t>	fDetected_Position_Y;
   vector<Double_t>	fDetected_Position_Z;
   // Particle angles
   vector<Double_t>	fDetected_Angle_Theta;
   vector<Double_t>	fDetected_Angle_Phi;


public:
   TInteractionCoordinates();
   virtual ~TInteractionCoordinates();

   void  Clear();
   void  Dump();

   /////////////////////           SETTERS           ////////////////////////
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   void SetDetectedPositionX(Double_t PositionX) 		{fDetected_Position_X.push_back(PositionX);}
   void SetDetectedPositionY(Double_t PositionY) 		{fDetected_Position_Y.push_back(PositionY);}
   void SetDetectedPositionZ(Double_t PositionZ)		{fDetected_Position_Z.push_back(PositionZ);}
   // Incident particle angles
   void SetDetectedAngleTheta(Double_t AngleTheta)	{fDetected_Angle_Theta.push_back(AngleTheta);}
   void SetDetectedAnglePhi(Double_t AnglePhi)		{fDetected_Angle_Phi.push_back(AnglePhi);}

   /////////////////////           GETTERS           ////////////////////////
   // Number of interactions (multiplicity)
   Int_t    GetDetectedMultiplicity()		{return fDetected_Position_X.size();}
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   Double_t GetDetectedPositionX(Int_t i) 	{return fDetected_Position_X.at(i);}
   Double_t GetDetectedPositionY(Int_t i) 	{return fDetected_Position_Y.at(i);}
   Double_t GetDetectedPositionZ(Int_t i)	{return fDetected_Position_Z.at(i);}
   // Incident particle angles
   Double_t GetDetectedAngleTheta(Int_t i)	{return fDetected_Angle_Theta.at(i);}
   Double_t GetDetectedAnglePhi(Int_t i)	{return fDetected_Angle_Phi.at(i);}
   
   ClassDef(TInteractionCoordinates, 1) // InteractionCoordinates structure
};

#endif
