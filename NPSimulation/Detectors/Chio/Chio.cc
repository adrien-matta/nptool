/*****************************************************************************
 * Copyright (C) 2009-2016     this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta contact address: matta@lpccaen.in2p3.fr     *
 *                                                                           *
 * Creation Date  : December 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Chio simulation                                     *
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

// G4 Field
#include "G4FieldManager.hh"
#include "G4ElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4TransportationManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4MaterialPropertiesTable.hh"

// NPTool header
#include "Chio.hh"
#include "DriftElectronScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "FastDriftElectron.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

// ROOT
#include "TH1D.h"
#include "TF1.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Chio_NS{
  // Energy and time Resolution
  const double ChargeThreshold = 1;
  const double ResoTime = 4.5*ns ;
  // const double ResoEnergy = 1.0*MeV ;
  //const double Radius = 50*mm ; 
  // const double Width = 100*mm ;
  const double Thickness = 300*mm ;
  const string Material = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Chio Specific Method
Chio::Chio(){
  m_Event_an = new TChio_anData() ;
  m_Event_dig = new TChio_digData() ;

  m_ChioScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;


  // RGB Color + Transparency
  m_VisChamber = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.25));   
  m_VisWindows = new G4VisAttributes(G4Colour(1, 0, 0, 0.25));   
  m_VisGas     = new G4VisAttributes(G4Colour(0, 0.5, 0.5, 0.5));   

}

Chio::~Chio(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Chio::AddDetector(G4ThreeVector POS, string  Shape){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Chio::AddDetector(double  R, double  Theta, double  Phi, string  Shape){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Chio::BuildDetector(){
  if(!m_SquareDetector){
    // Main volume
    G4Box* sChamber = new G4Box("Chio_Box",7*cm*0.5,
        7*cm*0.5,12*cm*0.5+2*12*micrometer*0.5);

    // Gaz volume
    G4Box* sGas = new G4Box("Chio_Gas",6*cm*0.5,
        6*cm*0.5,12*cm*0.5-2*12*micrometer*0.5);

    // Frish grid
    G4Box* sGrid = new G4Box("Chio_Grid",1*um*0.5,
        6*cm*0.5,12*cm*0.5-2*12*micrometer*0.5);

    // Cathode
    G4Box* sCathode = new G4Box("Chio_Cathode",1*um*0.5,
        6*cm*0.5,12*cm*0.5-2*12*micrometer*0.5);


    // Entrance/Exit windows
    G4Box* sWindows = new G4Box("Chio_Windows",7*cm*0.5,
        7*cm*0.5,12*micrometer*0.5);


    G4Material* Fe= MaterialManager::getInstance()->GetMaterialFromLibrary("Fe");
    G4Material* Al= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

    G4Material* CF4= MaterialManager::getInstance()->GetGasFromLibrary("CF4",0.0693276*bar,273.15*kelvin);
    G4Material* Mylar= MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");

    G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();      
    MPT->AddConstProperty("DE_PAIRENERGY",30*eV);
    //  MPT->AddConstProperty("DE_AMPLIFICATION",1e4);
    MPT->AddConstProperty("DE_ABSLENGTH",1*pc);
    MPT->AddConstProperty("DE_DRIFTSPEED",11*cm/microsecond);
    MPT->AddConstProperty("DE_TRANSVERSALSPREAD",6e-5*mm2/ns);
    MPT->AddConstProperty("DE_LONGITUDINALSPREAD",4e-5*mm2/ns);

    CF4->SetMaterialPropertiesTable(MPT);

    G4MaterialPropertiesTable* MPT2 = new G4MaterialPropertiesTable();      
    MPT2->AddConstProperty("DE_YIELD",1);
    MPT2->AddConstProperty("DE_AMPLIFICATION",2);
    MPT2->AddConstProperty("DE_ABSLENGTH",1*pc);

    Al->SetMaterialPropertiesTable(MPT2);

    m_SquareDetector = new G4LogicalVolume(sChamber,Fe,"logic_Chio_Box",0,0,0);
    G4LogicalVolume* logicGas = new G4LogicalVolume(sGas,CF4,"logic_Gas",0,0,0);
    G4LogicalVolume* logicGrid = new G4LogicalVolume(sGrid,Al,"logic_Grid",0,0,0);
    G4LogicalVolume* logicCathode = new G4LogicalVolume(sCathode,Fe,"logic_Cathode",0,0,0);
    G4LogicalVolume* logicWindows = new G4LogicalVolume(sWindows,Mylar,"logic_Windows",0,0,0);

    G4RotationMatrix* Rot = new G4RotationMatrix();

    new G4PVPlacement(G4Transform3D(*Rot,G4ThreeVector(0,0,0)),
        logicGas,
        "ChioGas",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,G4ThreeVector(2.5*cm,0,0)),
        logicGrid,
        "ChioGrid",logicGas,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,G4ThreeVector(3*cm-0.5*1*um,0,0)),
        logicCathode,
        "ChioCathode",logicGas,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,G4ThreeVector(0,0,6*cm-6*micrometer)),
        logicWindows,
        "ChioExitWindows",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,G4ThreeVector(0,0,-6*cm+6*micrometer)),
        logicWindows,
        "ChioEntranceWindows",m_SquareDetector,false,0);

    G4ElectricField* field = new G4UniformElectricField(G4ThreeVector(0.0,-1000*volt/cm,0.0));
    // Create an equation of motion for this field
    G4EqMagElectricField*  Equation = new G4EqMagElectricField(field); 
    G4MagIntegratorStepper* Stepper = new G4ClassicalRK4( Equation, 8 );       

    // Get the global field manager 
    G4FieldManager* FieldManager= new G4FieldManager();
    // Set this field to the global field manager 
    FieldManager->SetDetectorField(field );
    logicGas->SetFieldManager(FieldManager,true);

    G4MagInt_Driver* IntgrDriver = new G4MagInt_Driver(0.1*mm, 
        Stepper, 
        Stepper->GetNumberOfVariables() );

    G4ChordFinder* ChordFinder = new G4ChordFinder(IntgrDriver);
    FieldManager->SetChordFinder( ChordFinder );

    logicCathode->SetSensitiveDetector(m_ChioScorer); 
    m_SquareDetector->SetVisAttributes(m_VisChamber);
    logicGas->SetVisAttributes(m_VisGas);
    logicWindows->SetVisAttributes(m_VisWindows);
    m_SquareDetector->SetSensitiveDetector(m_ChioScorer);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Chio::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Chio");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Chio " << i+1 <<  endl;

      G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(Pos,Shape);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Chio " << i+1 <<  endl;
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
void Chio::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit()*Chio_NS::Thickness*0.5;
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

    if(m_Shape[i] == "Square"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildDetector(),
          "Chio",world,false,i+1);
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Chio::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("ChioAn")){
    pTree->Branch("ChioAn", "TChio_anData", &m_Event_an) ;
  }
  pTree->SetBranchAddress("ChioAn", &m_Event_an) ;

 ///////////////
 if(!pTree->FindBranch("ChioDig")){
    pTree->Branch("ChioDig", "TChio_digData", &m_Event_dig) ;
  }
  pTree->SetBranchAddress("ChioDig", &m_Event_dig) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Chio::ReadSensitive(const G4Event* event){
  m_Event_an->Clear();

  ///////////
  // Cathode analogic scorer
  NPS::HitsMap<G4double*>* CathodeHitMap;
  std::map<G4int, G4double**>::iterator Cathode_itr;

  G4int CathodeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ChioScorer/Cathode_an");
  CathodeHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CathodeCollectionID));

  // Loop on the Cathode map
  for (Cathode_itr = CathodeHitMap->GetMap()->begin() ; Cathode_itr != CathodeHitMap->GetMap()->end() ; Cathode_itr++){
    G4double* Info = *(Cathode_itr->second);
    double Count= Info[0];
    if(Count>Chio_NS::ChargeThreshold-1){
      double Time = RandGauss::shoot(Info[1],Chio_NS::ResoTime);
      m_Event_an->SetEnergyAndTime(Count,Time,Info[2]);
    }
  }
  // clear map for next event
  CathodeHitMap->clear();

  m_Event_dig->Clear();

  ///////////
  CathodeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ChioScorer/Cathode_dig");
  CathodeHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CathodeCollectionID));

  // Loop on the Cathode map
  TH1D* h = new TH1D("h","h",25000,0,25000);
  for (Cathode_itr = CathodeHitMap->GetMap()->begin() ; Cathode_itr != CathodeHitMap->GetMap()->end() ; Cathode_itr++){
    G4double* Info = *(Cathode_itr->second);
    if(Info[0]){
      h->Fill(Info[1],Info[0]);
    }
  }

  vector<double> E,T;
  for(int i = 0 ; i < h->GetNbinsX() ; i++){
    double count = h->GetBinContent(i);
    double time  = h->GetBinCenter(i);
    if(count)
  //  m_Event_dig->AddEnergyPoint(count,time);
    E.push_back(count);
    T.push_back(time+500);
  }

  SimulateDigitizer(E,T,1.40*microsecond,0,8750,25,5);

  delete h;
  // clear map for next event
  CathodeHitMap->clear();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Chio::SimulateDigitizer(vector<double> E, vector<double> T, double fallTime,double start,double stop, double step,double noise){

  static string formula; 
  formula= "";
  static string Es,Ts,var,cond;
  static string fall;
  fall=std::to_string(fallTime);

  for(unsigned int i = 0 ; i < E.size() ; i++){
    if(E[i]!=0 && T[i]!=0){
      Es = std::to_string(E[i]);
      Ts = std::to_string(T[i]);
      cond = ")*(x>"+Ts+")+";
      var = "(x-"+Ts+")";
      formula += Es+"*-1*exp(-"+var+"/"+fall+cond;
    }
  }
  formula+="0";
  TF1* f = new TF1("f",formula.c_str(),start,stop);  
  unsigned int size = (stop-start)/step;
  for(unsigned int i = 0 ; i < size ; i++){
    double time = start+i*step;
    double energy = f->Eval(time)+noise*(1-2*G4UniformRand());
    m_Event_dig->AddEnergyPoint(energy,time);
  }
  
  delete f;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Chio::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_ChioScorer = CheckScorer("ChioScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  G4VPrimitiveScorer* Cathode_an= new DRIFTELECTRONSCORERS::PS_DECathode("Cathode_an",0) ;
  G4VPrimitiveScorer* Cathode_dig= new DRIFTELECTRONSCORERS::PS_DEDigitizer("Cathode_dig",0) ;

  //and register it to the multifunctionnal detector
  m_ChioScorer->RegisterPrimitive(Cathode_an);
  m_ChioScorer->RegisterPrimitive(Cathode_dig);

  G4SDManager::GetSDMpointer()->AddNewDetector(m_ChioScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Chio::Construct(){
  return  (NPS::VDetector*) new Chio();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Chio{
    public:
      proxy_nps_Chio(){
        NPS::DetectorFactory::getInstance()->AddToken("Chio","Chio");
        NPS::DetectorFactory::getInstance()->AddDetector("Chio",Chio::Construct);
      }
  };

  proxy_nps_Chio p_nps_Chio;
}
