/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : 26/08/2010  (M. Labiche)                                 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class manage the virtual detector and call their method.            *
 *  Every time a new detector is added to the project, a few line in the     *
 *  ReadConfigurationFile method are needed in order to detect the associate *
 *  token.                                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    25/08/10: Added Shield around PARIS   (marc.labiche@stfc.ac.uk)        *
 *    15/01/10: Added Chamber (marc.labiche@stfc.ac.uk)                      *
 *    04/12/09: Added PARIS detector (marc.labiche@stfc.ac.uk)               *
 *****************************************************************************/

#include "DetectorConstruction.hh"

// G4
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4String.hh"
#include "G4RotationMatrix.hh"

// Detector class
#include "../../NPLib/DetectorList.inc"
#include "AnnularS1.hh"
#include "Chamber.hh"
#include "ComptonTelescope.hh"
#include "DummyDetector.hh"
#include "Eurogam.hh"
#include "GaspardTracker.hh"
#include "HydeTracker.hh"
#include "MUST2Array.hh"
#include "Paris.hh"
#include "Plastic.hh"
#include "Target.hh"
#include "ThinSi.hh"
#include "Sharc.hh"
#include "Shield.hh"
#include "W1.hh"

// STL
#include<cstdlib>
#include<fstream>

// NPL
#include"NPOptionManager.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction():  world_log(0), world_phys(0){
  m_Target   = 0;
  m_Chamber  = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction(){
  delete m_Target;
  delete m_Chamber;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct(){
  //------------------------------------------------------ materials
  
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  
  
  G4Element* N = new G4Element("Nitrogen", "N", z = 7., a = 14.01 * g / mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z = 8., a = 16.00 * g / mole);
  
  //  Vacuum
  density = 0.000000001 * mg / cm3;
  G4Material* Vacuum = new G4Material("Vacuum", density, 2);
  Vacuum->AddElement(N, .7);
  Vacuum->AddElement(O, .3);
  
  //------------------------------world volume
  
  G4double world_x = 10.0 * m;
  G4double world_y = 10.0 * m;
  G4double world_z = 10.0 * m;
  
  G4Box* world_box
  = new G4Box("world_box", world_x, world_y, world_z);
  
  world_log = new G4LogicalVolume(world_box, Vacuum, "world_log", 0, 0, 0);
  
  world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);
  
  //   G4VisAttributes* VisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
  G4VisAttributes* VisAtt = new G4VisAttributes(G4VisAttributes::Invisible);
  world_log->SetVisAttributes(VisAtt);
  
  //------------------------------------------------------------------
  
  //------------------------------Reaction Chamber volume
  /*
   G4double Chamber_Rmin = 20.0 * cm;
   G4double Chamber_Rmax = 20.2 * cm;
   G4double Chamber_ThetaMin = 0.0 * rad;
   G4double Chamber_ThetaMax = 3.14 * rad;
   G4double Chamber_PhiMin = 0.0 * rad;
   G4double Chamber_PhiMax = 6.26 * rad;
   
   G4Sphere* Chamber_sphere
   = new G4Sphere("Chamber_sphere", Chamber_Rmin, Chamber_Rmax, Chamber_PhiMin, Chamber_PhiMax, Chamber_ThetaMin, Chamber_ThetaMax );
   
   Chamber_log = new G4LogicalVolume(Chamber_sphere, Vacuum, "Chamber_log", 0, 0, 0);
   
   Chamber_phys = new G4PVPlacement(0, G4ThreeVector(), Chamber_log, "Chamber", world_log, false, 0);
   
   G4VisAttributes* VisAttChamber = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
   //G4VisAttributes* VisAtt = new G4VisAttributes(G4VisAttributes::Invisible);
   Chamber_log->SetVisAttributes(VisAttChamber);
   */
  //------------------------------------------------------------------
  
  
  
  return world_phys;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddDetector(VDetector* NewDetector){
  // Add new detector to vector
  m_Detectors.push_back(NewDetector);
  
  // Initialize Scorer
  NewDetector->InitializeScorers();
  
  // Construct new detector
  NewDetector->ConstructDetector(world_log);
  
  // Add Detector to TTree
  NewDetector->InitializeRootOutput();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ReadConfigurationFile(string Path){
  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;
  
  bool cAddThinSi        = false;
  bool cComptonTelescope = false;
  bool cDummy            = false;
  bool cEurogam          = false;
  bool cGeneralTarget    = false;
  bool cGeneralChamber   = false;
  bool cGPDTracker       = false;   
  bool cHYDTracker       = false;  
  bool cMUST2            = false;
  bool cPlastic          = false;
  bool cParis            = false;   
  bool cS1               = false;
  bool cSharc            = false;
  bool cShield           = false;  
  bool cW1               = false;
  
  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
  
  ifstream ConfigFile;
  ConfigFile.open(Path.c_str());
  
  if (ConfigFile.is_open()) {   // should be always be true
    cout << " Configuration file " << Path << " loading " << endl;
  }
  else {
    cout << " Error, no configuration file" << Path << " found" << endl;
  }
  
  while (!ConfigFile.eof()) {
    //Pick-up next line
    getline(ConfigFile, LineBuffer);
    //Search for comment Symbol: %
    if (LineBuffer.compare(0, 1, "%") == 0) {   /*Do  Nothing*/;}
    
    ////////////////////////////////////////////
    /////// Search for a Dummy Detector ////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 16, "TheDUMMYDetector") == 0 && cDummy == false) {
      cDummy = true ;
      if(VerboseLevel==1)  G4cout << G4endl << "//////// DUMMY DETECTOR ////////" << G4endl << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new DUMMYDetector()                  ;
      
      // Read Position of detector
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
    }
    
    
    /////////////////////////////////////////////////////
    //////////// Search for ComptonTelescope ////////////
    /////////////////////////////////////////////////////
    else if (LineBuffer.compare(0, 16, "ComptonTelescope") == 0 && cComptonTelescope == false) {
#ifdef INC_COMPTONTELESCOPE
      cComptonTelescope = true;
      if(VerboseLevel==1) G4cout << "//////// ComptonTelescope  ////////" << G4endl;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new ComptonTelescope();
      
      // Read Position of detector
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }
    
    
    ////////////////////////////////////////////
    //////////// Search for Eurogam ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 7, "Eurogam") == 0 && cEurogam == false) {
#ifdef INC_EUROGAM
      cEurogam = true;
      if(VerboseLevel==1) G4cout << "//////// Eurogam  ////////" << G4endl;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Eurogam();
      
      // Read Position of detector
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }
    
    
    ////////////////////////////////////////////
    //////////// Search for Gaspard ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 14, "GaspardTracker") == 0 && cGPDTracker == false) {
#ifdef INC_GASPARD
      cGPDTracker = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Gaspard Tracker ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new GaspardTracker()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    
    ////////////////////////////////////////////
    //////////// Search for Hyde    ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 11, "HydeTracker") == 0 && cHYDTracker == false) {
#ifdef INC_HYDE
      cHYDTracker = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Hyde Tracker ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new HydeTracker()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    
    ////////////////////////////////////////////
    //////////// Search for paris   ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "Paris") == 0 && cParis == false) {
#ifdef INC_PARIS
      cParis = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Paris  ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Paris()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    ////////////////////////////////////////////
    //////////// Search for paris shield   ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 6, "Shield") == 0 && cShield == false) {
#ifdef INC_SHIELD
      cShield = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Shield  ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Shield()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    ////////////////////////////////////////////
    ///// Search for S1 Annular detector  //////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 9, "AnnularS1") == 0 && cS1 == false) {
#ifdef INC_ANNULARSi1
      cS1 = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// S1 Annular detector ////////" << G4endl   << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new AnnularS1()                 ;
      
      // Read Position of Telescope
      ConfigFile.close()                                    ;
      myDetector->ReadConfiguration(Path)                      ;
      ConfigFile.open(Path.c_str())                         ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                               ;
#endif
    }
    
    ////////////////////////////////////////////
    ///// Search for S1 Annular detector  //////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 2, "W1") == 0 && cW1 == false) {
#ifdef INC_W1
      cW1 = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// W1 Square detector ////////" << G4endl   << G4endl;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new W1();
      
      // Read Position of Telescope
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }
    
    ////////////////////////////////////////////
    //////// Search for MUST2 Array  ////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && cMUST2 == false) {
#ifdef INC_MUST2
      cMUST2 = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// MUST2 Array ////////" << G4endl   << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new MUST2Array()                 ;
      
      // Read Position of Telescope
      ConfigFile.close()                                    ;
      myDetector->ReadConfiguration(Path)                      ;
      ConfigFile.open(Path.c_str())                         ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                               ;
#endif
    }
    
    ////////////////////////////////////////////
    ////////// Search for     ThinSi ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 9, "SSSDArray") == 0 && cAddThinSi == false) {
#ifdef INC_SSSD
      cAddThinSi = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// SSSD ////////" << G4endl << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new ThinSi()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }

    ////////////////////////////////////////////
    ////////// Search for Sharc      ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 5, "Sharc") == 0 && cSharc == false) {
#ifdef INC_SHARC
      cSharc = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Sharc ////////" << G4endl << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Sharc();
      
      // Read Position of detector
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }

    ////////////////////////////////////////////
    ////////// Search for Plastic      ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && cPlastic == false) {
#ifdef INC_PLASTIC
      cPlastic = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Plastic ////////" << G4endl << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Plastic()                  ;
      
      // Read Position of detector
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    ////////////////////////////////////////////
    //////////// Search for Target /////////////
    ////////////////////////////////////////////
    
    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && cGeneralTarget == false) {
      cGeneralTarget = true ;
      if(VerboseLevel==1) G4cout << G4endl << "////////// Target ///////////" << G4endl   << G4endl   ;
      
      // Instantiate the new array as a VDetector Objects
      VDetector* myDetector = new Target();
      
      // Read Position and target specification
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add Target to DetectorConstruction
      m_Target = (Target*) myDetector;
      
      // Add target to the VDetector Vector
      AddDetector(myDetector);
    }

    ////////////////////////////////////////////
    //////////// Search for Chamber /////////////
    ////////////////////////////////////////////
    
    else if (LineBuffer.compare(0, 14, "GeneralChamber") == 0 && cGeneralChamber == false) {
      cGeneralChamber = true ;
      if(VerboseLevel==1) G4cout << G4endl << "////////// Chamber ///////////" << G4endl   << G4endl   ;
      
      // Instantiate the new array as a VDetector Objects
      VDetector* myDetector = new Chamber();
      
      // Read Position and target specification
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add Target to DetectorConstruction
      m_Chamber = (Chamber*) myDetector;
      
      // Add target to the VDetector Vector
      AddDetector(myDetector);
    }
    
    //Nothing understandable
    //else ;
  }
  
  ConfigFile.close();
  return   ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ReadAllSensitive(const G4Event* event){
  // Before looping on each sub-detector, clear the static variable
  // ms_InterCoord
  // This is done on the first element of the m_Modules vector.
  // This should be done here since this variable (of type TIneractionCoordinates)
  // deals with multiplicity of events > 1.
  if(m_Detectors.size()>0)
    m_Detectors[0]->GetInterCoordPointer()->Clear();
  
  for (unsigned short i = 0 ; i < m_Detectors.size() ; i++) {
    m_Detectors[i]->ReadSensitive(event);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* DetectorConstruction::GetWorldLogic()
{
  return world_log;
}
