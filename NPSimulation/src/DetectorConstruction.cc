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
#include "G4GeometryManager.hh"
#include "G4GeometryMessenger.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
// Detector class
#include "../../NPLib/DetectorList.inc"
#include "Chamber.hh"

#ifdef INC_ANNULARS1
#include "AnnularS1.hh"
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

#ifdef INC_FATIMA
#include "Fatima.hh"
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

#ifdef INC_NANA
#include "Nana.hh"
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
#include "RootOutput.h"
#include "NPOptionManager.h"
#include "MaterialManager.hh"
#include "DetectorMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction():  world_log(0), world_phys(0){
  m_Target   = 0;
  m_Chamber  = 0 ;
  m_Messenger =  new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct(){
  return ReadConfigurationFile();
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
G4VPhysicalVolume* DetectorConstruction::ReadConfigurationFile(){
  // Construct the World volume
  G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");

  //------------------------------world volume
  G4double world_x = 10.0 * m;
  G4double world_y = 10.0 * m;
  G4double world_z = 10.0 * m;

  G4Box* world_box
    = new G4Box("world_box", world_x, world_y, world_z);

  world_log = new G4LogicalVolume(world_box, Vacuum, "world_log", 0, 0, 0);
  world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

  G4VisAttributes* VisAtt = new G4VisAttributes(G4VisAttributes::Invisible);
  world_log->SetVisAttributes(VisAtt);

  //------------------------------------------------------------------

  //------------------------------------------------------------------

  string Path = NPOptionManager::getInstance()->GetDetectorFile();

  ////////General Reading needs////////
  string LineBuffer;
  string DataBuffer;


  bool cAddThinSi        = false;
  bool cComptonTelescope = false;
  bool cDummy            = false;
  bool cEurogam          = false;
  bool cFatima           = false;
  bool cGeneralTarget    = false;
  bool cGeneralChamber   = false;
  bool cGPDTracker       = false;   
  bool cHYD2Tracker      = false;
  bool cHelios           = false;
  bool cMUST2            = false;
  bool cNana             = false;
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
    G4cout << " Configuration file " << Path << " loading " << G4endl;
  }
  else {
    G4cout << " Error, no configuration file" << Path << " found" << G4endl;
    exit(1);
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
      if(VerboseLevel==1)  G4cout << G4endl << "//////// DUMMY DETECTOR ////////" << G4endl << G4endl   ;

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
    //////////// Search for FATIMA   ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 6, "Fatima") == 0 && cFatima == false) {
#ifdef INC_FATIMA
      cFatima = true ;
      G4cout << "//////// Fatima  ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Fatima()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
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
    //////////// Search for Hyde2   ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 12, "Hyde2Tracker") == 0 && cHYD2Tracker == false) {
#ifdef INC_HYDE2
      cHYD2Tracker = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Hyde2 Tracker ////////" << G4endl   ;

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
#ifdef INC_ANNULARS1
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
    ///////////// Search for Nana //////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 4, "Nana") == 0 && cNana == false) {
#ifdef INC_NANA
      cNana = true ;
      G4cout << "//////// Nana  ////////" << G4endl   ;
      
      // Instantiate the new array as a VDetector Object
      VDetector* myDetector = new Nana()                  ;
      
      // Read Position of Telescope
      ConfigFile.close()                                 ;
      myDetector->ReadConfiguration(Path)                   ;
      ConfigFile.open(Path.c_str())                      ;
      
      // Add array to the VDetector Vector
      AddDetector(myDetector)                            ;
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
    ////////// Search for Tigress    ///////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0,7, "Tigress") == 0 && cTigress == false) {
#ifdef INC_TIGRESS
      cTigress = true ;
      if(VerboseLevel==1) G4cout << G4endl << "//////// Tigress ////////" << G4endl << G4endl   ;

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
      if(VerboseLevel==1) G4cout << G4endl << "//////// Tiara ////////" << G4endl << G4endl   ;

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
    //////////// Search for Helios ////////////
    ////////////////////////////////////////////
    else if (LineBuffer.compare(0, 6, "Helios") == 0 && cHelios == false) {
#ifdef INC_HELIOS
      cHelios = true ;
      G4cout << "//////// Helios detector ////////" << G4endl   ;

      //      bool check_MField      = false;
      double Bz=0.;

      ConfigFile >> DataBuffer ;
      if (DataBuffer.compare(0, 7, "MField=") == 0){
        //        check_MField = true;
        ConfigFile >> DataBuffer ;
        Bz = atof(DataBuffer.c_str()) ;
        G4cout << "//////// Magentic Field set at Bz= " << Bz << " ////////" << G4endl   ;
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

  if(m_Target==NULL){
    G4cout << "ERROR: No target define in detector file. Cannot perform simulation without target" << G4endl ;
    exit(1);
  }

  // Create the Material sample for DEDX tables
  MaterialManager::getInstance()->CreateSampleVolumes(world_log);

  return world_phys;
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

  unsigned int mysize =  m_Detectors.size();
  for (unsigned short i = 0 ; i < mysize ; i++) {
    m_Detectors[i]->ReadSensitive(event);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* DetectorConstruction::GetWorldLogic(){
  return world_log;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ClearGeometry(){
  unsigned int mySize = m_Detectors.size();

  for (unsigned short i = 0 ; i < mySize ; i++) {
    delete m_Detectors[i];
  }

  m_Detectors.clear();
  m_Target = 0;
  m_Chamber = 0 ;
  world_log=0;
  world_phys=0;

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::RedefineGeometry(string file){
  ClearGeometry() ;

  RootOutput::getInstance()->GetTree()->ResetBranchAddresses(); 
  RootOutput::getInstance()->GetTree()->GetListOfBranches()->Clear(); 

  if(file!="")
    NPOptionManager::getInstance()->SetDetectorFile(file);

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->Initialize();
}
