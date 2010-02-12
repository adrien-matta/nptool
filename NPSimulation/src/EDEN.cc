/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a simple Dummy Detector :                            *
 *	a simple cylinder of predifined material. user can choose to place it    *
 *  where he want using spherical coordinate and choose between two naterial *
 *	                                                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  This detector respect all the NPTool convention in naming volume,        * 
 *  reading scorers and file. Use it as a basis for your own detector        *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Tubs.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// NPTool header
#include "EDEN.hh"
#include "GeneralScorers.hh"
#include "RootOutput.h"
using namespace GENERALSCORERS;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace DUMMYDETECTOR
{
   // Energy and time Resolution
   const G4double ResoTime    = 4.2      	;// = 10ns of Resolution   //   Unit is MeV/2.35
   const G4double ResoEnergy  = 5.0   		;// Resolution in %

}

using namespace DUMMYDETECTOR ;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// EDEN Specific Method
EDEN::EDEN()
{
   InitializeMaterial();
//   m_Event = new TEDENData();
}



EDEN::~EDEN()
{
   delete m_MaterialEDEN_liqSci;
//   delete m_Event;
   delete m_EDENScorer;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EDEN::AddEDENModule(G4double R,
			 G4double Theta,
			 G4double Phi) 
{
   m_R.push_back(R);
   m_Theta.push_back(Theta);
   m_Phi.push_back(Phi);
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void EDEN::ReadConfiguration(string Path)
{
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer, DataBuffer;

   G4double R = 0, Theta = 0, Phi = 0;
   G4String Scintillator;

   bool check_Theta   = false;
   bool check_Phi     = false;
   bool check_R       = false;
   bool ReadingStatus = false;


   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);

      // If line is a Start Up EDEN bloc, Reading toggle to true
      if (LineBuffer.compare(0, 4, "EDEN") == 0) {
         G4cout << "///" << G4endl;
         G4cout << "EDEN Module found: " << G4endl;
         ReadingStatus = true;
      }
      // Else don't toggle to Reading Block Status
      else ReadingStatus = false;

      // Reading Block
      while (ReadingStatus) {
         // Pickup Next Word 
         ConfigFile >> DataBuffer;

         // Comment Line 
         if (DataBuffer.compare(0, 1, "%") == 0) {
            ConfigFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }

         // Finding another telescope (safety), toggle out
         else if (DataBuffer.compare(0, 4, "EDEN") == 0) {
            cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << endl;
            ReadingStatus = false;
         }

         // Angle method
         else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
            check_Theta = true;
            ConfigFile >> DataBuffer ;
            Theta = atof(DataBuffer.c_str()) ;
            Theta = Theta * deg;
            cout << "Theta:  " << Theta / deg << endl;
         }

         else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
            check_Phi = true;
            ConfigFile >> DataBuffer ;
            Phi = atof(DataBuffer.c_str()) ;
            Phi = Phi * deg;
            cout << "Phi:  " << Phi / deg << endl;
         }

         else if (DataBuffer.compare(0, 2, "R=") == 0) {
            check_R = true;
            ConfigFile >> DataBuffer ;
            R = atof(DataBuffer.c_str()) ;
            R = R * mm;
            cout << "R:  " << R/mm << endl;
         }

         ///////////////////////////////////////////////////
         // If no Detector Token and no comment, toggle out
         else {
            ReadingStatus = false; 
            G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl;
         }

         /////////////////////////////////////////////////
         // If All necessary information there, toggle out
         if (check_Theta && check_Phi && check_R) { 
            AddEDENModule(R,
		          Theta,
		          Phi); 

            // Reinitialisation of Check Boolean 
            check_Theta   = false;
            check_Phi     = false;
            check_R       = false;
            ReadingStatus = false;
            cout << "///"<< endl;
      	 }
      }
   }
}



// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void EDEN::ConstructDetector(G4LogicalVolume* world)
{
   G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0);

   for (unsigned short i = 0; i < m_R.size(); i++) {
      G4double wX = m_R[i] * sin(m_Theta[i]) * cos(m_Phi[i]);
      G4double wY = m_R[i] * sin(m_Theta[i]) * sin(m_Phi[i]);
      G4double wZ = m_R[i] * cos(m_Theta[i]);
      Det_pos = G4ThreeVector(wX, wY, wZ);

      VolumeMaker(Det_pos , i+1, world) ;
   }
}



void EDEN::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world)
{
   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////		
   G4PVPlacement* PVPBuffer;

   // Name of the module
   std::ostringstream DetectorNumber;
   DetectorNumber << DetNumber;
   G4String Name = "EDEN" + DetectorNumber.str();

   // Definition of the volume containing the sensitive detector
   G4Tubs* solidEDEN = new G4Tubs(Name, 0, 10*cm, 2.5*cm, 0*deg, 360*deg);

   G4LogicalVolume* logicEDEN = new G4LogicalVolume(solidEDEN, m_MaterialEDEN_liqSci, Name + "_Scintillator", 0, 0, 0);
   logicEDEN->SetSensitiveDetector(m_EDENScorer);

   G4VisAttributes* EDENVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9));
   logicEDEN->SetVisAttributes(EDENVisAtt);

   PVPBuffer = new G4PVPlacement(0,
				 Det_pos,
		               	 logicEDEN,
		               	 Name  + "_Scintillator",
		               	 world,
		               	 false,
		               	 0);
}



// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void EDEN::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
//   pTree->Branch("EDEN", "TEDENData", &m_Event) ;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void EDEN::ReadSensitive(const G4Event* event)
{
   G4String DetectorNumber;
//   m_Event->Clear();
}



////////////////////////////////////////////////////////////////
void EDEN::InitializeMaterial()
{
   ////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol;
   G4double density = 0, a = 0, z = 0;
   G4int ncomponents = 0, natoms = 0;

   // for EDEN
   G4Element* H = new G4Element("Hydrogen", symbol = "H", z = 1, a = 1.01   * g / mole);
   G4Element* C = new G4Element("Carbon"  , symbol = "C", z = 6, a = 12.011 * g / mole);

   ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   // EDEN NE213
   density = 0.87 * g / cm3;
   m_MaterialEDEN_liqSci = new G4Material("EDEN_liqSci", density, ncomponents = 2);
   m_MaterialEDEN_liqSci->AddElement(H, natoms = 10);
   m_MaterialEDEN_liqSci->AddElement(C, natoms = 8);
}



////////////////////////////////////////////////////////////////	
void EDEN::InitializeScorers() 
{ 
   // EDEN associated scorer
   m_EDENScorer = new G4MultiFunctionalDetector("EDENScorer");

   // Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_EDENScorer);
}

