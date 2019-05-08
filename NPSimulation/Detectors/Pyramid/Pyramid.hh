#ifndef Pyramid_h
#define Pyramid_h 1
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Pyramid simulation                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/


// C++ header
#include <string>
#include <vector>
using namespace std;

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
#include "TPyramidData.h"
#include "NPInputParser.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace PYRAMID{
  // Energy and time Resolution
  const G4double ResoTime    = 0      ;
  //const G4double ResoEnergyInner  = 0.017*MeV ;// = 136keV FWHM
  //const G4double ResoEnergyOuter  = 0.017*MeV ;// = 136keV FWHM
  const G4double ResoEnergyInner  = 0.030*MeV ;// = 136keV FWHM
  const G4double ResoEnergyOuter  = 0.030*MeV ;// = 136keV FWHM
  const G4double EnergyThreshold = 0*keV; // 200 keV

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

  // Inner Pyramid //
  const G4double Inner_PCB_Length = 79.00*mm;
  const G4double Inner_PCB_Width  = 45.6*mm;
  const G4double Inner_PCB_Thickness = 1.60*mm;
  const G4double Inner_PCB_HoleLength = 82*mm;
  const G4double Inner_PCB_WaferDepth = 1.1*mm;
  const G4double Inner_PCB_Bevel1_Theta = 50*deg ;
  const G4double Inner_PCB_Bevel2_Theta = 67.5*deg;
  // offset between the edge of the PCB and the Edge of the hole
  const G4double Inner_PCB_Offset = 15*mm;
  const G4double Inner_ActiveWafer_Length = 75.00*mm;
  const G4double Inner_ActiveWafer_Width = 41.5*mm;
  const G4double Inner_ActiveWafer_Thickness = 1000*um;
  const G4double Inner_InertWafer_Length = (75.0+3)*mm;
  const G4double Inner_InertWafer_Width = (41.5+3)*mm;

  const G4double Inner_ActiveWafer_DeadLayerThickness = 0.001*um;
  const G4double Inner_InertWafer_Thickness =
    Inner_ActiveWafer_Thickness+ 2*Inner_ActiveWafer_DeadLayerThickness;
  const G4int    Inner_NumberOfStrip = 8;

  // Outer Pyramid //
  const G4double Outer_PCB_Length = 79.00*mm;
  const G4double Outer_PCB_Width  = 45.6*mm;
  const G4double Outer_PCB_Thickness = 1.60*mm;
  const G4double Outer_PCB_HoleLength = 82*mm;
  const G4double Outer_PCB_WaferDepth = 1.1*mm;
  const G4double Outer_PCB_Bevel1_Theta = 50*deg ;
  const G4double Outer_PCB_Bevel2_Theta = 67.5*deg;
  // offset between the edge of the PCB and the Edge of the hole
  const G4double Outer_PCB_Offset = 15*mm;
  // Different from Marc code, to be checked
  const G4double Outer_ActiveWafer_Length = 75.00*mm;
  const G4double Outer_ActiveWafer_Width = 41.5*mm;
  const G4double Outer_ActiveWafer_Thickness = 1000*um;
  const G4double Outer_InertWafer_Length = (75.0+3)*mm;
  const G4double Outer_InertWafer_Width = (41.5+3)*mm;
  const G4int    Outer_NumberOfStrip = 8;

}


using namespace PYRAMID;
class Pyramid : public NPS::VDetector
{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Pyramid() ;
   ~Pyramid() ;

  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a box detector
  // Effectively construct Volume
  void ConstructInner(G4LogicalVolume* world, double angle, double z);
  void ConstructOuter(G4LogicalVolume* world, double angle, double z);
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
  TPyramidData*    m_EventPyramid ;


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
  G4MultiFunctionalDetector*   m_InnerScorer ;
  G4MultiFunctionalDetector*   m_OuterScorer ;


  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  bool m_boolChamber;
  vector<bool> m_boolInner;
  vector<bool> m_boolOuter;
  vector<double> m_Z; // shift on the z-axis
  vector<double> m_ANGLE; // angle of rotation around the downstream width 

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
