#ifndef Target2Array_h
#define Target2Array_h 1
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
 * Last update    : 06/11/2009                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Cryogenic and standard Target. Derived fromNPS::VDetector*
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Some improvment need to be done in material dealing                      *
 *                                                                           *
 *  + 16/09/2009: Add support for positioning the target with an angle with  *
 *                respect to the beam (N. de Sereville)                      *
 *  + 06/11/2009: Add new Token NBLAYERS defining the number of steps used   *
 *                to slow down the beam in the target (N. de Sereville)      *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>
#include <cmath>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4Event.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"

// NPTool headers
#include "NPSVDetector.hh"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Target : public NPS::VDetector{
public:
  Target();
  ~Target(){};
  
public:
  //   Read stream at Configfile to pick-up parameters of detector (Position,...)
  //   Called in DetecorConstruction::ReadDetextorConfiguration Method
  void ReadConfiguration(NPL::InputParser);
  
  //   Construct detector and inialise sensitive part.
  //   Called After DetecorConstruction::AddDetector Method
  void ConstructDetector(G4LogicalVolume* world);
  
  //   Add Detector branch to the EventTree.
  //   Called After DetecorConstruction::AddDetector Method
  void InitializeRootOutput();
  
  //   Read sensitive part and fill the Root tree.
  //   Called at in the EventAction::EndOfEventAvtion
  void ReadSensitive(const G4Event* event);
  
public:
  //   method for debug purpose (still to be implemented)
  //   This method should check if the results of the beam interaction within the target
  //   (interaction coordinates) are well located inside the target volume
  bool IsInsideTarget() {return false;};
  
  // Return the slow down beam energy after interaction at ZInteraction with initial beam energy before target IncidentEnergy
  G4double SlowDownBeam(G4ParticleDefinition* Beam, G4double IncidentEnergy, G4double ZInteraction, G4double IncidentTheta);
  
  // Used to simulate beam emmitance effect
  void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma = 10000);
  
public:
  //   Return Material from the Target Material Library
  G4Material* GetMaterialFromLibrary(G4String MaterialName);
  
public:
  G4double            GetTargetThickness()   {return m_TargetThickness;}
  G4Material*         GetTargetMaterial()    {return m_TargetMaterial;}
  G4double            GetTargetRadius()      {return m_TargetRadius;}
  G4double            GetTargetAngle()       {return m_TargetAngle;}
  G4double            GetTargetX()           {return m_TargetX;}
  G4double            GetTargetY()           {return m_TargetY;}
  G4double            GetTargetZ()           {return m_TargetZ;}
  G4ThreeVector       GetTargetPosition()    {return G4ThreeVector(m_TargetX,m_TargetY,m_TargetZ);}
  G4int               GetTargetNbLayers()    {return m_TargetNbLayers;}
  G4Tubs*             GetTargetSolid()       {return m_TargetSolid;}
  G4LogicalVolume*    GetTargetLogic()       {return m_TargetLogic;}  
  
private:
  // Target type : true = normal ; false = cryo
  bool     m_TargetType;
  
  // Solid and Logic Volume
  G4Tubs* m_TargetSolid;
  G4LogicalVolume* m_TargetLogic;
  
  // Standard parameter
  G4double    m_TargetThickness;
  G4double    m_TargetRadius;
  G4double    m_TargetAngle;
  G4double    m_EffectiveThickness;
  G4Material* m_TargetMaterial;
  G4int       m_TargetNbLayers;
  
  // For Cryo Target
  G4double    m_TargetDensity;
  G4double    m_WindowsThickness;
  G4Material* m_WindowsMaterial;
  
  // Positioning
  G4double    m_TargetX;
  G4double    m_TargetY;
  G4double    m_TargetZ;

  // Pointer to the last target generated (0 if none)
  private:
    static Target*  TargetInstance ;

  public:
    static Target* GetTarget(){return TargetInstance;}
};

#endif
