#ifndef Plastic_h
#define Plastic_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : September 2009                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a Modular cylindrical Plastic Scintillator           *
 *   Few Material are instantiate and user can choose position and dimension * 
 *  but also the adding of a lead plate on the rear side of the detector     *
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
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TPlasticData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Plastic : public NPS::VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   Plastic() ;
   virtual ~Plastic() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // Cylindric plastic
   void AddPlastic(  G4double   R                  ,
                     G4double   Theta              ,
                     G4double   Phi                ,
                     G4double   PlasticThickness   ,
                     G4double   PlasticRadius      ,
                     G4String   Scintillator       ,
                     G4double   LeadThickness      );  
   
   // Squared Plastic
   void AddPlastic(  G4double   R               ,
                     G4double   Theta           ,
                     G4double   Phi             ,
                     G4double   Height          ,
                     G4double   Width           ,
                     G4double   Thickness       ,
                     G4String   Scintillator    ,
                     G4double   LeadThickness   );
      
   void VolumeMaker(G4ThreeVector Det_pos, int DetNumber,G4LogicalVolume* world) ;
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

public:   //   Scorer
   //   Initialize all Scorer used by the MUST2Array
   void InitializeScorers() ;

   //   Silicon Associate Scorer
   G4MultiFunctionalDetector* m_PlasticScorer ;
   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TPlasticData* m_Event ;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   
   
   
   // Lead plate is added in front or back of the detector
   vector<double>     m_LeadThickness ;
   
   vector<double>      m_PlasticThickness ;
   vector<double>      m_PlasticRadius    ; // cylindrical shape
   vector<double>      m_PlasticHeight    ; // squared shape
   vector<double>      m_PlasticWidth     ; // squared shape
         
   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate plastic volume center
   vector<G4double>  m_Phi    ; //  |

   //   Scintillator type
   vector<G4String> m_Scintillator ;


public:
    static NPS::VDetector* Construct();
};
#endif
