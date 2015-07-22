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

#ifdef NPS_GDML
#include"G4GDMLParser.hh"
#endif

// STL
#include<cstdlib>
#include<fstream>
#include<string>
#include<set>
// NPL
#include "RootOutput.h"
#include "NPOptionManager.h"

// NPS
#include "NPSDetectorFactory.hh"
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
void DetectorConstruction::AddDetector(NPS::VDetector* NewDetector){
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

  std::string Path = NPOptionManager::getInstance()->GetDetectorFile();

  ////////General Reading needs////////
  std::string LineBuffer;
  std::string DataBuffer;
  bool cGeneralTarget=false;
  bool cGeneralChamber=false;
  std::set<std::string> check;
  
  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
  cout << "\033[1;36m" ;
  
  // Access the DetectorFactory and ask it to load the Class List
  std::string classlist = getenv("NPTOOL");
  classlist += "/NPLib/DetectorClassList.txt";
  NPS::DetectorFactory* theFactory = NPS::DetectorFactory::getInstance();
  theFactory->ReadClassList(classlist);

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
    //////////// Search for Target /////////////
    ////////////////////////////////////////////

    else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && cGeneralTarget == false) {
      cGeneralTarget = true ;
      if(VerboseLevel==1) G4cout << G4endl << "////////// Target ///////////" << G4endl   << G4endl   ;

      // Instantiate the new array as aNPS::VDetector Objects
     NPS::VDetector* myDetector = new Target();

      // Read Position and target specification
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add Target to DetectorConstruction
      m_Target = (Target*) myDetector;

      // Add target to theNPS::VDetector Vector
      AddDetector(myDetector);
    }

    ////////////////////////////////////////////
    //////////// Search for Chamber /////////////
    ////////////////////////////////////////////

    else if (LineBuffer.compare(0, 14, "GeneralChamber") == 0 && cGeneralChamber == false) {
      cGeneralChamber = true ;
      if(VerboseLevel==1) G4cout << G4endl << "////////// Chamber ///////////" << G4endl   << G4endl   ;

      // Instantiate the new array as aNPS::VDetector Objects
     NPS::VDetector* myDetector = new Chamber();

      // Read Position and target specification
      ConfigFile.close();
      myDetector->ReadConfiguration(Path);
      ConfigFile.open(Path.c_str());

      // Add Target to DetectorConstruction
      m_Chamber = (Chamber*) myDetector;

      // Add target to theNPS::VDetector Vector
      AddDetector(myDetector);
    }

    else{
      std::stringstream oss(LineBuffer);
      std::string token;
      oss >> token ;
      NPS::VDetector* detector = theFactory->Construct(token);
      if(detector!=NULL && check.find(token)==check.end()){
        cout << "/////////////////////////////////////////" << endl;
        cout << "//// Adding Detector " << token << endl; 
        detector->ReadConfiguration(Path);
        cout << "/////////////////////////////////////////" << endl;
        // Add array to the VDetector Vector
        AddDetector(detector);
        check.insert(token);
      }
    }
  }

  ConfigFile.close();
  cout << "\033[0m" ;

  if(m_Target==NULL){
    G4cout << "\033[1;31mERROR: No target define in detector file. Cannot perform simulation without target\033[0m" << G4endl ;
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
void DetectorConstruction::RedefineGeometry(std::string file){
  ClearGeometry() ;

  RootOutput::getInstance()->GetTree()->ResetBranchAddresses(); 
  RootOutput::getInstance()->GetTree()->GetListOfBranches()->Clear(); 

  if(file!="")
    NPOptionManager::getInstance()->SetDetectorFile(file);

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->Initialize();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ExportGeometry(string file){
  G4GDMLParser parser ;
  parser.Write(file.c_str(),world_log);
}

