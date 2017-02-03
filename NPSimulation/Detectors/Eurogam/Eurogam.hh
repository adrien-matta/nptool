#ifndef Eurogam_h
#define Eurogam_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : December 2010                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class describes the Eurogam germanium detector                   *
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
#include "NPSVDetector.hh"
#include "TEurogamData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Eurogam : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Eurogam() ;
    virtual ~Eurogam() ;

    ////////////////////////////////////////////////////
    //////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // By Angle Method
    void AddEurogamModule(G4double R, G4double Theta, G4double Phi,
        G4double beta_u, G4double beta_v, G4double beta_w);

    void VolumeMaker(G4int             DetectorNumber, 
        G4ThreeVector     DetectorPosition, 
        G4RotationMatrix* DetectorRotation,
        G4LogicalVolume*  world);


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


    /////////////////////////////////////////////////
    /////////////////// Materials ///////////////////
    /////////////////////////////////////////////////
  private:
    void InitializeMaterial();
    G4Material* m_Material_Vacuum;
    G4Material* m_Material_Aluminium;
    G4Material* m_Material_Silicon;
    G4Material* m_Material_Germanium;


    /////////////////////////////////////////////////
    //////////////////// Scorers ////////////////////
    /////////////////////////////////////////////////
  public:
    // Initialize all Scorer used by Eurogam
    void InitializeScorers();

  private:
    // Eurogam Scorer
    G4MultiFunctionalDetector* m_EurogamScorer;


    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TEurogamData* m_Event;


    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private:
    // Used for "By Angle Definition"
    vector<G4double>  m_R;     //  |
    vector<G4double>  m_Theta; //  > Spherical coordinate Eurogam volume center
    vector<G4double>  m_Phi;   //  |
    vector<G4double>  m_beta_u; //  |
    vector<G4double>  m_beta_v; //  > Tilt angle of the detector
    vector<G4double>  m_beta_w; //  |
  public:
    static NPS::VDetector* Construct();
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace EUROGAMDETECTOR
{
  // Energy and time Resolution
  const G4double ResoTime    = 4.2             ;// = 10ns of Resolution   //   Unit is MeV/2.35
  const G4double ResoEnergy  = 0.2             ;// Resolution in %

  // Geometry
  const G4double EurogamSize  =  90*mm;
  const G4double EurogamDepth = 280*mm;

  // definition of the 
  //   const G4double 

  // Definition of the crystal
  //   const G4double CrystalLength = 
}

#endif
