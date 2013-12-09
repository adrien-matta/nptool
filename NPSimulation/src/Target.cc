/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : 06/11/2009                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Cryogenic and standard Target. Derived from VDetector*
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Some improvment need to be done in material dealing                      *
 *                                                                           *
 *  + 16/09/2009: Add support for positioning the target with an angle with  *
 *                respect to the beam (N. de Sereville)                      *
 *  + 16/09/2009: Add CH2 material for targets (N. de Sereville)             *
 *  + 06/11/2009: Add new Token m_TargetNbLayers defining the number of steps used   *
 *                to slow down the beam in the target (N. de Sereville)      *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <fstream>
#include <limits>

// G4 
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4EmCalculator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
using namespace CLHEP ;

// NPS
#include"Target.hh"

// NPL
#include "NPOptionManager.h"

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Target::Target()
{
  m_TargetType         = true;
  m_TargetThickness    = 0   ;
  m_TargetAngle        = 0   ;
  m_TargetRadius       = 0   ;
  m_WindowsThickness   = 0   ;
  m_TargetTemperature  = 0   ;
  m_TargetPressure     = 0   ;
  m_EffectiveThickness = 0   ; // effective thickness at 0 deg, compute using angle and thickness
  m_TargetNbLayers     = 50;   // Number of steps by default
}

G4Material* Target::GetMaterialFromLibrary(G4String MaterialName, G4double Temperature, G4double Pressure)
{
  
  G4Material* myMaterial;
  
  if (MaterialName == "D2") {
    G4double density = 0;
   
    if(Pressure == 0 )
     if (Temperature == 0) {
        density = 0.000083771* g / cm3;
        G4cout << "CryoTarget temp set to 300K with P = 1bar" << G4endl;
      }
 
    else if (Pressure == 1) {
      G4cout << "CryoTarget pressure set to 1 bar" << G4endl;
      
      if (Temperature == 24) {
        density = 0.0020182 * g / cm3;
        G4cout << "CryoTarget temp set to 24K" << G4endl;
      }
      
      else if (Temperature == 25) {
        density = 0.0019377 * g / cm3;
        G4cout << "CryoTarget temp set to 25K" << G4endl;
      }
      
      else if (Temperature == 26) {
        density = 0.001863 * g / cm3;
        G4cout << "CryoTarget temp set to 26K" << G4endl;
      }
      
      else if (Temperature == 30) {
        density = 0.00020475 * g / cm3;
        G4cout << "CryoTarget temp set to 30K" << G4endl;
      }
      
      else if (Temperature == 300) {
        density = 8.3771e-5* g / cm3;
        G4cout << "CryoTarget temp set to 30K" << G4endl;
      }
      
      
      else {
        G4cout << ">>>  !!!!WARNING INVALID TEMP FOR CRYOGENIC TARGET!!!!  <<<" << G4endl;
      }
    }
    
    else if (Pressure == 0.5) {
      G4cout << "CryoTarget pressure set to 0.5 bar" << G4endl;
      
      if (Temperature == 24) {
        density = 0.0010091 * g / cm3;
        G4cout << "CryoTarget temp set to 24K" << G4endl;
      }
      
      else if (Temperature == 25) {
        density = 0.00096875 * g / cm3;
        G4cout << "CryoTarget temp set to 25K" << G4endl;
      }
      
      else if (Temperature == 26) {
        density = 0.00093149 * g / cm3;
        G4cout << "CryoTarget temp set to 26K" << G4endl;
      }
      
      
      else if (Pressure == 0.7) {
        G4cout << "CryoTarget pressure set to 0.7 bar" << G4endl;
        
        if (Temperature == 26) {
          density = 0.0013125 * g / cm3;
          G4cout << "CryoTarget temp set to 26K" << G4endl;
        }
      }
      
      
      else {
        G4cout << ">>>  !!!!WARNING INVALID TEMP FOR CRYOGENIC TARGET!!!!  <<<" << G4endl;
      }
    }
    
    
    
    G4Element* D  = new G4Element("Deuteron"  , "D" , 1., 2.0141*g / mole);
    myMaterial    = new G4Material("D2", density, 1, kStateGas, Temperature, Pressure);
    myMaterial->AddElement(D, 2);
    return(myMaterial);
  }
  
  else if (MaterialName == "D2_solid") {
    G4Element* D           = new G4Element("Deuteron", "D", 1., 2.0141*g / mole);
    G4Material* myMaterial = new G4Material("D2_solid", 0.0715*g/cm3, 1);
    myMaterial->AddElement(D, 2);
    return(myMaterial);
  }
  
  else if (MaterialName == "H2_solid") {
    G4Element* H           = new G4Element("Hydrogen", "H", 1., 1.01*g / mole);
    G4Material* myMaterial = new G4Material("H2_solid", 0.0715*g/cm3, 1);
    myMaterial->AddElement(H, 2);
    return(myMaterial);
  }
  
  else if (MaterialName == "Mylar") {
    G4cout << "Mylar Material" << G4endl ;
    G4Element* H  = new G4Element("Hydrogen", "H" , 1. ,  1.01 *g / mole);
    G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
    G4Element* O  = new G4Element("Oxygen"  , "O" , 8. , 16.00 *g / mole);
    
    G4Material* myMaterial = new G4Material("Mylar", 1.397*g / cm3, 3);
    myMaterial->AddElement(C , 10);
    myMaterial->AddElement(H ,  8);
    myMaterial->AddElement(O ,  4);
    return myMaterial;
  }
  
  else if (MaterialName == "Harvar") {
    G4Element* Co  = new G4Element("Cobalt" , "Co" , 27 , 58.933*g / mole);
    G4Element* Cr  = new G4Element("Cromium", "Cr" , 24 , 51.996*g / mole);
    G4Element* Ni  = new G4Element("Nickel"   , "Ni" , 28 , 58.69*g / mole);
    G4Element* Fe  = new G4Element("Iron"  , "Fe" , 26 , 55.847*g / mole);
    G4Element* W   = new G4Element("Teflon"   , "W"  , 74 , 183.5*g / mole);
    
    G4Material* myMaterial = new G4Material("Havar", 8.3*g / cm3, 5);
    myMaterial->AddElement(Co , 42);
    myMaterial->AddElement(Cr , 20);
    myMaterial->AddElement(Ni , 13);
    myMaterial->AddElement(Fe , 19);
    myMaterial->AddElement(W  ,  1);
    return myMaterial;
  }
  
  else if (MaterialName == "CD2") {
    G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
    G4Element* D  = new G4Element("Deuteron"  , "D" , 1., 2.0141*g / mole);
    
    G4Material* myMaterial = new G4Material("CD2", 1.06*g / cm3, 2);
    myMaterial->AddElement(C , 1);
    myMaterial->AddElement(D , 2);
    return myMaterial;
  }
  
  else if (MaterialName == "CH2") {
    G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
    G4Element* H  = new G4Element("Hydrogen", "H" , 1. ,  1.01 *g / mole);
    
    G4Material* myMaterial = new G4Material("CH2", 0.93*g / cm3, 2);
    myMaterial->AddElement(C , 1);
    myMaterial->AddElement(H , 2);
    return myMaterial;
  }
  
  else if (MaterialName == "208Pb") {
    G4Element* Pb  = new G4Element("Lead"  , "Pb" , 82. , 207.2*g / mole);
    
    G4Material* myMaterial = new G4Material("208Pb", 11.342*g / cm3, 1);
    myMaterial->AddElement(Pb , 1);
    return myMaterial;
  }
  
  else if (MaterialName == "Si") {
    G4Material* myMaterial = new G4Material("Si", 14., 28.0855 * g / mole,  2.321 * g / cm3);
    return myMaterial;
  }
  
  else if (MaterialName == "Al") {
    G4Material* myMaterial = new G4Material("Aluminium",  13.,  26.98 * g / mole, 2.702 * g / cm3);
    return myMaterial;
  }
  
  else {
    G4cout << "No Matching Material in the Target Library Default is Vacuum" << G4endl;
    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01*g / mole);
    G4Element* O = new G4Element("Oxygen"  , "O", 8., 16.00*g / mole);
    myMaterial = new G4Material("Vacuum", 0.000000001*mg / cm3, 2);
    myMaterial->AddElement(N, .7);
    myMaterial->AddElement(O, .3);
    return(myMaterial);
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Inherite Method of Vdetector Class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Target::ReadConfiguration(string Path)
{
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer;
  string DataBuffer;
  
  bool ReadingStatusTarget = false ;
  bool ReadingStatusCryoTarget = false ;
  
  bool check_Thickness = false ;
  bool check_Radius = false ;
//  bool check_Angle = false ;
  bool check_Material = false ;
  bool check_X = false ;
  bool check_Y = false ;
  bool check_Z = false ;
//  bool check_m_TargetNbLayers = false;
  
  bool check_Temperature = false ;
  bool check_Pressure = false ;
  bool check_WinThickness = false ;
  bool check_WinMaterial = false ;
  
  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();

  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);
    if (LineBuffer.compare(0, 6, "Target") == 0) {
      if(VerboseLevel==1) cout << "Target Found" << endl;
      m_TargetType = true ;
      ReadingStatusTarget = true ;
    }
    else if (LineBuffer.compare(0, 10, "CryoTarget") == 0) {
      if(VerboseLevel==1) cout << "Cryogenic Target Found" << endl;
      m_TargetType = false ;
      ReadingStatusCryoTarget = true ;
    }
    
    while (ReadingStatusTarget) {
      ConfigFile >> DataBuffer;
      
      //Search for comment Symbol %
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
        check_Thickness = true ;
        ConfigFile >> DataBuffer;
        m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
        if(VerboseLevel==1) cout << "Target Thickness: "  << m_TargetThickness / micrometer << " micrometer" << endl;
      }
      
      else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
//        check_Angle = true ;
        ConfigFile >> DataBuffer;
        m_TargetAngle = atof(DataBuffer.c_str()) * deg;
        if(VerboseLevel==1) cout << "Target Angle: "  << m_TargetAngle / deg << endl     ;
      }
      
      else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
        check_Radius = true ;
        ConfigFile >> DataBuffer;
        m_TargetRadius = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << "Target Radius: "     <<  m_TargetRadius / mm << " mm " << endl;
      }
      
      else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
        check_Material = true ;
        ConfigFile >> DataBuffer;
        m_TargetMaterial = GetMaterialFromLibrary(DataBuffer);
        if(VerboseLevel==1) cout << "Target Material: "      << m_TargetMaterial  << endl  ;
      }
      
      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true ;
        ConfigFile >> DataBuffer;
        m_TargetX = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
      }
      
      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true ;
        ConfigFile >> DataBuffer;
        m_TargetY = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << m_TargetY / mm << " ; ";
      }
      
      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true ;
        ConfigFile >> DataBuffer;
        m_TargetZ = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout  << m_TargetZ / mm << " )" << endl ;
      }
      
      else if (DataBuffer.compare(0, 9, "NBLAYERS=") == 0) {
//        check_m_TargetNbLayers = true ;
        ConfigFile >> DataBuffer;
        m_TargetNbLayers = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) cout  << "Number of steps for slowing down the beam in target: " << m_TargetNbLayers << endl;
      }
      
      ///////////////////////////////////////////////////
      //   If no Beam Token and no comment, toggle out
      else{ReadingStatusTarget = false;
        G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;
      }
      
      ///////////////////////////////////////////////////
      //   If all Token found toggle out
      if( check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z ){
        ReadingStatusTarget = false ;
      }
    }
    
    while(ReadingStatusCryoTarget){
      
      ConfigFile >> DataBuffer;
      
      //Search for comment Symbol %
      if (DataBuffer.compare(0, 1, "%") == 0) {/*Do Nothing*/;}
      
      else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
        check_Thickness = true ;
        ConfigFile >> DataBuffer;
        m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
        if(VerboseLevel==1) cout << "Target Thickness: "  << m_TargetThickness / micrometer  << "um" << endl   ;
      }
      
      else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
        check_Radius = true ;
        ConfigFile >> DataBuffer;
        m_TargetRadius = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << "Target Radius: "     <<  m_TargetRadius / mm           << "mm" << endl ;
      }
      
      else if (DataBuffer.compare(0, 12, "TEMPERATURE=") == 0) {
        check_Temperature = true ;
        ConfigFile >> DataBuffer;
        if(VerboseLevel==1) m_TargetTemperature = atof(DataBuffer.c_str());
      }
      
      else if (DataBuffer.compare(0, 9, "PRESSURE=") == 0) {
        check_Pressure = true ;
        ConfigFile >> DataBuffer;
        m_TargetPressure = atof(DataBuffer.c_str());
      }
      
      else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
        check_Material = true ;
        ConfigFile >> DataBuffer;
        m_TargetMaterial = GetMaterialFromLibrary(DataBuffer, m_TargetTemperature, m_TargetPressure);
        if(VerboseLevel==1) cout << "Target Material: "      << m_TargetMaterial          << endl         ;
      }
      
      else if (DataBuffer.compare(0, 17, "WINDOWSTHICKNESS=") == 0) {
        check_WinThickness = true ;
        ConfigFile >> DataBuffer;
        m_WindowsThickness = atof(DataBuffer.c_str()) * micrometer;
        if(VerboseLevel==1) cout << "Windows Thickness: " <<    m_WindowsThickness / micrometer << "um" << endl   ;
      }
      
      else if (DataBuffer.compare(0, 16, "WINDOWSMATERIAL=") == 0) {
        check_WinMaterial = true ;
        ConfigFile >> DataBuffer;
        m_WindowsMaterial = GetMaterialFromLibrary(DataBuffer);
        if(VerboseLevel==1) cout << "Windows Material: "  <<    m_WindowsMaterial         << endl         ;
      }
      
      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true ;
        ConfigFile >> DataBuffer;
        m_TargetX = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
      }
      
      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true ;
        ConfigFile >> DataBuffer;
        m_TargetY = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << m_TargetY / mm << " ; ";
      }
      
      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true ;
        ConfigFile >> DataBuffer;
        m_TargetZ = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) cout << m_TargetZ / mm << " )" << endl ;
      }
      
      else if (DataBuffer.compare(0, 9, "NBLAYERS=") == 0) {
//        check_m_TargetNbLayers = true ;
        ConfigFile >> DataBuffer;
        m_TargetNbLayers = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) cout  << "Number of steps for slowing down the beam in target: " << m_TargetNbLayers << endl;
      }
      
      ///////////////////////////////////////////////////
      //   If no Beam Token and no comment, toggle out
      else{
        ReadingStatusCryoTarget = false;
        G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;
      }
      
      ///////////////////////////////////////////////////
      //   If all Token found toggle out
      if( check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z && check_WinThickness && check_WinMaterial && check_Pressure && check_Temperature)
        m_EffectiveThickness = m_TargetThickness / cos(m_TargetAngle);
      ReadingStatusCryoTarget = false ;
      
    }
    
  }
  // if the target as a null radius then no target exist
  if(m_TargetRadius==0) {m_TargetThickness=0;m_TargetRadius=0.1*um;}
  
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Target::ConstructDetector(G4LogicalVolume* world)
{

  
  if (m_TargetType) {   // case of standard target
    
    if (m_TargetThickness > 0) {
      G4Tubs*            solidTarget = new G4Tubs("solidTarget", 0, m_TargetRadius, 0.5*m_TargetThickness, 0*deg, 360*deg);
      G4LogicalVolume*   logicTarget = new G4LogicalVolume(solidTarget, m_TargetMaterial, "logicTarget");
      
      // rotation of target
      G4RotationMatrix *rotation = new G4RotationMatrix();
      rotation->rotateY(m_TargetAngle);
      
      new G4PVPlacement(rotation, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), logicTarget, "Target", world, false, 0);
      
      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));//Blue
      logicTarget->SetVisAttributes(TargetVisAtt);
    }
  }
  
  else {   // case of cryogenic target
    
    if (m_TargetThickness > 0) {
      G4Tubs*            solidTarget = new G4Tubs("solidTarget", 0, m_TargetRadius, 0.5*m_TargetThickness, 0*deg, 360*deg);
      G4LogicalVolume*   logicTarget = new G4LogicalVolume(solidTarget, m_TargetMaterial, "logicTarget");
      new G4PVPlacement(0, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), logicTarget, "Target", world, false, 0);
      
      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));//Blue
      logicTarget->SetVisAttributes(TargetVisAtt);
    }
    
    
    if (m_WindowsThickness > 0) {
      G4ThreeVector TargetPos = G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ);
      
      G4Tubs*        solidWindowsF =
      new G4Tubs("solidTargetWindowsF", 0, m_TargetRadius, 0.5*m_WindowsThickness, 0*deg, 360*deg);
      G4LogicalVolume*   logicWindowsF = new G4LogicalVolume(solidWindowsF, m_WindowsMaterial, "logicTargetWindowsF");
      
      G4Tubs*        solidWindowsB =
      new G4Tubs("solidTargetWindowsB", 0, m_TargetRadius, 0.5*m_WindowsThickness, 0*deg, 360*deg);
      G4LogicalVolume*   logicWindowsB = new G4LogicalVolume(solidWindowsB, m_WindowsMaterial, "logicTargetWindowsB");
      
      new G4PVPlacement(   0                                                                                         ,
                        TargetPos + G4ThreeVector(0., 0., 0.5*(m_TargetThickness + m_WindowsThickness)) ,
                        logicWindowsF                                                                             ,
                        "Target Window Front"                                                                     ,
                        world                                                                                     ,
                        false, 0                                                                                                            );
      
      new G4PVPlacement(   0                                                                                            ,
                        TargetPos + G4ThreeVector(0., 0., -0.5*(m_TargetThickness + m_WindowsThickness))  ,
                        logicWindowsB                                                                                ,
                        "Target Window Back"                                                                         ,
                        world                                                                                        ,
                        false, 0                                                                                                               );
      
      G4VisAttributes* WindowsVisAtt = new G4VisAttributes(G4Colour(0.5, 1., 0.5));
      logicWindowsF->SetVisAttributes(WindowsVisAtt);
      logicWindowsB->SetVisAttributes(WindowsVisAtt);
    }
  }
  
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Target::InitializeRootOutput()
{}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Target::ReadSensitive(const G4Event*)
{}


// Return the slow down beam energy after interaction at ZInteraction with initial beam energy before target IncidentEnergy
G4double Target::SlowDownBeam(G4ParticleDefinition* Beam, G4double IncidentEnergy, G4double ZInteraction, G4double IncidentTheta){
  G4double ThicknessBeforeInteraction = abs(ZInteraction - 0.5*m_EffectiveThickness) / cos(m_TargetAngle);
  G4double dedx,de;
  
  if(m_TargetType){
    G4EmCalculator emCalculator;
    if(m_TargetThickness!=0){
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_TargetMaterial);
        de   = dedx * ThicknessBeforeInteraction / m_TargetNbLayers;
        IncidentEnergy -= de;
      }
    }
  }
  
  else{
    G4EmCalculator emCalculator;
    //   Windows
    if(m_WindowsThickness!=0)
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_WindowsMaterial);
        de   = dedx * m_TargetNbLayers * m_WindowsThickness / cos(IncidentTheta);
        IncidentEnergy -= de;
      }
    
    // Target
    if(m_TargetThickness!=0)
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_TargetMaterial);
        de   = dedx * ThicknessBeforeInteraction / m_TargetNbLayers;
        IncidentEnergy -= de;
      }
  }
  return IncidentEnergy;
}


void Target::RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma){
  if (SigmaX != 0) {
    X = 2 * NumberOfSigma*SigmaX;
    while (X > NumberOfSigma*SigmaX) X = RandGauss::shoot(MeanX, SigmaX);
    
    double a = NumberOfSigma * SigmaX/2;
    double b = NumberOfSigma * SigmaY/2;
    double SigmaYPrim = b * sqrt(1 - X*X / (a*a));
    
    SigmaYPrim = 2*SigmaYPrim / NumberOfSigma;
    Y = RandGauss::shoot(MeanY, SigmaYPrim);
  }
  else {
    X = MeanX;
    Y = RandGauss::shoot(MeanY, SigmaY);
  }
}

//   Generate a DEDX file table using the material used in the target
void Target::WriteDEDXTable(G4ParticleDefinition* Particle ,G4double Emin,G4double Emax){
  if(m_TargetThickness>0){
    //   Opening hte output file
    G4String GlobalPath = getenv("NPTOOL");
    G4String Path = GlobalPath + "/Inputs/EnergyLoss/" + Particle->GetParticleName() + "_" + m_TargetMaterial->GetName() + ".G4table";
    ofstream File      ;
    File.open(Path)   ;
    
    if(!File) return ;
    
    File   << "Table from Geant4 generate using NPSimulation \t"
    << "Particle: " << Particle->GetParticleName() << "\tMaterial: " << m_TargetMaterial->GetName() << endl ;
    
    G4EmCalculator emCalculator;
    
    for (G4double E=Emin; E < Emax; E+=(Emax-Emin)/1000.){
      G4double dedx = emCalculator.ComputeTotalDEDX(E, Particle, m_TargetMaterial);
      File << E/MeV << "\t" << dedx/(MeV/micrometer) << endl ;
    }
    
    File.close();
    if(!m_TargetType){
      G4String Path = GlobalPath + "/Inputs/EnergyLoss/" + Particle->GetParticleName() + "_" + m_WindowsMaterial->GetName() + ".G4table";
      File.open(Path)      ;
      if(!File) return    ;
      File   << "Table from Geant4 generate using NPSimulation \t "
      << "Particle: " << Particle->GetParticleName() << "\tMaterial: " << m_WindowsMaterial->GetName() << endl ;
      
      for (G4double E=Emin; E < Emax; E+=(Emax-Emin)/10.){
        //                     G4double dedx = emCalculator.ComputeTotalDEDX(E, Particle, m_WindowsMaterial);
        G4double dedx = emCalculator.ComputeDEDX(   E, Particle ,
                                                 "ionIoni",  m_WindowsMaterial);
        File << E/MeV << "\t" << dedx/(MeV/micrometer) << endl ;
      }
    }
    File.close();
    
  }
}
