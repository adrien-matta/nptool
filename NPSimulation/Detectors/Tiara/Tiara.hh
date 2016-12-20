#ifndef Tiara_h
#define Tiara_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Tiara Silicon detector                           *
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
#include "TTiaraBarrelData.h"
#include "TTiaraHyballData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace TIARA{
  // Energy and time Resolution
  const G4double ResoTime    = 0      ;
  const G4double ResoEnergyInnerBarrel  = 0.017*MeV ;// = 136keV FWHM 
  const G4double ResoEnergyOuterBarrel  = 0.017*MeV ;// = 136keV FWHM
  const G4double ResoEnergyHyball       = 0.017*MeV ;// =  70keV FWHM

  const G4double EnergyThreshold = 200*keV;

  // Geometric Information //
  // Chanber //
  // Central Tube
  const G4double CHAMBER_CentralTube_Inner_Radius = 4.86*cm;	//4.05->Original Value for the Single stage barrel
  const G4double CHAMBER_CentralTube_Outer_Radius = 5.05*cm;	//4.25->Original Value for the Single stage barrel
  const G4double CHAMBER_CentralTube_Length = 8.24*cm;
  
  // Outer Cone
  const G4double CHAMBER_OuterCone_Length = 9.88*cm;
  const G4double CHAMBER_OuterCone_Z_Pos = 9.06*cm;

  // Outer Cylinder
  const G4double CHAMBER_OuterCylinder_Inner_Radius = 24.6*cm;
  const G4double CHAMBER_OuterCylinder_Outer_Radius = 25.0*cm;
  const G4double CHAMBER_OuterCylinder_Length = 12.08*cm;
  const G4double CHAMBER_OuterCylinder_Z_Pos = 19.96*cm;

  // Inner Barrel //
  const G4double INNERBARREL_PCB_Length = 98.00*mm;
  const G4double INNERBARREL_PCB_Width  = 27.76*mm;
  const G4double INNERBARREL_PCB_Thickness = 1.60*mm;
  const G4double INNERBARREL_PCB_HoleLength = 82*mm;
  const G4double INNERBARREL_PCB_WaferDepth = 1.1*mm;
  const G4double INNERBARREL_PCB_Bevel1_Theta = 50*deg ;
  const G4double INNERBARREL_PCB_Bevel2_Theta = 67.5*deg;
  // offset between the edge of the PCB and the Edge of the hole
  const G4double INNERBARREL_PCB_Offset = 15*mm; 
  const G4double INNERBARREL_ActiveWafer_Length = 94.80*mm;
  const G4double INNERBARREL_ActiveWafer_Width = 24.0*mm;
  const G4double INNERBARREL_ActiveWafer_Thickness =400*um;
  const G4double INNERBARREL_InertWafer_Length = 97.00*mm;
  const G4double INNERBARREL_InertWafer_Width = 24.80*mm;
  const G4double INNERBARREL_ActiveWafer_DeadLayerThickness = 1*um;
  const G4double INNERBARREL_InertWafer_Thickness = 
    INNERBARREL_ActiveWafer_Thickness+ 2*INNERBARREL_ActiveWafer_DeadLayerThickness;
  const G4int    INNERBARREL_NumberOfStrip = 4;
 
  // Outer Barrel //
  const G4double OUTERBARREL_PCB_Length = 98.00*mm;
  const G4double OUTERBARREL_PCB_Width  = 33.16*mm;
  const G4double OUTERBARREL_PCB_Thickness = 1.60*mm;
  const G4double OUTERBARREL_PCB_HoleLength = 82*mm;
  const G4double OUTERBARREL_PCB_WaferDepth = 1.1*mm;
  const G4double OUTERBARREL_PCB_Bevel1_Theta = 50*deg ;
  const G4double OUTERBARREL_PCB_Bevel2_Theta = 67.5*deg;
  // offset between the edge of the PCB and the Edge of the hole
  const G4double OUTERBARREL_PCB_Offset = 15*mm; 
  // Different from Marc code, to be checked
  const G4double OUTERBARREL_ActiveWafer_Length = 94.80*mm;
  const G4double OUTERBARREL_ActiveWafer_Width = 29.4*mm;
  const G4double OUTERBARREL_ActiveWafer_Thickness =700*um;
  const G4double OUTERBARREL_InertWafer_Length = 97.00*mm;
  const G4double OUTERBARREL_InertWafer_Width = 30.2*mm;
  const G4int    OUTERBARREL_NumberOfStrip = 4;

  // Hyball //
  const G4double HYBALL_PCB_THICKNESS = 1.6*mm;
  const G4double HYBALL_ActiveWafer_InnerRadius = 32.6*mm;
  const G4double HYBALL_ActiveWafer_OuterRadius = 135.1*mm;
  const G4double HYBALL_ActiveWafer_Thickness = 400*um;
  const G4double HYBALL_ActiveWafer_Angle = 54.8*deg;
  const G4double HYBALL_NumberOfAnnularStrip = 16;
  const G4double HYBALL_NumberOfRadialStrip = 8 ;
}


using namespace TIARA;
class Tiara : public NPS::VDetector
{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Tiara() ;
   ~Tiara() ;
  
  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a box detector
  // Effectively construct Volume
  void ConstructInnerBarrel(G4LogicalVolume* world);
  void ConstructOuterBarrel(G4LogicalVolume* world);
  void ConstructHyball(G4LogicalVolume* world);
  void ConstructChamber(G4LogicalVolume* world);

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
  TTiaraBarrelData*    m_EventBarrel ;
  TTiaraHyballData*    m_EventHyball ;


  ////////////////////////////////////////////////////
  /////////////////// Material ///////////////////////
  ////////////////////////////////////////////////////
private:
  //    Initialize material used in detector definition
  void InitializeMaterial();
  
  //   List of material
  G4Material* m_MaterialSilicon ;
  G4Material* m_MaterialAl      ;
  G4Material* m_MaterialVacuum  ;
  G4Material* m_MaterialPCB     ;
   
  ////////////////////////////////////////////////////
  ///////////////// Scorer Related ///////////////////
  ////////////////////////////////////////////////////
  
private:
  //   Initialize all Scorer
  void InitializeScorers() ;
  
  //   Scorer Associate with the Silicon
  G4MultiFunctionalDetector*   m_InnerBarrelScorer ;
  G4MultiFunctionalDetector*   m_OuterBarrelScorer ;
  G4MultiFunctionalDetector*   m_HyballScorer ;
 
  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  bool m_boolChamber;
  bool m_boolInner;
  bool m_boolOuter;

  vector<double> m_HyballZ;
  vector<double> m_HyballR;
  vector<double> m_HyballPhi;

private:/// Visualisation Attribute:
  // Dark Grey
   G4VisAttributes* SiliconVisAtt  ;
  // Green
   G4VisAttributes* PCBVisAtt;
  // Gold Yellow
   G4VisAttributes* PADVisAtt  ;
  // Light Grey
   G4VisAttributes* FrameVisAtt ;
   // Light Blue
   G4VisAttributes* GuardRingVisAtt ; 

  public:
    static NPS::VDetector* Construct();

};

#endif
