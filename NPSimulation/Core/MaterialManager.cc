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

// NPS
#include "MaterialManager.hh"

// Geant4
#include "G4Box.hh"
#include "G4Element.hh"
#include "G4EmCalculator.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4ParticleTable.hh"
#include "G4VisAttributes.hh"
// STL
#include <iostream>
#include <string>
using namespace std;
using namespace CLHEP;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager* MaterialManager::instance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager* MaterialManager::getInstance() {
  if (instance == 0)
    instance = new MaterialManager();
  return instance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager::~MaterialManager() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager::MaterialManager() {
  m_D   = NULL;
  m_T   = NULL;
  m_He3 = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::Destroy() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::ClearMaterialLibrary() {
  map<string, G4Material*>::iterator it;
  for (it = m_Material.begin(); it != m_Material.end(); it++) {
    delete it->second;
  }

  m_Material.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* MaterialManager::GetMaterialFromLibrary(string Name,
                                                    double density) {
  // Search if the material is already instantiate
  map<string, G4Material*>::iterator it;
  it = m_Material.find(Name);

  // The element is not found
  if (it == m_Material.end()) {

    // Usual compound
    if (Name == "Vacuum" || Name == "Vaccum" || Name == "Vaccuum"
        || Name == "Vacum") {
      if (!density)
        density            = 0.000000001 * mg / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("N"), 7);
      material->AddElement(GetElementFromLibrary("O"), 3);
      m_Material[Name] = material;
      return material;
    }

    if (Name == "Air") {
      if (!density)
        density            = 1.290 * mg / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("N"), 7);
      material->AddElement(GetElementFromLibrary("O"), 3);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "PCB") {
      if (!density)
        density = 1.85 * g / cm3;
      // Actually taken value fron Epoxy
      G4Material* material = new G4Material("NPS_" + Name, density, 3);
      material->AddElement(GetElementFromLibrary("H"), .475);
      material->AddElement(GetElementFromLibrary("C"), .45);
      material->AddElement(GetElementFromLibrary("O"), .075);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Epoxy") {
      if (!density)
        density = 1.2 * g / cm3;
      // Actually taken value fron Epoxy
      G4Material* material = new G4Material("NPS_" + Name, density, 3);
      material->AddElement(GetElementFromLibrary("H"), 8);
      material->AddElement(GetElementFromLibrary("C"), 5);
      material->AddElement(GetElementFromLibrary("O"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Mylar") {
      if (!density)
        density            = 1.397 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 3);
      material->AddElement(GetElementFromLibrary("H"), 8);
      material->AddElement(GetElementFromLibrary("C"), 10);
      material->AddElement(GetElementFromLibrary("O"), 4);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Kapton") {
      if (!density)
        density            = 1.39 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 3);
      material->AddElement(GetElementFromLibrary("H"), 4);
      material->AddElement(GetElementFromLibrary("C"), 10);
      material->AddElement(GetElementFromLibrary("O"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Kovar") {
      if (!density)
        density            = 8 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 5);
      material->AddElement(GetElementFromLibrary("Ni"), 290);
      material->AddElement(GetElementFromLibrary("Co"), 170);
      material->AddElement(GetElementFromLibrary("Si"), 2);
      material->AddElement(GetElementFromLibrary("Mg"), 3);
      material->AddElement(GetElementFromLibrary("Fe"), 535);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Havar") {
      if (!density)
        density            = 8.3 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 5);
      material->AddElement(GetElementFromLibrary("Co"), 42);
      material->AddElement(GetElementFromLibrary("Cr"), 20);
      material->AddElement(GetElementFromLibrary("Ni"), 13);
      material->AddElement(GetElementFromLibrary("Fe"), 19);
      material->AddElement(GetElementFromLibrary("W"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "LiF") {
      if (!density)
        density            = 2.64 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Li"), 1);
      material->AddElement(GetElementFromLibrary("F"), 1);
      m_Material[Name] = material;
      return material;
    }

    // Cooling
    else if (Name == "N2_liquid") {
      if (!density)
        density            = 0.808 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, 7, 14.01 * g / mole,
                                            density, kStateLiquid, 77 * kelvin);
      m_Material[Name] = material;
      return material;
    }

    // Usual Target
    else if (Name == "CD2") {
      if (!density)
        density            = 1.06 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("C"), 1);
      material->AddElement(GetElementFromLibrary("D"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "WO3") { // Tungsten trioxide
      if (!density)
        density            = 5.907 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("W"), 1);
      material->AddElement(GetElementFromLibrary("O"), 3);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "CH2") {
      if (!density)
        density            = 0.93 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("C"), 1);
      material->AddElement(GetElementFromLibrary("H"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Cu") {
      if (!density)
        density            = 8.96 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Cu"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Au") {
      if (!density)
        density            = 19.3 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Au"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "C") { // Graphite
      if (!density)
        density            = 2.267 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("C"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Pb") {
      if (!density)
        density            = 11.342 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Pb"), 1);
      m_Material[Name] = material;

      return material;
    }

    else if (Name == "D2") {
      if (!density)
        density            = 0.0715 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("D"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "H2") {
      if (!density)
        density            = 0.0715 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("H"), 2);
      m_Material[Name] = material;
      return material;
    } else if (Name == "H2_gas") {
      if (!density)
        density            = 3.34e-11 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("H"), 2);
      m_Material[Name] = material;
      return material;
    } else if (Name == "He_gas") {
      if (!density)
        density            = 0.0001665 * g / cm3; // room temp, 1 atm
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("He"), 1);
      m_Material[Name] = material;
      return material;
    } else if (Name == "O2_gas") {
      if (!density)
        density            = 0.001331 * g / cm3; // room temp, 1 atm
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("O"), 2);
      m_Material[Name] = material;
      return material;
    } else if (Name == "Ti") {
      if (!density)
        density            = 4.5189 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Ti"), 1);
      m_Material[Name] = material;
      return material;
    }
    // Usual detector material
    else if (Name == "Si") {
      if (!density)
        density            = 2.321 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Si"), 1);

      // Adding Optical property:
      double* energy_r   = new double[2];
      double* rindex     = new double[2];
      double* absorption = new double[2];

      energy_r[0] = 1 * eV;
      energy_r[1] = 1 * MeV;

      rindex[0]     = 1;
      rindex[1]     = 1;
      absorption[0] = 1 * um;
      absorption[1] = 1 * um;

      G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();

      // From St Gobain
      MPT->AddProperty("RINDEX", energy_r, rindex, 2);
      MPT->AddProperty("ABSLENGTH", energy_r, absorption, 2);
      material->SetMaterialPropertiesTable(MPT);

      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Ge") {
      if (!density)
        density            = 5.323 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Ge"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Boric_Oxyde") {
      if (!density)
        density            = 2.55 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("B"), 2);
      material->AddElement(GetElementFromLibrary("O"), 3);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Sodium_Oxyde") {
      if (!density)
        density            = 2.27 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Na"), 2);
      material->AddElement(GetElementFromLibrary("O"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Borosillicate_Glass") {
      if (!density)
        density            = 2.23 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 4);
      material->AddElement(GetElementFromLibrary("Si"), 80 * perCent);
      G4Material* BO = GetMaterialFromLibrary("Boric_Oxyde");
      material->AddMaterial(BO, 13 * perCent);
      G4Material* NaO = GetMaterialFromLibrary("Sodium_Oxyde");
      material->AddMaterial(NaO, 4 * perCent);
      material->AddElement(GetElementFromLibrary("Al"), 3 * perCent);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "BC400") {
      if (!density)
        density            = 1.032 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("H"), 10);
      material->AddElement(GetElementFromLibrary("C"), 9);
      m_Material[Name] = material;
      return material;
    }
    // para-Terphenyl
    else if (Name == "para-Terphenyl") {
      if (!density)
        density            = 1.23 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("H"), 14);
      material->AddElement(GetElementFromLibrary("C"), 18);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "para-Terphenyl_Scintillator") {
      if (!density)
        density            = 1.23 * g / cm3; // good
      G4Material* material = new G4Material("NPS_" + Name, density, 2); // check
      material->AddElement(GetElementFromLibrary("H"), 14); // good
      material->AddElement(GetElementFromLibrary("C"), 18); // good
      // Adding Scintillation property:
      int     NumberOfPoints = 10; // check
      double  wlmin          = 0.25 * um; // check
      double  wlmax          = 67 * um; // check
      double  step           = (wlmax - wlmin) / NumberOfPoints;
      double* energy_r       = new double[NumberOfPoints];
      double* rindex         = new double[NumberOfPoints];
      double* absorption     = new double[NumberOfPoints];

      double* energy_e = new double[5];
      double* fast     = new double[5];
      double* slow     = new double[5];
      double* scint    = new double[5];

      // check block below
      energy_e[0] = h_Planck * c_light / (450 * nm);
      energy_e[1] = h_Planck * c_light / (500 * nm);
      energy_e[2] = h_Planck * c_light / (550 * nm);
      energy_e[3] = h_Planck * c_light / (600 * nm);
      energy_e[4] = h_Planck * c_light / (650 * nm);

      for (int i = 0; i < 5; i++) {
        // fast[0] = 1 ; fast[1]=1;
        // slow[0] = 1 ; slow[1]=1;
        fast[i] = 2.1; // good
        slow[i] = 22.6; // check
      }
      // check below block
      scint[0] = 0.25;
      scint[1] = 0.75;
      scint[2] = 1.0;
      scint[3] = 0.7;
      scint[4] = 0.4;

      double wl;

      // check below block
      for (int i = 0; i < NumberOfPoints; i++) {
        wl = wlmin + i * step;
        // Formula from www.refractiveindex.info
        rindex[i] = sqrt(1 + 0.27587 + 0.68689 / (1 - pow(0.130 / wl, 2))
                         + 0.26090 / (1 - pow(0.147 / wl, 2))
                         + 0.06256 / (1 - pow(0.163 / wl, 2))
                         + 0.06527 / (1 - pow(0.177 / wl, 2))
                         + 0.14991 / (1 - pow(0.185 / wl, 2))
                         + 0.51818 / (1 - pow(0.206 / wl, 2))
                         + 0.01918 / (1 - pow(0.218 / wl, 2))
                         + 3.38229 / (1 - pow(161.29 / wl, 2)));
        // check below block
        energy_r[i] = h_Planck * c_light / wl;
        // To be defined properly
        absorption[i] = 344.8 * cm;
      }

      G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();

      // From St Gobain
      MPT->AddConstProperty("SCINTILLATIONYIELD", 27000000 / keV); // good
      MPT->AddProperty("SCINTILLATION", energy_e, scint, 5); // check
      MPT->AddProperty("RINDEX", energy_r, rindex, NumberOfPoints); // check
      MPT->AddProperty("ABSLENGTH", energy_r, absorption,
                       NumberOfPoints); // check
      MPT->AddProperty("FASTCOMPONENT", energy_e, fast, 2.1); // good
      MPT->AddProperty("SLOWCOMPONENT", energy_e, slow, 22.6); // good
      MPT->AddConstProperty("RESOLUTIONSCALE", 1.0); // check
      MPT->AddConstProperty("FASTTIMECONSTANT", 1000 * ns); // check
      MPT->AddConstProperty("SLOWTIMECONSTANT", 1000 * ns); // check
      MPT->AddConstProperty("YIELDRATIO", 1.0); // check
      material->SetMaterialPropertiesTable(MPT); // good
      m_Material[Name] = material; // good
      return material;
    }

    else if (Name == "NaI") {
      if (!density)
        density            = 3.67 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Na"), 1);
      material->AddElement(GetElementFromLibrary("I"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "CsI") {
      if (!density)
        density            = 4.51 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Cs"), 1);
      material->AddElement(GetElementFromLibrary("I"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "NaturalUranium") {
      if (!density)
        density            = 19.1 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("U"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "NaturalTin") {
      if (!density)
        density            = 7.31 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Sn"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "CsI_Scintillator") {
      if (!density)
        density            = 4.51 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Cs"), 1);
      material->AddElement(GetElementFromLibrary("I"), 1);
      // Adding Scintillation property:
      int     NumberOfPoints = 10;
      double  wlmin          = 0.25 * um;
      double  wlmax          = 67 * um;
      double  step           = (wlmax - wlmin) / NumberOfPoints;
      double* energy_r       = new double[NumberOfPoints];
      double* rindex         = new double[NumberOfPoints];
      double* absorption     = new double[NumberOfPoints];

      double* energy_e = new double[5];
      double* fast     = new double[5];
      double* slow     = new double[5];
      double* scint    = new double[5];

      energy_e[0] = h_Planck * c_light / (450 * nm);
      energy_e[1] = h_Planck * c_light / (500 * nm);
      energy_e[2] = h_Planck * c_light / (550 * nm);
      energy_e[3] = h_Planck * c_light / (600 * nm);
      energy_e[4] = h_Planck * c_light / (650 * nm);

      for (int i = 0; i < 5; i++) {
        // fast[0] = 1 ; fast[1]=1;
        // slow[0] = 1 ; slow[1]=1;
        fast[i] = 0.6;
        slow[i] = 3.5;
      }
      scint[0] = 0.25;
      scint[1] = 0.75;
      scint[2] = 1.0;
      scint[3] = 0.7;
      scint[4] = 0.4;

      double wl;

      for (int i = 0; i < NumberOfPoints; i++) {
        wl = wlmin + i * step;
        // Formula from www.refractiveindex.info
        rindex[i] = sqrt(1 + 0.27587 + 0.68689 / (1 - pow(0.130 / wl, 2))
                         + 0.26090 / (1 - pow(0.147 / wl, 2))
                         + 0.06256 / (1 - pow(0.163 / wl, 2))
                         + 0.06527 / (1 - pow(0.177 / wl, 2))
                         + 0.14991 / (1 - pow(0.185 / wl, 2))
                         + 0.51818 / (1 - pow(0.206 / wl, 2))
                         + 0.01918 / (1 - pow(0.218 / wl, 2))
                         + 3.38229 / (1 - pow(161.29 / wl, 2)));

        energy_r[i] = h_Planck * c_light / wl;
        // To be defined properly
        absorption[i] = 344.8 * cm;
      }

      G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();

      // From St Gobain
      MPT->AddConstProperty("SCINTILLATIONYIELD", 54 / keV);
      MPT->AddProperty("SCINTILLATION", energy_e, scint, 5);
      MPT->AddProperty("RINDEX", energy_r, rindex, NumberOfPoints);
      MPT->AddProperty("ABSLENGTH", energy_r, absorption, NumberOfPoints);
      MPT->AddProperty("FASTCOMPONENT", energy_e, fast, 5);
      MPT->AddProperty("SLOWCOMPONENT", energy_e, slow, 5);
      MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
      MPT->AddConstProperty("FASTTIMECONSTANT", 1000 * ns);
      MPT->AddConstProperty("SLOWTIMECONSTANT", 1000 * ns);
      MPT->AddConstProperty("YIELDRATIO", 1.0);
      material->SetMaterialPropertiesTable(MPT);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "LaBr3") {
      if (!density)
        density            = 5.06 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("La"), 1);
      material->AddElement(GetElementFromLibrary("Br"), 3);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "LaBr3_Ce") {
      if (!density)
        density            = 5.29 * g / cm3;
      G4Material* base     = GetMaterialFromLibrary("LaBr3");
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddMaterial(base, 95 * perCent);
      material->AddElement(GetElementFromLibrary("Ce"), 5 * perCent);

      m_Material[Name] = material;
      return material;
    }

    else if (Name == "BaF2") {
      if (!density)
        density            = 4.89 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("Ba"), 1);
      material->AddElement(GetElementFromLibrary("F"), 2);
      m_Material[Name] = material;
      return material;
    }

    // Misc
    else if (Name == "Al") {
      if (!density)
        density            = 2.702 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Al"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Fe") {
      if (!density)
        density            = 7.874 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Fe"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Ta" || Name == "Tantalum") {
      if (!density)
        density            = 16.601 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Ta"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Ca") {
      if (!density)
        density            = 1.54 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 1);
      material->AddElement(GetElementFromLibrary("Ca"), 1);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "P10_1atm") {
      if (!density)
        density = 1.74 * mg / cm3;
      G4Material* material
          = new G4Material("NPS_" + Name, density, 3); //@ 0K, 1 atm
      material->AddElement(GetElementFromLibrary("Ar"), 0.9222);
      material->AddElement(GetElementFromLibrary("C"), 0.0623);
      material->AddElement(GetElementFromLibrary("H"), 0.0155);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "P10") {
      if (!density)
        density = 0.57 * mg / cm3;
      G4Material* material
          = new G4Material("NPS_" + Name, density, 3); //@ 0K, 1/3 atm
      material->AddElement(GetElementFromLibrary("Ar"), 0.9222);
      material->AddElement(GetElementFromLibrary("C"), 0.0623);
      material->AddElement(GetElementFromLibrary("H"), 0.0155);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Air") { // 1 atm
      if (!density)
        density            = 1.290 * mg / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("N"), 0.7);
      material->AddElement(GetElementFromLibrary("O"), 0.3);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "CF4") { // 52 torr
      if (!density)
        density            = 3.78 * mg / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2,
                                            kStateGas, 300, 0.0693276 * bar);
      material->AddElement(GetElementFromLibrary("C"), 1);
      material->AddElement(GetElementFromLibrary("F"), 4);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Wood") {
      if (!density)
        density            = 0.9 * mg / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 3);
      material->AddElement(GetElementFromLibrary("H"), 4);
      material->AddElement(GetElementFromLibrary("O"), 1);
      material->AddElement(GetElementFromLibrary("C"), 2);
      m_Material[Name] = material;
      return material;
    }

    else if (Name == "Pyrex") {
      if (!density)
        density            = 2.23 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 5);
      material->AddElement(GetElementFromLibrary("Si"), 25);
      material->AddElement(GetElementFromLibrary("O"), 65);
      material->AddElement(GetElementFromLibrary("B"), 7);
      material->AddElement(GetElementFromLibrary("Na"), 2);
      material->AddElement(GetElementFromLibrary("Al"), 1);

      //--------------------- PMMA optical Properties ---------------------//
      const G4int NUMENTRIES = 15;

      G4double PMMA_PP[NUMENTRIES]
          = {10 * eV,    3.25 * eV,  3.099 * eV, 2.88 * eV,  2.695 * eV,
             2.53 * eV,  2.38 * eV,  2.254 * eV, 2.138 * eV, 2.033 * eV,
             1.937 * eV, 1.859 * eV, 1.771 * eV, 1.6 * eV,   0 * eV};
      G4double PMMA_RIND[NUMENTRIES]
          = {1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47,
             1.47, 1.47, 1.47, 1.47, 1.47, 1.47, 1.47};
      G4double PMMA_ABSL[NUMENTRIES]
          = {35. * cm, 35. * cm, 35. * cm, 35. * cm, 35. * cm,
             35. * cm, 35. * cm, 35. * cm, 35. * cm, 35. * cm,
             35. * cm, 35. * cm, 35. * cm, 35. * cm, 35. * cm};
      /*G4double THICK_ABSL[NUMENTRIES]  =
         {0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,
                                          0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm,0.01*mm};*/

      G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
      myMPT2->AddProperty("RINDEX", PMMA_PP, PMMA_RIND, NUMENTRIES);
      myMPT2->AddProperty("ABSLENGTH", PMMA_PP, PMMA_ABSL, NUMENTRIES);

      material->SetMaterialPropertiesTable(myMPT2);

      m_Material[Name] = material;
      return material;
    }

    else if (Name == "NE213") {
      if (!density)
        density            = 0.874 * g / cm3;
      G4Material* material = new G4Material("NPS_" + Name, density, 2);
      material->AddElement(GetElementFromLibrary("C"), 5);
      material->AddElement(GetElementFromLibrary("H"), 6);

      //--------------------- Optical Properties ---------------------//
      const G4int NUMENTRIES = 15;
      G4double    CsI_PP[NUMENTRIES]
          = {10 * eV,    3.5 * eV,  3.25 * eV, 3.2 * eV,  3.15 * eV,
             3.099 * eV, 3.0 * eV,  2.95 * eV, 2.88 * eV, 2.75 * eV,
             2.695 * eV, 2.53 * eV, 2.38 * eV, 2.30 * eV, 0 * eV};

      G4double CsI_SCINT[NUMENTRIES]
          = {0.0,  0.0, 0.1, 0.2,  0.4,  0.65, 0.8, 0.95,
             0.82, 0.7, 0.5, 0.21, 0.05, 0,    0};

      G4double CsI_RIND[NUMENTRIES]
          = {1.505, 1.505, 1.505, 1.505, 1.505, 1.505, 1.505, 1.505,
             1.505, 1.505, 1.505, 1.505, 1.505, 1.505, 1.505};
      G4double CsI_ABSL[NUMENTRIES]
          = {1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m,
             1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m,
             1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m, 1.5 * m};

      G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
      myMPT1->AddProperty("RINDEX", CsI_PP, CsI_RIND, NUMENTRIES); /// Constant?
      myMPT1->AddProperty("ABSLENGTH", CsI_PP, CsI_ABSL,
                          NUMENTRIES); // Constant?
      myMPT1->AddProperty("FASTCOMPONENT", CsI_PP, CsI_SCINT,
                          NUMENTRIES); // Spectrum
      myMPT1->AddProperty("SLOWCOMPONENT", CsI_PP, CsI_SCINT,
                          NUMENTRIES); // Spectrum

      myMPT1->AddConstProperty("SCINTILLATIONYIELD", 13000. / MeV);
      myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
      myMPT1->AddConstProperty("FASTTIMECONSTANT", 10.3 * ns); // Decay time
      myMPT1->AddConstProperty("SLOWTIMECONSTANT", 220 * ns);
      myMPT1->AddConstProperty("YIELDRATIO", 0.8);

      material->SetMaterialPropertiesTable(myMPT1);

      m_Material[Name] = material;
      return material;
    }

    else {
      G4cout << "ERROR: Material requested \"" << Name
             << "\" is not available in the Material Library, trying with NIST"
             << G4endl;
      G4NistManager* man = G4NistManager::Instance();
      return man->FindOrBuildMaterial(Name.c_str());
    }
  }

  else
    return it->second;

  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::AddMaterialToLibrary(G4Material* material) {
  m_Material[material->GetName()] = material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Element* MaterialManager::GetElementFromLibrary(string Name) {
  if (Name == "D" || Name == "d") {
    if (!m_D) {
      m_D = new G4Element(Name.c_str(), Name.c_str(), 1);
      G4Isotope* isotope
          = new G4Isotope(Name.c_str(), 1, 2, 2.01410178 * g / mole);
      m_D->AddIsotope(isotope, 1);
    }
    return m_D;
  }

  else if (Name == "T" || Name == "t") {
    if (!m_T) {
      m_T = new G4Element(Name.c_str(), Name.c_str(), 1);
      G4Isotope* isotope
          = new G4Isotope(Name.c_str(), 1, 3, 3.0160492 * g / mole);
      m_T->AddIsotope(isotope, 1);
    }
    return m_T;
  }

  else if (Name == "He3" || Name == "3He") {
    if (!m_He3) {
      m_He3 = new G4Element(Name.c_str(), Name.c_str(), 1);
      G4Isotope* isotope
          = new G4Isotope(Name.c_str(), 2, 1, 3.0160293 * g / mole);
      m_He3->AddIsotope(isotope, 1);
    }
    return m_He3;
  }

  G4NistManager* man = G4NistManager::Instance();
  return man->FindOrBuildElement(Name.c_str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//   
G4Material* MaterialManager::GetGasFromLibrary(string Name, double Pressure, double Temperature){
    ostringstream oss;
    oss << Name<< "_"<<Pressure<<"_"<<Temperature;
    string newName= oss.str();
    map<string,G4Material*>::iterator it;
    it = m_Material.find(Name);

    double density = 0 ;
    
    G4double Vm=0.08206*Temperature*atmosphere/(Pressure*kelvin);
    
    // The element is not found
    if(it==m_Material.end()){
        if(Name == "CF4"){ // 52 torr
            density =  3.72*kg/m3;
            double refTemp= (273.15+15)*kelvin;
            double refPres= 1.01325*bar;
            density = density*(refTemp/Temperature)/(refPres/Pressure);
            G4Material* material = new G4Material("NPS_"+newName,density,2,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("C"), 1);
            material->AddElement(GetElementFromLibrary("F"), 4);
            m_Material[newName]=material;
            return material;
        }

        if(Name == "He"){
            density =  (4.0026/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,1,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("He"), 1);
            m_Material[newName]=material;
            return material;
        }
        
        if(Name == "iC4H10" || Name == "Isobutane" || Name == "isobutane"){
            density	= ((4*12.0107+10*1.00794)/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,2,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("C"), 4);
            material->AddElement(GetElementFromLibrary("H"), 10);
            m_Material[newName]=material;
            return material;
        }
        
        if(Name == "CH4"){
            density	= ((12.0107+4*1.00794)/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,2,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("C"), 1);
            material->AddElement(GetElementFromLibrary("H"), 4);
            m_Material[newName]=material;
            return material;
        }

        if(Name == "CO2"){
            density	= ((12.0107+2*16)/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,2,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("C"), 1);
            material->AddElement(GetElementFromLibrary("O"), 2);
            m_Material[newName]=material;
            return material;
        }

        if(Name == "H2"){
            density	= (2*1.00794/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,1,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("H"), 2);
            //material->AddElement(GetElementFromLibrary("H"), 1);
            m_Material[newName]=material;
            return material;
        }
        
        if(Name == "D2"){
            density	= (2*2.0140/Vm)*mg/cm3;
            G4Material* material = new G4Material("NPS_"+newName,density,1,kStateGas,Temperature,Pressure);
            material->AddElement(GetElementFromLibrary("D"), 2);
            //material->AddElement(GetElementFromLibrary("D"), 1);
            m_Material[newName]=material;
            return material;
        }
        

        else{
          exit(1);
        }
     }
  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//   Generate a DEDX file table using the material used in the geometry
void MaterialManager::WriteDEDXTable(G4ParticleDefinition* Particle,
                                     G4double Emin, G4double Emax) {
  map<string, G4Material*>::iterator it;
  if (Particle->GetPDGCharge() == 0)
    return;
  for (it = m_Material.begin(); it != m_Material.end(); it++) {
    //   Opening hte output file
    G4String GlobalPath = getenv("NPTOOL");
    G4String Name       = it->second->GetName();
    // Remove NPS name
    Name.erase(0, 4);
    G4String Path = GlobalPath + "/Inputs/EnergyLoss/"
                    + Particle->GetParticleName() + "_" + Name + ".G4table";

    ofstream File;
    File.open(Path);

    if (!File)
      return;

    File << "Table from Geant4 generate using NPSimulation \t"
         << "Particle: " << Particle->GetParticleName()
         << "\tMaterial: " << it->second->GetName() << G4endl;
    // G4cout <<  Particle->GetParticleName() << "\tMaterial: " <<
    // it->second->GetName()  <<endl;

    G4EmCalculator emCalculator;
    G4double       dedx;
    // Tipical Range needed, if Emax is larger, then adapted
    if (Emax < 1 * TeV)
      Emax        = 1 * TeV;
    double step   = 1 * keV;
    double before = 0;

    for (G4double E = Emin; E < Emax; E += step) {
      dedx = emCalculator.ComputeTotalDEDX(E, Particle, it->second)
             / (MeV / micrometer);
      if (before) {
        if (abs(before - dedx) / abs(before) < 0.01)
          step *= 2;
      }

      before = dedx;
      File << E / MeV << "\t" << dedx << G4endl;
    }

    File.close();
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//   Generate a DEDX file table using the material used in the geometry
void MaterialManager::WriteDEDXTable(std::set<string> Particle, G4double Emin,
                                     G4double Emax) {
  std::set<string>::iterator it;
  for (it = Particle.begin(); it != Particle.end(); it++) {
    G4ParticleDefinition* p
        = G4ParticleTable::GetParticleTable()->FindParticle((*it));
    WriteDEDXTable(p, Emin, Emax);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::CreateSampleVolumes(G4LogicalVolume* world_log) {

  // Crate a micrometer big cube for each material
  G4double SampleSize = 1 * um;
  G4double WorldSize  = 10.0 * m;
  G4Box*   sample_box
      = new G4Box("sample_box", SampleSize, SampleSize, SampleSize);
  G4int    i      = 1;
  G4double Coord1 = WorldSize - SampleSize;
  G4double Coord2 = 0;
  map<string, G4Material*>::iterator it;
  for (it = m_Material.begin(); it != m_Material.end(); it++) {
    G4LogicalVolume* sample_log
        = new G4LogicalVolume(sample_box, it->second, "sample_log", 0, 0, 0);
    sample_log->SetVisAttributes(G4VisAttributes::Invisible);
    Coord2 = WorldSize - i * 2 * SampleSize;
    i++;
    new G4PVPlacement(0, G4ThreeVector(Coord1, Coord2, -Coord1), sample_log,
                      "sample", world_log, false, 0);
  }
}
