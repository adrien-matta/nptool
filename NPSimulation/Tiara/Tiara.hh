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
  void ConstructBarrel(G4LogicalVolume* world);
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
