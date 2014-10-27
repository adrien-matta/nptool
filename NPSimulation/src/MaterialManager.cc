/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
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
#include "G4EmCalculator.hh"

// STL
#include<iostream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager* MaterialManager::instance = 0 ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager* MaterialManager::getInstance(){
  if(instance == 0) instance = new MaterialManager();
  return instance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager::~MaterialManager(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MaterialManager::MaterialManager(){

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::Destroy(){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
G4Material* MaterialManager::GetMaterialFromLibrary(string Name){
  // Search if the material is already instantiate  
  map<string,G4Material*>::iterator it;
  it = m_Material.find(Name);

  // The element is not found
  if(it==m_Material.end()){

    //Usual compound
    if(Name == "Vacuum"){
      G4Material* material = new G4Material(Name, 0.000000001 * mg / cm3,2);
      material->AddElement(GetElementFromLibrary("N"),7);
      material->AddElement(GetElementFromLibrary("O"),3);
      m_Material[Name]=material;
      return material; 
    }

    else if(Name == "PCB"){
      // Actually taken value fron Epoxy
      G4Material* material = new G4Material(Name, 1.85 * g / cm3,3);
      material->AddElement(GetElementFromLibrary("H"),.475);
      material->AddElement(GetElementFromLibrary("C"),.45);
      material->AddElement(GetElementFromLibrary("O"),.075);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Mylar"){
      G4Material* material = new G4Material(Name, 1.397*g,3);
      material->AddElement(GetElementFromLibrary("H"),8);
      material->AddElement(GetElementFromLibrary("C"),10);
      material->AddElement(GetElementFromLibrary("O"),4);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Kapton"){
      G4Material* material = new G4Material(Name, 1.39*g,3);
      material->AddElement(GetElementFromLibrary("H"),4);
      material->AddElement(GetElementFromLibrary("C"),5);
      material->AddElement(GetElementFromLibrary("O"),2);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Havar"){
      G4Material* material = new G4Material(Name, 8.3*g / cm3,5);
      material->AddElement(GetElementFromLibrary("Co"),42);
      material->AddElement(GetElementFromLibrary("Cr"),20);
      material->AddElement(GetElementFromLibrary("Ni"),13);
      material->AddElement(GetElementFromLibrary("Fe"),19);
      material->AddElement(GetElementFromLibrary("W"),1);
      m_Material[Name]=material;
      return material; 
    }
    
    // Usual Target
     else if(Name == "CD2"){
      G4Material* material = new G4Material(Name, 1.06*g/cm3,2);
      material->AddElement(GetElementFromLibrary("C"),1);
      material->AddElement(GetElementFromLibrary("D"),2);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "CH2"){
      G4Material* material = new G4Material(Name, 0.93*g/cm3,2);
      material->AddElement(GetElementFromLibrary("C"),1);
      material->AddElement(GetElementFromLibrary("D"),2);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Cu"){
      G4Material* material = new G4Material(Name, 8.96*g/cm3,1);
      material->AddElement(GetElementFromLibrary("Cu"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "C"){ // Graphite
      G4Material* material = new G4Material(Name, 2.267*g/cm3,1);
      material->AddElement(GetElementFromLibrary("C"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Pb"){
      G4Material* material = new G4Material(Name, 11.342*g / cm3,1);
      material->AddElement(GetElementFromLibrary("Pb"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "D2_solid"){ // density should be checked
      G4Material* material = new G4Material(Name, 0.0715*g/cm3,1);
      material->AddElement(GetElementFromLibrary("D"),2);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "H2_solid"){
      G4Material* material = new G4Material(Name, 0.0715*g/cm3,1);
      material->AddElement(GetElementFromLibrary("H"),2);
      m_Material[Name]=material;
      return material; 
    }
    
    // Usual detector material
    else  if(Name == "Si"){
      G4Material* material = new G4Material(Name,2.321*g/cm3 ,1);
      material->AddElement(GetElementFromLibrary("Si"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Ge"){
      G4Material* material = new G4Material(Name,5.323*g/cm3 ,1);
      material->AddElement(GetElementFromLibrary("Ge"),1);
      m_Material[Name]=material;
      return material; 
    }
   
    else  if(Name == "BC400"){
      G4Material* material = new G4Material(Name,1.032*g/cm3 ,2);
      material->AddElement(GetElementFromLibrary("H"),10);
      material->AddElement(GetElementFromLibrary("C"),9);
      m_Material[Name]=material;
      return material; 
    }
    
    else  if(Name == "NaI"){
      G4Material* material = new G4Material(Name, 3.67*g/cm3,2);
      material->AddElement(GetElementFromLibrary("Na"),1);
      material->AddElement(GetElementFromLibrary("I"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "CsI"){
      G4Material* material = new G4Material(Name, 4.51*g/cm3,2);
      material->AddElement(GetElementFromLibrary("Cs"),1);
      material->AddElement(GetElementFromLibrary("I"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "LaBr3"){
      G4Material* material = new G4Material(Name, 5.29*g/cm3 , 2);
      material->AddElement(GetElementFromLibrary("La"),1);
      material->AddElement(GetElementFromLibrary("Br"),3);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "BaF2"){
      G4Material* material = new G4Material(Name, 4.89*g/cm3 , 2);
      material->AddElement(GetElementFromLibrary("Ba"),1);
      material->AddElement(GetElementFromLibrary("F"),2);
      m_Material[Name]=material;
      return material; 
    }

    // Misc
    else  if(Name == "Al"){
      G4Material* material = new G4Material(Name, 2.702*g/cm3,1);
      material->AddElement(GetElementFromLibrary("Al"),1);
      m_Material[Name]=material;
      return material; 
    }

    else  if(Name == "Fe"){
      G4Material* material = new G4Material(Name, 7.874*g/cm3,1);
      material->AddElement(GetElementFromLibrary("Fe"),1);
      m_Material[Name]=material;
      return material; 
    }

    else{
      cout << "ERROR: Material requested \""<< Name <<"\" is not available in the Material Librairy" << endl;
      exit(1);
    }

  }

  else return it->second;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MaterialManager::AddMaterialToLibrary(G4Material* material){
  m_Material[material->GetName()]=material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Element* MaterialManager::GetElementFromLibrary(string Name){
  // Search if the element is already instantiate  
  map<string,G4Element*>::iterator it;
  it = m_Element.find(Name);

  // The element is not found
  if(it==m_Element.end()){
    if(Name == "H"){
      G4Element* element = new G4Element(Name,Name,1, 1.008*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "D"){
      G4Element* element = new G4Element(Name,Name,1, 2.01410178*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "T"){
      G4Element* element = new G4Element(Name,Name,1, 3.0160492*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "C"){
      G4Element* element = new G4Element(Name,Name,6, 12.011*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "O"){
      G4Element* element = new G4Element(Name,Name,8, 15.999*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "N"){
      G4Element* element = new G4Element(Name,Name,7, 14.007*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Si"){
      G4Element* element = new G4Element(Name,Name,14, 28.085*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Al"){
      G4Element* element = new G4Element(Name,Name,13, 26.9815385*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Co"){
      G4Element* element = new G4Element(Name,Name,27, 58.933194*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Cr"){
      G4Element* element = new G4Element(Name,Name,24, 51.9961*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Ni"){
      G4Element* element = new G4Element(Name,Name,28, 58.6934*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Fe"){
      G4Element* element = new G4Element(Name,Name,26, 55.845*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "W"){
      G4Element* element = new G4Element(Name,Name,74, 183.84*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Pb"){
      G4Element* element = new G4Element(Name,Name,82, 207.2*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Ge"){
      G4Element* element = new G4Element(Name,Name,32, 72.630*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Na"){
      G4Element* element = new G4Element(Name,Name,11, 22.98976928*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "I"){
      G4Element* element = new G4Element(Name,Name,53, 126.90447*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "La"){
      G4Element* element = new G4Element(Name,Name,57, 138.90547*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Br"){
      G4Element* element = new G4Element(Name,Name,35, 79.904*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Bi"){
      G4Element* element = new G4Element(Name,Name,83, 208.98040*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Ba"){
      G4Element* element = new G4Element(Name,Name,56, 137.327*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "F"){
      G4Element* element = new G4Element(Name,Name,9, 18.998403163*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Cs"){
      G4Element* element = new G4Element(Name,Name,55, 132.90545196*g/mole);
      m_Element[Name] = element;
      return element;
    }

    else if(Name == "Cu"){
      G4Element* element = new G4Element(Name,Name,29, 63.546*g/mole);
      m_Element[Name] = element;
      return element;
    }


    else{
      cout << "ERROR: Element requested \""<< Name <<"\" is not available in the Material Librairy" << endl;
      exit(1);
    }


  }

  else return it->second;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//   Generate a DEDX file table using the material used in the geometry
void MaterialManager::WriteDEDXTable(G4ParticleDefinition* Particle ,G4double Emin,G4double Emax){
  map<string,G4Material*>::iterator it;
  for(it = m_Material.begin() ; it != m_Material.end() ; it++){
    //   Opening hte output file
    G4String GlobalPath = getenv("NPTOOL");
    G4String Path = GlobalPath + "/Inputs/EnergyLoss/" + Particle->GetParticleName() + "_" + it->second->GetName() + ".G4table";
  
    ofstream File ;
    File.open(Path) ;

    if(!File) return ;

    File   << "Table from Geant4 generate using NPSimulation \t"
      << "Particle: " << Particle->GetParticleName() << "\tMaterial: " << it->second->GetName() << endl ;
    // cout <<  Particle->GetParticleName() << "\tMaterial: " << it->second->GetName()  <<endl;
    
    G4EmCalculator emCalculator;
    G4double dedx ;
    // Tipical Range needed, if Emax is larger, then adapted
    if(Emax < 1*TeV) Emax = 1*TeV;    
    double step = 1*keV;
    double before = 0 ;
    
    for (G4double E=Emin; E < Emax; E+=step){
      dedx = emCalculator.ComputeTotalDEDX(E, Particle, it->second)/(MeV/micrometer);
      if(before){
        if(abs(before-dedx)/abs(before)<0.01) step*=10; 
      }

      before = dedx;
      File << E/MeV << "\t" << dedx << endl ;
    }

    File.close();
  }
}

