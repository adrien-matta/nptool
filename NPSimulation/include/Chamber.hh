#ifndef Chamber2Array_h
#define Chamber2Array_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@atfc.ac.uk   *
 *                                                                           *
 * Creation Date  : January 2010                                             *
 * Last update    : 11/03/2010                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Cryogenic and standard Chamber.                      *
 *  Derived from VDetector                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *
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
#include "G4LogicalVolume.hh"

// NPTool headers
#include "VDetector.hh"

using namespace std;

class Chamber : public VDetector
{
   public:
      Chamber();
      ~Chamber(){};


   public:
      //   Read stream at Configfile to pick-up parameters of detector (Position,...)
      //   Called in DetecorConstruction::ReadDetextorConfiguration Method
      void ReadConfiguration(string Path);

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
      //   Return Material from the Chamber Material Library
      G4Material* GetMaterialFromLibrary(G4String MaterialName, G4double Temperature = 0, G4double Pressure = 0);
      
      //   Generate a DEDX file table using the material used in the target
      void             WriteDEDXTable(G4ParticleDefinition* Particle,G4double Emin,G4double Emax);

   public:
      G4double    GetChamberRmin()     {return m_ChamberRmin;}
      G4double    GetChamberRmax()     {return m_ChamberRmax;}
      G4double    GetChamberPhiMin()   {return m_ChamberPhiMin;}
      G4double    GetChamberPhiMax()   {return m_ChamberPhiMax;}
      G4double    GetChamberThetaMin() {return m_ChamberThetaMin;}
      G4double    GetChamberThetaMax() {return m_ChamberThetaMax;}
      G4Material* GetChamberMaterial() {return m_ChamberMaterial;}
  //G4int       GetChamberNbLayers()      {return m_ChamberNbLayers;}


   private:
      // Chamber type : true = normal ; false = cryo
      bool     m_ChamberType;

      // Standard parameter
      G4double    m_ChamberRmin;
      G4double    m_ChamberRmax;
      G4double    m_ChamberPhiMin;
      G4double    m_ChamberPhiMax;
      G4double    m_ChamberThetaMin;
      G4double    m_ChamberThetaMax;
      G4Material* m_ChamberMaterial;
      G4int       m_ChamberNbLayers;
  /*
      // For Cryo Chamber
      G4double    m_ChamberTemperature;
      G4double    m_ChamberPressure;
      G4double    m_WindowsThickness;
      G4Material* m_WindowsMaterial;

      // Positioning
      G4double    m_ChamberX;
      G4double    m_ChamberY;
      G4double    m_ChamberZ;
  */
};

#endif
