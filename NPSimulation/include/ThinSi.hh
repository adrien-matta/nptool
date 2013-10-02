#ifndef ThinSi_h
#define ThinSi_h 1

// C++ header
#include <string>
#include <vector>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"

// NPTool header
#include "VDetector.hh"

using namespace std;



class ThinSi : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   ThinSi() ;
   virtual ~ThinSi() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddTelescope(G4ThreeVector  TL       ,
         G4ThreeVector  BL       ,
         G4ThreeVector  BR       ,
         G4ThreeVector  TR       );
   // By Angle Method
   void AddTelescope(G4double    R        ,
         G4double    Theta    ,
         G4double    Phi         ,
         G4double    beta_u       ,
         G4double    beta_v       ,
         G4double    beta_w       );

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int DetectorNumber     ,
         G4ThreeVector     MMpos ,
         G4RotationMatrix* MMrot ,
         G4LogicalVolume*  world );


   ////////////////////////////////////////////////////
   /////////  Inherite from VDetector class ///////////
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


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   G4double    m_Energy ;

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
   vector<G4double>  m_R         ; //  |
   vector<G4double>  m_Theta     ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  >Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |
   // Set to true if you want to see Telescope Frame in your visualisation
   bool           m_non_sensitive_part_visiualisation ;

};
#endif
