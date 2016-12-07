#ifndef MUST2Array_h
#define MUST2Array_h 1
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
 *  This file describe the MUST2 charge particle Detector                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * MUST2 is a modular array made of Telescope (1 to 8 telescope). Each       *
 *  Telescope is made of Three Stage:                                        *
 *  - A 300um Silicium, double-sided strip                                   *
 *  - 16 Si(Li) pad                                                          *
 *  - 16 CsI scintillator Crystal                                            *
 *****************************************************************************/
#include "NPSVDetector.hh"
#include "TMust2Data.h"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "NPInputParser.h"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace MUST2
{
   // Resolution
   const G4double ResoTimeMust = 0.212765957    ;// = 500ps                 //   Unit is  ns/2.35
   const G4double ResoSiLi     = 0.066          ;// = 130 keV of resolution //   Unit is MeV/2.35
   const G4double ResoCsI      = 0.08           ;// = 188 kev of resolution //   Unit is MeV/2.35
   const G4double ResoStrip    = 0.017          ;// = 52keV of Resolution   //   Unit is MeV/2.35

   // Geometry
   const G4double FaceFront = 11.*cm ;
   const G4double FaceBack = 16.5*cm ;
   const G4double Length  = 7.2*cm ;

   const G4double AluStripThickness = 0.4*micrometer ;
   const G4double SiliconThickness  = 300*micrometer ;
   const G4double SiliconFace       = 98*mm ;
   const G4double VacBoxThickness   = 3*cm ;

   const G4double SiLiThickness     = 5.1*mm;  // Must be checked
   const G4double SiLiFaceX         = 48.25*mm;
   const G4double SiLiFaceY         = 92*mm;
   const G4double MylarCsIThickness = 3*micrometer;
   const G4double CsIThickness      = 4.*cm + 2*MylarCsIThickness ;
   const G4double CsIFaceFront      = 12.2*cm;
   const G4double CsIFaceBack       = 16*cm;
   const G4double DistInterCsI      = 0.2*mm;
   
   // Starting at the front and going to CsI
   const G4double AluStripFront_PosZ   = Length* -0.5 + 0.5*AluStripThickness;
   const G4double Silicon_PosZ      = AluStripFront_PosZ + 0.5*AluStripThickness + 0.5*SiliconThickness;
   const G4double AluStripBack_PosZ = Silicon_PosZ + 0.5*SiliconThickness + 0.5*AluStripThickness;
   const G4double VacBox_PosZ    = AluStripBack_PosZ + 0.5*AluStripThickness + 0.5* VacBoxThickness;
   const G4double CsI_PosZ       = VacBox_PosZ + 0.5*VacBoxThickness + 0.5*CsIThickness;
}

class MUST2Array : public NPS::VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   MUST2Array()   ;
   virtual ~MUST2Array()   ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddTelescope(   G4ThreeVector  TL       ,
                        G4ThreeVector  BL       ,
                        G4ThreeVector  BR       ,
                        G4ThreeVector  CT       ,
                        bool           wSi      ,
                        bool           wSiLi    ,
                        bool           wCsI     );
   // By Angle Method
   void AddTelescope(   G4double    R        ,
                        G4double    Theta    ,
                        G4double    Phi      ,
                        G4double    beta_u   ,
                        G4double    beta_v   ,
                        G4double    beta_w   ,
                        bool        wSi      ,
                        bool        wSiLi    ,
                        bool        wCsI     );

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker( G4int             TelescopeNumber   ,
                     G4ThreeVector     MMpos             ,
                     G4RotationMatrix* MMrot             ,
                     bool              wSi               ,
                     bool              wSiLi             ,
                     bool              wCsI              ,
                     G4LogicalVolume*  world             );


   ////////////////////////////////////////////////////
   /////////  Inherite from NPS::VDetector class ///////////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(NPL::InputParser);

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world);

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TMust2Data* m_Event;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_X1_Y1     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_X1_Y128   ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y1   ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y128 ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  > Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |

   // If Set to true if you want this stage on you telescope
   vector<bool>      m_wSi    ; // Silicium Strip 300um 128*128 Strip
   vector<bool>      m_wSiLi  ; // Si(Li) 2*4 Pad
   vector<bool>      m_wCsI   ; // CsI 4*4 crystal
   vector<bool>      m_wAddSi ; // Additionnal Thin Silicium Strip

   // Set to true if you want to see Telescope Frame in your visualisation
   bool m_non_sensitive_part_visiualisation ;
   
   
   ////////////////////////////////////////////////////
   ///////////////////// Scorer ///////////////////////
   ////////////////////////////////////////////////////
private:
   //   Initialize all Scorer used by the MUST2Array
   void InitializeScorers() ;

   //   Silicon Associate Scorer
   G4MultiFunctionalDetector* m_StripScorer ;
     
     
     //   SiLi Associate Scorer
     G4MultiFunctionalDetector* m_SiLiScorer ;
    
    //   CsI Associate Scorer 
    G4MultiFunctionalDetector* m_CsIScorer ;
    
    
   ////////////////////////////////////////////////////
   //////////////////// Material //////////////////////
   ////////////////////////////////////////////////////
private:
   //   Declare all material used by the MUST2Array
   void InitializeMaterial() ;
   // Si
   G4Material* m_MaterialSilicon;
   // Al
   G4Material* m_MaterialAluminium;
   // Iron
   G4Material* m_MaterialIron;
   // CsI
   G4Material* m_MaterialCsI;
   //  Vacuum
   G4Material* m_MaterialVacuum ;
   //  Mylar
   G4Material* m_MaterialMyl;
public:
    static NPS::VDetector* Construct();
};

extern G4RotationMatrix* Rotation(double tetaX, double tetaY, double tetaZ);
#endif
