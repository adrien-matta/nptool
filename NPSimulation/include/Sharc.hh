#ifndef Sharc_h
#define Sharc_h 1
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Sharc Silicon detector                           *
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

// NPSimulation header
#include "VDetector.hh"

// NPLib
//#include "TSharcData.h"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace SHARC
{
  // Energy and time Resolution
  const G4double ResoTime    = 0      ;
  const G4double ResoEnergy  = 0.035  ;// = zzkeV of Resolution   //   Unit is MeV/2.35
  
  // Geometry
  // BOX PCB
  const G4double BOX_PCB_Width  = 0;
  const G4double BOX_PCB_Length = 0;
  const G4double BOX_PCB_Thickness = 0;
  const G4double BOX_PCB_Trench_Position = 0;
  const G4double BOX_PCB_Trench_Width = 0;
  const G4double BOX_PCB_Trench_Deepness = 0;
  
  // BOX Wafer
  const G4double BOX_Wafer_Width  = 0;
  const G4double BOX_Wafer_Length = 0;
  const G4double BOX_Wafer_Thickness = 0;
  const G4double BOX_Wafer_DeadLayer_Thickness = 0;
  const G4int    BOX_Wafer_Front_NumberOfStrip = 16 ;
  const G4int    BOX_Wafer_Back_NumberOfStrip = 16 ;
  
  // QQQ PCB
  const G4double QQQ_PCB_Outer_Radius = 0;
  const G4double QQQ_PCB_Inner_Radius = 0;
  const G4double QQQ_PCB_Thickness = 0;
  
  // BOX Wafer
  const G4double QQQ_Wafer_Outer_Radius = 0;
  const G4double QQQ_Wafer_Inner_Radius = 0;
  const G4double QQQ_Wafer_Thickness = 0;
  const G4int    QQQ_Wafer_Front_NumberOfStrip = 16 ;
  const G4int    QQQ_Wafer_Back_NumberOfStrip = 16 ;
}

using namespace SHARC ;

class Sharc : public VDetector
{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Sharc() ;
  virtual ~Sharc() ;
  
  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a box detector
  void AddBoxDetector(G4ThreeVector Pos);
  // To add a Quadrant detector
  void AddQQQDetector(G4ThreeVector Pos);
  
  // Effectively construct Volume
  // Avoid to have two time same code for Angle and Point definition
  void VolumeMaker(G4int DetectorNumber    ,
                   G4ThreeVector     MMpos ,
                   G4RotationMatrix* MMrot ,
                   G4LogicalVolume*  world );
  
  
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
  //TSharcData*    m_Event ;
  
  ////////////////////////////////////////////////////
  ///////////////// Scorer Related ///////////////////
  ////////////////////////////////////////////////////
  
private:
  //   Initialize all Scorer
  void InitializeScorers() ;
  
  //   Scorer Associate to the Silicon
  G4MultiFunctionalDetector*   m_StripScorer ;
  
private:
  //    Initialize material used in detector definition
  void InitializeMaterial();
  
  //   List of material
  G4Material* m_MaterialSilicon ;
  G4Material* m_MaterialAl      ;
  G4Material* m_MaterialVacuum  ;
  
  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  // True if the detector is a Box, false if a quadrant
  vector<bool>   m_Type  ;
  
  // Used for Box and Quadrant detectors
  vector<G4ThreeVector>   m_Pos   ;
  
  // Set to true if you want to see Telescope Frame in your visualisation
  bool m_non_sensitive_part_visiualisation ;
  
};
#endif
