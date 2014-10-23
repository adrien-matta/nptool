/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : January 2010                                             *
 * Last update    : 11/03/2010                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe standard Chamber. Derived from VDetector*
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <fstream>
#include <limits>

// G4 geometry header
#include "G4Tubs.hh"
#include "G4Sphere.hh"

//G4 various headers
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4EmCalculator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
using namespace CLHEP ;
// NPTool header
#include"Chamber.hh"
#include"MaterialManager.hh"

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Chamber::Chamber()
{
   m_ChamberType     = true;
   m_ChamberRmin     = 0   ;
   m_ChamberRmax     = 0   ;
   m_ChamberPhiMin   = 0   ;
   m_ChamberPhiMax   = 0   ;
   m_ChamberThetaMin = 0   ;
   m_ChamberThetaMax = 0   ;
}

G4Material* Chamber::GetMaterialFromLibrary(G4String MaterialName, G4double Temperature, G4double Pressure)
{
   if (MaterialName == "Alu") {
    return MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
   }
   if (MaterialName == "Cu") {
    return MaterialManager::getInstance()->GetMaterialFromLibrary("Cu");
   }
   if (MaterialName == "C" || MaterialName == "12C") { // keeping legacy name
    return MaterialManager::getInstance()->GetMaterialFromLibrary("C");
   }

   else {
      G4cout << "No Matching Material in the Chamber Library Default is Vacuum" << G4endl;
      return  MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Inherite Method of Vdetector Class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Chamber::ReadConfiguration(string Path)
{
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer;
   string DataBuffer;
   
   bool ReadingStatusChamber = false ;
   //   bool ReadingStatusCryoChamber = false ;

   bool check_Rmin = false ;
   bool check_Rmax = false ;
   bool check_PhiMin = false ;
   bool check_PhiMax = false ;
   bool check_ThetaMin = false ;
   bool check_ThetaMax = false ;
   bool check_Material = false ;

      
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 8, "Chamber") == 0) {
         cout << "Chamber Found" << endl;
         m_ChamberType = true ;
         ReadingStatusChamber = true ;
      }
      /*
      else if (LineBuffer.compare(0, 10, "CryoChamber") == 0) {
         cout << "Cryogenic Chamber Found" << endl;
         m_ChamberType = false ;
         ReadingStatusCryoChamber = true ;
      }
      */

      while (ReadingStatusChamber) {
         ConfigFile >> DataBuffer;
   
            //Search for comment Symbol %
            if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
            
           else if (DataBuffer.compare(0, 5, "Rmin=") == 0) {
              check_Rmin = true ;
               ConfigFile >> DataBuffer;
               m_ChamberRmin = atof(DataBuffer.c_str()) * mm;
                cout << "Chamber inner radius: "  << m_ChamberRmin << endl     ;
            }

           else if (DataBuffer.compare(0, 5, "Rmax=") == 0) {
              check_Rmax = true ;
               ConfigFile >> DataBuffer;
               m_ChamberRmax = atof(DataBuffer.c_str()) * mm;
                cout << "Chamber outer radius: "  << m_ChamberRmax << endl     ;
            }

           else if (DataBuffer.compare(0, 8, "PhiMin=") == 0) {
              check_PhiMin = true ;
               ConfigFile >> DataBuffer;
               m_ChamberPhiMin = atof(DataBuffer.c_str()) * rad;
               cout << "Chamber PhiMin: "     <<  m_ChamberPhiMin      << endl     ;
            }
           else if (DataBuffer.compare(0, 8, "PhiMax=") == 0) {
              check_PhiMax = true ;
               ConfigFile >> DataBuffer;
               m_ChamberPhiMax = atof(DataBuffer.c_str()) * rad;
               cout << "Chamber PhiMax: "     <<  m_ChamberPhiMax      << endl     ;
            }
           else if (DataBuffer.compare(0, 9, "ThetaMin=") == 0) {
              check_ThetaMin = true ;
               ConfigFile >> DataBuffer;
               m_ChamberThetaMin = atof(DataBuffer.c_str()) * rad;
               cout << "Chamber ThetaMin: "     <<  m_ChamberThetaMin      << endl     ;
            }
           else if (DataBuffer.compare(0, 9, "ThetaMax=") == 0) {
              check_ThetaMax = true ;
               ConfigFile >> DataBuffer;
               m_ChamberThetaMax = atof(DataBuffer.c_str()) * rad;
               cout << "Chamber ThetaMax: "     <<  m_ChamberThetaMax      << endl     ;
            }

           else if (DataBuffer.compare(0, 9, "MATERIAL=") == 0) {
              check_Material = true ;
               ConfigFile >> DataBuffer;
               m_ChamberMaterial = GetMaterialFromLibrary(DataBuffer);
               cout << "Chamber Material: "      << m_ChamberMaterial  << endl  ;
            }

           ///////////////////////////////////////////////////
         //   If no Beam Token and no comment, toggle out
            else 
               {ReadingStatusChamber = false; G4cout << "WARNING : Wrong Token Sequence: Getting out " << G4endl ;}
            
           ///////////////////////////////////////////////////
         //   If all Token found toggle out
            if( check_Rmin && check_Rmax && check_Material && check_PhiMin && check_PhiMax && check_ThetaMin && check_ThetaMax )
               ReadingStatusChamber = false ;   

      }            
      
      
         
   }


}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Chamber::ConstructDetector(G4LogicalVolume* world)
{
   if (m_ChamberType) {   // case of standard Chamber

    G4Sphere* solidChamber
      = new G4Sphere("solidChamber", m_ChamberRmin, m_ChamberRmax, m_ChamberPhiMin, m_ChamberPhiMax, m_ChamberThetaMin, m_ChamberThetaMax );

    G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, m_ChamberMaterial, "logicChamber");

         // rotation of target
         //G4RotationMatrix *rotation = new G4RotationMatrix();
         //rotation->rotateY(m_ChamberAngle);

         new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicChamber, "Chamber", world, false, 0);

         G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.));
         logicChamber->SetVisAttributes(ChamberVisAtt);
    //  }
   }

   else {   // case of cryogenic target
   }

}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Chamber::InitializeRootOutput()
{}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Chamber::ReadSensitive(const G4Event*)
{}
   
