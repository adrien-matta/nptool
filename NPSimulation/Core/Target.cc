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
 *  This class describe Cryogenic and standard Target. Derived from          *
 *  NPS::VDetector                                                           * 
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
#include "G4UserLimits.hh"
#include "Randomize.hh"
#include "BeamReaction.hh"
#include "G4FastSimulationManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4RegionStore.hh"
#include "G4RunManager.hh"
using namespace CLHEP ;

// NPS
#include "Target.hh"
#include "MaterialManager.hh"
#include "Decay.hh"
// NPL
#include "NPOptionManager.h"
#include "NPInputParser.h"
using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Target::Target(){
  m_TargetType         = true;
  m_TargetThickness    = 0   ;
  m_TargetAngle        = 0   ;
  m_TargetRadius       = 0   ;
  m_TargetDensity      = 0   ;
  m_TargetNbLayers     = 5;   // Number of steps by default
  m_ReactionRegion=NULL;

  m_TargetDensity = 0 ;
  m_FrontDeformation = 0 ;
  m_FrontThickness = 0 ;
  m_FrontRadius = 0 ;
  m_FrontMaterial = 0 ;
  m_BackDeformation = 0 ;
  m_BackRadius = 0 ;
  m_BackThickness = 0 ;
  m_BackMaterial = 0 ;
  m_FrameRadius = 0 ;
  m_FrameThickness = 0;
  m_FrontCone = 0 ;
  m_BackCone = 0 ;
  m_FrameMaterial = 0 ;
  m_ShieldInnerRadius = 0 ;
  m_ShieldOuterRadius = 0 ;
  m_ShieldBottomLength = 0 ;
  m_ShieldTopLength = 0 ;
  m_ShieldFrontRadius = 0 ; 
  m_ShieldBackRadius = 0 ;
  m_ShieldMaterial = 0 ;




}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Target::~Target(){
  unsigned int size = m_ReactionModel.size();
  for(unsigned int i = 0 ; i < size ; i++){
    delete m_ReactionModel[i];
    }  
  m_ReactionModel.clear();
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
  vector<NPL::InputBlock*>  ctarget = parser.GetAllBlocksWithToken("CryogenicTarget");

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
    m_TargetType=false;
    cout << "//// Cryogenic Target found " << endl;

    vector<string> CoreToken   = {"NominalThickness","Material","Density","Radius","Angle","X","Y","Z"};
    vector<string> FrontToken  = {"FrontDeformation","FrontThickness","FrontRadius","FrontMaterial"};
    vector<string> BackToken   = {"BackDeformation","BackThickness","BackRadius","BackMaterial"};
    vector<string> FrameToken  = {"FrameRadius","FrameThickness","FrontCone","BackCone","FrameMaterial"};
    vector<string> ShieldToken = {"ShieldInnerRadius","ShieldOuterRadius""ShieldBottomLength","ShieldTopLength","ShieldFrontRadius","ShieldBackRadius","ShieldMaterial"};




    if(ctarget[0]->HasTokenList(CoreToken)){
      MaterialManager* Mat =  MaterialManager::getInstance();
      // Target 
      m_TargetThickness = ctarget[0]->GetDouble("NominalThickness","micrometer");
      m_TargetAngle = ctarget[0]->GetDouble("Angle","deg");
      m_TargetMaterial = Mat->GetMaterialFromLibrary(ctarget[0]->GetString("Material"),ctarget[0]->GetDouble("Density","g/cm3"));
      m_TargetRadius = ctarget[0]->GetDouble("Radius","mm");
      m_TargetX = ctarget[0]->GetDouble("X","mm");
      m_TargetY = ctarget[0]->GetDouble("Y","mm");
      m_TargetZ = ctarget[0]->GetDouble("Z","mm");
      m_TargetDensity = ctarget[0]->GetDouble("Density","g/cm3"); 
      m_TargetRadius = ctarget[0]->GetDouble("Radius","mm");

      // Front Window
      m_FrontDeformation = ctarget[0]->GetDouble("FrontDeformation","mm");
      m_FrontThickness = ctarget[0]->GetDouble("FrontThickness","micrometer");
      m_FrontRadius = ctarget[0]->GetDouble("FrontRadius","mm");
      m_FrontMaterial = Mat->GetMaterialFromLibrary(ctarget[0]->GetString("FrontMaterial"));

      // Back Window
      m_BackDeformation = ctarget[0]->GetDouble("BackDeformation","mm");
      m_BackRadius = ctarget[0]->GetDouble("BackRadius","mm");
      m_BackThickness = ctarget[0]->GetDouble("BackThickness","micrometer");
      m_BackMaterial = Mat->GetMaterialFromLibrary( ctarget[0]->GetString("BackMaterial"));

      // Cell Frame
      m_FrameRadius = ctarget[0]->GetDouble("FrameRadius","mm");
      m_FrameThickness = ctarget[0]->GetDouble("FrameThickness","mm");
      m_FrontCone = ctarget[0]->GetDouble("FrontCone","deg");
      m_BackCone = ctarget[0]->GetDouble("BackCone","deg");
      m_FrameMaterial = Mat->GetMaterialFromLibrary(ctarget[0]->GetString("FrameMaterial"));
      // Heat Shield
      m_ShieldInnerRadius = ctarget[0]->GetDouble("ShieldInnerRadius","mm");
      m_ShieldOuterRadius = ctarget[0]->GetDouble("ShieldOuterRadius","mm");
      m_ShieldBottomLength = ctarget[0]->GetDouble("ShieldBottomLength","mm");
      m_ShieldTopLength = ctarget[0]->GetDouble("ShieldTopLength","mm");
      m_ShieldFrontRadius = ctarget[0]->GetDouble("ShieldFrontRadius","mm"); 
      m_ShieldBackRadius = ctarget[0]->GetDouble("ShieldBackRadius","mm");
      m_ShieldMaterial = Mat->GetMaterialFromLibrary(ctarget[0]->GetString("ShieldMaterial"));
    }
    else{
      cout << "ERROR: Target token list incomplete, check your input file" << endl;
      exit(1);
    }

    if(ctarget[0]->HasToken("360AYERS"))
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
    // X-Z target profile
    unsigned int size = 100;
    std::vector<double> OuterRadius;
    std::vector<double> InnerRadius;
    std::vector<double> Z;

    // Front Bulge
    if(m_FrontDeformation!=0){
      double step = m_FrontRadius/size;
      for(unsigned int i = 0 ; i < size ; i++){
        OuterRadius.push_back(i*step);
        Z.push_back(FrontProfile(i*step,m_TargetThickness*0.5,m_FrontDeformation,m_FrontRadius));
      } 
    }

    // Nominal Part
    OuterRadius.push_back(m_FrontRadius);
    Z.push_back(0.5*m_TargetThickness);

    OuterRadius.push_back(m_TargetRadius);
    Z.push_back(0.5*m_TargetThickness);

    OuterRadius.push_back(m_TargetRadius);
    Z.push_back(-0.5*m_TargetThickness);

    OuterRadius.push_back(m_BackRadius);
    Z.push_back(-0.5*m_TargetThickness);


    // Back Bulge
    if(m_FrontDeformation!=0){
      double step = m_BackRadius/size;
      for(int i = size-1 ; i>=0 ; i--){
        OuterRadius.push_back(i*step);
        Z.push_back(BackProfile(i*step,m_TargetThickness*0.5,m_BackDeformation,m_BackRadius));
      } 
    }


    InnerRadius.resize(Z.size(),0);

    m_CryoTargetSolid = 
      new G4Polycone("solidTarget", 0, 360*deg,Z.size(),&Z[0],&InnerRadius[0],&OuterRadius[0]);

    m_TargetLogic = 
      new G4LogicalVolume(m_CryoTargetSolid, 
          m_TargetMaterial,
          "logicTarget");

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
        m_TargetLogic, "Target", world, false, 0);

    G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
    m_TargetLogic->SetVisAttributes(TargetVisAtt);


    // Front Window 
    OuterRadius.clear();
    InnerRadius.clear();
    Z.clear();

    // Front Bulge
    if(m_FrontDeformation!=0){
      double step = m_FrontRadius/size;
      for(unsigned int i = 0 ; i < size ; i++){
        InnerRadius.push_back(i*step);
        OuterRadius.push_back(i*step+m_FrontThickness);
        Z.push_back(FrontProfile(i*step+m_FrontThickness,m_FrontThickness+m_TargetThickness*0.5,m_FrontDeformation,m_FrontRadius));
      } 
    }

    G4Polycone* FrontSolid = 
      new G4Polycone("solidFront", 0, 360*deg,Z.size(),&Z[0],&InnerRadius[0],&OuterRadius[0]);

    G4LogicalVolume* FrontLogic = 
      new G4LogicalVolume(FrontSolid, 
          m_FrontMaterial,
          "logicFront");

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
        FrontLogic, "Target", world, false, 0);
    G4VisAttributes* WindowsVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5,0.5));
    FrontLogic->SetVisAttributes(WindowsVisAtt);


    // Back Window 
    OuterRadius.clear();
    InnerRadius.clear();
    Z.clear();

    // Back Bulge
    if(m_BackDeformation!=0){
      double step = m_BackRadius/size;
      for(unsigned int i = 0 ; i < size ; i++){
        InnerRadius.push_back(i*step);
        OuterRadius.push_back(i*step+m_BackThickness);
        Z.push_back(BackProfile(i*step+m_BackThickness,m_BackThickness+m_TargetThickness*0.5,m_BackDeformation,m_BackRadius));
      } 
    }

    G4Polycone* BackSolid = 
      new G4Polycone("solidBack", 0, 360*deg,Z.size(),&Z[0],&InnerRadius[0],&OuterRadius[0]);

    G4LogicalVolume* BackLogic = 
      new G4LogicalVolume(BackSolid, 
          m_BackMaterial,
          "logicBack");

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
        BackLogic, "Target", world, false, 0);

    BackLogic->SetVisAttributes(WindowsVisAtt);

    // Frame
    OuterRadius.clear();
    InnerRadius.clear();
    Z.clear();
    double FrontInner = 0.5*(m_FrameThickness-m_TargetThickness) / tan(0.5*M_PI-m_FrontCone);
    double BackInner  = 0.5*(m_FrameThickness-m_TargetThickness) / tan(0.5*M_PI-m_BackCone);

    Z.push_back(m_FrameThickness*0.5); InnerRadius.push_back(FrontInner+m_FrontRadius); OuterRadius.push_back(m_FrameRadius);
    Z.push_back(m_TargetThickness*0.5); InnerRadius.push_back(m_FrontRadius); OuterRadius.push_back(m_FrameRadius);
    Z.push_back(m_TargetThickness*0.5); InnerRadius.push_back(m_TargetRadius); OuterRadius.push_back(m_FrameRadius);
    Z.push_back(-m_TargetThickness*0.5); InnerRadius.push_back(m_TargetRadius); OuterRadius.push_back(m_FrameRadius);
    Z.push_back(-m_TargetThickness*0.5); InnerRadius.push_back(m_BackRadius); OuterRadius.push_back(m_FrameRadius);
    Z.push_back(-m_FrameThickness*0.5); InnerRadius.push_back(BackInner+m_BackRadius); OuterRadius.push_back(m_FrameRadius);

    G4Polycone* FrameSolid = 
      new G4Polycone("solidFrame", 0, 360*deg,Z.size(),&Z[0],&InnerRadius[0],&OuterRadius[0]);

    G4LogicalVolume* FrameLogic = 
      new G4LogicalVolume(FrameSolid, 
          m_FrameMaterial,
          "logicFrame");

    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), 
        FrameLogic, "FrameTarget", world, false, 0);
    G4VisAttributes* FrameVisAtt = new G4VisAttributes(G4Colour(0.3, 0.4, 0.4,1));
    FrameLogic->SetVisAttributes(FrameVisAtt);

    G4RotationMatrix* Rotation=new G4RotationMatrix();
    Rotation->rotateX(90*deg);


    // Heat Shield
    G4Tubs* ShieldMain = 
      new G4Tubs("HeatShieldMain",
          m_ShieldInnerRadius,m_ShieldOuterRadius,
          m_ShieldBottomLength+m_ShieldTopLength,
          0,360);

    G4Tubs* ShieldBack = 
      new G4Tubs("HeatShieldBack",
          0,m_ShieldBackRadius,
          m_ShieldInnerRadius,
          0,360);

    G4Tubs* ShieldFront = 
      new G4Tubs("HeatShieldFront",
          0,m_ShieldFrontRadius,
          m_ShieldInnerRadius,
          0,360);

    G4RotationMatrix Rot ;  Rot.rotateX(90*deg);    

    G4Transform3D transformBack(Rot, G4ThreeVector(0,m_ShieldInnerRadius,-m_ShieldTopLength));

    G4SubtractionSolid* subtraction =
      new G4SubtractionSolid("Shield-Back", ShieldMain, ShieldBack,transformBack);

    G4Transform3D transformFront(Rot, G4ThreeVector(0,-m_ShieldInnerRadius,-m_ShieldTopLength));

    G4SubtractionSolid* ShieldSolid =
      new G4SubtractionSolid("Shield-Front", subtraction, ShieldFront,transformFront);

    G4LogicalVolume* ShieldLogic = 
      new G4LogicalVolume(ShieldSolid, 
          m_ShieldMaterial,
          "logicShield");

    new G4PVPlacement(Rotation, G4ThreeVector(0,m_ShieldTopLength, 0), 
        ShieldLogic, "ShieldTarget", world, false, 0);
    G4VisAttributes* ShieldVisAtt = new G4VisAttributes(G4Colour(0.7, 0.9, 0.9,0.5));
    ShieldLogic->SetVisAttributes(ShieldVisAtt);


  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Target::SetReactionRegion(){
  m_ReactionRegion = G4RegionStore::GetInstance()->FindOrCreateRegion("NPSimulationProcess");
  m_ReactionRegion->AddRootLogicalVolume(m_TargetLogic);
  m_ReactionRegion->SetUserLimits(new G4UserLimits(m_TargetThickness/10.));
  G4FastSimulationManager* mng = m_ReactionRegion->GetFastSimulationManager();

  unsigned int size = m_ReactionModel.size();
  for(unsigned int i = 0 ; i < size ; i++)
  mng->RemoveFastSimulationModel(m_ReactionModel[i]);

  m_ReactionModel.clear();
  G4VFastSimulationModel* fsm;
  fsm = new NPS::BeamReaction("BeamReaction",m_ReactionRegion);
  ((NPS::BeamReaction*) fsm)->SetStepSize(m_TargetThickness/10.);
  m_ReactionModel.push_back(fsm); 
  fsm = new NPS::Decay("Decay",m_ReactionRegion);
  m_ReactionModel.push_back(fsm); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Target::InitializeRootOutput()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
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

  if((0.5*m_TargetThickness+ZInteraction)<0){
    cout<< "Something is wrong with the Z coordinate of the interaction position"<<endl;
    cout<< "Check the value of Z(interaction) " << ZInteraction << endl;
  }

  G4double ThicknessBeforeInteraction = 
    (0.5*m_TargetThickness + ZInteraction) / cos(m_TargetAngle);

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
    if(m_FrontThickness!=0)
      for (G4int i = 0; i < m_TargetNbLayers; i++){
        dedx = emCalculator.ComputeTotalDEDX(IncidentEnergy, Beam, m_FrontMaterial);
        de   = dedx  * m_FrontThickness / (cos(IncidentTheta)* m_TargetNbLayers);
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
