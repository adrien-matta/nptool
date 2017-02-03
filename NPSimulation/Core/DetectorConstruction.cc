/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

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
#include "NPInputParser.h"

// NPS
#include "NPSDetectorFactory.hh"
#include "MaterialManager.hh"
#include "DetectorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction():  world_log(0), world_phys(0){
  m_Target   = 0;
  m_Chamber  = 0 ;
  m_Messenger =  new DetectorMessenger(this);
  m_ReadSensitivePtr = &NPS::VDetector::ReadSensitive;
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
  G4double world_x = 100.0 * m;
  G4double world_y = 100.0 * m;
  G4double world_z = 100.0 * m;

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
  static bool already=false;
  std::set<std::string> check;
  int VerboseLevel = 0 ;
  if(!already){
    VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
    already = 1;
  }
  else
    VerboseLevel = 0;
  
   if(VerboseLevel)
    cout << endl << "\033[1;36m//// Reading detector file  "<< Path  << endl<<endl; 

  // Access the DetectorFactory and ask it to load the Class List
  std::string classlist = getenv("NPTOOL");
  classlist += "/NPLib/ClassList.txt";
  NPS::DetectorFactory* theFactory = NPS::DetectorFactory::getInstance();
  theFactory->ReadClassList(classlist);

  NPL::InputParser parser(Path);
  ////////////////////////////////////////////
  /////////// Search for Target  /////////////
  ////////////////////////////////////////////
  std::vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Target");
  if(blocks.size()==1){
    m_Target = new Target();
    m_Target->ReadConfiguration(parser);
    AddDetector(m_Target);
  }
  else{
     blocks = parser.GetAllBlocksWithToken("CryoTarget");
     if(blocks.size()==1){
      m_Target = new Target();
      m_Target->ReadConfiguration(parser);
      AddDetector(m_Target);
     }
    else{
      cout << "WARNING: No target found in detector input file! Use with caution" << endl;
    }
  }
  ////////////////////////////////////////////
  /////////// Search for Chamber /////////////
  ////////////////////////////////////////////
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("Chamber");
  if(blocks.size()==1){
    m_Chamber = new Chamber();
    m_Chamber->ReadConfiguration(parser);
    AddDetector(m_Chamber);
  }  
  ////////////////////////////////////////////
  /////////// Search for Detectors ///////////
  ////////////////////////////////////////////
  // Get the list of main token
  std::vector<std::string> token = parser.GetAllBlocksToken();
  // Look for detectors among them
  for(unsigned int i = 0 ; i < token.size() ; i++){
  NPS::VDetector* detector = theFactory->Construct(token[i]);
  if(detector!=NULL && check.find(token[i])==check.end()){
    cout << "/////////////////////////////////////////" << endl;
    cout << "//// Adding Detector " << token[i] << endl; 
    detector->ReadConfiguration(parser);
    cout << "/////////////////////////////////////////" << endl;
    // Add array to the VDetector Vector
    AddDetector(detector);
    check.insert(token[i]);
  }
  else if(detector!=NULL)
    delete detector;
  }

  cout << "\033[0m" ;

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
    (m_Detectors[i]->*m_ReadSensitivePtr)(event);    
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
  
  if(file!="")
    NPOptionManager::getInstance()->SetDetectorFile(file);

  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->PhysicsHasBeenModified() ;
  G4RunManager::GetRunManager()->Initialize();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ExportGeometry(string file){
#ifdef NPS_GDML
  G4GDMLParser parser ;
  parser.Write(file.c_str(),world_log);
#else
  // To silenced the compiler
  file = "";
  G4cout << "You need to compile Geant4 with GDML support to use this command" << G4endl;
#endif
}

