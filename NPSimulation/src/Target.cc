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
 * Last update    : 16/09/2009                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Cryogenic and standard Target. Derived from VDetector*
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  Some improvment need to be done in material dealing                      *
 *                                                                           *
 *  + 16/09/2009: Add support for positioning the target with an angle with  *
 *                respect to the beam (N. de Sereville)                      *
 *  + 16/09/2009: Add CH2 material for targets (N. de Sereville)             *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <fstream>

// G4 geometry header
#include "G4Tubs.hh"

//G4 various headers
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// NPTool header
#include"Target.hh"

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Target::Target()
{
   m_TargetType		= true;
   m_TargetThickness	= 0   ;
   m_TargetAngle	= 0   ;
   m_TargetRadius	= 0   ;
   m_WindowsThickness   = 0   ;
   m_TargetTemperature  = 0   ;
   m_TargetPressure  	= 0   ;
}

G4Material* Target::GetMaterialFromLibrary(G4String MaterialName, G4double Temperature, G4double Pressure)
{

   G4Material* myMaterial;

   if (MaterialName == "D2") {
      G4double density = 0;

      if (Pressure == 1) {
         G4cout << "CryoTarget pressure set to 1 bar" << G4endl;
         if (Temperature == 24) {
            density = 0.0020182 * g / cm3;
            G4cout << "CryoTarget temp set to 24K" << G4endl;
         }

         else if (Temperature == 25) {
            density = 0.0019377 * g / cm3;
            G4cout << "CryoTarget temp set to 25K" << G4endl;
         }

         else if (Temperature == 26) {
            density = 0.001863 * g / cm3;
            G4cout << "CryoTarget temp set to 26K" << G4endl;
         }

         else {
            G4cout << ">>>  !!!!WARNING INVALID TEMP FOR CRYOGENIC TARGET!!!!  <<<" << G4endl;
         }
      }



      else if (Pressure == 0.5) {
         G4cout << "CryoTarget pressure set to 0.5 bar" << G4endl;

         if (Temperature == 24) {
            density = 0.0010091 * g / cm3;
            G4cout << "CryoTarget temp set to 24K" << G4endl;
         }

         else if (Temperature == 25) {
            density = 0.00096875 * g / cm3;
            G4cout << "CryoTarget temp set to 25K" << G4endl;
         }

         else if (Temperature == 26) {
            density = 0.00093149 * g / cm3;
            G4cout << "CryoTarget temp set to 26K" << G4endl;
         }

         else {
            G4cout << ">>>  !!!!WARNING INVALID TEMP FOR CRYOGENIC TARGET!!!!  <<<" << G4endl;
         }
      }



      G4Element* D  = new G4Element("Deuteron"  , "D" , 1., 2.0141*g / mole);
      myMaterial    = new G4Material("D2", density, 1, kStateGas, Temperature, Pressure);
      myMaterial->AddElement(D, 2);
      return(myMaterial);
   }



   else if (MaterialName == "Mylar") {
      G4cout << "Mylar Material" << G4endl ;
      G4Element* H  = new G4Element("Hydrogen", "H" , 1. ,  1.01 *g / mole);
      G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
      G4Element* O  = new G4Element("Oxygen"  , "O" , 8. , 16.00 *g / mole);

      G4Material* myMaterial = new G4Material("Mylar", 1.397*g / cm3, 3);
      myMaterial->AddElement(C , 10);
      myMaterial->AddElement(H ,  8);
      myMaterial->AddElement(O ,  4);
      return myMaterial;
   }

   else if (MaterialName == "Harvar") {
      G4Element* Co  = new G4Element("Cobalt" , "Co" , 27 , 58.933*g / mole);
      G4Element* Cr  = new G4Element("Cromium", "Cr" , 24 , 51.996*g / mole);
      G4Element* Ni  = new G4Element("Nickel"   , "Ni" , 28 , 58.69*g / mole);
      G4Element* Fe  = new G4Element("Iron"  , "Fe" , 26 , 55.847*g / mole);
      G4Element* W   = new G4Element("Teflon"   , "W"  , 74 , 183.5*g / mole);

      G4Material* myMaterial = new G4Material("Havar", 8.3*g / cm3, 5);
      myMaterial->AddElement(Co , 42);
      myMaterial->AddElement(Cr , 20);
      myMaterial->AddElement(Ni , 13);
      myMaterial->AddElement(Fe , 19);
      myMaterial->AddElement(W  ,  1);
      return myMaterial;
   }

   else if (MaterialName == "CD2") {
      G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
      G4Element* D  = new G4Element("Deuteron"  , "D" , 1., 2.0141*g / mole);

      G4Material* myMaterial = new G4Material("CD2", 0.97*g / cm3, 2);
      myMaterial->AddElement(C , 1);
      myMaterial->AddElement(D , 2);
      return myMaterial;
   }

   else if (MaterialName == "CH2") {
      G4Element* C  = new G4Element("Carbon"  , "C" , 6. , 12.011*g / mole);
      G4Element* H  = new G4Element("Hydrogen", "H" , 1. ,  1.01 *g / mole);

      G4Material* myMaterial = new G4Material("CH2", 0.9*g / cm3, 2);
      myMaterial->AddElement(C , 1);
      myMaterial->AddElement(H , 2);
      return myMaterial;
   }

   else {
      G4cout << "No Matching Material in the Target Library Default is Vacuum" << G4endl;
      G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01*g / mole);
      G4Element* O = new G4Element("Oxygen"  , "O", 8., 16.00*g / mole);
      myMaterial = new G4Material("Vacuum", 0.000000001*mg / cm3, 2);
      myMaterial->AddElement(N, .7);
      myMaterial->AddElement(O, .3);
      return(myMaterial);
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Inherite Method of Vdetector Class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Target::ReadConfiguration(string Path)
{
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer;
   string DataBuffer;
	
   bool ReadingStatusTarget = false ;
   bool ReadingStatusCryoTarget = false ;

   bool check_Thickness = false ;
   bool check_Radius = false ;
   bool check_Angle = false ;
   bool check_Material = false ;
   bool check_X = false ;
   bool check_Y = false ;
   bool check_Z = false ;

   bool check_Temperature = false ;
   bool check_Pressure = false ;
   bool check_WinThickness = false ;
   bool check_WinMaterial = false ;
   	
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 6, "Target") == 0) {
         cout << "Target Found" << endl;
         m_TargetType = true ;
         ReadingStatusTarget = true ;
      }
      else if (LineBuffer.compare(0, 10, "CryoTarget") == 0) {
         cout << "Cryogenic Target Found" << endl;
         m_TargetType = false ;
         ReadingStatusCryoTarget = true ;
      }

      while (ReadingStatusTarget) {
         ConfigFile >> DataBuffer;
	
      		//Search for comment Symbol %
      		if (DataBuffer.compare(0, 1, "%") == 0) {/*Do Nothing*/;}
      		
	        else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
	        	check_Thickness = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
	             cout << "Target Thickness: "  << m_TargetThickness << endl     ;
	         }

	        else if (DataBuffer.compare(0, 6, "ANGLE=") == 0) {
	        	check_Angle = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetAngle = atof(DataBuffer.c_str()) * deg;
	             cout << "Target Angle: "  << m_TargetAngle << endl     ;
	         }

	        else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
	        	check_Radius = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetRadius = atof(DataBuffer.c_str()) * mm;
	            cout << "Target Radius: "     <<  m_TargetRadius      << endl     ;
	         }

	        else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
	        	check_Material = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetMaterial = GetMaterialFromLibrary(DataBuffer);
	            cout << "Target Material: "      << m_TargetMaterial  << endl  ;
	         }

	        else if (DataBuffer.compare(0, 2, "X=") == 0) {
	        	check_X = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetX = atoi(DataBuffer.c_str()) * mm;
	            cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
	         }

	        else if (DataBuffer.compare(0, 2, "Y=") == 0) {
	        	check_Y = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetY = atoi(DataBuffer.c_str()) * mm;
	             cout << m_TargetY / mm << " ; ";
	         }

	        else if (DataBuffer.compare(0, 2, "Z=") == 0) {
	        	check_Z = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetZ = atoi(DataBuffer.c_str()) * mm;
	            cout  << m_TargetZ / mm << " )" << endl ;           
	         }

	        ///////////////////////////////////////////////////
			//	If no Beam Token and no comment, toggle out
	         else 
	         	{ReadingStatusTarget = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         
	        ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if( check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z )
	         	ReadingStatusTarget = false ;	

		}				

     	while(ReadingStatusCryoTarget){
      
	       ConfigFile >> DataBuffer;
	
      		//Search for comment Symbol %
      		if (DataBuffer.compare(0, 1, "%") == 0) {/*Do Nothing*/;}

	        else if (DataBuffer.compare(0, 10, "THICKNESS=") == 0) {
	        	check_Thickness = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetThickness = atof(DataBuffer.c_str()) * micrometer;
	            cout << "Target Thickness: "  << m_TargetThickness / micrometer  << "um" << endl   ;
	         }

	        else if (DataBuffer.compare(0, 7, "RADIUS=") == 0) {
	        	check_Radius = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetRadius = atof(DataBuffer.c_str()) * mm;
	             cout << "Target Radius: "     <<  m_TargetRadius / mm           << "mm" << endl ;
	         }

	        else if (DataBuffer.compare(0, 12, "TEMPERATURE=") == 0) {
	        	check_Temperature = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetTemperature = atoi(DataBuffer.c_str());
	         }

	        else if (DataBuffer.compare(0, 9, "PRESSURE=") == 0) {
	        	check_Pressure = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetPressure = atoi(DataBuffer.c_str());
	         }

	        else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
	        	check_Material = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetMaterial = GetMaterialFromLibrary(DataBuffer, m_TargetTemperature, m_TargetPressure);
	            cout << "Target Material: "      << m_TargetMaterial          << endl         ;
	         }

	        else if (DataBuffer.compare(0, 17, "WINDOWSTHICKNESS=") == 0) {
	        	check_WinThickness = true ;
	            ConfigFile >> DataBuffer;
	            m_WindowsThickness = atoi(DataBuffer.c_str()) * micrometer;
	            cout << "Windows Thickness: " <<    m_WindowsThickness / micrometer << "um" << endl   ;
	         }

	        else if (DataBuffer.compare(0, 16, "WINDOWSMATERIAL=") == 0) {
	        	check_WinMaterial = true ;
	            ConfigFile >> DataBuffer;
	            m_WindowsMaterial = GetMaterialFromLibrary(DataBuffer);
	            cout << "Windows Material: "  <<    m_WindowsMaterial         << endl         ;
	         }

	        else if (DataBuffer.compare(0, 2, "X=") == 0) {
	        	check_X = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetX = atoi(DataBuffer.c_str()) * mm;
	            cout << "Target coordinate (mm): ( " << m_TargetX / mm << " ; ";
	         }

	       else if (DataBuffer.compare(0, 2, "Y=") == 0) {
	            check_Y = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetY = atoi(DataBuffer.c_str()) * mm;
	            cout << m_TargetY / mm << " ; ";
	         }

	        else if (DataBuffer.compare(0, 2, "Z=") == 0) {
	            check_Z = true ;
	            ConfigFile >> DataBuffer;
	            m_TargetZ = atoi(DataBuffer.c_str()) * mm;
	            cout << m_TargetZ / mm << " )" << endl ;
	         }

	        ///////////////////////////////////////////////////
			//	If no Beam Token and no comment, toggle out
	         else 
	         	{ReadingStatusCryoTarget = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
	         
	        ///////////////////////////////////////////////////
			//	If all Token found toggle out
	         if( check_Thickness && check_Radius && check_Material && check_X && check_Y && check_Z && check_WinThickness && check_WinMaterial && check_Pressure && check_Temperature)
	         	ReadingStatusCryoTarget = false ;	       
	              
	      }
	      
      }


}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Target::ConstructDetector(G4LogicalVolume* world)
{

// Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
// If don't you will have a Warning unused variable 'myPVP'
   G4VPhysicalVolume* PVPBuffer ;

   if (m_TargetType) {	// case of standard target

      if (m_TargetThickness > 0) {
         G4Tubs*            solidTarget = new G4Tubs("solidTarget", 0, m_TargetRadius, 0.5*m_TargetThickness, 0*deg, 360*deg);
         G4LogicalVolume*   logicTarget = new G4LogicalVolume(solidTarget, m_TargetMaterial, "logicTarget");

         // rotation of target
         G4RotationMatrix *rotation = new G4RotationMatrix();
         rotation->rotateY(m_TargetAngle);

         PVPBuffer =
            new G4PVPlacement(rotation, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), logicTarget, "Target", world, false, 0);

         G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));//Blue
         logicTarget->SetVisAttributes(TargetVisAtt);
      }
   }

   else {	// case of cryogenic target

      if (m_TargetThickness > 0) {
         G4Tubs*            solidTarget = new G4Tubs("solidTarget", 0, m_TargetRadius, 0.5*m_TargetThickness, 0*deg, 360*deg);
         G4LogicalVolume*   logicTarget = new G4LogicalVolume(solidTarget, m_TargetMaterial, "logicTarget");
         PVPBuffer =
            new G4PVPlacement(0, G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ), logicTarget, "Target", world, false, 0);

         G4VisAttributes* TargetVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));//Blue
         logicTarget->SetVisAttributes(TargetVisAtt);
      }


      if (m_WindowsThickness > 0) {
         G4ThreeVector TargetPos = G4ThreeVector(m_TargetX, m_TargetY, m_TargetZ);

         G4Tubs*        solidWindowsF =
            new G4Tubs("solidTargetWindowsF", 0, m_TargetRadius, 0.5*m_WindowsThickness, 0*deg, 360*deg);
         G4LogicalVolume*   logicWindowsF = new G4LogicalVolume(solidWindowsF, m_WindowsMaterial, "logicTargetWindowsF");

         G4Tubs*        solidWindowsB =
            new G4Tubs("solidTargetWindowsB", 0, m_TargetRadius, 0.5*m_WindowsThickness, 0*deg, 360*deg);
         G4LogicalVolume*   logicWindowsB = new G4LogicalVolume(solidWindowsB, m_WindowsMaterial, "logicTargetWindowsB");

         PVPBuffer =
            new G4PVPlacement(0                                                           ,
                  TargetPos + G4ThreeVector(0., 0., 0.5*(m_TargetThickness + m_WindowsThickness))      ,
                  logicWindowsF                                                  ,
                  "Target Window Front"                                             ,
                  world                                                       ,
                  false, 0);

         PVPBuffer =
            new G4PVPlacement(0                                                           ,
                  TargetPos + G4ThreeVector(0., 0., -0.5*(m_TargetThickness + m_WindowsThickness))  ,
                  logicWindowsB                                                  ,
                  "Target Window Back"                                           ,
                  world                                                       ,
                  false, 0);

         G4VisAttributes* WindowsVisAtt = new G4VisAttributes(G4Colour(0.5, 1., 0.5));
         logicWindowsF->SetVisAttributes(WindowsVisAtt);
         logicWindowsB->SetVisAttributes(WindowsVisAtt);
      }
   }

}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Target::InitializeRootOutput()
{}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Target::ReadSensitive(const G4Event*)
{}

