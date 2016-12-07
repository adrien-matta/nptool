#ifndef W1_h
#define W1_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 12/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define the W1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// C++ headers
#include <vector>

// NPTool header
#include "NPSVDetector.hh"

// Geant4 headers
#include "G4MultiFunctionalDetector.hh"

// NPTool - ROOT headers
#include "TW1Data.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;



class W1 : public NPS::VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   W1();
   virtual ~W1();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // Detector positionning
   // By Position Method
   void AddDetector(G4ThreeVector TL, G4ThreeVector  BL,
                    G4ThreeVector BR, G4ThreeVector  CT);
   // By Angle Method
   void AddDetector(G4double R, G4double Theta, G4double Phi,
                    G4double beta_u, G4double beta_v, G4double beta_w);

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int             DetecNumber,
                    G4ThreeVector     pos,
                    G4RotationMatrix* rot,
                    G4LogicalVolume*  world);


   /////////////////////////////////////////
   ////  Inherite from NPS::VDetector class /////
   /////////////////////////////////////////
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

   // Initialize all scorers necessary for the detector
   void InitializeScorers();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TW1Data* m_Event;


   ////////////////////////////////////////////////////
   //////////////////// Scorers ///////////////////////
   ////////////////////////////////////////////////////
private:
   G4MultiFunctionalDetector* m_Scorer;


   ////////////////////////////////////////////////////
   //////////////////// Material //////////////////////
   ////////////////////////////////////////////////////
private:
   // Declare all material used by theW1 detector
   void InitializeMaterials();
   
   //  Vacuum
   G4Material* m_MaterialVacuum;
   // Si
   G4Material* m_MaterialSilicon;
   // Al
   G4Material* m_MaterialAluminium;
   // Iron
   G4Material* m_MaterialIron;


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_X1_Y1;    // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_X1_Y16;   // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_X16_Y1;   // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_X16_Y16;  // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R;     //  |
   vector<G4double>  m_Theta; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi;   //  |

   vector<G4double>  m_beta_u; //  |
   vector<G4double>  m_beta_v; //  > Tilt angle of the Telescope
   vector<G4double>  m_beta_w; //  |

   // Set to true if you want to see Telescope Frame in your visualisation
   bool  m_non_sensitive_part_visiualisation;
public:
    static NPS::VDetector* Construct();
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace W1SQUARE
{
   // Energy/Time resolutions for the different layers
   const G4double EnergyResolution = 12.8e-3;   // 30 keV FWHM 
   const G4double TimeResolution   = 0.638;     // 1.5 ns (FWHM)

   // Geometry
   const G4double FaceFront = 50*mm;
   const G4double Length    = 1*mm;

   // First stage
//   const G4double AluStripThickness = 0.00000001*micrometer;
   const G4double AluStripThickness = 0.4*micrometer;
   const G4double SiliconThickness  = 500*micrometer;
   const G4double SiliconFace       = 49.6*mm;

   // Characteristics
   const G4int NbStrips = 16;

   // Starting at the front and going in direction of third stage
   const G4double AluStripFront_PosZ = Length* -0.5       + 0.5*AluStripThickness;
   const G4double Silicon_PosZ       = AluStripFront_PosZ + 0.5*AluStripThickness + 0.5*SiliconThickness;
   const G4double AluStripBack_PosZ  = Silicon_PosZ       + 0.5*SiliconThickness  + 0.5*AluStripThickness;
}

#endif
