#ifndef Tiara_h
#define Tiara_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
#include "VDetector.hh"

// NPLib
#include "TTiaraBarrelData.h"
#include "TTiaraHyballData.h"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace TIARA
{
  // Energy and time Resolution
  const G4double ResoTime    = 0      ;
  //const G4double ResoEnergy  = 0.035*MeV ;// = zzkeV of Resolution   //   Unit is MeV/2.35
  const G4double ResoEnergy  = 0.042*MeV ;// = zzkeV of Resolution   //   Unit is MeV/2.35


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
  const G4double INNERBARREL_PCB_Bevel1_Theta = 50*deg ;
  const G4double INNERBARREL_PCB_Bevel2_Theta = 67.5*deg;
  // offset between the edge of the PCB and the Edge of the Waffer
  const G4double INNERBARREL_PCB_Offset = 15*mm; 
  // Different from Marc code, to be checked
  const G4double INNERBARREL_Wafer_Length = 82.00*mm;
  const G4double INNERBARREL_Wafer_Width = 24.80*mm;
  const G4double INNERBARREL_Wafer_Thickness =400*um;
 


}

using namespace TIARA;
class Tiara : public VDetector
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
  /////////  Inherite from VDetector class ///////////
  ////////////////////////////////////////////////////
public:
  // Read stream at Configfile to pick-up parameters of detector (Position,...)
  // Called in DetecorConstruction::ReadDetextorConfiguration Method
  void ReadConfiguration(string Path) ;
  
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
  // True if the detector is a Box, false if a quadrant
  vector<bool>   m_Type  ;
  
  // Used for Quadrant detectors
  vector<G4ThreeVector>   m_Pos   ; // R , Phi , Z
  vector<G4double>        m_ThicknessQQQ;
  
  // Used for Box detectors
  vector<G4double>   m_Z   ;
  vector< vector<G4double> >   m_ThicknessBOX;
  vector< vector<G4double> >   m_ThicknessPAD;
  
  // Set to true if you want to see Telescope Frame in your visualisation
  bool m_non_sensitive_part_visiualisation ;
  
private:/// Visualisation Attribute:
  // Dark Grey
   G4VisAttributes* SiliconVisAtt  ;
  // Green
   G4VisAttributes* PCBVisAtt;
  // Gold Yellow
   G4VisAttributes* PADVisAtt  ;
  // Light Grey
   G4VisAttributes* FrameVisAtt ;
  
};
#endif
