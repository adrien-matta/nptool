#ifndef Paris_h
#define Paris_h 1
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
 *  This class describe the Paris scintillator array                         *
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
#include "TParisData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Paris : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  Paris() ;
   ~Paris() ;
  
  ////////////////////////////////////////////////////
  //////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  // To add a Cluster detector
  // By corner position
  void AddCluster(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4);
  // By Center position
  void AddCluster(G4ThreeVector Pos, double beta_u=0, double beta_v=0, double beta_w=0);
  // To add a Phoswich detector
  // By corner position
  void AddPhoswich(G4ThreeVector Pos1,G4ThreeVector Pos2,G4ThreeVector Pos3,G4ThreeVector Pos4);
  // By Center position
  void AddPhoswich(G4ThreeVector Pos, double beta_u=0, double beta_v=0, double beta_w=0);
  
  // Return a logical volume of the detector
  G4LogicalVolume* ConstructPhoswich();
  G4LogicalVolume* ConstructCluster();

private: // Guarranty that each volume is created only once
 G4LogicalVolume* m_LogicalPhoswich;
 G4LogicalVolume* m_LogicalCluster;

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
  TParisData* m_Event ;
  
  ////////////////////////////////////////////////////
  ///////////////// Scorer Related ///////////////////
  ////////////////////////////////////////////////////
  
private:
  //   Initialize all Scorer
  void InitializeScorers() ;
  
  //   Scorer Associate to the Silicon
  G4MultiFunctionalDetector* m_NaIScorer ;
  G4MultiFunctionalDetector* m_LaBr3Scorer ;

private:
  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private:
  // True if the detector is a Cluster, false if a single Phoswich
  vector<bool> m_Type;
  
  // Detector position
  vector<G4ThreeVector> m_Pos;
  vector<G4RotationMatrix*> m_Rot;
 
private:/// Visualisation Attribute:
   G4VisAttributes* m_NaIVisAtt  ;
   G4VisAttributes* m_LaBr3VisAtt;
   G4VisAttributes* m_PhoswichCasingVisAtt  ;
   G4VisAttributes* m_ClusterCasingVisAtt ;
   G4VisAttributes* m_PMTVisAtt; 
public:
    static NPS::VDetector* Construct();
};

namespace PARIS{

  // Resolution and Threshold
  const G4double EnergyThreshold = 100*keV;
  const G4double ResoFirstStage = 0.0085;	// = 3% at .662 MeV of Resolution   //   Unit is MeV/2.35
  const G4double ResoSecondStage = 0.0366;	// = 13% at .662 MeV of resolution //   Unit is MeV/2.35
  const G4double ResoTime = 0.212765957;// = 500ps                 //   Unit is  ns/2.35
  
  // Phoswich geometry
  const G4double PhoswichCasingLenth = 226*mm;
  const G4double PhoswichCasingWidth = 56*mm;
  
  // LaBr3
  const G4double LaBr3Face = 50.8*mm;
  const G4double LaBr3Thickness =  50.8*mm; // for phoswich 2x2x3
  const G4double InterSpace = 0*mm;
  
  // NaI
  const G4double NaIFace = LaBr3Face;
  const G4double NaIThickness = 152.4*mm; // for phoswich 2x2x6
 
  // PMT geometry
  const G4double PMTRadius = 26*mm;
  const G4double PMTLength = 150*mm; // PMT + connector
 
  // Starting form the LaBr3 and going to the NaI
  const G4double CasingThickness     = 0.5*(PhoswichCasingWidth-LaBr3Face);
  const G4double LaBr3Stage_PosZ =CasingThickness+PhoswichCasingLenth* -0.5 + 0.5*LaBr3Thickness;
  const G4double NaIStage_PosZ = CasingThickness+ PhoswichCasingLenth* -0.5 + LaBr3Thickness + 0.5*NaIThickness;

  // Cluster geometry
  const G4double ClusterFrameWidth = 188*mm;
  const G4double ClusterFrameLength = 15*mm;
  const G4double ClusterOffset = 30*mm;
  const G4double FaceFront = 3*PhoswichCasingWidth;
  const G4double FaceBack = FaceFront;
  const G4double Length = PhoswichCasingLenth+PMTLength;
 

}

#endif
