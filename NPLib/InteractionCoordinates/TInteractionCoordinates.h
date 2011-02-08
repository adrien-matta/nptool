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
   vector<Double_t>   fDetected_Position_X;
   vector<Double_t>   fDetected_Position_Y;
   vector<Double_t>   fDetected_Position_Z;
   // Particle angles
   vector<Double_t>   fDetected_Angle_Theta;
   vector<Double_t>   fDetected_Angle_Phi;


public:
   TInteractionCoordinates();
   virtual ~TInteractionCoordinates();
/*
   void  Clear();
   void  Dump();
*/
   void  Clear();
   void  Clear(const Option_t*) {};
   void  Dump() const;

   /////////////////////           SETTERS           ////////////////////////
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   void SetDetectedPositionX(Double_t PositionX)      {fDetected_Position_X.push_back(PositionX);}
   void SetDetectedPositionY(Double_t PositionY)      {fDetected_Position_Y.push_back(PositionY);}
   void SetDetectedPositionZ(Double_t PositionZ)      {fDetected_Position_Z.push_back(PositionZ);}
   // Incident particle angles
   void SetDetectedAngleTheta(Double_t AngleTheta)  {fDetected_Angle_Theta.push_back(AngleTheta);}
   void SetDetectedAnglePhi(Double_t AnglePhi)      {fDetected_Angle_Phi.push_back(AnglePhi);}

   /////////////////////           GETTERS           ////////////////////////
   // Number of interactions (multiplicity)
   Int_t    GetDetectedMultiplicity()      {return fDetected_Position_X.size();}
   // Incident particle properties (before interactions in the target)
   // Vertex of interaction
   Double_t GetDetectedPositionX(Int_t i)   {return fDetected_Position_X.at(i);}
   Double_t GetDetectedPositionY(Int_t i)   {return fDetected_Position_Y.at(i);}
   Double_t GetDetectedPositionZ(Int_t i)   {return fDetected_Position_Z.at(i);}
   // Incident particle angles
   Double_t GetDetectedAngleTheta(Int_t i) {return fDetected_Angle_Theta.at(i);}
   Double_t GetDetectedAnglePhi(Int_t i)   {return fDetected_Angle_Phi.at(i);}
   
   ClassDef(TInteractionCoordinates, 1) // InteractionCoordinates structure
};

#endif
