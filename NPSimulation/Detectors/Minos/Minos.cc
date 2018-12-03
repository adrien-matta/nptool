/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: tronchin@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : October 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Minos simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Tubs.hh"
#include "G4Box.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// NPTool header
#include "Minos.hh"
#include "CalorimeterScorers.hh"
#include "InteractionScorers.hh"
#include "TPCScorers.hh"

#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "NPSHitsMap.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Minos_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 1.0*MeV ;
  const double Radius = 50*mm ; 
  const double Width = 100*mm ;
  const double Thickness = 300*mm ;
  const string Material = "BC400";

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Minos Specific Method
Minos::Minos(){
  m_Event = new TMinosData() ;
  m_MinosTargetScorer = 0;
  m_MinosTPCScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;
  m_ReactionRegion=NULL;

  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   

}

Minos::~Minos(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Minos::AddDetector(G4ThreeVector POS, string  Shape){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Minos::AddDetector(double  R, double  Theta, double  Phi, string  Shape){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// G4LogicalVolume* Minos::BuildSquareDetector(){
//   if(!m_SquareDetector){
//     G4Box* box = new G4Box("Minos_Box",Minos_NS::Width*0.5,
//         Minos_NS::Width*0.5,Minos_NS::Thickness*0.5);

//     G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Minos_NS::Material);
//     m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_Minos_Box",0,0,0);
//     m_SquareDetector->SetVisAttributes(m_VisSquare);
//     m_SquareDetector->SetSensitiveDetector(m_MinosTargetScorer);
//   }
//   return m_SquareDetector;
// }



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Minos::DefineMaterials()
{ 
  //This function illustrates the possible ways to define materials
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  
  G4int ncomponents, natoms;
  G4double fractionmass;
  
  //
  // define Elements
  //

  
  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* F  = new G4Element("Fluorin"  ,symbol="F" , z= 9., a= 19.0*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  G4Element* Ar = new G4Element("Argon", symbol="Ar", z=18, a=39.948*g/mole);
  G4Element* Fe = new G4Element("Fer",symbol="Fe" , z= 26., a= 55.9*g/mole);
  G4Element* Cr = new G4Element("Chrome",symbol="Cr" , z= 24., a= 52.*g/mole);
  
  
  new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
  new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);
  new G4Material("Silicium", z=14., a=28.09*g/mole, density=2.330*g/cm3);
  new G4Material("Titanium", z=22., a=47.87*g/mole, density=4.510*g/cm3);
  

  
  G4Material* iso = new G4Material("isobutane", density=0.002506*g/cm3, ncomponents=2);
  iso->AddElement(C, natoms=4);
  iso->AddElement(H, natoms=10);
  G4Material* CF4 = 
    new G4Material("CF4", density= 0.0036586*g/cm3, ncomponents=2);
  CF4->AddElement(C, natoms=1);
  CF4->AddElement(F, natoms=4);
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  CF4->GetIonisation()->SetMeanExcitationEnergy(20.0*eV);
  G4Material* mix = 
    new G4Material("mix", density= 0.0019836*g/cm3, ncomponents=3);
  mix->AddMaterial(CF4, fractionmass=15.*perCent);
  mix->AddMaterial(iso, fractionmass=3.*perCent);
  mix->AddElement(Ar, fractionmass=82.*perCent);
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  mix->GetIonisation()->SetMeanExcitationEnergy(25.0*eV);
  
  G4Material* Ar_CF4_95_5 = 
    new G4Material("Ar_CF4_95_5", density= 0.0017611*g/cm3, ncomponents=2);
  Ar_CF4_95_5->AddMaterial(CF4, fractionmass=5.*perCent);
  Ar_CF4_95_5->AddElement(Ar, fractionmass=95.*perCent);
  Ar_CF4_95_5->GetIonisation()->SetMeanExcitationEnergy(30.0*eV);
  
  G4Material* Ar_CF4_90_10 = 
    new G4Material("Ar_CF4_90_10", density= 0.0018610*g/cm3, ncomponents=2);
  Ar_CF4_90_10->AddMaterial(CF4, fractionmass=10.*perCent);
  Ar_CF4_90_10->AddElement(Ar, fractionmass=90.*perCent);
  Ar_CF4_90_10->GetIonisation()->SetMeanExcitationEnergy(25.0*eV);
  
  G4Material* Ar_iso_97_3 = 
    new G4Material("Ar_iso_97_3", density= 0.0016838*g/cm3, ncomponents=2);
  Ar_iso_97_3->AddMaterial(iso, fractionmass=3.*perCent);
  Ar_iso_97_3->AddElement(Ar, fractionmass=97.*perCent);
  Ar_iso_97_3->GetIonisation()->SetMeanExcitationEnergy(25.0*eV);
  
  G4Material* Ar_iso_95_5 = 
    new G4Material("Ar_iso_95_5", density= 0.0016990*g/cm3, ncomponents=2);
  Ar_iso_95_5->AddMaterial(iso, fractionmass=5.*perCent);
  Ar_iso_95_5->AddElement(Ar, fractionmass=95.*perCent);
  Ar_iso_95_5->GetIonisation()->SetMeanExcitationEnergy(25.0*eV);
  
  G4Material* LH2 = 
    new G4Material("LH2", density= 0.0715*g/cm3, ncomponents=1);
  LH2->AddElement(H, natoms=2);
  
  G4Material* Myl = 
    new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
  Myl->AddElement(C, natoms=10);
  Myl->AddElement(H, natoms= 8);
  Myl->AddElement(O, natoms= 4);

  G4Material* Epo = 
    new G4Material("Epoxy", density= 1.85*g/cm3, ncomponents=3); //density of FR4 (Wikipedia)
  Epo->AddElement(C, natoms=18);
  Epo->AddElement(H, natoms= 20);
  Epo->AddElement(O, natoms= 3);
  
  G4Material* Air = 
    new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=3);
  Air->AddElement(N, fractionmass=0.781);
  Air->AddElement(O, fractionmass=0.21);
  Air->AddElement(Ar, fractionmass=0.009);
  
  
  G4Material* Vacuum =
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                   kStateGas, 2.73*kelvin, 3.e-18*pascal);
  
  G4Material* beam = 
    new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                   kStateGas, STP_Temperature, 2.e-2*bar);
  beam->AddMaterial(Air, fractionmass=1.);
  
  G4Material* Inox = 
    new G4Material("Inox", density= 8.02*g/cm3, ncomponents=3);
  Inox->AddElement(C, fractionmass=0.001);
  Inox->AddElement(Fe, fractionmass=0.829);
  Inox->AddElement(Cr, fractionmass=0.17);
  
  G4Material* Kapton = 
    new G4Material("Kapton", density= 1.42*g/cm3, ncomponents=4);
  Kapton->AddElement(C, fractionmass=0.691133);
  Kapton->AddElement(H, fractionmass=0.026362);
  Kapton->AddElement(O, fractionmass=0.209235);
  Kapton->AddElement(N, fractionmass=0.073270);
  
  G4Material* Rohacell = 
    new G4Material("Rohacell", density= 0.075*g/cm3, ncomponents=4);
  Rohacell->AddElement(C, fractionmass=0.6014);
  Rohacell->AddElement(H, fractionmass=0.0805);
  Rohacell->AddElement(O, fractionmass=0.3154);
  Rohacell->AddElement(N, fractionmass=0.00276);
  
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  //default materials of the World
  defaultMaterial  = Vacuum;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//SET MATERIALS
void Minos::SetTargetMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) TargetMaterial = pttoMaterial;
}
void Minos::SetChamberMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) ChamberMaterial = pttoMaterial;
}

void Minos::SetTPCMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) TPCMaterial = pttoMaterial;
}

void Minos::SetWindowMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) WindowMaterial = pttoMaterial;
}
void Minos::SetInnerRohacellMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) InnerRohacellMaterial = pttoMaterial;
}
void Minos::SetOuterRohacellMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) OuterRohacellMaterial = pttoMaterial;
}
void Minos::SetKaptonMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) KaptonMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildCylindricalDetector(){
  if(!m_CylindricalDetector){
    /*
    G4Tubs* tub = new G4Tubs("Minos_Cyl",0,Minos_NS::Radius,Minos_NS::Thickness*0.5,0,360*deg);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Minos_NS::Material);
    m_CylindricalDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_Minos_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_MinosScorer);
    */

  
 }
  return m_CylindricalDetector;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildTarget(){
  if(!logicTarget){
    //                               
    // Target
    //  
    solidTarget=0; logicTarget=0; /*physiTarget=0;*/
    solidChamber=0; logicChamber=0; /*physiChamber=0;*/
    solidTPC=0; logicTPC=0; /*physiTPC=0;*/
    
    solidTarget = new G4Tubs("Target",		//its name
                             0.,TargetRadius,TargetLength,0,360.);//size
    
    logicTarget = new G4LogicalVolume(solidTarget,	//its solid
                                      TargetMaterial,	//its material
                                      "Target");	//its name    
    
    
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.6,1.,1.));
      atb->SetForceSolid(true);
      logicTarget->SetVisAttributes(atb);}
    logicTarget->SetSensitiveDetector(m_MinosTargetScorer);
    
    
  }
  return logicTarget;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildChamber(){
  if(!logicChamber){
//                                 
  // Chamber
  //
      solidChamber = new G4Tubs("Chamber",			//its name
                       ChamberInnerRadius,ChamberInnerRadius+ChamberThickness,ChamberLength,0,360.); //size
                       
      logicChamber = new G4LogicalVolume(solidChamber,	//its solid
                                       ChamberMaterial,	//its material
                                       "Chamber");	//its name                               
  
{G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(0,1,0));
  simpleBoxVisAtt->SetVisibility(true);
  logicChamber->SetVisAttributes(simpleBoxVisAtt);}

   }
  return logicChamber;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildInnerRohacell(){
  if(!logicInnerRohacell){
    //                                 
  // Inner Rohacell
  //
      solidInnerRohacell = new G4Tubs("InnerRohacell",			//its name
                                      ChamberInnerRadius /*+ ChamberThickness*/,ChamberInnerRadius + ChamberThickness+InnerRohacellThickness,ChamberLength,0,360.); //size
                       
      logicInnerRohacell = new G4LogicalVolume(solidInnerRohacell,	//its solid
                                       InnerRohacellMaterial,	//its material
                                       "InnerRohacell");	//its name
  
 


  }
  return logicInnerRohacell;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildOuterRohacell(){
  if(!logicOuterRohacell){
//                                 
  // Outer Rohacell
  //
      solidOuterRohacell = new G4Tubs("OuterRohacell",			//its name
                                      ChamberInnerRadius /*+ ChamberThickness + InnerRohacellThickness + KaptonThickness*/,ChamberInnerRadius + ChamberThickness + InnerRohacellThickness + KaptonThickness+OuterRohacellThickness,ChamberLength,0,360.); //size
                       
      logicOuterRohacell = new G4LogicalVolume(solidOuterRohacell,	//its solid
                                       OuterRohacellMaterial,	//its material
                                       "OuterRohacell");	//its name
  }
  return logicOuterRohacell;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildKapton(){
  if(!logicKapton){
    //                                 
  // Kapton
  //
      solidKapton = new G4Tubs("Kapton",			//its name
                               ChamberInnerRadius+ ChamberThickness +InnerRohacellThickness ,ChamberInnerRadius + ChamberThickness+InnerRohacellThickness+KaptonThickness,ChamberLength,0,360.); //size
                       
      logicKapton = new G4LogicalVolume(solidKapton,	//its solid
                                       KaptonMaterial,	//its material
                                       "Kapton");	//its name
  }
  return logicKapton;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildTPC(){
  if(!logicTPC){
 //                               
  // TPC
  
      solidTPC = new G4Tubs("TPC",		//its name
                            ChamberInnerRadius /*+ ChamberThickness + InnerRohacellThickness + KaptonThickness + OuterRohacellThickness*/,TPCRadiusExt,ChamberLength,0,360.); 
                          
      logicTPC = new G4LogicalVolume(solidTPC,    //its solid
      			                  TPCMaterial, //its material
      			                  "TPC"); //name

 
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.,1.,0.6));
   logicTPC->SetVisAttributes(atb);}
  // logicTPC->SetSensitiveDetector(m_MinosTPCScorer);


   }
  return logicTPC;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 //                               
  // windows
  //
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildWindow0(){
  if(!logicWindow0){
solidWindow0 = new G4Tubs("WindowTube",		//its name
                          0./*TargetRadius*/,TargetRadius+WindowThickness*2.,TargetLength+WindowThickness*2.,0,360.);
                          
      logicWindow0 = new G4LogicalVolume(solidWindow0,    //its solid
      			                  WindowMaterial, //its material
      			                  "WindowTube"); //name
  
   {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0,0,1));
  atb->SetForceSolid(true);
  logicWindow0->SetVisAttributes(atb);}



   }
  return logicWindow0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildWindow1(){
  if(!logicWindow1){
   solidWindow1 = new G4Tubs("WindowEntrance",		//its name
                          0.,TargetRadius+2.*WindowThickness,WindowThickness,0,360.); 
                          
      logicWindow1 = new G4LogicalVolume(solidWindow1,    //its solid
      			                  WindowMaterial, //its material
      			                  "WindowEntrance"); //name
   
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0,0,1));
  atb->SetForceSolid(true);
  logicWindow1->SetVisAttributes(atb);} 


   }
  return logicWindow1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Minos::BuildWindow2(){
  if(!logicWindow2){
solidWindow2 = new G4Tubs("WindowOutcoming",		//its name
                          0.,TargetRadius+2.*WindowThickness,WindowThickness,0,360.); 
                          
      logicWindow2 = new G4LogicalVolume(solidWindow2,    //its solid
      			                  WindowMaterial, //its material
      			                  "WindowOutcoming"); //name
  
   {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0,0,1));
  atb->SetForceSolid(true);
  logicWindow2->SetVisAttributes(atb);} 
  
  }
  return logicWindow2;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Minos::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Minos");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Minos " << i+1 <<  endl;
    
      G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(Pos,Shape);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Minos " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(R,Theta,Phi,Shape);
    }
    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Minos::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

  TargetRadius = 28.*mm; TargetLength = 150./2.*mm;
  ChamberInnerRadius = 37.*mm; ChamberThickness = 1.*mm; 
  ChamberLength = 300./2.*mm;
  InnerRohacellThickness = 1.*mm; KaptonThickness = 0.125*mm; OuterRohacellThickness = 2.*mm;
  TPCRadiusExt = 100.*mm; WindowThickness = 0.150/2.*mm;
    
    DefineMaterials();

    SetTargetMaterial("LH2");  
    SetChamberMaterial("Inox");
    SetTPCMaterial("mix"); 
    SetWindowMaterial("Mylar");  
    SetKaptonMaterial("Kapton");  
    SetInnerRohacellMaterial("Rohacell");
    SetOuterRohacellMaterial("Rohacell");
     
    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
              Det_pos+=Det_pos.unit()*Minos_NS::Thickness*0.5;       
    // Building Detector reference frame
    G4double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
    G4double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
    G4double kk = -sin(m_Theta[i]);
    G4ThreeVector Y(ii,jj,kk);
    G4ThreeVector w = Det_pos.unit();
    G4ThreeVector u = w.cross(Y);
    G4ThreeVector v = w.cross(u);
    v = v.unit();
    u = u.unit();

    G4RotationMatrix* Rot = new G4RotationMatrix(u,v,w);

    // if(m_Shape[i] == "Cylindrical"){
    //   // new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
    //   //  BuildCylindricalDetector(),
    //   //  "Minos",world,false,i+1);
    // }
    // else if(m_Shape[i] == "Square"){
          

    // }

    new G4PVPlacement(0,//its name
                      G4ThreeVector(wX,wY, wZ/* ChamberLength*/),	//at (0,0,0)
                      BuildTPC(),	//its logical volume
                      "TPC",	//its name
                      world,	//its mother  volume
                      false,		//no boolean operation
                      0);		//copy number

    new G4PVPlacement(0,		//its name
                       G4ThreeVector(0,0,0/*ChamberLength*/),	//at (0,0,0)
                       BuildOuterRohacell(),	//its logical volume
                      "Rohacell"/*"OuterRohacell"*/,	//its name
                       logicTPC/*world*/,	//its mother  volume
                       false,		//no boolean operation
                       0);		//copy number
    new G4PVPlacement(0,//its name
                      G4ThreeVector(0,0,0/*ChamberLength*/),	//at (0,0,0)
                      BuildChamber(),	//its logical volume
                      "Chamber",	//its name
                      logicOuterRohacell,	//its mother  volume
                      false,		//no boolean operation
                      0);		//copy number

   /*new G4PVPlacement(0,		//its name
                      G4ThreeVector(0,0,ChamberLength),	//at (0,0,0)
                      BuildInnerRohacell(),	//its logical volume
                      "InnerRohacell",	//its name
                      world,	//its mother  volume
                      false,		//no boolean operation
                      0);*/   
 
    
     new G4PVPlacement(0,		//its name
                       G4ThreeVector(0,0,0/*ChamberLength*/),	//at (0,0,0)
                       BuildKapton(),	//its logical volume
                       "Kapton",	//its name
                       logicOuterRohacell,	//its mother  volume
                       false,		//no boolean operation
                       0);		//copy number


    new G4PVPlacement(0,		//its name
                      G4ThreeVector(wX,wY, wZ-1.*TargetLength+10*mm),	//at (0,0,0)
                      BuildWindow0(),	//its logical volume
                      "WindowTube",	//its name
                      world,	//its mother  volume
                      false,		//no boolean operation
                      0);
    /*new G4PVPlacement(0,		//its name
                                             G4ThreeVector(0,0, -1.*(TargetLength+WindowThickness)),	//at (0,0,0)
                                             BuildWindow1(),	//its logical volume
                                             "WindowEntrance",	//its name
                                             logicWindow0,	//its mother  volume
                                             false,		//no boolean operation
                                             0);  new G4PVPlacement(0,		//its name
                                                                    G4ThreeVector(0,0, (TargetLength+WindowThickness)),	//at (0,0,0)
                                                                    BuildWindow2(),	//its logical volume
                                                                    "WindowOutcoming",	//its name
                                                                    logicWindow0,	//its mother  volume
                                                                    false,		//no boolean operation
                                                                    0);		//copy number
    */                    
     
    new G4PVPlacement(0,//no rotation
                      G4ThreeVector(0,0,0/*TargetLength*/),	//at (0,0,0)
                      BuildTarget(),	//its logical volume
                      "Target",	//its name
                      logicWindow0,	//its mother  volume
                      false,		//no boolean operation
                      0);		//copy number
    
     
    // G4ProductionCuts* ecut = new G4ProductionCuts();
    //G4ProductionCuts* pcut = new G4ProductionCuts();
    if(!m_ReactionRegion){
      
      
        //    ecut->SetProductionCut(1000,"e-");
        //  pcut->SetProductionCut(1,"p");
      
      m_ReactionRegion= new G4Region("NPSimulationProcess");
      m_ReactionRegion -> AddRootLogicalVolume(logicTarget);
      
      // logicTPC -> SetRegion(m_ReactionRegion);
      //m_ReactionRegion->SetProductionCuts(ecut);
      //m_ReactionRegion->SetProductionCuts(ecut);     
      // m_ReactionRegion -> AddRootLogicalVolume(logicTPC);
      
      m_ReactionRegion->SetUserLimits(new G4UserLimits(1.2*mm)); //???
    }
    
    G4FastSimulationManager* mng = m_ReactionRegion->GetFastSimulationManager();
    unsigned int size = m_ReactionModel.size();
    for(unsigned int o = 0 ; o < size ; o++){
      mng->RemoveFastSimulationModel(m_ReactionModel[o]);
    }
    m_ReactionModel.clear();
    G4VFastSimulationModel* fsm;
    fsm = new NPS::BeamReaction("BeamReaction",m_ReactionRegion);
    ((NPS::BeamReaction*) fsm)->SetStepSize(1.2*mm);
    m_ReactionModel.push_back(fsm);
    fsm = new NPS::Decay("Decay",m_ReactionRegion);
    m_ReactionModel.push_back(fsm);
    
    // G4Region* Region_cut = new G4Region("RegionCut");
    // logicTPC->SetRegion(Region_cut);
    // Region_cut->SetProductionCuts(ecut);
    // Region_cut->SetProductionCuts(pcut);
    // Region_cut->AddRootLogicalVolume(logicTPC);                          
    
  }
  //                                        
  // Visualization attributes
  //
  world->SetVisAttributes (G4VisAttributes::Invisible);
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Minos::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Minos")){
    pTree->Branch("Minos", "TMinosData", &m_Event) ;
  }
  pTree->SetBranchAddress("Minos", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void Minos::ReadSensitive(const G4Event* ){
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  CalorimeterScorers::PS_Calorimeter* Scorer= (CalorimeterScorers::PS_Calorimeter*) m_MinosTargetScorer->GetPrimitive(0);

  
  unsigned int size = Scorer->GetMult(); 
  for(unsigned int i = 0 ; i < size ; i++){
    vector<unsigned int> level = Scorer->GetLevel(i); 
    double Energy = RandGauss::shoot(Scorer->GetEnergy(i),Minos_NS::ResoEnergy);
    if(Energy>Minos_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Scorer->GetTime(i),Minos_NS::ResoTime);
      int DetectorNbr = level[0];
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time); 
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Minos::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_MinosTargetScorer = CheckScorer("MinosTargetScorer",already_exist) ;
  m_MinosTPCScorer = CheckScorer("MinosTPCScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* CalorimeterMinosTargetScorer= new CalorimeterScorers::PS_Calorimeter("CalorimeterMinosTargetScore",level, 0) ;
  G4VPrimitiveScorer* InteractionMinosTargetScorer= new InteractionScorers::PS_Interactions("InteractionMinosTargetScore",ms_InterCoord, 0) ;
  //and register it to the multifunctionnal detector
  m_MinosTargetScorer->RegisterPrimitive(CalorimeterMinosTargetScorer);
  m_MinosTargetScorer->RegisterPrimitive(InteractionMinosTargetScorer);


  G4VPrimitiveScorer* TPCScorer= new TPCScorers::PS_TPCCathode("MinosTPC", 0);
  m_MinosTPCScorer->RegisterPrimitive(TPCScorer);
  
  G4SDManager::GetSDMpointer()->AddNewDetector(m_MinosTPCScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_MinosTargetScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Minos::Construct(){
  return  (NPS::VDetector*) new Minos();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Minos{
    public:
      proxy_nps_Minos(){
        NPS::DetectorFactory::getInstance()->AddToken("Minos","Minos");
        NPS::DetectorFactory::getInstance()->AddDetector("Minos",Minos::Construct);
      }
  };

  proxy_nps_Minos p_nps_Minos;
}
