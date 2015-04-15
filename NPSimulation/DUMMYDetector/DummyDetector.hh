#ifndef DUMMYDetector_h
#define DUMMYDetector_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a simple Dummy Detector :                            *
 *   a simple cylinder of predifined material. user can choose to place it    *
 *  where he want using spherical coordinate and choose between two naterial *
 *                                                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  This detector respect all the NPTool convention in naming volume,        * 
 *  reading scorers and file. Use it as a basis for your own detector        *
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
#include "NPSVDetector.hh"
#include "TDUMMYDetectorData.h"

using namespace std;
using namespace CLHEP;

class DUMMYDetector : public NPS::VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   DUMMYDetector() ;
   virtual ~DUMMYDetector() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Angle Method
   void AddDUMMYDetector(  G4double    R                       ,
                           G4double    Theta                   ,
                           G4double    Phi                     ,
                           G4double    DUMMYDetectorThickness  ,
                           G4double    DUMMYDetectorRadius     ,
                           G4String    Scintillator            );  
      
   void VolumeMaker(G4ThreeVector Det_pos, int DetNumber,G4LogicalVolume* world) ;
   ////////////////////////////////////////////////////
   /////////  Inherite from NPS::VDetector class ///////////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path)          ;

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world)  ;

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput()            ;

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event)     ;

public: // Material 
   void InitializeMaterial()       ;
   // The dummy materials
   G4Material* m_MaterialDUMMYDetector_material1      ; //the dummy material 2
   G4Material* m_MaterialDUMMYDetector_material2      ; //the dummy material 2

   
public:   //   Scorer
   //   Initialize all Scorer used by the MUST2Array
   void InitializeScorers() ;

   //   Silicon Associate Scorer
   G4MultiFunctionalDetector* m_DUMMYDetectorScorer             ;


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TDUMMYDetectorData* m_Event ;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   
   vector<double>      m_DUMMYDetectorThickness   ;
   vector<double>      m_DUMMYDetectorRadius      ;
      
   // Used for "By Angle Definition"
   vector<G4double>  m_R         ; //  |
   vector<G4double>  m_Theta     ; //  > Spherical coordinate DUMMYDetector volume center
   vector<G4double>  m_Phi        ; //  |

   //   Scintillator type
   vector<G4String> m_Scintillator ;


};
#endif
