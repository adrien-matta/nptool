#ifndef EDEN_h
#define EDEN_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : February 2010                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class describes the EDEN neutron detector                        *
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
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "VDetector.hh"
//#include "TEDENData.h"

using namespace std;

class EDEN : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   EDEN() ;
   virtual ~EDEN() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Angle Method
   void AddEDENModule(G4double R,
        	      G4double Theta,
         	      G4double Phi);

   void VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world);


   ////////////////////////////////////////////////////
   /////////  Inherite from VDetector class ///////////
   ////////////////////////////////////////////////////
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

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);


   /////////////////////////////////////////////////
   /////////////////// Materials ///////////////////
   /////////////////////////////////////////////////
private:
   void InitializeMaterial();
   G4Material* m_MaterialEDEN_liqSci;


   /////////////////////////////////////////////////
   //////////////////// Scorers ////////////////////
   /////////////////////////////////////////////////
public:
   // Initialize all Scorer used by EDEN
   void InitializeScorers() ;

private:
   // EDEN Scorer
   G4MultiFunctionalDetector* m_EDENScorer				 ;


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
//   TEDENData* m_Event ;


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Used for "By Angle Definition"
   vector<G4double>  m_R         ; //  |
   vector<G4double>  m_Theta     ; //  > Spherical coordinate EDEN volume center
   vector<G4double>  m_Phi    	 ; //  |
};
#endif
