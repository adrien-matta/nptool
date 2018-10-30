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

// NPTool header
#include "NPSVDetector.hh"
#include "TMinosData.h"
#include "NPInputParser.h"

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
    G4LogicalVolume* BuildKapton();
    G4LogicalVolume* BuildTPC();
    G4LogicalVolume* BuildWindow0();
    G4LogicalVolume* BuildWindow1();
    G4LogicalVolume* BuildWindow2();
  
  public:
     
     G4double    GetTargetLength()      {return TargetLength*2.;};
     G4Material* GetTargetMaterial()    {return TargetMaterial;};
     
     G4double    GetTargetRadius()      {return TargetRadius;};
     
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

  
    G4LogicalVolume* m_SquareDetector;
    G4LogicalVolume* m_CylindricalDetector;

     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     G4Tubs*             solidTarget;   
     G4LogicalVolume*   logicTarget;   
     G4VPhysicalVolume* physiTarget;   
     
     G4Tubs*             solidChamber;  
     G4LogicalVolume*   logicChamber;  
     G4VPhysicalVolume* physiChamber;  
         
     G4Tubs*             solidTPC; 
     G4LogicalVolume*   logicTPC; 
     G4VPhysicalVolume* physiTPC; 
     
     G4Tubs*             solidWindow0; 
     G4LogicalVolume*   logicWindow0; 
     G4VPhysicalVolume* physiWindow0; 
     
     G4Tubs*             solidWindow1; 
     G4LogicalVolume*   logicWindow1; 
     G4VPhysicalVolume* physiWindow1; 

     G4Tubs*             solidWindow2; 
     G4LogicalVolume*   logicWindow2; 
     G4VPhysicalVolume* physiWindow2; 
    
     G4Tubs*             solidInnerRohacell;   
     G4LogicalVolume*   logicInnerRohacell;   
     G4VPhysicalVolume* physiInnerRohacell;   
     
     G4Tubs*             solidOuterRohacell;   
     G4LogicalVolume*   logicOuterRohacell;   
     G4VPhysicalVolume* physiOuterRohacell;   
     
     G4Tubs*             solidKapton;   
     G4LogicalVolume*   logicKapton;   
     G4VPhysicalVolume* physiKapton;   
     
      
  
    
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
    G4MultiFunctionalDetector* m_MinosScorer ;
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

  //   Shape type
    vector<string> m_Shape ;
   
    // Visualisation Attribute
    G4VisAttributes* m_VisSquare;
    G4VisAttributes* m_VisCylinder;

  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
