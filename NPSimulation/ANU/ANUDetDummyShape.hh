/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 * Modified for ANU by: Lee Evitts  contact address: evitts@triumf.ca        *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Modified Date  : 07/04/14                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the ANU detector                    *
 *             Taken from the HELIOS detector and modified
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: 
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef ANUDetDummyShape_h
#define ANUDetDummyShape_h 1

// C++ headers
#include <vector>

// NPTool header
#include "ANUModule.hh"
#include "TInteractionCoordinates.h"

using namespace std;



class ANUDetDummyShape : public ANUModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   ANUDetDummyShape();
   virtual ~ANUDetDummyShape();

   ////////////////////////////////////////////////////
   ////  Inherit from ANUModule class /////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path);

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world);

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput();

   // Initialize all scorers necessary for the detector
   void InitializeScorers();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);

   // Give the static TInteractionCoordinates from VDetector to the classes
   // deriving from ANUModule
   // This is mandatory since the ANU*** does not derive from VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()	{return ms_InterCoord;};


  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  // Interaction Coordinates coming from VDetector through the 
  // SetInteractionCoordinatesPointer method
  TInteractionCoordinates* ms_InterCoord;

  // Arrays used to describe position/size of detector element
  vector<G4double>  m_Z  ; 
  vector<G4double>  m_T  ; 
  vector<G4double>  m_R1 ; 
  vector<G4double>  m_R2 ; 
  vector<G4double>  m_PC ; 
  vector<G4double>  m_PD ; 

};



namespace ANUDUMMYSHAPE
{
  // TODO: Check all these, what they do etc..
   // Resolution
  const G4double ResoSiLi  = 0.0085;	// = 20 keV of Resolution   //   Unit is MeV/2.35
  const G4double ResoTimeGpd     = 0.4255; // = 1ns        // 0.212765957;// = 500ps                 //   Unit is  ns/2.35
  const G4double ResoPosZ     = 0.4255;// = 1mm  for ANU               //   Unit is  mm/2.35

   // Geometry for the mother volume containing the different layers of your dummy shape module
   const G4double FaceFrontWidth          = 1.2*cm;
   const G4double FaceFrontLength         = 5.6*cm;
   const G4double FaceBackWidth           = 1.2*cm;
   const G4double FaceBackLength          = 5.6*cm;
   const G4double Thickness             = 0.1*cm;  // ie: thickness 1 mm
   const G4double InterStageDistance = 5*mm;
   
   // First stage
   const G4double SiLiFaceWidth       = 0.9*cm;
   const G4double SiLiFaceLength      = 5.05*cm;
   const G4double SiLiThickness  = 700*micrometer;
//   const G4double SiLiThickness  = 2*mm;
//   for testing the excitation energy reconstruction
//   const G4double SiLiThickness  = 1.3*cm;
  const G4int NumberOfStrips	       = 1; // PSD resistive strip


}

#endif
