/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : December 2009                                            *
 * Last update    : December 2014                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Nana scintillator array                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
using namespace std;

//Geant4
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ExtrudedSolid.hh"
#include "G4TwoVector.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"

// NPS
#include "Nana.hh"
using namespace NANA;

#include "CalorimeterScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"

// CLHEP header
#include "CLHEP/Random/RandGauss.h"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Nana Specific Method
Nana::Nana(){
  m_Event = new TNanaData();

  // Blue
  m_LaBr3VisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, .0));

  // Dark Grey
  m_PMTVisAtt = new G4VisAttributes(G4Colour(0.1, 0.3, 0.5));

  // Grey wireframe
  m_DetectorCasingVisAtt = new G4VisAttributes(G4Colour(0.125, 0.125, 0.125, 0.4));

  m_LogicalDetector = 0;
  m_LaBr3Scorer = 0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Nana::~Nana(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Nana::AddDetector(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4){
  G4ThreeVector Pos=(Pos1+Pos2+Pos3+Pos4)/4.;
  G4ThreeVector u = Pos1-Pos2;
  G4ThreeVector v = Pos1-Pos4;
  u = u.unit(); v = v.unit();
  G4ThreeVector w = Pos.unit();
  Pos = Pos + w*Length*0.5;

  m_Pos.push_back(Pos);
  m_Rot.push_back(new G4RotationMatrix(u,v,w));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Nana::AddDetector(G4ThreeVector Pos, double beta_u, double beta_v, double beta_w){
  double Theta = Pos.theta();
  double Phi = Pos.phi();

  // vector parallel to one axis of silicon plane
  G4double ii = cos(Theta / rad) * cos(Phi / rad);
  G4double jj = cos(Theta / rad) * sin(Phi / rad);
  G4double kk = -sin(Theta / rad);
  G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

  G4ThreeVector w = Pos.unit();
  G4ThreeVector u = w.cross(Y);
  G4ThreeVector v = w.cross(u);
  v = v.unit();
  u = u.unit();

  G4RotationMatrix* r = new G4RotationMatrix(u,v,w);
  r->rotate(beta_u,u);
  r->rotate(beta_v,v);
  r->rotate(beta_w,w);

  m_Pos.push_back(Pos);
  m_Rot.push_back(r);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Nana::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Nana");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 


  vector<string> cart = {"A","B","C","D"};
  vector<string> sphe= {"R","Theta","Phi","Beta"};

  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Nana " << i+1 <<  endl;
      G4ThreeVector A = NPS::ConvertVector( blocks[i]->GetTVector3("A","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("B","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("C","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("D","mm"));
      AddDetector(A,B,C,D) ;
    }
  
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Nana " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      vector<double>  Beta = blocks[i]->GetVectorDouble("Beta","def");
      R = R +  0.5*Length;
      G4ThreeVector Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
      AddDetector(Pos,Beta[0],Beta[1],Beta[2]) ;
    }
    else{
      cout << "ERROR: Missing token for M2Telescope blocks, check your input file" << endl;
      exit(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Nana::ConstructDetector(G4LogicalVolume* world){

  // Lead block supporting Detector
  G4Material* Lead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");

 // G4Box* solidLead = new G4Box("solidLead",10*cm,5*cm,12.5*cm);

  std::vector<G4TwoVector> polygon;
    polygon.push_back(G4TwoVector(0,50));
    polygon.push_back(G4TwoVector(-25,25));
    polygon.push_back(G4TwoVector(-25,-75));
    polygon.push_back(G4TwoVector(0,-50));
    polygon.push_back(G4TwoVector(25,-75));
    polygon.push_back(G4TwoVector(25,25));
 
    std::vector<G4ExtrudedSolid::ZSection> zsections;
    zsections.push_back(G4ExtrudedSolid::ZSection(-6.25*cm,G4TwoVector(0,0),1));
    zsections.push_back(G4ExtrudedSolid::ZSection(+6.25*cm,G4TwoVector(0,0),1));

 G4ExtrudedSolid* solidLead = new G4ExtrudedSolid("solidLead",
                 polygon,
                 zsections); 

 G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, Lead, "logicLead", 0, 0, 0);
  G4VisAttributes* lead_vis= new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));
  logicLead->SetVisAttributes(lead_vis);
 unsigned int mysize = m_Pos.size();
  for(unsigned int i = 0 ; i < mysize ; i++){
    new G4PVPlacement(G4Transform3D(*m_Rot[i], m_Pos[i]), ConstructDetector(),  "NanaDetector", world, false, i+2); 
 

  G4Material* Wood = MaterialManager::getInstance()->GetMaterialFromLibrary("Wood");

  G4Box* table = new G4Box("Table",1*m,1*m,1*cm);
    G4LogicalVolume* logicTable= new G4LogicalVolume(table, Wood, "logicTable", 0, 0, 0);

   G4RotationMatrix* r = new G4RotationMatrix();
   r->rotateX(90*deg);
  new G4PVPlacement(r,G4ThreeVector(0,-5*cm,0), 
        logicTable, 
        "Nana_Table", 
        world, 
        false, 
        0);




/*    G4RotationMatrix* r =  new G4RotationMatrix();
    r->rotateX(-90*deg);
     r->rotateY(60*deg);

    G4ThreeVector Pos(PMTFace,0.25*6.25*cm, 90*mm);
    Pos.setTheta(60*deg); 
    new G4PVPlacement(G4Transform3D(*r, Pos+G4ThreeVector(0,0, +PMTFace)), 
        logicLead, 
        "Nana_Lead_Support", 
        world, 
        false, 
        i+1);

    new G4PVPlacement(G4Transform3D(*r, Pos+G4ThreeVector(1.1*PMTFace,0, -0.5*PMTFace)), 
        logicLead, 
        "Nana_Lead_Support", 
        world, 
        false, 
        i+1);

   G4RotationMatrix* r2 =  new G4RotationMatrix();
    r2->rotateX(-90*deg);
     r2->rotateY(300*deg);

    G4ThreeVector Pos2(PMTFace,-0.25*6.25*cm, 90*mm);
    Pos2.setTheta(300*deg); 
    new G4PVPlacement(G4Transform3D(*r2, Pos2+G4ThreeVector(0,0, +PMTFace)), 
        logicLead, 
        "Nana_Lead_Support", 
        world, 
        false, 
        i+1);

    new G4PVPlacement(G4Transform3D(*r2, Pos2+G4ThreeVector(-1.1*PMTFace,0, -0.5*PMTFace)), 
        logicLead, 
        "Nana_Lead_Support", 
        world, 
        false, 
        i+1);
*/
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Nana::ConstructDetector(){
  if(!m_LogicalDetector){

    G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    G4Material* Alu = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    G4Material* Kovar= MaterialManager::getInstance()->GetMaterialFromLibrary("Kovar");
    G4Material* Lead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");
    G4Material* LaBr3 = MaterialManager::getInstance()->GetMaterialFromLibrary("LaBr3_Ce");
    G4Material* Glass= MaterialManager::getInstance()->GetMaterialFromLibrary("Borosillicate_Glass");

    // Mother Volume
    G4Tubs* solidNanaDetector = 
      new G4Tubs("Nana",0, 0.5*FaceFront, 0.5*Length, 0.*deg, 360.*deg);
    m_LogicalDetector = 
      new G4LogicalVolume(solidNanaDetector, Vacuum, "Nana", 0, 0, 0);

    m_LogicalDetector->SetVisAttributes(G4VisAttributes::Invisible);

    // Detector construction
    // LaBr3
    G4ThreeVector  positionLaBr3 = G4ThreeVector(0, 0, LaBr3_PosZ);

    G4Tubs* solidLaBr3 = new G4Tubs("solidLaBr3", 0., 0.5*LaBr3Face, 0.5*LaBr3Thickness, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLaBr3 = new G4LogicalVolume(solidLaBr3, LaBr3, "logicLaBr3", 0, 0, 0);

    new G4PVPlacement(0, 
        positionLaBr3, 
        logicLaBr3, 
        "Nana_LaBr3", 
        m_LogicalDetector, 
        false, 
        0);

    // Set LaBr3 sensible
    logicLaBr3->SetSensitiveDetector(m_LaBr3Scorer);

    // Visualisation of LaBr3 Strip
    logicLaBr3->SetVisAttributes(m_LaBr3VisAtt);

    // Aluminium can around LaBr3
    // LaBr3 Can
    G4ThreeVector  positionLaBr3Can = G4ThreeVector(0, 0, LaBr3Can_PosZ);

    G4Tubs* solidLaBr3Can = new G4Tubs("solidLaBr3Can", 0.5*CanInnerDiameter, 0.5*CanOuterDiameter, 0.5*CanLength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLaBr3Can = new G4LogicalVolume(solidLaBr3Can, Glass, "logicLaBr3Can", 0, 0, 0);

    new G4PVPlacement(0, 
        positionLaBr3Can, 
        logicLaBr3Can, 
        "Nana_LaBr3Can", 
        m_LogicalDetector, 
        false, 
        0);

    // Visualisation of LaBr3Can
    logicLaBr3Can->SetVisAttributes(m_DetectorCasingVisAtt);

    // Aluminium window in front of LaBr3
    // LaBr3 Window
    G4ThreeVector  positionLaBr3Win = G4ThreeVector(0, 0, LaBr3Win_PosZ);

    G4Tubs* solidLaBr3Win = new G4Tubs("solidLaBr3Win", 0.5*WinInnerDiameter, 0.5*WinOuterDiameter, 0.5*WinLength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLaBr3Win = new G4LogicalVolume(solidLaBr3Win, Alu, "logicLaBr3Win", 0, 0, 0);

    new G4PVPlacement(0, 
        positionLaBr3Win, 
        logicLaBr3Win, 
        "Nana_LaBr3Win", 
        m_LogicalDetector, 
        false, 
        0);

    // Visualisation of LaBr3Win
    logicLaBr3Win->SetVisAttributes(m_DetectorCasingVisAtt);

    // PMT
    G4ThreeVector  positionPMT = G4ThreeVector(0, 0, PMT_PosZ);

    /*    G4Tubs* solidPMout = new G4Tubs("solidPMOut", 0.5*LaBr3Face, 0.5*PMTFace, 0.5*PMTThickness, 0.*deg, 360.*deg);
          G4Tubs* solidPMin = new G4Tubs("solidPMIn", 0.5*LaBr3Face-0.1*cm, 0.5*PMTFace-0.5*cm, 0.5*(PMTThickness-2.*cm)-0.1*cm, 0.*deg, 360.*deg);
          G4RotationMatrix* RotMat=NULL;
          const G4ThreeVector &Trans= G4ThreeVector(0.,0.,1.*cm); 
          G4SubtractionSolid*           solidPMT = new G4SubtractionSolid("solidPMT", solidPMout,solidPMin, RotMat, Trans);
          */
    G4Tubs* solidPMT= new G4Tubs("solidPMOut", 0.5*LaBr3Face, 0.5*PMTFace, 0.5*PMTThickness, 0.*deg, 360.*deg);
    G4LogicalVolume* logicPMT = new G4LogicalVolume(solidPMT, Kovar, "logicPMT", 0, 0, 0);
    G4Tubs* solidPMTWin = new G4Tubs("solidPMTWin", 0, 0.5*LaBr3Face, 0.5*WinLength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicPMTWin = new G4LogicalVolume(solidPMTWin, Glass, "logicPMTWin", 0, 0, 0);


    new G4PVPlacement(0, 
        positionPMT, 
        logicPMT, 
        "Nana_PMT", 
        m_LogicalDetector, 
        false, 
        0);
  
    new G4PVPlacement(0, 
        positionPMT+G4ThreeVector(0,0,-0.45*PMTThickness), 
        logicPMTWin, 
        "Nana_PMTWind", 
        m_LogicalDetector, 
        false, 
        0);


    // Visualisation of PMT Strip
    logicPMT->SetVisAttributes(m_PMTVisAtt);


    /*
    // Plastic Lead shielding
    //plastic definition
    // LaBr3
    G4int ncomponents, natoms;
    G4double z, density;
    G4double percent = .5;
    G4Element* C  = new G4Element("Carbon","C" , z= 6., 12.0107*g/mole);
    G4Element* H  = new G4Element("Hydrogen","H" , z= 1.,1.00794*g/mole );
    G4Element* O  = new G4Element("Oxygen","O" , z= 8., 15.999*g/mole);
    G4Element* W  = new G4Element("Tungsten","W" , z= 74., 183.84*g/mole);

    density = percent * 4.5*g/cm3;
    G4Material* PCA = new G4Material("PCA", density, ncomponents=4);
    PCA->AddElement(C, natoms=6);
    PCA->AddElement(H, natoms=8);
    PCA->AddElement(O, natoms=4);
    PCA->AddElement(W, natoms=1);
    // A
    G4ThreeVector  positionLeadAShield = G4ThreeVector(0, 0, LeadAShield_PosZ);
    G4Tubs* solidLeadA = new G4Tubs("solidLead", 0.5*LeadAMinR, 0.5*LeadAMaxR, 0.5*LeadALength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLeadAShield = new G4LogicalVolume(solidLeadA, PCA, "logicLeadAShield", 0, 0, 0);

    new G4PVPlacement(0, 
    positionLeadAShield, 
    logicLeadAShield, 
    "Nana_LeadAShield", 
    m_LogicalDetector, 
    false, 
    0);
    // B
    G4ThreeVector  positionLeadBShield = G4ThreeVector(0, 0, LeadBShield_PosZ);
    G4Tubs*           solidLeadB = new G4Tubs("solidLead", 0.5*LeadBMinR, 0.5*LeadBMaxR, 0.5*LeadBLength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLeadBShield = new G4LogicalVolume(solidLeadB, PCA, "logicLeadBShield", 0, 0, 0);

    new G4PVPlacement(0, 
    positionLeadBShield, 
    logicLeadBShield, 
    "Nana_LeadBShield", 
    m_LogicalDetector, 
    false, 
    0);




    // Visualisation of PMT Strip
    G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(1., 1., 1.));
    logicLeadAShield->SetVisAttributes(LeadVisAtt);
    logicLeadBShield->SetVisAttributes(LeadVisAtt);


*/
    // Lead shielding
    // A
    G4ThreeVector  positionLeadAShield = G4ThreeVector(0, 0, LeadAShield_PosZ);
    G4Tubs* solidLeadA = new G4Tubs("solidLead", 0.5*LeadAMinR, 0.5*LeadAMaxR, 0.5*LeadALength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLeadAShield = new G4LogicalVolume(solidLeadA, Lead, "logicLeadAShield", 0, 0, 0);

    new G4PVPlacement(0, 
        positionLeadAShield, 
        logicLeadAShield, 
        "Nana_LeadAShield", 
        m_LogicalDetector, 
        false, 
        0);

    // B
    G4ThreeVector  positionLeadBShield = G4ThreeVector(0, 0, LeadBShield_PosZ);
    G4Tubs*           solidLeadB = new G4Tubs("solidLead", 0.5*LeadBMinR, 0.5*LeadBMaxR, 0.5*LeadBLength, 0.*deg, 360.*deg);
    G4LogicalVolume* logicLeadBShield = new G4LogicalVolume(solidLeadB, Lead, "logicLeadBShield", 0, 0, 0);

    new G4PVPlacement(0, 
        positionLeadBShield, 
        logicLeadBShield, 
        "Nana_LeadBShield", 
        m_LogicalDetector, 
        false, 
        0);

    // Visualisation of PMT Strip
    G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(.66, .66, .66));
    logicLeadAShield->SetVisAttributes(LeadVisAtt);
    logicLeadBShield->SetVisAttributes(LeadVisAtt);

  }

  return m_LogicalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Nana::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Nana")){
    pTree->Branch("Nana", "TNanaData", &m_Event) ;
  }
  pTree->SetBranchAddress("Nana", &m_Event) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Nana::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // LaBr3
  G4THitsMap<G4double*>* LaBr3HitMap;
  std::map<G4int, G4double**>::iterator LaBr3_itr;

  G4int LaBr3CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Nana_LaBr3Scorer/NanaLaBr3");
  LaBr3HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(LaBr3CollectionID));

  // Loop on the LaBr3 map
  for (LaBr3_itr = LaBr3HitMap->GetMap()->begin() ; LaBr3_itr != LaBr3HitMap->GetMap()->end() ; LaBr3_itr++){

    G4double* Info = *(LaBr3_itr->second);
    //(Info[0]/2.35)*((Info[0]*1.02)*pow((Info[0]*1.8),.5))
    // double Energy = RandGauss::shoot(Info[0],((Info[0]*1000*1.02/2.35)*pow((Info[0]*1000*1.8),.5)) );
    double Energy = RandGauss::shoot(Info[0],(Info[0]*0.0325637)/(2.35*pow(Info[0]-0.00975335,0.475759)));
    if(Energy>EnergyThreshold){
      double Time = Info[1];
      int DetectorNbr = (int) Info[2];

      m_Event->SetNanaLaBr3(DetectorNbr,Energy,Energy,(unsigned short) Time,0,0);
    }
  }
  // clear map for next event
  LaBr3HitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Nana::InitializeScorers(){
  vector<G4int> NestingLevel;
  NestingLevel.push_back(1);

  //   LaBr3 Associate Scorer
  bool already_exist = false;
  m_LaBr3Scorer = CheckScorer("Nana_LaBr3Scorer",already_exist);

  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* LaBr3Scorer =
    new  CALORIMETERSCORERS::PS_Calorimeter("NanaLaBr3",NestingLevel);
  //and register it to the multifunctionnal detector
  m_LaBr3Scorer->RegisterPrimitive(LaBr3Scorer);

  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_LaBr3Scorer) ;
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Nana::Construct(){
  return  (NPS::VDetector*) new Nana();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_nana{
    public:
      proxy_nps_nana(){
        NPS::DetectorFactory::getInstance()->AddToken("Nana","Nana");
        NPS::DetectorFactory::getInstance()->AddDetector("Nana",Nana::Construct);
      }
  };

  proxy_nps_nana p_nps_nana;
}
