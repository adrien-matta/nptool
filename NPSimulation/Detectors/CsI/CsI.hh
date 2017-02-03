#ifndef CsI_h
#define CsI_h 1
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
 *  This class describe a Modular cylindrical CsI Scintillator               *
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
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TCsIData.h"
#include "NPInputParser.h"
using namespace std;

using namespace CLHEP;

class CsI : public NPS::VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   CsI() ;
   virtual ~CsI() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
    
    G4double PhotoDiodeFace ;
    G4double PhotoDiodeThickness  ;
    G4double ResoCsI; 
   // Cylindric CsI
   void AddCsI(  G4double   R                  ,
                     G4double   Theta              ,
                     G4double   Phi                ,
                     G4double   CsIThickness   ,
                     G4double   CsIRadius      ,
                     G4String   Scintillator       ,
                     G4double   LeadThickness      );  
   
   // Squared CsI
   void AddCsI(  G4double   R               ,
                     G4double   Theta           ,
                     G4double   Phi             ,
                     G4double   Height          ,
                     G4double   Width           ,
                     G4double   Thickness       ,
                     G4String   Scintillator    ,
                     G4double   LeadThickness   );
      
   void VolumeMaker(G4ThreeVector Det_pos, int DetNumber,G4LogicalVolume* world) ;
   ////////////////////////////////////////////////////
   /////////  Inherite from VDetector class ///////////
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
    G4MultiFunctionalDetector* m_CsIScorer ;
    G4MultiFunctionalDetector* m_PDScorer;
   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TCsIData* m_Event ;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   
   
   
   // Lead plate is added in front or back of the detector
   vector<double>     m_LeadThickness ;
   
   vector<double>      m_CsIThickness ;
   vector<double>      m_CsIRadius    ; // cylindrical shape
   vector<double> 	m_CsIFaceFront; 
  vector<double> 	m_CsIFaceBack;
         
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
