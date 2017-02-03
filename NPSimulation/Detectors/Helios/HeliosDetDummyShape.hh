/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact   address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    : 31/08/15                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the Helios detector                 *
 *             The goal of this class is to be a starting point to create a  *
 *             new shape to be added to the Helios detector.                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef HeliosDetDummyShape_h
#define HeliosDetDummyShape_h 1

// C++ headers
#include <vector>

// NPTool header
#include "HeliosModule.hh"
#include "TInteractionCoordinates.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;



class HeliosDetDummyShape : public HeliosModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   HeliosDetDummyShape();
   virtual ~HeliosDetDummyShape();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4ThreeVector TL           ,
                  G4ThreeVector BL           ,
                  G4ThreeVector BR           ,
                  G4ThreeVector CT           ,
                  bool          wFirstStage  );

   // By Angle Method
   void AddModule(G4double R            ,
                  G4double Theta        ,
                  G4double Phi          ,
                  G4double beta_u       ,
                  G4double beta_v       ,
                  G4double beta_w       ,
                  bool     wFirstStage  );

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int TelescopeNumber          ,
                    G4ThreeVector     MMpos        ,
                    G4RotationMatrix* MMrot        ,
                    bool              wFirstStage  ,
                    G4LogicalVolume*  world);


   ////////////////////////////////////////////////////
   ////  Inherite from HeliosModule class /////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(NPL::InputParser);

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

   // Give the static TInteractionCoordinates from NPS::VDetector to the classes
   // deriving from HeliosModule
   // This is mandatory since the Helios*** does not derive from NPS::VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()	{return ms_InterCoord;};


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Interaction Coordinates coming from NPS::VDetector through the 
   // SetInteractionCoordinatesPointer method
   TInteractionCoordinates* ms_InterCoord;

   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_X1_Y1     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_X1_Y128   ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y1   ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y128 ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  > Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |
};



namespace HELIOSDUMMYSHAPE
{
   // Resolution

  const G4double ResoFirstStage  = 0.0170;	// = 40 keV of Resolution <=> Helios range is 25 to 55 keV resolution  //   Unit is MeV/2.35
//  const G4double ResoFirstStage  = 0.0085;	// = 20 keV of Resolution   //   Unit is MeV/2.35
  const G4double ResoTimeGpd     = 0.4255; // = 1ns        // 0.212765957;// = 500ps                 //   Unit is  ns/2.35
  //const G4double ResoFirstStage  = 0.;	// =  detector energy resolution is taken care in the analysis code !!!!
  //const G4double ResoTimeGpd     = 0.; // = 1ns        // 0.212765957;// = 500ps                 //   Unit is  ns/2.35
  //const G4double ResoPosZ     = 0.; // =  detector energy resolution is taken care in the analysis code !!!!
  const G4double ResoPosZ     = 0.4255;// = 1mm  for Helios               //   Unit is  mm/2.35

   // Geometry for the mother volume containing the different layers of your dummy shape module
   // Note : dimension has to be the same as the sensitive volume otherwise there are issues with the NPAnalysis/Helios

   /* For Helisol :
   //const G4double FaceFrontWidth          = 22.*mm;  // for helisol
   const G4double FaceFrontWidth          = 24.*mm;  // for helisol
   const G4double FaceFrontLength         = 125*mm;  // for helisol
  //const G4double FaceBackWidth           = 22.*mm;  // for helisol
   const G4double FaceBackWidth           = 24.*mm;  // for helisol
   const G4double FaceBackLength          = 125*mm;  // for helisol
   const G4double Thickness             = 1.002*mm;  // ie: thickness 1 mm + 2um
   const G4double InterStageDistance = 2.5*mm; // for helisol

  //const G4double AlThickness             = 0.001*mm;  // ie: thickness 1um
  const G4double AlThickness             = 0.00055*mm;  // ie: thickness 0.55um
  //const G4double AlThickness             = 0.00015*mm;  // ie: thickness 0.15um
    */

	/* For helios: */
   const G4double FaceFrontWidth          = 1.2*cm; // for helios
   const G4double FaceFrontLength         = 5.6*cm; // for helios
   const G4double FaceBackWidth           = 1.2*cm; // for helios
   const G4double FaceBackLength          = 5.6*cm; // for helios
   const G4double Thickness             = 1.*mm;  // for helios
   const G4double InterStageDistance = 5*mm;  // for helios
	

    // for testing the excitation energy reconstruction
//   const G4double Length             = 4*cm;
//   const G4double InterStageDistance = 15*mm;

   // First stage

   /* For Helisol :
   //const G4double FirstStageFaceWidth       = 20*mm;   // for helisol50.6
   //const G4int NumberOfLStrips	            = 10; // 2 mm strip pitch for strips parallel to beam axis 
     const G4double FirstStageFaceWidth       = 22*mm;   // for helisol54
     const G4int NumberOfLStrips	      = 11; // if 2 mm strip pitch for strips parallel to beam axis 
   //const G4int NumberOfLStrips	       = 22; // if 1 mm strip pitch for strips parallel to beam axis 
    //const G4double FirstStageFaceWidth       = 24*mm;   // for helisol57.5
   //const G4int NumberOfLStrips	            = 12; // 2 mm strip pitch for strips parallel to beam axis 

   const G4double FirstStageFaceLength      = 125*mm;  // for helisol
   const G4double FirstStageThickness  = 1000*micrometer;  // for helisol
    */
	/* For helios: */
   const G4double FirstStageFaceWidth       = 0.9*cm; // for helios
   const G4double FirstStageFaceLength      = 5.05*cm; // for helios
   //const G4double FirstStageThickness  = 1000*micrometer;  // for helios 
   const G4double FirstStageThickness  = 700*micrometer;
//   const G4double FirstStageThickness  = 2*mm;
//   for testing the excitation energy reconstruction
//   const G4double FirstStageThickness  = 1.3*cm;

   const G4int NumberOfStrips	       = 1; // PSD resistive strip

//  const G4int NumberOfStrips	       = 250; // PSD resistive strip
//   const G4int NumberOfLStrips	       = 22; // 1 mm strip pitch for strips parallel to beam axis 
//   const G4int NumberOfTStrips	       = 250; // .5 mm strip pitch for strips perpendicular to beam axis
//   const G4int NumberOfLStrips	       = 22; // 1 mm strip pitch for strips parallel to beam axis 
//   const G4int NumberOfLStrips	       = 11; // 2 mm strip pitch for strips parallel to beam axis 
//   const G4int NumberOfTStrips	       = 128; // ~1. mm strip pitch for strips perpendicular to beam axis
//   const G4int NumberOfTStrips	       = 25; // 1. mm strip pitch for strips perpendicular to beam axis
//   const G4int NumberOfTStrips	       = 62; // 2. mm strip pitch for strips perpendicular to beam axis


   // Starting at the front of the first stage and going to the third stage 
   //const G4double FirstStage_PosZ  = Thickness* -0.5 + 0.5*FirstStageThickness;
   const G4double FirstStage_PosZ  = 0.;
}

#endif
