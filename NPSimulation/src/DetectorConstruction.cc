/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class manage the virtual detector and call their method.            *
 *  Every time a new detector is added to the project, a few line in the     *
 *  ReadConfigurationFile method are needed in order to detect the associate *
 *  token.                                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
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
#include "MUST2Array.hh"
#include "GaspardTracker.hh"
#include "AnnularS1.hh"
#include "Target.hh"
#include "ThinSi.hh"
#include "Plastic.hh"
//Not G4
#include <cstdlib>
#include<fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
      :  world_log(0), world_phys(0)
{
   m_Target	= 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction()
{
   delete m_Target;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* DetectorConstruction::Construct()
{
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

   G4double world_x = 80.0 * cm;
   G4double world_y = 80.0 * cm;
   G4double world_z = 80.0 * cm;

   G4Box* world_box
   = new G4Box("world_box", world_x, world_y, world_z);

   world_log = new G4LogicalVolume(world_box, Vacuum, "world_log", 0, 0, 0);

   world_phys = new G4PVPlacement(0, G4ThreeVector(), world_log, "world", 0, false, 0);

   G4VisAttributes* VisAtt = new G4VisAttributes(G4Colour(0.2, 0.2, 0.2));
   world_log->SetVisAttributes(VisAtt);

   //------------------------------------------------------------------

   return world_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddDetector(VDetector* NewDetector)
{
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
void DetectorConstruction::ReadConfigurationFile(string Path)
{
   ////////General Reading needs////////
   string LineBuffer;
   string DataBuffer;

   /////////Boolean////////////////////
   bool MUST2           = false;
   bool AddThinSi       = false;
   bool GeneralTarget   = false;
   bool GPDTracker      = false;	// Gaspard Tracker
   bool S1              = false;
   bool cPlastic         = false;
   //////////////////////////////////////////////////////////////////////////////////////////
   // added by Nicolas [07/05/09]
   string GlobalPath = getenv("NPTOOL");
   Path = GlobalPath + "/Inputs/DetectorConfiguration/" + Path;
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   if (ConfigFile.is_open())
      cout << " Configuration file " << Path << " loading " << endl;
   else {
      cout << " Error, no configuration file" << Path << " found" << endl;
      return;
   }


   while (!ConfigFile.eof()) {
      //Pick-up next line
      getline(ConfigFile, LineBuffer);
      //Search for comment Symbol: %
      if (LineBuffer.compare(0, 1, "%") == 0) {   /*Do  Nothing*/;}

      ////////////////////////////////////////////
      //////////// Search for Gaspard ////////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 14, "GaspardTracker") == 0 && GPDTracker == false) {
         GPDTracker = true ;
         G4cout << "//////// Gaspard Tracker ////////" << G4endl   ;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new GaspardTracker()                  ;

         // Read Position of Telescope
         ConfigFile.close()                                 ;
         myDetector->ReadConfiguration(Path)                   ;
         ConfigFile.open(Path.c_str())                      ;

         // Add array to the VDetector Vector
         AddDetector(myDetector)                            ;
      }

      ////////////////////////////////////////////
      ///// Search for S1 Annular detector  //////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 9, "AnnularS1") == 0 && S1 == false) {
         S1 = true ;
         G4cout << "//////// S1 Annular detector ////////" << G4endl   << G4endl   ;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new AnnularS1()                 ;

         // Read Position of Telescope
         ConfigFile.close()                                    ;
         myDetector->ReadConfiguration(Path)                      ;
         ConfigFile.open(Path.c_str())                         ;

         // Add array to the VDetector Vector
         AddDetector(myDetector)                               ;
      }

      ////////////////////////////////////////////
      //////// Search for MUST2 Array  ////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 10, "MUST2Array") == 0 && MUST2 == false) {
         MUST2 = true ;
         G4cout << "//////// MUST2 Array ////////" << G4endl   << G4endl   ;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new MUST2Array()                 ;

         // Read Position of Telescope
         ConfigFile.close()                                    ;
         myDetector->ReadConfiguration(Path)                      ;
         ConfigFile.open(Path.c_str())                         ;

         // Add array to the VDetector Vector
         AddDetector(myDetector)                               ;
      }

      ////////////////////////////////////////////
      ////////// Search for     ThinSi ///////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 9, "AddThinSi") == 0 && AddThinSi == false) {
         AddThinSi = true ;
         G4cout << "//////// Thin Si ////////" << G4endl << G4endl   ;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new ThinSi()                  ;

         // Read Position of Telescope
         ConfigFile.close()                                 ;
         myDetector->ReadConfiguration(Path)                   ;
         ConfigFile.open(Path.c_str())                      ;

         // Add array to the VDetector Vector
         AddDetector(myDetector)                            ;
      }
      
      ////////////////////////////////////////////
      ////////// Search for Plastic	   ///////////
      ////////////////////////////////////////////
      else if (LineBuffer.compare(0, 19, "ScintillatorPlastic") == 0 && cPlastic == false) {
         cPlastic = true ;
         G4cout << "//////// Plastic ////////" << G4endl << G4endl   ;

         // Instantiate the new array as a VDetector Object
         VDetector* myDetector = new Plastic()                  ;

         // Read Position of detector
         ConfigFile.close()                                 ;
         myDetector->ReadConfiguration(Path)                   ;
         ConfigFile.open(Path.c_str())                      ;

         // Add array to the VDetector Vector
         AddDetector(myDetector)                            ;
      }

      ////////////////////////////////////////////
      //////////// Search for Target /////////////
      ////////////////////////////////////////////

      else if (LineBuffer.compare(0, 13, "GeneralTarget") == 0 && GeneralTarget == false) {
         GeneralTarget = true ;
         G4cout << "////////// Target ///////////" << G4endl   << G4endl   ;

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

      //Nothing understandable
      //else ;
   }

   ConfigFile.close();
   return   ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ReadAllSensitive(const G4Event* event)
{
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
