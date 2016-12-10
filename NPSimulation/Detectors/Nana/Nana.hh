#ifndef Nana_h
#define Nana_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : December 2009                                            *
 * Last update    : December 2014                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Nana scintillator array                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>
#include <vector>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4MultiFunctionalDetector.hh"

// NPSimulation header
#include "NPSVDetector.hh"

// NPLib
#include "TNanaData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Nana : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Nana() ;
   ~Nana() ;
  
  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a Detector 
  // By corner position
  void AddDetector(G4ThreeVector Pos1,G4ThreeVector Pos2,G4ThreeVector Pos3,G4ThreeVector Pos4);
  // By Center position
  void AddDetector(G4ThreeVector Pos, double beta_u=0, double beta_v=0, double beta_w=0);
  
  // Return a logical volume of the detector
  G4LogicalVolume* ConstructDetector();

private: // Guarranty that each volume is created only once
 G4LogicalVolume* m_LogicalDetector;

  ////////////////////////////////////////////////////
  /////////  Inherite from NPS::VDetector class ///////////
  ////////////////////////////////////////////////////
public:
  // Read stream at Configfile to pick-up parameters of detector (Position,...)
  // Called in DetecorConstruction::ReadDetextorConfiguration Method
  void ReadConfiguration(NPL::InputParser) ;
  
  // Construct detector and inialise sensitive part.
  // Called After DetecorConstruction::AddDetector Method
  void ConstructDetector(G4LogicalVolume* world) ;
  
  // Add Detector branch to the EventTree.
  // Called After DetecorConstruction::AddDetector Method
  void InitializeRootOutput() ;
  
  // Read sensitive part and fill the Root tree.
  // Called at in the EventAction::EndOfEventAvtion
  void ReadSensitive(const G4Event* event) ;
  
  ////////////////////////////////////////////////////
  ///////////Event class to store Data////////////////
  ////////////////////////////////////////////////////
private:
  TNanaData* m_Event ;
  
  ////////////////////////////////////////////////////
  ///////////////// Scorer Related ///////////////////
  ////////////////////////////////////////////////////
  
private:
  //   Initialize all Scorer
  void InitializeScorers() ;
  
  //   Scorer Associate to the Silicon
  G4MultiFunctionalDetector* m_LaBr3Scorer ;

private:
  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  // Detector position
  vector<G4ThreeVector> m_Pos;
  vector<G4RotationMatrix*> m_Rot;
 
private:/// Visualisation Attribute:
   G4VisAttributes* m_LaBr3VisAtt;
   G4VisAttributes* m_DetectorCasingVisAtt  ;
   G4VisAttributes* m_PMTVisAtt; 
public:
    static NPS::VDetector* Construct();
};

namespace NANA{
   // Resolution
  const G4double EnergyResolution = 0.0099;	// = 3.5% at .662MeV of Resolution   //   Unit is MeV/2.35
  const G4double EnergyThreshold = 1*eV;   
  // Geometry for the mother volume 
  const G4double FaceFront = 7.5*cm;
  const G4double Length = 26.33*cm; 

   // LaBr3
   const G4double LaBr3Face = 3.81*cm;
   const G4double LaBr3Thickness  = 5.08*cm;

   // Al Can
   const G4double CanOuterDiameter = 4.3*cm;
   const G4double CanInnerDiameter = 4.15*cm;
   const G4double CanLength = 4.33*cm; 
   // Al front Window
   const G4double WinOuterDiameter = 4.15*cm;
   const G4double WinInnerDiameter = 0*cm;
   const G4double WinLength  = 0.1*cm; //

   // PMT 
   const G4double PMTFace = FaceFront;
   const G4double PMTThickness = 22.*cm; // for detector

   // Lead tube 
   const G4double LeadAMinR = CanOuterDiameter;
   const G4double LeadAMaxR = CanOuterDiameter+1*cm; 
   const G4double LeadALength = 4.33*cm; 
 
   const G4double LeadBMinR = CanOuterDiameter;
   const G4double LeadBMaxR = FaceFront; 
   const G4double LeadBLength = 1.*cm; 

  // Position 
   const G4double LaBr3_PosZ  = -Length*0.5 + 0.5*LaBr3Thickness + 0.1*cm;
   const G4double LaBr3Can_PosZ    = -Length*0.5 + 0.5*CanLength;
   const G4double LaBr3Win_PosZ    = -Length*0.5 + 0.5*WinLength;

   const G4double PMT_PosZ    = -Length*0.5 + (Length-PMTThickness) + 0.5*PMTThickness;

   const G4double LeadAShield_PosZ  = -Length*0.5 + 0.5*LeadALength;
   const G4double LeadBShield_PosZ  = -Length*0.5 + LeadALength - 0.5*LeadBLength ;

}

#endif
