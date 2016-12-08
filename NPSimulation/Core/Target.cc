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
#include "NPInputParser.h"
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
void Target::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*>  starget = parser.GetAllBlocksWithToken("Target");
  vector<NPL::InputBlock*>  ctarget = parser.GetAllBlocksWithToken("CryoTarget");

  if(starget.size()==1){
    cout << "////       TARGET      ////" << endl;
    cout << "//// Solid Target found " << endl;
    vector<string> token = {"Thickness","Radius","Material","Angle","X","Y","Z"};
    if(starget[0]->HasTokenList(token)){
      m_TargetThickness= starget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle=starget[0]->GetDouble("Angle","deg");
      m_TargetRadius=starget[0]->GetDouble("Radius","mm");
      m_TargetMaterial= GetMaterialFromLibrary(starget[0]->GetString("Material"));
      m_TargetX=starget[0]->GetDouble("X","mm");
      m_TargetY=starget[0]->GetDouble("Y","mm");
      m_TargetZ=starget[0]->GetDouble("Z","mm");
    }
    else{
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }
    if(starget[0]->HasToken("NBLAYERS"))
      m_TargetNbLayers = starget[0]->GetInt("NBLAYERS");


  }
  else if(ctarget.size()==1){
    cout << " Solid Target found " << endl;
    vector<string> token = {"Thickness","Radius","Material","Density","WindowsThickness","WindowsMaterial","Angle","X","Y","Z"};
    if(ctarget[0]->HasTokenList(token)){
      m_TargetThickness= ctarget[0]->GetDouble("Thickness","micrometer");
      m_TargetAngle=ctarget[0]->GetDouble("Angle","deg");
      m_TargetMaterial= GetMaterialFromLibrary(ctarget[0]->GetString("Material"));
      m_WindowsThickness= ctarget[0]->GetDouble("WindowsThickness","micrometer");
      m_WindowsMaterial= GetMaterialFromLibrary(ctarget[0]->GetString("WindowsMaterial"));
      m_TargetX=ctarget[0]->GetDouble("X","mm");
      m_TargetY=ctarget[0]->GetDouble("Y","mm");
      m_TargetZ =ctarget[0]->GetDouble("Z","mm");
    }
    else{
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }
    
    if(ctarget[0]->HasToken("NBLAYERS"))
      m_TargetNbLayers = ctarget[0]->GetInt("NBLAYERS");

  }
  else{
    cout << "ERROR: One and only one target shall be declared in your detector file" << endl;
    exit(1);
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
