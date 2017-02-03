#ifndef Lassa_h
#define Lassa_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address:						           *
 *                                                                           *
 * Creation Date  : Feb 2015												                         *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Lassa Telescops                                  *
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
#include "TLassaData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace LASSA
{
    
    // Resolution
    const G4double ResoTime         = 0.212765957;      // = 500ps                 //   Unit is  ns/2.35
    const G4double ResoCsI      	= 0.200/2.35;       // = 250 kev of resolution //   Unit is MeV/2.35
    const G4double ResoThickSi  	= 0.065/2.35;       // = 120keV of Resolution   //   Unit is MeV/2.35
    // const G4double ResoThinSi       = 0.050/2.35;       // = 80keV of Resolution   //   Unit is MeV/2.35
    
    const G4double EnergyThreshold = 0.;//100*keV;
    
    
    
    //LASSA values
    const G4double FaceFront        = 7.*cm ;
    const G4double FaceBack         = 7.*cm ;
    const G4double Length           = 10.*cm ;
    const G4int NumberOfStrip       = 16;
    
    
    
    // LASSA values
    const G4double SiliconFace       = 50*mm ;
    const G4double AluStripThickness = 0.4*micrometer ;
    const G4double ThinSiThickness   = 65*micrometer ;
    const G4double ThickSiThickness  = 500*micrometer ;
    const G4double DistBetweenSi     = (1.52+5.21)*mm;
    const G4double VacBoxThickness   = (2.46+5.21)*mm ;
    const G4double MylarCsIThickness = 3*micrometer;
    const G4double CsIThickness      = 6.*cm;// + 2*MylarCsIThickness ;*
    
    const G4double CsIXFront         = 26.5*mm;
    const G4double CsIXBack          = 33.8*mm;
    const G4double CsIYFront         = 26.5*mm;
    const G4double CsIYBack          = 33.8*mm;
    const G4double DistInterCsI      = 0.2*mm;
    
    const G4double ClusterFaceFront  = 2*CsIXFront + 2+DistInterCsI ;
    const G4double ClusterFaceBack   = 2*CsIXBack + 2+DistInterCsI ;
    
    // Starting at the front and going to CsI
    const G4double AluStripFrontThinSi_PosZ		= Length* -0.5 + 0.5*AluStripThickness;
    const G4double ThinSi_PosZ                  = AluStripFrontThinSi_PosZ + 0.5*AluStripThickness + 0.5*ThinSiThickness;
    const G4double AluStripBackThinSi_PosZ		= ThinSi_PosZ + 0.5*ThinSiThickness + 0.5*AluStripThickness;
    
    const G4double AluStripFrontThickSi_PosZ	= AluStripBackThinSi_PosZ + DistBetweenSi + 0.5*AluStripThickness + 0.5*AluStripThickness;
    const G4double ThickSi_PosZ                 = AluStripFrontThickSi_PosZ + 0.5*AluStripThickness + 0.5*ThickSiThickness;
    const G4double AluStripBackThickSi_PosZ		= ThickSi_PosZ + 0.5*ThickSiThickness + 0.5*AluStripThickness;
    
    const G4double VacBox_PosZ                  = AluStripBackThickSi_PosZ + 0.5*AluStripThickness + 0.5* VacBoxThickness;
    const G4double CsI_PosZ                     = VacBox_PosZ + 0.5*VacBoxThickness + 0.5*CsIThickness;
    
    
    
    
}

using namespace LASSA ;
class Lassa : public NPS::VDetector
{
    ////////////////////////////////////////////////////
    /////// Default Constructor and Destructor /////////
    ////////////////////////////////////////////////////
public:
    Lassa() ;
    ~Lassa() ;
    
    ////////////////////////////////////////////////////
    //////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
public:
    // By Position Method
    void AddTelescope(  G4ThreeVector  Pos1    ,
                      G4ThreeVector  Pos2    ,
                      G4ThreeVector  Pos3    ,
                      G4ThreeVector  Pos4    );
    
    // Effectively construct Volume
    // Avoid to have two time same code for Angle and Point definition
    void VolumeMaker(   G4int DetectorNumber    ,
                     G4ThreeVector     MMpos ,
                     G4RotationMatrix* MMrot ,
                     G4LogicalVolume*  world );
    
    
private:
    //G4LogicalVolume* m_LogicThinSi;
    G4LogicalVolume* m_LogicThickSi;
    G4LogicalVolume* m_LogicCsICrystal;
    G4LogicalVolume* m_LogicCluster;
    G4LogicalVolume* m_logicMotherVolume;
    ////////////////////////////////////////////////////
    //////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
    
    
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
    
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
private:
    TLassaData*    m_EventLassa ;
    
    ////////////////////////////////////////////////////
    ///////////////// Scorer Related ///////////////////
    ////////////////////////////////////////////////////
    
private:
    //   Initialize all Scorer
    void InitializeScorers() ;
    
    //   Silicon Associate Scorer
    //G4MultiFunctionalDetector* m_ThinSiStripScorer ;
    G4MultiFunctionalDetector* m_ThickSiStripScorer ;
    
    //   CsI Associate Scorer
    G4MultiFunctionalDetector* m_CsIScorer ;
    
private:
    //    Initialize material used in detector definition
    void InitializeMaterial();
    
    //   List of material
    // Si
    G4Material* m_MaterialSilicon;
    // Al
    G4Material* m_MaterialAluminium;
    // CsI
    G4Material* m_MaterialCsI;
    //  Vacuum
    G4Material* m_MaterialVacuum ;
    //  Mylar
    G4Material* m_MaterialMylar;
    //  PCB
    G4Material* m_MaterialPCB;
    
    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
private:
    
    // True if the detector is a Cluster, false if a single Phoswich
    vector<bool> m_Type;
    
    // Detector position
    vector<G4ThreeVector> m_Pos;
    vector<G4RotationMatrix*> m_Rot;
    
    
    // Used for "By Point Definition"
    //vector<G4ThreeVector>   m_TL     ; // Top Left Corner Position Vector
    //vector<G4ThreeVector>   m_BL     ; // Bottom Left Corner Position Vector
    //vector<G4ThreeVector>   m_BR     ; // Bottom Right Corner Position Vector
    //vector<G4ThreeVector>   m_TR     ; // Center Corner Position Vector
    
    // Used for "By Angle Definition"
    //vector<G4double>  m_R      ; //  |
    //vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
    //vector<G4double>  m_Phi    ; //  |
    
    vector<G4double>  m_beta_u ; //  |
    vector<G4double>  m_beta_v ; //  >Tilt angle of the Telescope
    vector<G4double>  m_beta_w ; //  |
    
    
private:/// Visualisation Attribute:
    G4VisAttributes* m_SiliconVisAtt  ;
    G4VisAttributes* m_CsIVisAtt;
    
public:
    static NPS::VDetector* Construct();
    
};
#endif
