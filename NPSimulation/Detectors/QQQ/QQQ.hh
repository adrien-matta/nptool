#ifndef QQQ_h
#define QQQ_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a 20um Silicium detector                             *
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
#include "NPSVDetector.hh"

// NPLib 
#include "TQQQData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace QQQ_LOCAL{
   // Energy and time Resolution
   const G4double ResoTime    = 0      ;
   const G4double ResoEnergy  = 0.064  ;// = 150keV of Resolution   //   Unit is MeV/2.35
   const G4double EnergyThreshold = 100*keV;
   // Geometry
  const G4double DetectorSize      = 50*mm           ;
   const G4double DetectorRMin      = 9*mm           ;
   const G4double DetectorRMax      = 50*mm           ;
   const G4double DetStartPhi     = 5*deg           ;
   const G4double DetDeltaPhi     = 80*deg           ;
   const G4double SiliconThickness1  = 80*micrometer   ;
   const G4double SiliconThickness2  = 140*micrometer   ;

   const G4double FrameRMin      	= 8*mm           ;
   const G4double FrameRMax      	= 60*mm           ;
   const G4double FrameThickness    = 3*mm            ;
   const G4double FrameStartPhi     = 0*deg           ;
   const G4double FrameDeltaPhi     = 90*deg           ;
 
   const G4double SiliconSize       = 50*mm           ;
   const G4double AluThickness      = 0.4*micrometer  ;
   const G4int  NumberOfStrip       = 16              ;

   const G4double AluStripFront_PosZ   = -0.5*SiliconThickness1 - 0.5*AluThickness   ;
   const G4double Si_PosZ              = 0                                          ;
   const G4double AluStripBack_PosZ    = 0.5*SiliconThickness1 + 0.5*AluThickness    ;
   const G4double DetGap   = 1*mm   ;
}

class QQQ : public NPS::VDetector{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   QQQ() ;
   ~QQQ() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddTelescope(   G4ThreeVector  TL    ,
                        G4ThreeVector  BL    ,
                        G4ThreeVector  BR    ,
                        G4ThreeVector  TR    );
   // By Angle Method
   void AddTelescope(   G4double    R        ,
                        G4double    Theta    ,
                        G4double    Phi      ,
                        G4double    beta_u   ,
                        G4double    beta_v   ,
                        G4double    beta_w   );

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(    G4int DetectorNumber    ,
                        G4ThreeVector     MMpos ,
                        G4RotationMatrix* MMrot ,
                        G4LogicalVolume*  world );


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
   TQQQData*    m_Event ;

    ////////////////////////////////////////////////////
   ///////////////// Scorer Related ///////////////////
   ////////////////////////////////////////////////////

private:
      //   Initialize all Scorer used by the MUST2Array
      void InitializeScorers() ;
    
      //   Scorer Associate to the 20um Silicon stage
      G4MultiFunctionalDetector*   m_StripScorer ; 



private:
      //    Initialize mmaterial used in detector definition
      void InitializeMaterial();

      //   List of material
      G4Material* m_MaterialSilicon ;
      G4Material* m_MaterialAl      ;
      G4Material* m_MaterialVacuum  ;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_TL     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_BL     ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_BR     ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_TR     ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  >Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |
public:
    static NPS::VDetector* Construct();
};
#endif
