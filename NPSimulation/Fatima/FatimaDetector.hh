/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a phoswich module for the Fatima detector.              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef FatimaDetector_h
#define FatimaDetector_h 1

// C++ headers
#include <vector>

// NPTool header
#include "FatimaModule.hh"
#include "TInteractionCoordinates.h"

using namespace CLHEP;



class FatimaDetector : public FatimaModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   FatimaDetector();
   virtual ~FatimaDetector();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4ThreeVector TL,
                  G4ThreeVector BL,
                  G4ThreeVector BR,
                  G4ThreeVector CT);

   // By Angle Method
   void AddModule(G4double R,
                  G4double Theta,
                  G4double Phi,
                  G4double beta_u,
                  G4double beta_v,
                  G4double beta_w); 

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int             DetecNumber,
                    G4ThreeVector     MMpos,
                    G4RotationMatrix* MMrot,
                    G4LogicalVolume*  world);


   ///////////////////////////////////////////
   ////  Inherite from FatimaModule class /////
   ///////////////////////////////////////////
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
   // deriving from FatimaModule
   // This is mandatory since the Fatima*** does not derive from VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()	{return ms_InterCoord;};


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Interaction Coordinates coming from VDetector through the 
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

   G4ThreeVector momentum;

};



namespace FATIMADETECTOR
{



   // Resolution
//   const G4double ResoFirstStage  = 0;	// = 50 keV of Resolution   //   Unit is MeV/2.35
     const G4double ResoFirstStage  = 0.0099;	// = 3.5% at .662MeV of Resolution   //   Unit is MeV/2.35
  //     const G4double ResoSecondStage = 0.0;	// = 13% at .662 MeV of resolution //   Unit is MeV/2.35
  //  const G4double ResoThirdStage  = 0.0;	// = 50 keV of resolution //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957;// = 500ps                 //   Unit is  ns/2.35

   // Geometry for the mother volume containing the different layers of your dummy shape module
  const G4double FaceFront          = 7.5*cm; //;
  const G4double Length             = 26.33*cm; //

   // Geometry for the phoswitch
   // LaBr3
   const G4double LaBr3Face       = 3.81*cm;
   const G4double LaBr3Thickness  = 5.08*cm; // for detector 2x2x2

   // Al Can
   const G4double CanOuterDiameter       = 4.3*cm;
   const G4double CanInnerDiameter       = 4.15*cm;
   const G4double CanLength  = 4.33*cm; // = (5.08-(22-0.85)+0.1  with  0.85=(26.33-0.1-5.08)-22
   // Al front Window
   const G4double WinOuterDiameter       = 4.15*cm;
   const G4double WinInnerDiameter       = 0*cm;
   const G4double WinLength  = 0.1*cm; //

   // PM  (TODO Change CsI to Alu)
   const G4double CsIFace      = FaceFront;
   const G4double CsIThickness = 22.*cm; // for detector
  //const G4double CsIThickness = 0.001*cm; // For LaBr long

   // Lead tube (TODO Change CsI to Alu)
   const G4double LeadAMinR = CanOuterDiameter;
   const G4double LeadAMaxR = CanOuterDiameter+1*cm; // for detector
   const G4double LeadALength = 4.33*cm; // For LaBr long
 
   const G4double LeadBMinR = CanOuterDiameter;
   const G4double LeadBMaxR = FaceFront; // for detector
   const G4double LeadBLength = 1.*cm; // For LaBr long

  // Position 
   const G4double LaBr3Stage_PosZ  = -Length*0.5 + 0.5*LaBr3Thickness + 0.1*cm;
   const G4double LaBr3Can_PosZ    = -Length*0.5 + 0.5*CanLength;
   const G4double LaBr3Win_PosZ    = -Length*0.5 + 0.5*WinLength;

   const G4double CsIStage_PosZ    = -Length*0.5 + (Length-CsIThickness) + 0.5*CsIThickness;

   const G4double LeadAShield_PosZ  = -Length*0.5 + 0.5*LeadALength;
   const G4double LeadBShield_PosZ  = -Length*0.5 + LeadALength - 0.5*LeadBLength ;

}

#endif
