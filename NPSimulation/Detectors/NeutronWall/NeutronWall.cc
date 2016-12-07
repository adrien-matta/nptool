/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : June 2016                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  NeutronWall simulation                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
#include <cstring>
#include <string>

//G4 Geometry object
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

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
#include "NeutronWall.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace NeutronWall_NS{
    // Energy and time Resolution
    const double EnergyThreshold = 0.1*MeV;
    /*const double ResoTime = 0*ns ;
    const double ResoEnergy = 0*MeV ;
    const double ResoPosition = 0*cm;*/
        //using Alouter minus Alinner, one get an Al frame (including front and back sheets)
    const double Alinner_X = 2811.0*mm;
    const double Alinner_Y = 2500.0*mm;
    const double Alinner_Z = 143.5*mm;
    const double Alouter_X = 2831.0*mm;
    const double Alouter_Y = 2520.0*mm;
    const double Alouter_Z = (143.5+0.8+0.8)*mm;
    
    const double frame_thickness = 10*mm;
    
    const double Scintillator_X = 1994.0*mm;
    const double Scintillator_Y = 70.2*mm;
    const double Scintillator_Z = 57.5*mm;
    //do the same thing to Pyrex tube to create a volume to store Scintillator
    
    const double Py_Xinner = 1994.0*mm;
    const double Py_Yinner = 70.2*mm;
    const double Py_Zinner = 57.5*mm;
    const double Py_Xouter = 2000.0*mm;
    const double Py_Youter = 76.2*mm;
    const double Py_Zouter = 63.5*mm;
    
    const double seperation_between_pyrex = 3.0*mm;
    const double upper_gap = 10.0*mm;
    
    //Add elements about the plastic bars
    double PlasticBar_X = 94.0*mm;
    double PlasticBar_Y = 2500.0*mm;  //2000->2500 by Kuan
    double PlasticBar_Z = 10.0*mm;
    
    double PlasticBarTip_X = 56.54*mm;
    double PlasticBarTip_Z = 113.868*mm;
    
    //The size of the most outside box(OB) which contains everything
    double OB_X = 3000*mm;
    double OB_Y = 3000*mm;
    double OB_Z = 3000*mm;
    
    
    //The size of NS should depend on the distance between NeutronWall and plastic Bar right now
    double NS_X = 24*PlasticBar_X ;//2020.0*mm;
    double NS_Y = 2020.0*mm;
    //the front and back aluminum sheet are both 0.8 thick whereas 143.5 is user assumed heigh in z
    double NS_Z = (143.5+0.8+0.8)*mm;  //(143.5+0.8+0.8)*mm

    
    //Add total height of neutronwall and vetowall for comparision
    double TotalHeightOfNeutronWall = 0.0*mm;
    double TotalHeightOfVetoWall = 0.0*mm;
    
    
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// NeutronWall Specific Method
NeutronWall::NeutronWall(){
    m_Event = new TNeutronWallData() ;
    m_NeutronWallScorer = 0;
    m_VetoWallScorer = 0;
    m_NeutronWall_out_log = 0;
    m_AlCase_log = 0;
    m_Quartz_log = 0;
    m_QuartzCap_log = 0;
    m_PMTube_log = 0;
    m_Scintillator_log = 0;
    m_ShadowBar_log = 0;
    m_PlasticBar_log = 0;
    
    
    // RGB Color + Transparency
    m_VisScintillator = new G4VisAttributes(G4Colour(1, 0.843137, 0, 1)); //gold
    m_VisQuartz = new G4VisAttributes(G4Colour(0,1, 0, 0.1)); //green
    m_VisAl = new G4VisAttributes(G4Colour(173.0/255.0,178.0/255.0,189.0/255.0,0.2)); //Al
    m_VisNW = new G4VisAttributes(G4Colour(0.972549,0.972549,1,0)); //ghostwhite
    m_VisPlasticBar = new G4VisAttributes(G4Colour(0.9,0,0.9,0.25)); //pink
}

NeutronWall::~NeutronWall(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NeutronWall::AddNeutronWall(double  R, double  Theta, double  Phi, double X, double Y, double Z, double Rotation, int Bars, string NWMaterial, double VWDistance, int VetoWall, string VWMaterial, double Overlap){
    m_R.push_back(R);
    m_Theta.push_back(Theta);
    m_Phi.push_back(Phi);
    m_X.push_back(X);
    m_Y.push_back(Y);
    m_Z.push_back(Z);
    m_Rot.push_back(Rotation);
    m_Bars.push_back(Bars);
    m_NWMaterial.push_back(NWMaterial);
    m_VWDistance.push_back(VWDistance);
    m_VetoWall.push_back(VetoWall);
    m_VWMaterial.push_back(VWMaterial);
    m_Overlap.push_back(Overlap);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NeutronWall::BuildDetector(){
    
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void NeutronWall::ReadConfiguration(NPL::InputParser parser){

  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("NeutronWall");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"R","THETA","PHI","BARS","VETOWALL","VWDISTANCE","NWMATERIAL","VWMATERIAL","OVERLAP"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Neutron Wall " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      int Bars = blocks[i]->GetInt("BARS");
      int VetoWall = blocks[i]->GetInt("VETOWALL");
      double VWDistance = blocks[i]->GetDouble("VWDISTANCE","mm");
      string NWMaterial = blocks[i]->GetString("NWMATERIAL");
      string VWMaterial = blocks[i]->GetString("VWMATERIAL");

      int Overlap = blocks[i]->GetInt("OVERLAP");
      AddNeutronWall(R,Theta,Phi,0,0,0,0, Bars, NWMaterial, VWDistance, VetoWall, VWMaterial, Overlap);
                
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
void NeutronWall::ConstructDetector(G4LogicalVolume* world){
    
    for (unsigned short i = 0 ; i < m_R.size() ; i++) {
        
        G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
        G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
        G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
        G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
        // So the face of the Scintillator Bar is at R instead of the middle
        Det_pos+=Det_pos.unit()*NeutronWall_NS::Scintillator_Z*0.5;
        // Building Detector reference frame
        G4double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
        G4double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
        G4double kk = -sin(m_Theta[i]);
        G4ThreeVector Y(ii,jj,kk);
        G4ThreeVector w = Det_pos.unit();
        G4ThreeVector u = w.cross(Y);
        G4ThreeVector v = u.cross(w);
        v = v.unit();
        u = u.unit();
        
        
        if (m_VetoWall[i] == 1){
            // If VetoWall exists, then extend the space of NeutronWall_NS add 3mm to Z in order to house the 1mm seperation between front and back layer of the veto wall
            NeutronWall_NS::NS_Z = 2.0*(m_VWDistance[i]+1.5*NeutronWall_NS::PlasticBar_Z+3.0*mm);
        }
        
        G4RotationMatrix* Rot = new G4RotationMatrix(v,u,w);
        
        G4Material* ScintMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(m_NWMaterial[i]);
        G4Material* vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
        G4Material* Aluminum = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
        G4Material* Pyrex = MaterialManager::getInstance()->GetMaterialFromLibrary("Pyrex");
        
        //Most outside Box
        G4Box* NeutronWall_out_box = new G4Box("NeutronWall_Out_Box",NeutronWall_NS::OB_X*0.5,
                                           NeutronWall_NS::OB_Y*0.5,NeutronWall_NS::OB_Z*0.5);
        m_NeutronWall_out_log = new G4LogicalVolume(NeutronWall_out_box,vacuum,"NeutronWall_Out_Log",0,0,0);
        m_NeutronWall_out_log->SetVisAttributes(m_VisNW);
        
        //Aluminum inner box (subtractee)
        G4Box* Alinner_box = new G4Box("Alinner_box",NeutronWall_NS::Alinner_X*0.5,NeutronWall_NS::Alinner_Y*0.5,NeutronWall_NS::Alinner_Z*0.5);
        //Aluminum outer box (subtracter)
        G4Box* Alouter_box = new G4Box("Alouter_box",NeutronWall_NS::Alouter_X*0.5,NeutronWall_NS::Alouter_Y*0.5,NeutronWall_NS::Alouter_Z*0.5);
        
        G4SubtractionSolid* AlCase_frame = new G4SubtractionSolid("AlCase_frame", Alouter_box, Alinner_box);
        
        m_AlCase_log = new G4LogicalVolume(AlCase_frame,Aluminum,"AlCase_Log",0,0,0);
        
        m_AlCase_log->SetVisAttributes(m_VisAl);
        
        //Quartz tube
        G4Box* Quartz_boxinner = new G4Box("Quartz_Boxinner",NeutronWall_NS::Py_Xinner*0.5,NeutronWall_NS::Py_Yinner*0.5,NeutronWall_NS::Py_Zinner*0.5);
        G4Box* Quartz_boxouter = new G4Box("Quartz_Box",NeutronWall_NS::Py_Xouter*0.5,NeutronWall_NS::Py_Youter*0.5,NeutronWall_NS::Py_Zouter*0.5);
        G4SubtractionSolid* Quartz_box = new G4SubtractionSolid("Quartz_box", Quartz_boxouter, Quartz_boxinner);
        
        m_Quartz_log = new G4LogicalVolume(Quartz_box,Pyrex,"Quartz_Log",0,0,0);
        
        m_Quartz_log->SetVisAttributes(m_VisQuartz);
        
        //??currently unused
        G4Tubs* QuartzCap_tube = new G4Tubs("QuartsCap_Tube",0,7.99*cm,0.3175*cm,0.0*deg,360.0*deg);
        m_QuartzCap_log = new G4LogicalVolume(QuartzCap_tube,Aluminum,"QuartzCap_Log",0,0,0);
        
        //Scintillator
        G4Box* Scintillator_box = new G4Box("Scintillator_Box",NeutronWall_NS::Scintillator_X*0.5,NeutronWall_NS::Scintillator_Y*0.5,NeutronWall_NS::Scintillator_Z*0.5);

        
        m_Scintillator_log = new G4LogicalVolume(Scintillator_box,ScintMaterial,"Scintillator_Log",0,0,0);
        
        m_Scintillator_log->SetVisAttributes(m_VisScintillator);
        m_Scintillator_log->SetSensitiveDetector(m_NeutronWallScorer);
        
        //Shadow bar construction??currently unused
        G4Trd* ShadowBar_trd = new G4Trd("ShadowBar_Trd",6.51/2*cm, 7.23/2*cm, 6.79/2*cm, 7.66/2*cm, 30./2*cm);
        m_ShadowBar_log = new G4LogicalVolume(ShadowBar_trd, Aluminum, "ShadowBar_Log");
        
        
        if (m_VetoWall[i] == 1){
            //PlasticBar
            G4Material* Plastic = MaterialManager::getInstance()->GetMaterialFromLibrary(m_VWMaterial[i]);
            
            
            //Plastic Bar
            G4Box* PlasticBar_main_box = new G4Box("PlasticBar_main_Box", NeutronWall_NS::PlasticBar_X*0.5, NeutronWall_NS::PlasticBar_Y*0.5-NeutronWall_NS::PlasticBarTip_Z*0.5, NeutronWall_NS::PlasticBar_Z*0.5);
            //Plastic tip
            G4Trd* PlatsticBar_up_tip = new G4Trd("Scintillator_Up_Tip",NeutronWall_NS::PlasticBarTip_X*0.5, NeutronWall_NS::PlasticBar_X*0.5, NeutronWall_NS::PlasticBar_Z*0.5, NeutronWall_NS::PlasticBar_Z*0.5, NeutronWall_NS::PlasticBarTip_Z*0.5);
            
            G4RotationMatrix* xRot = new G4RotationMatrix;  // Rotates y axes only
            xRot->rotateX(-M_PI/2.*rad);
            G4ThreeVector yTrans(0, NeutronWall_NS::PlasticBar_Y*0.5, 0);  //move it to the top of plastic bar
            G4UnionSolid* PlasticBar_box_first = new G4UnionSolid("PlasticBar_Box_first", PlasticBar_main_box, PlatsticBar_up_tip, xRot, yTrans);
            
            G4RotationMatrix* xRot_1 = new G4RotationMatrix;  // Rotates y axes only
            xRot_1->rotateX(M_PI/2.*rad);
            G4ThreeVector yTrans_1(0, -NeutronWall_NS::PlasticBar_Y*0.5, 0); //move it to the bottom of plastic bar
            G4UnionSolid* PlasticBar_box = new G4UnionSolid("PlasticBar_Box", PlasticBar_box_first, PlatsticBar_up_tip, xRot_1, yTrans_1);
            
            
	        m_PlasticBar_log = new G4LogicalVolume(PlasticBar_box, Plastic, "PlasticBar_Log");
            m_PlasticBar_log->SetSensitiveDetector(m_VetoWallScorer);
            m_PlasticBar_log->SetVisAttributes(m_VisPlasticBar);
        }
        
        
        //******************* Placement *******************//
        //----NW+VW World---
        new G4PVPlacement(G4Transform3D(*Rot, Det_pos), m_NeutronWall_out_log, "NeutronWall_out_phys",world,false,i);
        
        //----Aluminum Case----
        m_AlCase_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),m_AlCase_log,
                                          "AlCase_phys",m_NeutronWall_out_log,false,0);
        //----Scintillator and Quartz tube----
        for (int j = 0; j < 25; j++ ) {
            double CenterOfScintillator_X = 0*mm;
            double CenterOfScintillator_Y = (NeutronWall_NS::NS_Y*0.5-NeutronWall_NS::frame_thickness-NeutronWall_NS::upper_gap-NeutronWall_NS::Py_Youter*0.5-j*(NeutronWall_NS::Py_Youter+NeutronWall_NS::seperation_between_pyrex));
            double CenterOfScintillator_Z = 0*mm;
            G4ThreeVector ScintillatorDisplacement(CenterOfScintillator_X,CenterOfScintillator_Y,CenterOfScintillator_Z);
            m_ScintillatorTube_phys = new G4PVPlacement(0,ScintillatorDisplacement,m_Scintillator_log,"ScintillatorTube_phys",m_NeutronWall_out_log,false,j);
            //Quartz center coincide with Scintillator's, therefore, they have the same displacement.
            m_Quartz_phys = new G4PVPlacement(0,ScintillatorDisplacement,m_Quartz_log, "Quartz_phys",m_NeutronWall_out_log,false,j);
        }
        
        for (int j = 0; j < 23; j++){
            if (m_VetoWall[i] == 1){
                //Even number is associated with 0th 2nd 4th ... plasticbar in vetowall which comprise of the backlayer (close to NeutronWall)
                //Odd number is associated with 1st 3rd 5th ... plasticbar in vetowall which comprise of the frontlayer (close to Source)
                //double CenterOfVetoWall_Even_X = NeutronWall_NS::NS_X*0.5-10*mm-NeutronWall_NS::PlasticBar_X*0.5 - j*NeutronWall_NS::PlasticBar_X;
                double CenterOfVetoWall_Even_X = -(23*NeutronWall_NS::PlasticBar_X-22*m_Overlap[i])*0.5 + NeutronWall_NS::PlasticBar_X*0.5 +j*(NeutronWall_NS::PlasticBar_X-m_Overlap[i]);
                double CenterOfVetoWall_Even_Y = 0*mm;
                double CenterOfVetoWall_Even_Z = -(NeutronWall_NS::Alouter_Z/2 + m_VWDistance[i] + NeutronWall_NS::PlasticBar_Z);
                
                double CenterOfVetoWall_Odd_X = -(23*NeutronWall_NS::PlasticBar_X-22*m_Overlap[i])*0.5 + NeutronWall_NS::PlasticBar_X*0.5 +j*(NeutronWall_NS::PlasticBar_X-m_Overlap[i]);
                double CenterOfVetoWall_Odd_Y = 0*mm;
                double CenterOfVetoWall_Odd_Z = -(NeutronWall_NS::Alouter_Z/2 + m_VWDistance[i] + NeutronWall_NS::PlasticBar_Z) - NeutronWall_NS::PlasticBar_Z - 1*mm;
                
                if (j%2 == 0){
                    m_PlasticBar_phys = new G4PVPlacement(0,G4ThreeVector(CenterOfVetoWall_Even_X,CenterOfVetoWall_Even_Y,CenterOfVetoWall_Even_Z),m_PlasticBar_log,"PlasticBar_phys",m_NeutronWall_out_log,false,j,true);
                }
                else {
                    m_PlasticBar_phys = new G4PVPlacement(0, G4ThreeVector(CenterOfVetoWall_Odd_X,CenterOfVetoWall_Odd_Y,CenterOfVetoWall_Odd_Z),m_PlasticBar_log,"PlasticBar_phys",m_NeutronWall_out_log,false,j,true);
                }
            }
        }
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void NeutronWall::InitializeRootOutput(){
    RootOutput *pAnalysis = RootOutput::getInstance();
    TTree *pTree = pAnalysis->GetTree();
    if(!pTree->FindBranch("NeutronWall")){
      pTree->Branch("NeutronWall", "TNeutronWallData", &m_Event) ;
    }
    pTree->SetBranchAddress("NeutronWall", &m_Event) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void NeutronWall::ReadSensitive(const G4Event* event){
    m_Event->Clear();
    
    ///////////
    // Calorimeter scorer
    G4THitsMap<G4double*>* CaloHitMap;
    std::map<G4int, G4double**>::iterator Calo_itr;
    
    G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("NeutronWallScorer/Calorimeter");
    CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));
    
    // Loop on the Calo map
    for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){
        
        G4double* Info = *(Calo_itr->second);
        //double Energy = RandGauss::shoot(Info[0],NeutronWall_NS::ResoEnergy);
        double Energy = Info[0];
        if(Energy>NeutronWall_NS::EnergyThreshold){
            //double Time = RandGauss::shoot(Info[1],NeutronWall_NS::ResoTime);
            double Time = Info[1];
            int DetectorNbr = (int) Info[8];
            int PadNbr = (int) Info[7];
            //cout << Info[2] << " " << Info[3] << endl;
            m_Event->SetEnergy(DetectorNbr,PadNbr,Energy);
            m_Event->SetTime(DetectorNbr,PadNbr,Time);
            
            // Interraction Coordinates
            //Info[2]=RandGauss::shoot(Info[2],NeutronWall_NS::ResoPosition) ;
            //Info[3]=RandGauss::shoot(Info[3],NeutronWall_NS::Py_Youter/10.0) ;
            /*ms_InterCoord->SetDetectedPositionX(Info[2]) ;
            ms_InterCoord->SetDetectedPositionY(Info[3]) ;
            ms_InterCoord->SetDetectedPositionZ(Info[4]) ;*/
            ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
            ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
            
            m_Event->Set_NeutronWall_PosX(Info[2]);
            m_Event->Set_NeutronWall_PosY(Info[3]);
            m_Event->Set_NeutronWall_PosZ(Info[4]);
            
        }
    }
    // clear map for next event
    CaloHitMap->clear();
    
    ///////////
    // Veto wall scorer
    G4THitsMap<G4double*>* VetoHitMap;
    std::map<G4int, G4double**>::iterator Veto_itr;
    
    G4int VetoCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("VetoWallScorer/VetoCalorimeter");
    VetoHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(VetoCollectionID));
    
    
    // Loop on the Calo map
    for (Veto_itr = VetoHitMap->GetMap()->begin() ; Veto_itr != VetoHitMap->GetMap()->end() ; Veto_itr++){
        
        G4double* Info = *(Veto_itr->second);
        //double Energy = RandGauss::shoot(Info[0],NeutronWall_NS::ResoEnergy);
        double Energy = Info[0];
        if(Energy>NeutronWall_NS::EnergyThreshold){
            //double Time = RandGauss::shoot(Info[1],NeutronWall_NS::ResoTime);
            double Time = Info[1];
            int DetectorNbr = (int) Info[8];
            int PadNbr = (int) Info[7];
            
            m_Event->SetVetoEnergy(DetectorNbr,PadNbr,Energy);
            m_Event->SetVetoTime(DetectorNbr,PadNbr,Time);
            
            // Interraction Coordinates
            //Info[2]=RandGauss::shoot(Info[2],NeutronWall_NS::PlasticBar_X/10.0) ;
            //Info[3]=RandGauss::shoot(Info[3],NeutronWall_NS::ResoPosition) ;
            /*ms_InterCoord->SetDetectedPositionX(Info[2]) ;
            ms_InterCoord->SetDetectedPositionY(Info[3]) ;
            ms_InterCoord->SetDetectedPositionZ(Info[4]) ;*/
            ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
            ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
            
            m_Event->Set_VetoWall_PosX(Info[2]);
            m_Event->Set_VetoWall_PosY(Info[3]);
            m_Event->Set_VetoWall_PosZ(Info[4]);
        }
    }
    // clear map for next event
    VetoHitMap->clear();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
void NeutronWall::InitializeScorers() {
    // Otherwise the scorer is initialised
    vector<int> level;
    level.push_back(0);
    level.push_back(1);
    
    // This check is necessary in case the geometry is reloaded
    bool already_exist = false;
    m_NeutronWallScorer = CheckScorer("NeutronWallScorer",already_exist) ;
    
    if(already_exist)
        return ;
    
    // Neutron Wall Scorer
    G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("Calorimeter",level,1) ;
    //and register it to the multifunctional detector
    m_NeutronWallScorer->RegisterPrimitive(Calorimeter);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_NeutronWallScorer) ;
    
    
    // Veto Wall Scorer
    already_exist = false;
    m_VetoWallScorer = CheckScorer("VetoWallScorer",already_exist) ;
    if(already_exist)
        return;
    
    G4VPrimitiveScorer* VetoCalorimeter= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("VetoCalorimeter",level,1) ;
    //and register it to the multifunctional detector
    m_VetoWallScorer->RegisterPrimitive(VetoCalorimeter);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_VetoWallScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* NeutronWall::Construct(){
    return  (NPS::VDetector*) new NeutronWall();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_plastic{
    public:
        proxy_nps_plastic(){
            NPS::DetectorFactory::getInstance()->AddToken("NeutronWall","NeutronWall");
            NPS::DetectorFactory::getInstance()->AddDetector("NeutronWall",NeutronWall::Construct);
        }
    };
    
    proxy_nps_plastic p_nps_plastic;
}
