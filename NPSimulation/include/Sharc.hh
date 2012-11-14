#ifndef Sharc_h
#define Sharc_h 1
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
  
  // BOX //
  // BOX PCB
  const G4double BOX_PCB_Width  = 61.0*mm;
  const G4double BOX_PCB_Length = 104.00*mm;
  const G4double BOX_PCB_Thickness = 3.5*mm;
  const G4double BOX_PCB_Border_LongSide = 1*mm;
  const G4double BOX_PCB_Border_ShortSide = 2*mm;
  
  // Single stage box case (DSSD only)
  const G4double BOX_PCB_Slot1_Width = 3.5*mm;
  const G4double BOX_PCB_Slot1_Border = 5*mm;
  const G4double BOX_PCB_Slot1_Deepness = 0.75*mm;
  
  // Double stage box case (DSSD+PAD)
  const G4double BOX_PCB_Slot2_Width = 7*mm;
  const G4double BOX_PCB_Slot2_Border = 3*mm;
  const G4double BOX_PCB_Slot2_Deepness = 0.75*mm;
  
  // BOX Wafer
  const G4double BOX_Wafer_Width  = 51.00*mm;
  const G4double BOX_Wafer_Length = 77.00*mm;
  const G4double BOX_Wafer_Thickness = 100*um;
  
  const G4double BOX_Wafer_DeadLayer_Thickness = 0.1*um;
  const G4int    BOX_Wafer_Front_NumberOfStrip = 16 ;
  const G4int    BOX_Wafer_Back_NumberOfStrip = 16 ;
  
  // Compute
  const G4double BOX_Wafer_Width_Offset =
  BOX_PCB_Width/2. - BOX_PCB_Border_LongSide - BOX_Wafer_Width/2.;
  const G4double BOX_Wafer_Length_Offset =
  BOX_PCB_Length/2. - BOX_PCB_Border_ShortSide - BOX_Wafer_Length/2.;
  const G4double BOX_PCB_Slot1_Position =(BOX_Wafer_Length/2.-BOX_Wafer_Length_Offset) + BOX_PCB_Slot1_Border + BOX_PCB_Slot1_Width/2.;
  const G4double BOX_PCB_Slot2_Position =(BOX_Wafer_Length/2.-BOX_Wafer_Length_Offset) + BOX_PCB_Slot2_Border + BOX_PCB_Slot2_Width/2.;
  
  // PAD //
  // PAD PCB
  const G4double PAD_PCB_Width  = 61.0*mm;
  const G4double PAD_PCB_Length = 104.00*mm;
  const G4double PAD_PCB_Thickness = 3.5*mm;
  const G4double PAD_PCB_Border_LongSide = 1*mm;
  const G4double PAD_PCB_Border_ShortSide = 2*mm;
  
  const G4double PAD_PCB_Slot_Width = 3*mm;
  const G4double PAD_PCB_Slot_Border = 5*mm;
  const G4double PAD_PCB_Slot_Deepness = 1*mm;
  
  // PAD Wafer
  const G4double PAD_Wafer_Width  = 51.00*mm;
  const G4double PAD_Wafer_Length = 77.00*mm;
  const G4double PAD_Wafer_Thickness = 100*um;
  const G4double PAD_Wafer_DeadLayer_Thickness = 0.1*um;
  
  // Compute
  const G4double PAD_Wafer_Width_Offset =
  PAD_PCB_Width/2. - PAD_PCB_Border_LongSide - PAD_Wafer_Width/2.;
  const G4double PAD_Wafer_Length_Offset =
  PAD_PCB_Length/2. - PAD_PCB_Border_ShortSide - PAD_Wafer_Length/2.;
  
  // QQQ //
  // QQQ PCB
  const G4double QQQ_PCB_Outer_Radius = 61*mm;
  const G4double QQQ_PCB_Inner_Radius = 7.4*mm;
  const G4double QQQ_PCB_Thickness = 2*mm;

  // QQQ Wafer
  const G4double QQQ_Wafer_Outer_Radius = 42.63*mm;
  const G4double QQQ_Wafer_Inner_Radius = 7.8*mm;
  const G4double QQQ_Wafer_Thickness = 100*um;
  const G4double QQQ_Wafer_Starting_Phi = 8*deg;
  const G4double QQQ_Wafer_Stopping_Phi = 162*deg;
  const G4int    QQQ_Wafer_NumberOf_RadialStrip = 16 ;
  const G4int    QQQ_Wafer_NumberOf_AnnularStrip = 24 ;
  
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
  void AddBoxDetector(G4double Z,G4double Thickness1,G4double Thickness2,G4double Thickness3,G4double Thickness4,G4double ThicknessPAD1,G4double ThicknessPAD2,G4double ThicknessPAD3,G4double ThicknessPAD4);
  // To add a Quadrant detector
  void AddQQQDetector(G4ThreeVector Pos);
  
  // Effectively construct Volume
  void ConstructBOXDetector(G4LogicalVolume* world);
  void ConstructQQQDetector(G4LogicalVolume* world);
  
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
  G4Material* m_MaterialPCB     ;
  
  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  // True if the detector is a Box, false if a quadrant
  vector<bool>   m_Type  ;
  
  // Used for Quadrant detectors
  vector<G4ThreeVector>   m_Pos   ; // R , Phi , Z
  vector<G4double>        m_Thickness;
  
  // Used for Box detectors
  vector<G4double>   m_Z   ;
  vector<G4double>   m_Thickness1;
  vector<G4double>   m_Thickness2;
  vector<G4double>   m_Thickness3;
  vector<G4double>   m_Thickness4;
  vector<G4double>   m_ThicknessPAD1;
  vector<G4double>   m_ThicknessPAD2;
  vector<G4double>   m_ThicknessPAD3;
  vector<G4double>   m_ThicknessPAD4;
  
  // Set to true if you want to see Telescope Frame in your visualisation
  bool m_non_sensitive_part_visiualisation ;
  
};
#endif
