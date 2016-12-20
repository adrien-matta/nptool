#ifndef Sharc_h
#define Sharc_h 1
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
#include "G4VisAttributes.hh"
#include "G4MultiFunctionalDetector.hh"

// NPSimulation header
#include "NPSVDetector.hh"

// NPLib
#include "TSharcData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace SHARC{
  // Energy and time Resolution
  const G4double ResoTime    = 0      ;
  const G4double ResoEnergy  = 0.035*MeV ;// = zzkeV of Resolution   //   Unit is MeV/2.35
  const G4double EnergyThreshold = 0.1*MeV;
  // Geometry
  
  // BOX //
  // BOX PCB
  const G4double BOX_PCB_Width  = 61.10*mm;
  const G4double BOX_PCB_Length = 104.00*mm;
  const G4double BOX_PCB_Thickness = 3.4*mm;
  const G4double BOX_PCB_Border_LongSide = 1*mm;
  const G4double BOX_PCB_Border_ShortSide = 2*mm;
  
  // Single stage box case (DSSD only)
  const G4double BOX_PCB_Slot_Width1 = BOX_PCB_Thickness;
  const G4double BOX_PCB_Slot_Border1 = 4*mm;
  const G4double BOX_PCB_Slot_Deepness1 = BOX_PCB_Border_ShortSide;
  
  // BOX Wafer
  double BOX_ActiveWafer_Width  = 48;
  double BOX_ActiveWafer_Length = 72;
  double BOX_Wafer_Width  = 52.20;
  double BOX_Wafer_Length = 76.20;  
  
  const G4double BOX_Wafer_DeadLayer_Thickness = 0.1*um;
  const G4int    BOX_Wafer_Front_NumberOfStrip = 24 ;
  const G4int    BOX_Wafer_Back_NumberOfStrip = 48 ;
  
  // Compute
  const G4double BOX_LeftOver1 =  BOX_PCB_Length - BOX_PCB_Border_ShortSide - BOX_Wafer_Length - BOX_PCB_Slot_Border1 - BOX_PCB_Slot_Width1 ;
  const G4double BOX_Exposed_Length1 = BOX_Wafer_Length + BOX_PCB_Slot_Border1 ;
  
  const G4double BOX_CenterOffset1 = - 0.5 * BOX_PCB_Length+BOX_PCB_Border_ShortSide+0.5*BOX_Exposed_Length1;
  const G4double BOX_DetectorSpacing1 = 0.5*BOX_Exposed_Length1+0.5*BOX_PCB_Slot_Width1;
  
  const G4double BOX_Wafer_Width_Offset1 = -0.5*BOX_PCB_Width + BOX_PCB_Border_LongSide + 0.5*BOX_Wafer_Width;
  const G4double BOX_Wafer_Length_Offset1 = -0.5*BOX_PCB_Length + BOX_PCB_Border_ShortSide + 0.5*BOX_Wafer_Length;
  
  const G4double BOX_PCB_Slot_Position1 = 0.5*BOX_PCB_Length-BOX_LeftOver1 - 0.5*BOX_PCB_Slot_Width1;

  // PAD //
  // PAD PCB
  const G4double PAD_PCB_Width  = 61.10*mm;
  const G4double PAD_PCB_Length = 104.00*mm;
  const G4double PAD_PCB_Thickness = 3.4*mm;
  const G4double PAD_PCB_Border_LongSide = 1*mm;
  const G4double PAD_PCB_Border_ShortSide = 2.5*mm;
  
  // PAD Wafer
  const G4double PAD_Wafer_Width  = 53.20*mm;
  const G4double PAD_Wafer_Length = 73.20*mm;
  const G4double PAD_ActiveWafer_Width  = 50*mm;
  const G4double PAD_ActiveWafer_Length = 70*mm;

//    const G4double PAD_Wafer_Width  = 42.00*mm;
//    const G4double PAD_Wafer_Length = 72.00*mm;
  
  const G4double PAD_Wafer_DeadLayer_Thickness = 0.1*um;
  
  // Compute
  const G4double PAD_Wafer_Width_Offset = -0.5*PAD_PCB_Width + PAD_PCB_Border_LongSide + 0.5*PAD_Wafer_Width;
  const G4double PAD_Wafer_Length_Offset = -0.5*PAD_PCB_Length + PAD_PCB_Border_ShortSide + 0.5*PAD_Wafer_Length;
  
  // Double stage box case (DSSD+PAD) (the wafer is the same but the slot is different to accomodate the additional PAD)
  const G4double BOX_PCB_Slot_Width2 = BOX_PCB_Thickness + PAD_PCB_Thickness ;
  const G4double BOX_PCB_Slot_Border2 = 2.7*mm;
  const G4double BOX_PCB_Slot_Deepness2 = BOX_PCB_Border_ShortSide;
  
  const G4double BOX_LeftOver2 =  BOX_PCB_Length - BOX_PCB_Border_ShortSide - BOX_Wafer_Length - BOX_PCB_Slot_Border2 - BOX_PCB_Slot_Width2;
  const G4double BOX_Exposed_Length2 = BOX_Wafer_Length + BOX_PCB_Slot_Border2 ;
  
  const G4double BOX_CenterOffset2 = - 0.5*BOX_PCB_Length+BOX_PCB_Border_ShortSide + 0.5*BOX_Exposed_Length2;
  const G4double BOX_DetectorSpacing2 = 0.5*BOX_Exposed_Length2 + 0.5*BOX_PCB_Thickness;

  const G4double BOX_Wafer_Width_Offset2 = - 0.5*BOX_PCB_Width + BOX_PCB_Border_LongSide + 0.5*BOX_Wafer_Width;
  const G4double BOX_Wafer_Length_Offset2 = - 0.5*BOX_PCB_Length + BOX_PCB_Border_ShortSide + 0.5*BOX_Wafer_Length;
  
  const G4double BOX_PCB_Slot_Position2 = 0.5*BOX_PCB_Length-BOX_LeftOver2 - 0.5*BOX_PCB_Slot_Width2;
  
  // QQQ //
  // QQQ PCB
  const G4double QQQ_PCB_Outer_Radius = 61*mm;
  const G4double QQQ_PCB_Inner_Radius = 7.4*mm;
  const G4double QQQ_PCB_Thickness = 2*mm;

  // QQQ Wafer
  const G4double QQQ_Wafer_Outer_Radius = 42.63*mm;
  const G4double QQQ_Wafer_Inner_Radius = 9.0*mm;
  const G4double QQQ_Wafer_Starting_Phi = 8*deg;
  const G4double QQQ_Wafer_Stopping_Phi = 81*deg;
  const G4int    QQQ_Wafer_NumberOf_RadialStrip = 24 ;
  const G4int    QQQ_Wafer_NumberOf_AnnularStrip = 16 ;
  
}

using namespace SHARC ;
class Sharc : public NPS::VDetector
{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Sharc() ;
   ~Sharc() ;
  
  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a box detector
  void AddBoxDetector(G4double Z,G4double Thickness1,G4double Thickness2,G4double Thickness3,G4double Thickness4,G4double ThicknessPAD1,G4double ThicknessPAD2,G4double ThicknessPAD3,G4double ThicknessPAD4);
  // To add a Quadrant detector
  void AddQQQDetector(G4ThreeVector Pos, G4double Thickness);
  
  // Effectively construct Volume
  void ConstructBOXDetector(G4LogicalVolume* world);
  void ConstructQQQDetector(G4LogicalVolume* world);
  void ConstructTargetFan(G4LogicalVolume* world); 
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
  TSharcData*    m_Event ;
  
  ////////////////////////////////////////////////////
  ///////////////// Scorer Related ///////////////////
  ////////////////////////////////////////////////////
  
private:
  //   Initialize all Scorer
  void InitializeScorers() ;
  
  //   Scorer Associate to the Silicon
  G4MultiFunctionalDetector*   m_BOXScorer ;
  G4MultiFunctionalDetector*   m_PADScorer ;
  G4MultiFunctionalDetector*   m_QQQScorer ;
  
private:
  //    Initialize material used in detector definition
  void InitializeMaterial();
  
  //   List of material
  G4Material* m_MaterialSilicon ;
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
  vector<G4double>        m_ThicknessQQQ;
  
  // Used for Box detectors
  vector<G4double>   m_Z   ;
  vector< vector<G4double> >   m_ThicknessBOX;
  vector< vector<G4double> >   m_ThicknessPAD;
  
private:/// Visualisation Attribute:
  // Dark Grey
   G4VisAttributes* SiliconVisAtt  ;
  // Green
   G4VisAttributes* PCBVisAtt;
  // Gold Yellow
   G4VisAttributes* PADVisAtt  ;
  // Light Grey
   G4VisAttributes* FrameVisAtt ;
  
public:
    static NPS::VDetector* Construct();
};
#endif
