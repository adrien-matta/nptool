/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Tiara Silicon array                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Box.hh"
#include "G4Tubs.hh"

//G4 sensitive
#include "G4SDManager.hh"

//G4 various object
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPS
#include "Tiara.hh"

// NPL
#include "NPOptionManager.h"

//#include "TiaraScorers.hh"
#include "RootOutput.h"
using namespace TIARA;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Tiara::Tiara(){
  //InitializeMaterial();
  m_EventBarrel = new TTiaraBarrelData();
  m_EventHyball = new TTiaraHyballData();

  // Dark Grey
  SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  // Gold Yellow
  PADVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.2)) ;
  // Light Grey
  FrameVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
 
}
Tiara::~Tiara(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Tiara::ReadConfiguration(string Path){
/*  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  
  G4double R,Phi,Thickness,Thickness1,Thickness2,Thickness3,Thickness4,ThicknessPAD1,ThicknessPAD2,ThicknessPAD3,ThicknessPAD4,Z;
  R=Phi=Thickness=Thickness1=Thickness2=Thickness3=Thickness4=ThicknessPAD1=ThicknessPAD2=ThicknessPAD3=ThicknessPAD4=Z=0;
  
  G4ThreeVector Pos;
  bool check_R   = false ;
  bool check_Phi = false ;
  bool check_Thickness   = false ;
  bool check_Thickness1   = false ;
  bool check_Thickness2   = false ;
  bool check_Thickness3   = false ;
  bool check_Thickness4   = false ;
  bool check_PAD1   = false ;
  bool check_PAD2   = false ;
  bool check_PAD3   = false ;
  bool check_PAD4   = false ;
  bool check_Z      = false ;
  
  bool ReadingStatusQQQ = false ;
  bool ReadingStatusBOX = false ;
  bool ReadingStatus    = false ;
  while (!ConfigFile.eof()){
    int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
    
    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 5, "Tiara") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      //   CD case
      if (DataBuffer=="TiaraQQQ"){
        if(VerboseLevel==1) G4cout << "///" << G4endl           ;
        if(VerboseLevel==1) G4cout << "QQQ Quadrant found: " << G4endl   ;
        ReadingStatusQQQ = true ;
      }
      
      //  Box case
      else if (DataBuffer=="TiaraBOX"){
        if(VerboseLevel==1) G4cout << "///" << G4endl           ;
        if(VerboseLevel==1) G4cout << "Box Detector found: " << G4endl   ;
        ReadingStatusBOX = true ;
      }
      
      //   Reading Block
      while(ReadingStatusQQQ){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1) cout << "  Z= " << Z/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1) cout << "  R= " << R/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1) cout << "  Phi= " << Phi/deg << "deg" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector=") {
          check_Thickness = true;
          ConfigFile >> DataBuffer ;
          Thickness= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessDetector= " << Thickness/um << "um" << endl;
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusQQQ = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_R && check_Phi && check_Z && check_Thickness){
          
          ReadingStatusQQQ = false;
          AddQQQDetector(G4ThreeVector(R,Phi,Z),Thickness);
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
          check_Thickness = false ;
        }
        
      }
      
      while(ReadingStatusBOX){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1) cout << "  Z= " << Z/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector1=") {
          check_Thickness1 = true;
          ConfigFile >> DataBuffer ;
          Thickness1= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessDetector1= " << Thickness1/um << "um" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector2=") {
          check_Thickness2 = true;
          ConfigFile >> DataBuffer ;
          Thickness2= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessDetector2= " << Thickness2/um << "um" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector3=") {
          check_Thickness3 = true;
          ConfigFile >> DataBuffer ;
          Thickness3= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessDetector3= " << Thickness3/um << "um" << endl;
        }
        
        else if (DataBuffer == "ThicknessDector4=") {
          check_Thickness4 = true;
          ConfigFile >> DataBuffer ;
          Thickness4= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessDetector4= " << Thickness4/um << "um" << endl;
        }
        
        else if (DataBuffer == "ThicknessPAD1=") {
          check_PAD1 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD1= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessPAD1= " << ThicknessPAD1<< "um"  << endl;
        }
        
        else if (DataBuffer == "ThicknessPAD2=") {
          check_PAD2 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD2= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessPAD2= " << ThicknessPAD2<< "um"  << endl;
        }
        
        else if (DataBuffer == "ThicknessPAD3=") {
          check_PAD3 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD3= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessPAD3= " << ThicknessPAD3<< "um"  << endl;
        }
        
        else if (DataBuffer == "ThicknessPAD4=") {
          check_PAD4 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD4= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) cout << "  ThicknessPAD4= " << ThicknessPAD4<< "um"  << endl;
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusBOX = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_Thickness1 && check_Thickness2 && check_Thickness3 && check_Thickness4
            && check_PAD1 && check_PAD2 && check_PAD3 && check_PAD4
            && check_Z){
          ReadingStatusBOX = false;
          AddBoxDetector(Z,Thickness1,Thickness2,Thickness3,Thickness4,
                         ThicknessPAD1,ThicknessPAD2,ThicknessPAD3,ThicknessPAD4);
          //   Reinitialisation of Check Boolean
          check_R = false ;
          check_Phi = false ;
          check_Thickness1 = false;
          check_Thickness2 = false;
          check_Thickness3 = false;
          check_Thickness4 = false;
          check_PAD1 = false;
          check_PAD2 = false;
          check_PAD3 = false;
          check_PAD4 = false;
          check_Z = false ;
          
        }
      }
    }
  }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Tiara::ConstructDetector(G4LogicalVolume* world){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Tiara::ReadSensitive(const G4Event* event){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeScorers(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeRootOutput(){
}


