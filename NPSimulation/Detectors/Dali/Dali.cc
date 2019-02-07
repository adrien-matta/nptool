/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: E. Tronchin                                              *
 * contact address: elidiano.tronchin@studenti.unipd.it                      *
 *                                                                           *
 * Creation Date  : septembre 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Dali simulation                                     *
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

//G4 Geometry object
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
// #ifndef G4UEXTRUDEDSOLID_hh
// #define G4UEXTRUDEDSOLID_hh
// #include "G4USolid.hh"
//  #if ( defined(G4GEOM_USE_USOLIDS) || defined(G4GEOM_USE_PARTIAL_USOLIDS) )

//#include "G4UExtrudedSolid.hh"
#include "G4TwoVector.hh"
#include "G4TessellatedSolid.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
//#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// NPTool header
#include "Dali.hh"
#include "CalorimeterScorers.hh"
#include "InteractionScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "NPSHitsMap.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Dali_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0*MeV;
  const double ResoTime = 0.0*ns; //4.5*ns ;
  const double ResoEnergy = 0.51*MeV ; // mean Resolution(FWHM) 1.7% of 80MeV from slides 20170214-SAMURAI34-setup-DALI.pdf if 1.7% of 30MeV = 0.51 MeV // 0.001*MeV ;
  const double Radius = 50*mm ; 
  const double Width = 49.76*mm ;
  const double Hight = 84.81*mm ;
  const double Thickness = 164.82*mm ;
  const double LengthPMT = 152.62*mm ;
  const string Material = "NaI";   
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Dali Specific Method
Dali::Dali(){
  m_Event = new TDaliData() ;
  m_DaliScorer = 0;
  m_SquareDetector = 0;
  m_SquareDetector_Can = 0;
  m_SquareDetector_CanMgO =0;
  m_CylindricalDetector = 0;
  m_SquareDetector_Crystal = 0;
  Logic_ArrayDali_1 =0;
  
  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 1/*, 0.3*/));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1/*, 0.3*/));   

}

Dali::~Dali(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Dali::AddDetector(G4ThreeVector POS, string  Shape){
  // Convert the POS value to R theta Phi as Cylindrical coordinate is easier in G4 
  m_R.push_back(POS.perp());
  m_Alpha.push_back(POS.phi());
  m_Zeta.push_back(POS.y());
  m_Shape.push_back(Shape);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Dali::AddDetector(double  R, double  Theta, double  Phi, string  Shape){

  double m_r, m_alpha, m_zeta;
  
  m_r = R*cos(Phi);
  m_alpha = Theta;
  m_zeta = R*sin(Phi);
       
  m_R.push_back(m_r);
  m_Alpha.push_back(m_alpha);
  m_Zeta.push_back(m_zeta);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Dali::AddDetector2(double  R, double  Alpha, double  Zeta, string  Shape){
  m_R.push_back(R);
  m_Alpha.push_back(Alpha);
  m_Zeta.push_back(Zeta);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Definition Materials MgO and NaI(Tl)

 void Dali::DefinitionMaterials()
 {
    //G4Element* H       = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
    
    G4Isotope* Mg24 = new G4Isotope ("Mg24", 12, 24, 23.985041*g/mole);
    G4Isotope* Mg25 = new G4Isotope ("Mg25", 12, 25, 24.985836*g/mole);
    G4Isotope* Mg26 = new G4Isotope ("Mg26", 12, 26, 25.982592*g/mole);
    G4Element* Mg= new G4Element("elMagnesium","Mg",3);
    Mg->AddIsotope(Mg24, 78.99*perCent);
    Mg->AddIsotope(Mg25, 10*perCent);
    Mg->AddIsotope(Mg26, 11.01*perCent);

    G4Isotope* O16 = new G4Isotope ("O16", 8, 16, 15.99*g/mole);
    G4Isotope* O17 = new G4Isotope ("O17", 8, 17, 17.00*g/mole);
    G4Isotope* O18 = new G4Isotope ("O18", 8, 18, 18.00*g/mole);
    G4Element* O = new G4Element("elOxygen","O",3);
    O->AddIsotope(O16, 99.76*perCent);
    O->AddIsotope(O17, 0.04*perCent);
    O->AddIsotope(O18, 0.20*perCent);

    
    MgO = new G4Material("MgO",3.6*g/cm3, 2 );
    MgO->AddElement(Mg,1);
    MgO->AddElement(O, 1);
    
    G4Element *elTl = new G4Element("Thallium","Tl",81.,204.383*g/mole );
    
    NaI_Tl = new G4Material("NaI_Tl",3.6667*g/cm3, 2);
    NaI_Tl->AddMaterial(MaterialManager::getInstance()->GetMaterialFromLibrary("NaI"), 99.6*perCent);
    NaI_Tl->AddElement(elTl, 0.4*perCent);

 }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4LogicalVolume* Dali::BuildSquareDetector(){
  if(!m_SquareDetector){

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


    G4Box* box_3can = new G4Box("Dali_3BoxCan", Dali_NS::Hight*0.5,
                                Dali_NS::Width*0.5*3, Dali_NS::Thickness*0.5 + Dali_NS::LengthPMT/2.+11.5/2.*mm /*last part is PMTVolume*/ );
    G4Material* Aria = MaterialManager::getInstance()->GetMaterialFromLibrary("Air");
    Logic_ArrayDali_1 = new G4LogicalVolume(box_3can,Aria,"logic_ArrayDali",0,0,0);

    Logic_ArrayDali_1->SetVisAttributes(G4VisAttributes(G4Colour(1,1,1, 0)));

    
    G4Box* box_can = new G4Box("Dali_BoxCan", Dali_NS::Hight*0.5,
                               Dali_NS::Width*0.5, Dali_NS::Thickness*0.5);

    G4Box* box_canandPMT = new G4Box("Dali_BoxCan", Dali_NS::Hight*0.5,
                               Dali_NS::Width*0.5, Dali_NS::Thickness*0.5 + Dali_NS::LengthPMT/2.+11.5/2.*mm /*last part is PMTVolume*/ );
 
    std::vector<G4TwoVector> polygon;
    polygon.push_back(G4TwoVector(Dali_NS::Hight*0.5, Dali_NS::Width*0.5*3.  )  ) ;
    polygon.push_back(G4TwoVector(Dali_NS::Hight*0.5, -Dali_NS::Width*0.5*3.  )  ) ;
    polygon.push_back(G4TwoVector(-Dali_NS::Hight*0.5, -Dali_NS::Width*0.5*3.  )  ) ;
    polygon.push_back(G4TwoVector(-Dali_NS::Hight*0.5, Dali_NS::Width*0.5*3.  )  ) ;

    // std::vector<ZSection> zsection;
    // zsection.push_back(ZSection (Dali_NS::Thickness*0.5, {0,0}, 1. ) );
    // zsection.push_back(ZSection (-Dali_NS::Thickness*0.5-19.5*2.*mm , {0,0}, 1. ) );

    
    G4Box* Extrudedbox_can  = new G4Box("Dali_BoxCan", Dali_NS::Hight*0.5,Dali_NS::Width*0.5, Dali_NS::LengthPMT/2.+11.5/2.*mm);

    AriaExtrude = new G4LogicalVolume(Extrudedbox_can,Aria, "logic_Ariaextrude",0,0,0);

    G4Material* DetectorCanMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    m_SquareDetector_Can = new G4LogicalVolume(box_can,DetectorCanMaterial,"logic_Dali_Can",0,0,0);
    m_Square2Detector_Can = new G4LogicalVolume(box_canandPMT, Aria,"logic_Dali_CanandPMT",0,0,0);


    //THE PMT
    G4Tubs* AlPMT = new G4Tubs("AlPMT",16.5*mm, 19.5*mm,Dali_NS::LengthPMT/2.,0*deg,360*deg);
    G4Tubs* MuPMT = new G4Tubs("MuPMT",16.5*mm,20.*mm,Dali_NS::LengthPMT/2.,0*deg,360*deg);
    G4Box* TopPlatePMT = new G4Box("TopPlatePMT",  Dali_NS::Hight*0.5-1*mm,
                                  Dali_NS::Width*0.5-1*mm, 11.5/2.*mm );
    G4Tubs* GlassPMT = new G4Tubs("GlassPMT", 0. , 16.5*mm , 11.5/2.*mm ,0*deg,360*deg);

    lAlPMT = new G4LogicalVolume(AlPMT, DetectorCanMaterial ,"lAlPMT",0,0,0);
    lMuPMT = new G4LogicalVolume(MuPMT, DetectorCanMaterial ,"lMuPMT",0,0,0);
    lTopPlatePMT = new G4LogicalVolume(TopPlatePMT, DetectorCanMaterial ,"lTopPlatePMT",0,0,0);
    lGlassPMT = new G4LogicalVolume(GlassPMT , MaterialManager::getInstance()->GetMaterialFromLibrary("Borosillicate_Glass") ,"lGlassPMT",0,0,0);


    G4VisAttributes* Can_Attributes = new G4VisAttributes(G4Colour(0.5,0.5,0.5, .3));
    m_SquareDetector_Can->SetVisAttributes(Can_Attributes);
    m_Square2Detector_Can->SetVisAttributes(G4VisAttributes(G4Colour(1,1,1,0)));
                                                
        AriaExtrude->SetVisAttributes(G4VisAttributes(G4Colour(1,1,1,0)));
        lAlPMT->SetVisAttributes(Can_Attributes);
        lMuPMT->SetVisAttributes(Can_Attributes);
        lTopPlatePMT->SetVisAttributes(Can_Attributes);
          
    G4Box* box_MgO = new G4Box("Dali_BoxMgO", Dali_NS::Hight*0.5-1*mm,
                               Dali_NS::Width*0.5-1*mm, Dali_NS::Thickness*0.5-1*mm);    // Size of Al Can but w/o thickness of AlCan
 
    m_SquareDetector_CanMgO = new G4LogicalVolume(box_MgO,MgO,"logic_Dali_CanMg0",0,0,0);

    G4Box* box_crystal = new G4Box("Dali_BoxNaI", Dali_NS::Hight*0.5-2.4*mm,
                           Dali_NS::Width*0.5-2.4*mm, Dali_NS::Thickness*0.5-2.4*mm); // Size of AlCan but w/o thickness of AlCan and MgO
    
    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Dali_NS::Material);

    m_SquareDetector_Crystal = new G4LogicalVolume(box_crystal,
                                                   NaI_Tl,
                                                   // DetectorMaterial,
                                                   "logic_Dali_Box",0,0,0);
 
    G4ThreeVector positionnull = G4ThreeVector(0,0,0);


     // PMT Volume -
    new G4PVPlacement(0, positionnull,
                      lAlPMT ,
                      "AlPMT",
                      lMuPMT,
                      false,
                      0);
    new G4PVPlacement(0, G4ThreeVector(0,0, -11.5/2.*mm ),
                      lMuPMT ,
                      "MuPMT",
                      AriaExtrude,
                      false,
                      0);
    
    
    new G4PVPlacement(0, positionnull,
                      lGlassPMT,
                      "GlassPMT",
                      lTopPlatePMT,
                      false,
                      0);
    new G4PVPlacement(0,  G4ThreeVector(0,0, Dali_NS::LengthPMT/2. ),
                      lTopPlatePMT,
                      "TopPlatePMT",
                      AriaExtrude,
                      false,
                      0);



    new G4PVPlacement(0,  G4ThreeVector(0,0, -Dali_NS::Thickness*0.5 ),
                      AriaExtrude,
                      "PMTVolume",
                      m_Square2Detector_Can,
                      false,
                      0);




    new G4PVPlacement(0,  G4ThreeVector(0,0, Dali_NS::LengthPMT/2.+11.5/2.*mm ),
                      m_SquareDetector_Can,
                      "DetectorVolume",
                      m_Square2Detector_Can,
                      false,
                      0);




    
                                                
    
    // MgO Volume -
    new G4PVPlacement(0, positionnull,
                                             m_SquareDetector_CanMgO,
                                             "MgO",
                                             m_SquareDetector_Can,
                                             false,
                                             0); 
    G4VisAttributes* MgO_Attributes = new G4VisAttributes(G4Colour(1,1,1, .3));
    m_SquareDetector_CanMgO->SetVisAttributes(MgO_Attributes);

    
    // NaI Volume -
    new G4PVPlacement(0, positionnull,
                                                 m_SquareDetector_Crystal,
                                                 "CrystalNaI",
                                                 m_SquareDetector_CanMgO,
                                                 false,
                                                 0); 

    m_SquareDetector_Crystal->SetVisAttributes(m_VisSquare);
    m_SquareDetector_Crystal->SetSensitiveDetector(m_DaliScorer);


    // new G4PVPlacement(0, positionnull,
    //                                              m_SquareDetector_Crystal,
    //                                              "CrystalNaI",
    //                                              m_SquareDetector_CanMgO,
    //                                              false,
    //                                              0); 


                                                
    new G4PVReplica("DaliArrayElement",
                                                        m_Square2Detector_Can,
                                                        Logic_ArrayDali_1 ,
                                                        kYAxis,
                                                        3,
                                                        Dali_NS::Width,    //?????????
                                                        0);


       
  }
  
  return Logic_ArrayDali_1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Dali::BuildCylindricalDetector(){
  if(!m_CylindricalDetector){
    G4Tubs* tub = new G4Tubs("Dali_Cyl",0,Dali_NS::Radius, Dali_NS::Thickness*0.5,0,360*deg);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Dali_NS::Material);
    m_CylindricalDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_Dali_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_DaliScorer);

  }
  return m_CylindricalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Dali::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Dali");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};
  vector<string> cyli = {"R","Alpha","Zeta","Shape"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Dali " << i+1 <<  endl;
    
      G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(Pos,Shape);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Dali " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(R,Theta,Phi,Shape);
    }
    else if(blocks[i]->HasTokenList(cyli)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Dali " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Alpha = blocks[i]->GetDouble("Alpha","deg");
      double Zeta = blocks[i]->GetDouble("Zeta","mm");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector2(R,Alpha,Zeta,Shape);
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
void Dali::ConstructDetector(G4LogicalVolume* world){    


  DefinitionMaterials();
  
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * cos(m_Alpha[i] ) ; 
    G4double wY = m_R[i] * sin(m_Alpha[i] ) ;
    G4double wZ = m_Zeta[i];
    if(m_Zeta[i]<0) wZ = wZ - Dali_NS::LengthPMT/2.+11.5/2.*mm;
    else wZ = wZ + Dali_NS::LengthPMT/2.+11.5/2.*mm;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;


    
    G4RotationMatrix* Rot = new G4RotationMatrix();




    Rot->rotateX(180*deg);
    if(m_Zeta[i]<0){
      Rot->rotateY(180*deg); Rot->rotateZ(m_Alpha[i]); 
    } else{Rot->rotateZ(m_Alpha[i]);}

   
    if(m_Shape[i] == "Cylindrical"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildCylindricalDetector(),
          "Dali",world,false,i+1);
    }

    else if(m_Shape[i] == "Square"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
                        BuildSquareDetector(),
          "Dali",world,false,i+1);

     
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Dali::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Dali")){
    pTree->Branch("Dali", "TDaliData", &m_Event) ;
  }
  pTree->SetBranchAddress("Dali", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Dali::ReadSensitive(const G4Event* ){
   m_Event->Clear();
  ///////////

  
  // Calorimeter scorer
  CalorimeterScorers::PS_Calorimeter* Scorer= (CalorimeterScorers::PS_Calorimeter*) m_DaliScorer->GetPrimitive(0);

  //cout << m_DaliScorer->GetNumberOfPrimitives()<<endl;

  
  unsigned int size = Scorer->GetMult();
  for(unsigned int i = 0 ; i < size ; i++){
    vector<unsigned int> level = Scorer->GetLevel(i); 
    double Energy = RandGauss::shoot(Scorer->GetEnergy(i),Dali_NS::ResoEnergy);
    // cout << Energy << endl;
    if(Energy>Dali_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Scorer->GetTime(i),Dali_NS::ResoTime);
      int ArrayNbr = level[1];
      int DetectinsArrayNbr = level[0]+1;
      int DetectorNbr = (ArrayNbr-1)*3+DetectinsArrayNbr;
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Dali::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false;
  vector<int> NestingLevel;
  NestingLevel.push_back(3);
  NestingLevel.push_back(4);
  
  m_DaliScorer = CheckScorer("DaliScorer",already_exist) ;

  if(already_exist) //Necessary?
    return ;  //Necessary?

  // Otherwise the scorer is initialised
//  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CalorimeterScorers::PS_Calorimeter("Calorimeter", NestingLevel) ;
  G4VPrimitiveScorer* Interaction= new InteractionScorers::PS_Interactions("Interaction",ms_InterCoord, 0) ;
  //and register it to the multifunctionnal detector
  m_DaliScorer->RegisterPrimitive(Calorimeter);
  m_DaliScorer->RegisterPrimitive(Interaction);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_DaliScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Dali::Construct(){
  return  (NPS::VDetector*) new Dali();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Dali{
    public:
      proxy_nps_Dali(){
        NPS::DetectorFactory::getInstance()->AddToken("Dali","Dali");
        NPS::DetectorFactory::getInstance()->AddDetector("Dali",Dali::Construct);
      }
  };

  proxy_nps_Dali p_nps_Dali;
}
