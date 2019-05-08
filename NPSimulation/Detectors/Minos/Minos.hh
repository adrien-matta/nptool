#ifndef Minos_h
#define Minos_h 1
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

// C++ header
#include <string>
#include <vector>
using namespace std;

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VFastSimulationModel.hh"
#include "G4UserLimits.hh"
#include "G4FastSimulationManager.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TMinosData.h"
#include "NPInputParser.h"
#include "Decay.hh"
#include "BeamReaction.hh"

class Minos : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Minos() ;
    virtual ~Minos() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Cartesian
    void AddDetector(G4ThreeVector POS, string Shape);
    // Spherical
    void AddDetector(double R,double Theta,double Phi,string Shape);  
    // With TargetLenght
    void AddDetector(double R,double Theta,double Phi, double TargetLength);
  // With TargetLenght
    void AddDetector(G4ThreeVector POS, double TargetLength);
 
  private:
  //For material definition
    void DefineMaterials();
  public:
    void SetTargetMaterial(G4String materialChoice);
    void SetChamberMaterial(G4String materialChoice);
    void SetTPCMaterial(G4String materialChoice);
    void SetWindowMaterial(G4String materialChoice);
    void SetInnerRohacellMaterial(G4String materialChoice);
    void SetOuterRohacellMaterial(G4String materialChoice);
    void SetKaptonMaterial(G4String materialChoice);


           
    G4LogicalVolume* BuildSquareDetector();
    G4LogicalVolume* BuildCylindricalDetector();
    G4LogicalVolume* BuildTarget();
    G4LogicalVolume* BuildChamber();
    G4LogicalVolume* BuildInnerRohacell();
    G4LogicalVolume* BuildOuterRohacell();
    G4LogicalVolume* BuildOuterOuterRohacell();
    G4LogicalVolume* BuildKapton();
    G4LogicalVolume* BuildOuterKapton();
    G4LogicalVolume* BuildTPC();
    G4LogicalVolume* BuildWindow0();
    G4LogicalVolume* BuildWindow1();
    G4LogicalVolume* BuildWindow2();
 
 public:
     
     G4double    GetTargetLength()      {return TargetLength*2.;};
     G4Material* GetTargetMaterial()    {return TargetMaterial;};
     
     G4double    GetTargetRadius()      {return TargetRadius;};

  /*
     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
     const G4VPhysicalVolume* GetTarget()     {return physiTarget;};
     const G4VPhysicalVolume* GetChamber()    {return physiChamber;};
     const G4VPhysicalVolume* GetTPC()        {return physiTPC;};
     const G4VPhysicalVolume* GetWindow0()     {return physiWindow0;};
     const G4VPhysicalVolume* GetWindow1()     {return physiWindow1;};                 
     const G4VPhysicalVolume* GetWindow2()     {return physiWindow2;}; 
     const G4VPhysicalVolume* GetInnerRohacell()        {return physiInnerRohacell;};
     const G4VPhysicalVolume* GetOuterRohacell()        {return physiOuterRohacell;};
     const G4VPhysicalVolume* GetKapton()        {return physiKapton;};
  */
  
  private:
     G4Material*        TargetMaterial;
     G4double           TargetRadius;
     G4double           TargetLength;

     G4Material*        WindowMaterial;
     G4double           WindowThickness;
     
     G4Material*        ChamberMaterial;
     G4double           ChamberInnerRadius;
     G4double           ChamberLength;
     G4double           ChamberThickness;
    
     G4Material*        InnerRohacellMaterial;
     G4double           InnerRohacellThickness;

     G4Material*        OuterRohacellMaterial;
     G4double           OuterRohacellThickness;
     
     G4Material*        KaptonMaterial;
     G4double           KaptonThickness;
     
     G4Material*        TPCMaterial;
     G4double           TPCRadiusExt;
         
     G4Material*        defaultMaterial;
     G4double           WorldSizeXY;
     G4double           WorldSizeZ;

     G4double           AnodeThickness;
  
    G4LogicalVolume* m_SquareDetector;
    G4LogicalVolume* m_CylindricalDetector;

  // G4Box*             solidWorld;    //pointer to the solid World 
  // G4LogicalVolume*   logicWorld;    //pointer to the logical World
  // G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     G4Tubs*             solidTarget;   
     G4LogicalVolume*   logicTarget;   
  // G4VPhysicalVolume* physiTarget;   
     
     G4Tubs*             solidChamber;  
     G4LogicalVolume*   logicChamber;  
  // G4VPhysicalVolume* physiChamber;  
         
     G4Tubs*             solidTPC; 
     G4LogicalVolume*   logicTPC; 
  // G4VPhysicalVolume* physiTPC; 
 
  ////////////////
     G4Tubs*             solidWindow0; 
     G4LogicalVolume*   logicWindow0; 
  // G4VPhysicalVolume* physiWindow0; 
     
     G4Tubs*             solidWindow1; 
     G4LogicalVolume*   logicWindow1; 
  // G4VPhysicalVolume* physiWindow1; 

     G4Tubs*             solidWindow2; 
     G4LogicalVolume*   logicWindow2; 
  // G4VPhysicalVolume* physiWindow2; 
    
     G4Tubs*             solidInnerRohacell;   
     G4LogicalVolume*   logicInnerRohacell;   
  // G4VPhysicalVolume* physiInnerRohacell;   
     
     G4Tubs*             solidOuterRohacell;   
     G4LogicalVolume*   logicOuterRohacell;   
  // G4VPhysicalVolume* physiOuterRohacell;   
     
     G4Tubs*             solidKapton;   
     G4LogicalVolume*   logicKapton;   
  // G4VPhysicalVolume* physiKapton;   
     
      
  
    
    ////////////////////////////////////////////////////
    //////  Inherite from NPS::VDetector class /////////
    ////////////////////////////////////////////////////
  public:
    // Read stream at Configfile to pick-up parameters of detector (Position,...)
    // Called in DetecorConstruction::ReadDetextorConfiguration Method
    void ReadConfiguration(NPL::InputParser) ;

    // Construct detector and inialise sensitive part.
    // Called After DetecorConstruction::AddDetector Method
    void ConstructDetector(G4LogicalVolume* world) ;

    // Add Detector branch to the EventTree.
    // Called After DetecorConstruction::AddDetector Method
    void InitializeRootOutput() ;

    // Read sensitive part and fill the Root tree.
    // Called at in the EventAction::EndOfEventAvtion
    void ReadSensitive(const G4Event* event) ;

  public:   // Scorer
    //   Initialize all Scorer used by the MUST2Array
    void InitializeScorers() ;

    //   Associated Scorer
    G4MultiFunctionalDetector* m_MinosTargetScorer ;
    G4MultiFunctionalDetector* m_MinosTPCScorer ;
    G4MultiFunctionalDetector* m_MinosPadScorer ;
  
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TMinosData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 

  //   Target Length
    vector<double>   m_TargetLength;

    // Visualisation Attribute
    G4VisAttributes* m_VisSquare;
    G4VisAttributes* m_VisCylinder;


  private:
    // Region were reaction can occure:
    G4Region* m_ReactionRegion;
    vector<G4VFastSimulationModel*> m_ReactionModel;


  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};

class PadParameterisation : public G4VPVParameterisation
{
 public:
 PadParameterisation(){};
 ~PadParameterisation(){};
 void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const{
   // Note: copyNo will start with zero!
    int PadsPerRing[18]={144,152,156,164,172,176,184,192,196,204,212,216,224,228,236,244,248,256};  
    G4int Ring = 0;
    if  (copyNo<144){
        Ring = 0;}
    else if (144<=copyNo && copyNo<296){
        Ring = 1;}
    else if (296<=copyNo && copyNo<452){
        Ring = 2;}
    else if (452<=copyNo && copyNo<616){
        Ring = 3;}
    else if (616<=copyNo && copyNo<788){
        Ring = 4;}
    else if (788<=copyNo && copyNo<964){
        Ring = 5;}
    else if (964<=copyNo && copyNo<1148){
        Ring = 6;}
    else if (1148<=copyNo && copyNo<1340){
        Ring = 7;}
    else if (1340<=copyNo && copyNo<1536){
        Ring = 8;}
    else if (1536<=copyNo && copyNo<1740){
        Ring = 9;}
    else if (1740<=copyNo && copyNo<1952){
        Ring = 10;}
    else if (1952<=copyNo && copyNo<2168){
        Ring = 11;}
    else if (2168<=copyNo && copyNo<2392){
        Ring = 12;}
    else if (2392<=copyNo && copyNo<2620){
        Ring = 13;}
    else if (2620<=copyNo && copyNo<2856){
        Ring = 14;}
    else if (2856<=copyNo && copyNo<3100){
        Ring = 15;}
    else if (3100<=copyNo && copyNo<3348){
        Ring = 16;}
    else if (3348<=copyNo && copyNo<3604){
        Ring = 17;}
 
    G4double R = (45.2+ (Ring+0.5)*2.1)*mm;
    G4double dPhi= (2.*M_PI/PadsPerRing[Ring]);

    G4double Phi = copyNo*dPhi;
    G4double Xposition = R*cos(Phi);
    G4double Yposition = R*sin(Phi);
    G4ThreeVector origin(Xposition,Yposition,0);
    physVol->SetTranslation(origin);
    //Rotation for trapezes
    G4RotationMatrix* Rot = new G4RotationMatrix();
    Rot->rotateY(-90*deg);
    Rot->rotateX(+Phi);
    physVol->SetRotation(Rot);
    //Rotation for boxes
    /* G4RotationMatrix* Rot2 = new G4RotationMatrix(); */
    /* Rot2->rotateZ(-20*deg); */
    /* physVol->SetRotation(Rot2); */
 } 

}; 



/* class PadParameterisation : public G4VPVParameterisation */
/* { */

/*   public: */

/*       PadParameterisation();//G4int NoPads, G4double StartZ, G4double Spacing); */

/*     ~PadParameterisation(); */
     
/*     void ComputeTransformation(const G4int copyNo, */
/*                              G4VPhysicalVolume *physVol) const; */
   
/* }; */


#endif
