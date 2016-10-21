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
 *  + 16/09/2009: Add CH2 material for targets (N. de Sereville)             *
 *  + 06/11/2009: Add new Token m_TargetNbLayers defining the number of      *
 *                steps used to slow down the beam in the target             *
 *                (N. de Sereville)                                          *
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
#include"MaterialManager.hh"

// NPL
#include "NPOptionManager.h"

using namespace std;

Target* Target::TargetInstance=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Target::Target(){
  m_TargetType         = true;
  m_TargetThickness    = 0   ;
  m_TargetAngle        = 0   ;
  m_TargetRadius       = 0   ;
  m_WindowsThickness   = 0   ;
  m_TargetDensity      = 0   ;
  m_EffectiveThickness = 0   ; // effective thickness at 0 deg, compute using angle and thickness
  m_TargetNbLayers     = 5;   // Number of steps by default
  // Set the global pointer
  TargetInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* Target::GetMaterialFromLibrary(G4String MaterialName){
  return MaterialManager::getInstance()->GetMaterialFromLibrary(MaterialName,m_TargetDensity);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Inherite Method of Vdetector Class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Target::ReadConfiguration(string Path){
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  string LineBuffer;
  string DataBuffer;

  bool ReadingStatusTarget = false ;
  bool ReadingStatusCryoTarget = false ;

  bool check_Thickness = false ;
  bool check_Radius = false ;
  bool check_Material = false ;
  bool check_X = false ;
  bool check_Y = false ;
  bool check_Z = false ;
  bool check_Angle = false;
  bool check_Density = false ;
  bool check_WinThickness = false ;
  bool check_WinMaterial = false ;

  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();

  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);
    if (LineBuffer.compare(0, 6, "Target") == 0) {
      if(VerboseLevel==1) G4cout << "Target Found" << G4endl;
      m_TargetType = true ;
      ReadingStatusTarget = true ;
    }
    else if (LineBuffer.compare(0, 10, "CryoTarget") == 0) {
      if(VerboseLevel==1) G4cout << "Cryogenic Target Found" << G4endl;
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
        if(VerboseLevel==1) G4cout << "Target Thickness: "  << m_TargetThickness / micrometer << " micrometer" << G4endl;
      }

      else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
        check_Angle = true ;
        ConfigFile >> DataBuffer;
        m_TargetAngle = atof(DataBuffer.c_str()) * deg;
        if(VerboseLevel==1) G4cout << "Target Angle: "  << m_TargetAngle / deg << G4endl     ;
      }

      else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
        check_Radius = true ;
        ConfigFile >> DataBuffer;
        m_TargetRadius = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout << "Target Radius: "     <<  m_TargetRadius / mm << " mm " << G4endl;
      }

      else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
        check_Material = true ;
        ConfigFile >> DataBuffer;
        m_TargetMaterial = GetMaterialFromLibrary(DataBuffer);
        if(VerboseLevel==1) G4cout << "Target Material: "      << m_TargetMaterial  << G4endl  ;
      }

      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true ;
        ConfigFile >> DataBuffer;
        m_TargetX = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
      }

      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true ;
        ConfigFile >> DataBuffer;
        m_TargetY = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout << m_TargetY / mm << " ; ";
      }

      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true ;
        ConfigFile >> DataBuffer;
        m_TargetZ = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout  << m_TargetZ / mm << " )" << G4endl ;
      }

      else if (DataBuffer.compare(0, 9, "NBLAYERS=") == 0) {
        //        check_m_TargetNbLayers = true ;
        ConfigFile >> DataBuffer;
        m_TargetNbLayers = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) 
          G4cout  << "Number of steps for slowing down the beam in target: " 
            << m_TargetNbLayers << G4endl;
      }

      ///////////////////////////////////////////////////
      //   If no Beam Token and no comment, toggle out
      else{ReadingStatusTarget = false;
        G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;
      }

      ///////////////////////////////////////////////////
      //   If all Token found toggle out
      if( check_Thickness && check_Radius && check_Material 
          && check_X && check_Y && check_Z && check_Angle){
        m_EffectiveThickness = m_TargetThickness / cos(m_TargetAngle);
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
        if(VerboseLevel==1) 
          G4cout << "Target Thickness: " << m_TargetThickness / micrometer  
            << "um" << G4endl   ;
      }

      else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
        check_Radius = true ;
        ConfigFile >> DataBuffer;
        m_TargetRadius = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) 
          G4cout << "Target Radius: " <<  m_TargetRadius / mm  << "mm" << G4endl ;
      }

      else if (DataBuffer.compare(0, 8, "DENSITY=") == 0) {
        check_Density= true ;
        ConfigFile >> DataBuffer;
        if(VerboseLevel==1) m_TargetDensity = atof(DataBuffer.c_str())*g/cm3;
      }

      else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
        check_Material = true ;
        ConfigFile >> DataBuffer;
        m_TargetMaterial = 
          GetMaterialFromLibrary(DataBuffer);
        if(VerboseLevel==1) 
          G4cout << "Target Material: " << m_TargetMaterial << G4endl;
      }

      else if (DataBuffer.compare(0, 17, "WINDOWSTHICKNESS=") == 0) {
        check_WinThickness = true ;
        ConfigFile >> DataBuffer;
        m_WindowsThickness = atof(DataBuffer.c_str()) * micrometer;
        if(VerboseLevel==1) 
          G4cout << "Windows Thickness: " 
            << m_WindowsThickness / micrometer << "um" << G4endl   ;
      }

      else if (DataBuffer.compare(0, 16, "WINDOWSMATERIAL=") == 0) {
        check_WinMaterial = true ;
        ConfigFile >> DataBuffer;
        m_WindowsMaterial = GetMaterialFromLibrary(DataBuffer);
        if(VerboseLevel==1) 
          G4cout << "Windows Material: " << m_WindowsMaterial << G4endl;
      }

      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true ;
        ConfigFile >> DataBuffer;
        m_TargetX = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) 
          G4cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
      }

      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true ;
        ConfigFile >> DataBuffer;
        m_TargetY = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout << m_TargetY / mm << " ; ";
      }

      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true ;
        ConfigFile >> DataBuffer;
        m_TargetZ = atof(DataBuffer.c_str()) * mm;
        if(VerboseLevel==1) G4cout << m_TargetZ / mm << " )" << G4endl ;
      }

      else if (DataBuffer.compare(0, 9, "NBLAYERS=") == 0) {
        ConfigFile >> DataBuffer;
        m_TargetNbLayers = atoi(DataBuffer.c_str());
        if(VerboseLevel==1) 
          G4cout  << "Number of steps for slowing down the beam in target: " 
            << m_TargetNbLayers << G4endl;
      }

      ///////////////////////////////////////////////////
      //   If no Beam Token and no comment, toggle out
      else{
        ReadingStatusCryoTarget = false;
        G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;
      }

      ///////////////////////////////////////////////////
      //   If all Token found toggle out
      if( check_Thickness && check_Radius && check_Material && check_X 
          && check_Y && check_Z && check_WinThickness && check_WinMaterial 
          && check_Density){
        m_EffectiveThickness = m_TargetThickness / cos(m_TargetAngle);
        ReadingStatusCryoTarget = false ;
      }

    }
  }
  // if the target as a null radius then no target exist
  if(m_TargetRadius==0) {
    m_TargetThickness=0;
    m_TargetRadius=0.1*um;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Target::ConstructDetector(G4LogicalVolume* world){
  if (m_TargetType) {   // case of standard target

    if (m_TargetThickness > 0) {
      m_TargetSolid = 
        new G4Tubs("solidTarget", 0, m_TargetRadius, 
            0.5*m_TargetThickness, 0*deg, 360*deg);
      m_TargetLogic = 
        new G4LogicalVolume(m_TargetSolid, m_TargetMaterial, "logicTarget");

      // rotation of target
      G4RotationMatrix *rotation = new G4RotationMatrix();
      rotation->rotateY(m_TargetAngle);

      new G4PVPlacement(rotation, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), 
          m_TargetLogic, "Target", world, false, 0);

      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
      m_TargetLogic->SetVisAttributes(TargetVisAtt);
    }
  }

  else {   // case of cryogenic target
    if (m_TargetThickness > 0) {
      m_TargetSolid = 
        new G4Tubs("solidTarget", 0, m_TargetRadius, 
            0.5*m_TargetThickness+m_WindowsThickness, 0*deg, 360*deg);

     m_TargetLogic = 
        new G4LogicalVolume(m_TargetSolid, 
                            GetMaterialFromLibrary("Vacuum")
                            , "logicTarget");

      m_TargetLogic->SetVisAttributes(G4VisAttributes::Invisible);
      G4Tubs* solidTarget = 
            new G4Tubs("solidTarget", 0, m_TargetRadius, 
            0.5*m_TargetThickness, 0*deg, 360*deg);

      G4LogicalVolume* logicTarget = 
        new G4LogicalVolume(solidTarget, m_TargetMaterial, "logicTarget");

      new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
          logicTarget, "Target", m_TargetLogic, false, 0);

       new G4PVPlacement(0, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), 
          m_TargetLogic, "Target", world, false, 0);

      G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
      logicTarget->SetVisAttributes(TargetVisAtt);
    }


    if (m_WindowsThickness > 0) {
      G4ThreeVector TargetPos = G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ);

      G4Tubs* solidWindowsF =
        new G4Tubs("solidTargetWindowsF", 0, m_TargetRadius, 
            0.5*m_WindowsThickness, 0*deg, 360*deg);
      G4LogicalVolume* logicWindowsF = 
        new G4LogicalVolume(solidWindowsF, m_WindowsMaterial, 
            "logicTargetWindowsF");

      G4Tubs* solidWindowsB =
        new G4Tubs("solidTargetWindowsB", 0, m_TargetRadius, 
            0.5*m_WindowsThickness, 0*deg, 360*deg);
      G4LogicalVolume* logicWindowsB = 
        new G4LogicalVolume(solidWindowsB, m_WindowsMaterial, 
            "logicTargetWindowsB");

      new G4PVPlacement(0,
          TargetPos 
          +G4ThreeVector(0., 0., 0.5*(m_TargetThickness + m_WindowsThickness)) ,
          logicWindowsF,"Target Window Front",m_TargetLogic,false, 0);

      new G4PVPlacement(   0,
          TargetPos + G4ThreeVector(0., 0., -0.5*(m_TargetThickness + m_WindowsThickness)),
          logicWindowsB,"Target Window Back",m_TargetLogic,false, 0);

      G4VisAttributes* WindowsVisAtt = new G4VisAttributes(G4Colour(0.5, 1., 0.5));
      logicWindowsF->SetVisAttributes(WindowsVisAtt);
      logicWindowsB->SetVisAttributes(WindowsVisAtt);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Target::InitializeRootOutput()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Target::ReadSensitive(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return the slow down beam energy after interaction at ZInteraction with initial beam energy before target IncidentEnergy
G4double Target::SlowDownBeam(G4ParticleDefinition* Beam, 
    G4double IncidentEnergy, 
    G4double ZInteraction, 
    G4double IncidentTheta){

  if(Beam->GetParticleName()=="neutron"){
    return IncidentEnergy;
  }

  G4double ThicknessBeforeInteraction = 
    abs(ZInteraction - 0.5*m_EffectiveThickness) / cos(m_TargetAngle);

  G4double dedx,de;
  static G4EmCalculator emCalculator;

  if(m_TargetType){
    if(m_TargetThickness!=0){
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_TargetMaterial);
        de   = dedx * ThicknessBeforeInteraction / m_TargetNbLayers;
        IncidentEnergy -= de;
        if(IncidentEnergy<0){
          IncidentEnergy = 0;
          break;
        }
      }
    }
  }

  else{
    //   Windows
    if(m_WindowsThickness!=0)
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_WindowsMaterial);
        de   = dedx * m_TargetNbLayers * m_WindowsThickness / cos(IncidentTheta);
        IncidentEnergy -= de;
        if(IncidentEnergy<0){
          IncidentEnergy = 0;
          break;
        }

      }

    // Target
    if(m_TargetThickness!=0)
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_TargetMaterial);
        de   = dedx * ThicknessBeforeInteraction / m_TargetNbLayers;
        IncidentEnergy -= de;
        if(IncidentEnergy<0){
          IncidentEnergy = 0;
          break;
        }

      }
  }

  if(IncidentEnergy<0) IncidentEnergy = 0 ;
  return IncidentEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
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
