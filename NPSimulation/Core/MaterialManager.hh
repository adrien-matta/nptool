#ifndef __MATERIALMANAGER__
#define __MATERIALMANAGER__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : October 2014                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This singleton class contain a librairy of material available for         *
 * the geometry. Instantiate the needed material on demand, and generate the *
 * associate DEDX table.                                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// Geant4
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4ParticleDefinition.hh"
#include "G4LogicalVolume.hh"
// STL
#include<map>
using namespace std;

class MaterialManager{

public:
  // Designed to be a singleton (i.e. only one instance
  // can exist). A member function called Instance is defined, which allows
  // the user to get a pointer to the existing instance or to create it if
  // it does not yet exist:
  // (see the constructor for an explanation of the arguments)
  static MaterialManager* getInstance();

  // The analysis class instance can be deleted by calling the Destroy
  // method (NOTE: The class destructor is protected, and can thus not be
  // called directly):
  static void Destroy();

protected:
    // Constructor and Destructor are not public
    MaterialManager();
    ~MaterialManager();

private:
  // The static instance of MaterialManager:
  static MaterialManager* instance;
  
  // Map of element and material:
  map<string,G4Material*> m_Material; 
 
private:
  G4Element* m_D;
  G4Element* m_T;
  G4Element* m_He3;
public:
  // clear all exising material from the library
  void ClearMaterialLibrary();

  // Look for a known material named Name.
  // If the Material is known but not instantiate it is created and return
  // If the Material is instantiate it return it
  G4Material* GetMaterialFromLibrary(string Name, double density = 0);

  // Same as above but for Element.
  G4Element*  GetElementFromLibrary(string Name);
  
  // Let the user directly add a custom material to the library
  // It is howver overwritting existing material having the same name
  void AddMaterialToLibrary(G4Material*);

  // Create tiny block of active material so the DEDX tables are generated 
  // prevent crash if the user define material but don't use it
  void CreateSampleVolumes(G4LogicalVolume* world_log);

  // Write the DEDx table for all material instantiate in the MaterialManager
  // for a given particle
  void WriteDEDXTable(G4ParticleDefinition* Particle ,G4double Emin,G4double Emax);
};
#endif
