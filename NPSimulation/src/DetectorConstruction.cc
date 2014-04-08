/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
#include "MyMagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4SubtractionSolid.hh"

// Detector class
#include "../../NPLib/DetectorList.inc"
#include "Chamber.hh"

#ifdef INC_ANNULARS1
#include "AnnularS1.hh"
#endif

#ifdef INC_ANU
#include "ANU.hh"
#endif

#ifdef INC_COMPTONTELESCOPE
#include "ComptonTelescope.hh"
#endif

#ifdef INC_DUMMYDETECTOR
#include "DummyDetector.hh"
#endif

#ifdef INC_EUROGAM
#include "Eurogam.hh"
#endif

#ifdef INC_GASPARD
#include "GaspardTracker.hh"
#endif

#ifdef INC_HELIOS
#include "Helios.hh"
#endif

#ifdef INC_HYDE2
#include "Hyde2Tracker.hh"
#endif

#ifdef INC_MUST2
#include "MUST2Array.hh"
#endif

#ifdef INC_PARIS
#include "Paris.hh"
#endif

#ifdef INC_PLASTIC
#include "Plastic.hh"
#endif

#include "Target.hh"

#ifdef INC_SSSD
#include "ThinSi.hh"
#endif

#ifdef INC_SHARC
#include "Sharc.hh"
#endif

#ifdef INC_SHIELD
#include "Shield.hh"
#endif

#ifdef INC_TIGRESS
#include "Tigress.hh"
#endif

#ifdef INC_TIARA
#include "Tiara.hh"
#endif

#ifdef INC_W1
#include "W1.hh"
#endif


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
  Vacuum->AddElement(N,.7);
  Vacuum->AddElement(O,.3);
  
  //------------------------------world volume
  G4double world_x = 10.0 * m;
  G4double world_y = 10.0 * m;
  G4double world_z = 10.0 * m;
  
  G4Box* world_box
  = new G4Box("world_box", world_x, world_y, world_z);
  
  world_log = new G4LogicalVolume(world_box, Vacuum, "world_log", 0, 0, 0);
  world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);
  
  //G4VisAttributes* VisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
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
  bool cANU              = false;
  bool cComptonTelescope = false;
  bool cDummy            = false;
  bool cEurogam          = false;
  bool cHelios           = false;
  bool cGeneralTarget    = false;
  bool cGeneralChamber   = false;
  bool cGPDTracker       = false;   
  bool cHYD2Tracker      = false;  
  bool cMUST2            = false;
  bool cPlastic          = false;
  bool cParis            = false;   
  bool cS1               = false;
  bool cSharc            = false;
  bool cShield           = false;
  bool cTigress          = false;
  bool cTiara            = false;
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
#ifdef INC_DUMMYDETECTOR
      if(VerboseLevel==1)  cout << endl << "//////// DUMMY DETECTOR ////////" << endl << endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new DUMMYDetector()                  ;
      
      // Read Position of detector
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
#endif
    }
    
    
    /////////////////////////////////////////////////////
    //////////// Search for ComptonTelescope ////////////
    /////////////////////////////////////////////////////
    else if (LineBuffer.compare(0, 16, "ComptonTelescope") == 0 && cComptonTelescope == false) {
#ifdef INC_COMPTONTELESCOPE
      cComptonTelescope = true;
      if(VerboseLevel==1) cout << "//////// ComptonTelescope  ////////" << endl;
      
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
      if(VerboseLevel==1) cout << "//////// Eurogam  ////////" << endl;
      
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
      if(VerboseLevel==1) cout << endl << "//////// Gaspard Tracker ////////" << endl   ;
      
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
    //////////// Search for Hyde2   ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 12, "Hyde2Tracker") == 0 && cHYD2Tracker == false) {
#ifdef INC_HYDE2
      cHYD2Tracker = true ;
      if(VerboseLevel==1) cout << endl << "//////// Hyde2 Tracker ////////" << endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Hyde2Tracker()                  ;
      
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
      if(VerboseLevel==1) cout << endl << "//////// Paris  ////////" << endl   ;
      
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
      if(VerboseLevel==1) cout << endl << "//////// Shield  ////////" << endl   ;
      
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
#ifdef INC_ANNULARS1
      cS1 = true ;
      if(VerboseLevel==1) cout << endl << "//////// S1 Annular detector ////////" << endl   << endl   ;
      
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
      if(VerboseLevel==1) cout << endl << "//////// W1 Square detector ////////" << endl   << endl;
      
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
      if(VerboseLevel==1) cout << endl << "//////// MUST2 Array ////////" << endl   << endl   ;
      
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
      if(VerboseLevel==1) cout << endl << "//////// SSSD ////////" << endl << endl   ;
      
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
      if(VerboseLevel==1) cout << endl << "//////// Sharc ////////" << endl << endl   ;
      
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
    ////////// Search for Tigress    ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0,7, "Tigress") == 0 && cTigress == false) {
#ifdef INC_TIGRESS
      cTigress = true ;
      if(VerboseLevel==1) cout << endl << "//////// Tigress ////////" << endl << endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Tigress();
      
      // Read Position of detector
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }
  
    ////////////////////////////////////////////
    ////////// Search for Tiara      ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0,5, "Tiara") == 0 && cTiara == false) {
#ifdef INC_TIARA
      cTiara = true ;
      if(VerboseLevel==1) cout << endl << "//////// Tiara ////////" << endl << endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Tiara();
      
      // Read Position of detector
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());
      
      // Add array to the VDetector Vector
      AddDetector(myDetector);
#endif
    }


    ////////////////////////////////////////////
    ////////// Search for Plastic    ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && cPlastic == false) {
#ifdef INC_PLASTIC
      cPlastic = true ;
      if(VerboseLevel==1) cout << endl << "//////// Plastic ////////" << endl << endl   ;
      
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
    //////////// Search for Helios ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 6, "Helios") == 0 && cHelios == false) {
#ifdef INC_HELIOS
      cHelios = true ;
      cout << "//////// Helios detector ////////" << endl   ;
      
//      bool check_MField      = false;
      double Bz=0.;

      ConfigFile >> DataBuffer ;
      if (DataBuffer.compare(0, 7, "MField=") == 0){
//        check_MField = true;
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;
        cout << "//////// Magentic Field set at Bz= " << Bz << " ////////" << endl   ;
      }
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Helios()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
      
      //------------------------------world volume
      //
      
      //  Aluminium material
      G4double a= 26.98 * g / mole;
      G4double density = 2.7 * g / cm3;
      G4double z = 13.;
      G4Material* Aluminium = new G4Material("Aluminium", z, a, density);
      
      
      
      // Add the Aluminium rod
      G4double Al_rod_x = 1. * cm;
      G4double Al_rod_y = 1. * cm;
      //G4double Al_rod_z = 20.0 * cm;
      G4double Al_rod_z = 40.0 * cm;
      G4Box* Al_rod_box
      = new G4Box("Al_rod_box", Al_rod_x, Al_rod_y, Al_rod_z);
      
      G4Tubs* Al_rod_tub
      = new G4Tubs("Al_rod_tub", 0, 0.5*cm, Al_rod_z+1.*mm, 0.*deg, 360*deg);
      
      G4SubtractionSolid* Al_rod=new G4SubtractionSolid("Rod",Al_rod_box, Al_rod_tub, 0, G4ThreeVector(0.,0.,0.));
      
      G4LogicalVolume* Al_rod_log = new G4LogicalVolume(Al_rod, Aluminium, "Al_rod", 0, 0, 0);
      
      
      new G4PVPlacement(0, G4ThreeVector(0.,0., Al_rod_z + 12.5*cm), Al_rod_log, "Al_rod", world_log, false, 0);
      new G4PVPlacement(0, G4ThreeVector(0.,0., -(Al_rod_z + 12.5*cm)), Al_rod_log, "Al_rod", world_log, false, 1);
      
      
      
      // Add the Aluminium chamber
      G4double Al_chamber_rmin = 50. * cm;
      G4double Al_chamber_rmax = 55. * cm;
      G4double Al_chamber_z = 100.0 * cm;
      
      //G4Tubs* Al_chamber_tub
      //  = new G4Tubs("Al_chamber_tub", Al_chamber_rmin, Al_chamber_rmax, Al_chamber_z, 0.*deg, 180*deg);
      G4Tubs* Al_chamber_tub
      = new G4Tubs("Al_chamber_tub", Al_chamber_rmin, Al_chamber_rmax, Al_chamber_z, 0.*deg, 360*deg);
      
      G4LogicalVolume* Al_chamber_log = new G4LogicalVolume(Al_chamber_tub, Aluminium, "Al_chamber", 0, 0, 0);
      
      G4RotationMatrix* RotZ = new G4RotationMatrix();
      RotZ->rotateZ(-90*deg);
      
      new G4PVPlacement(RotZ, G4ThreeVector(0.,0.,0.), Al_chamber_log, "Al_chamber", world_log, false, 0);
      
      
      G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.2, 0.5, 0.8));
      Al_rod_log->SetVisAttributes(VisAtt1);
      G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(0., 0.5, 0.3));
      Al_chamber_log->SetVisAttributes(VisAtt2);
      
      
      //-------------------------------------------------------------------------
      // add also My Magnetic field
      //-------------------------------------------------------------------------
      
      
      static G4bool fieldIsInitialized = false;
      
      if(!fieldIsInitialized)
        {
        MyMagneticField* myField = new MyMagneticField(G4ThreeVector(0.,0.,Bz));
        
        G4FieldManager* fieldMgr
        = G4TransportationManager::GetTransportationManager()
        ->GetFieldManager();
        fieldMgr->SetDetectorField(myField);
        
        
        /*
	       G4MagIntegratorStepper *pItsStepper;
	       G4ChordFinder* pChordFinder= new G4ChordFinder(myField,
         1.0e-2*mm,  // stepper size
         pItsStepper=0);
         fieldMgr->SetChordFinder(pChordFinder);
         */
        
        fieldMgr->CreateChordFinder(myField);
        
        fieldIsInitialized = true;
        }
      
#endif      
    }
    
    ////////////////////////////////////////////
    //////////// Search for ANU ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 3, "ANU") == 0 && cANU == false) {
#ifdef INC_ANU
      cANU = true ;
      if(VerboseLevel==1) cout << "//////// ANU detector ////////" << endl   ;
      
      G4double Bz=0.;

      ConfigFile >> DataBuffer ;
      if (DataBuffer.compare(0, 7, "MField=") == 0){
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;
        if(VerboseLevel==1) cout << "//////// Magentic Field set at Bz= " << Bz << " ////////" << endl   ;
      }
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new ANU();

      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
    
      static G4bool fieldIsInitialized = false;
      
      if(!fieldIsInitialized)
        {
        MyMagneticField* myField = new MyMagneticField(G4ThreeVector(0.,0.,Bz));
        
        G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        fieldMgr->SetDetectorField(myField);
        
        fieldMgr->CreateChordFinder(myField);
        
        fieldIsInitialized = true;
        }
      
#endif      
    }
    
    ////////////////////////////////////////////
    //////////// Search for Target /////////////
    ////////////////////////////////////////////
    
    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && cGeneralTarget == false) {
      cGeneralTarget = true ;
      if(VerboseLevel==1) cout << endl << "////////// Target ///////////" << endl   << endl   ;
      
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
      if(VerboseLevel==1) cout << endl << "////////// Chamber ///////////" << endl   << endl   ;
      
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
  
  if(m_Target==NULL){
    cout << "ERROR: No target define in detector file. Cannot perform simulation without target" << endl ;
    exit(1);
  }
  
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
