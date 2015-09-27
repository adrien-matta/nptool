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
 *  This class describe the Sharc Silicon array                              *
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
#include "Sharc.hh"
#include "SiliconScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
// NPL
#include "NPOptionManager.h"

#include "RootOutput.h"
using namespace SHARC;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Sharc Specific Method
Sharc::Sharc(){
  InitializeMaterial();
  m_Event = new TSharcData();
  // Dark Grey
  SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  // Gold Yellow
  PADVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.2)) ;
  // Light Grey
  FrameVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;

  m_BOXScorer = 0 ;
  m_PADScorer = 0 ;
  m_QQQScorer = 0;

}

Sharc::~Sharc(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::AddBoxDetector(G4double Z,G4double Thickness1,G4double Thickness2,G4double Thickness3,G4double Thickness4,G4double ThicknessPAD1,G4double ThicknessPAD2,G4double ThicknessPAD3,G4double ThicknessPAD4){
  
  m_Type.push_back(true);
  m_Z.push_back(Z);
  
  vector<G4double> ThicknessBOX;
  ThicknessBOX.push_back(Thickness1);
  ThicknessBOX.push_back(Thickness2);
  ThicknessBOX.push_back(Thickness3);
  ThicknessBOX.push_back(Thickness4);
  m_ThicknessBOX.push_back(ThicknessBOX);
  
  vector<G4double> ThicknessPAD;
  ThicknessPAD.push_back(ThicknessPAD1);
  ThicknessPAD.push_back(ThicknessPAD2);
  ThicknessPAD.push_back(ThicknessPAD3);
  ThicknessPAD.push_back(ThicknessPAD4);
  m_ThicknessPAD.push_back(ThicknessPAD);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::AddQQQDetector(G4ThreeVector Pos,G4double Thickness){
  m_Type.push_back(false);
  m_Pos.push_back(Pos);
  m_ThicknessQQQ.push_back(Thickness);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Sharc::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
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
    // G4cout << LineBuffer << G4endl;
    if (LineBuffer.compare(0, 5, "Sharc") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      //   CD case
      if (DataBuffer=="SharcQQQ"){
        if(VerboseLevel==1) G4cout << "///" << G4endl           ;
        if(VerboseLevel==1) G4cout << "QQQ Quadrant found: " << G4endl   ;
        ReadingStatusQQQ = true ;
      }
      
      //  Box case
      else if (DataBuffer=="SharcBOX"){
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
          if(VerboseLevel==1) G4cout << "  Z= " << Z/mm << "mm" << G4endl;
        }
        
        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1) G4cout << "  R= " << R/mm << "mm" << G4endl;
        }
        
        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1) G4cout << "  Phi= " << Phi/deg << "deg" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessDector=") {
          check_Thickness = true;
          ConfigFile >> DataBuffer ;
          Thickness= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessDetector= " << Thickness/um << "um" << G4endl;
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
          if(VerboseLevel==1) G4cout << "  Z= " << Z/mm << "mm" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessDector1=") {
          check_Thickness1 = true;
          ConfigFile >> DataBuffer ;
          Thickness1= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessDetector1= " << Thickness1/um << "um" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessDector2=") {
          check_Thickness2 = true;
          ConfigFile >> DataBuffer ;
          Thickness2= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessDetector2= " << Thickness2/um << "um" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessDector3=") {
          check_Thickness3 = true;
          ConfigFile >> DataBuffer ;
          Thickness3= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessDetector3= " << Thickness3/um << "um" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessDector4=") {
          check_Thickness4 = true;
          ConfigFile >> DataBuffer ;
          Thickness4= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessDetector4= " << Thickness4/um << "um" << G4endl;
        }
        
        else if (DataBuffer == "ThicknessPAD1=") {
          check_PAD1 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD1= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessPAD1= " << ThicknessPAD1<< "um"  << G4endl;
        }
        
        else if (DataBuffer == "ThicknessPAD2=") {
          check_PAD2 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD2= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessPAD2= " << ThicknessPAD2<< "um"  << G4endl;
        }
        
        else if (DataBuffer == "ThicknessPAD3=") {
          check_PAD3 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD3= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessPAD3= " << ThicknessPAD3<< "um"  << G4endl;
        }
        
        else if (DataBuffer == "ThicknessPAD4=") {
          check_PAD4 = true;
          ConfigFile >> DataBuffer ;
          ThicknessPAD4= atof(DataBuffer.c_str())*um;
          if(VerboseLevel==1) G4cout << "  ThicknessPAD4= " << ThicknessPAD4<< "um"  << G4endl;
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
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Sharc::ConstructDetector(G4LogicalVolume* world){
  ConstructBOXDetector(world);
  ConstructQQQDetector(world);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::ConstructBOXDetector(G4LogicalVolume* world){
  for(unsigned int i = 0 ; i < m_Z.size() ; i++){
    for (unsigned int j = 0 ; j < 4; j++) {
      
      int DetNbr = 0;
      if(m_Z[i]<0) DetNbr=j+1+4;
      else         DetNbr=j+1+8;
      // create the Box DSSD
      // Make the a single detector geometry
      G4Box*  PCBFull = new G4Box("PCBFull"  ,
                                  BOX_PCB_Length/2.,
                                  BOX_PCB_Width/2.,
                                  BOX_PCB_Thickness/2.);
      
      G4Box*  WaferShape = new G4Box("WaferShape",
                                     BOX_Wafer_Length/2.,
                                     BOX_Wafer_Width/2.,
                                     BOX_PCB_Thickness/2.+0.1*mm);
      
      G4Box*  Wafer = new G4Box("Wafer",
                                BOX_Wafer_Length/2.,
                                BOX_Wafer_Width/2.,
                                m_ThicknessBOX[i][j]/2.);
      
      G4Box*  ActiveWafer = new G4Box("ActiveWafer",
                                      BOX_ActiveWafer_Length/2.,
                                      BOX_ActiveWafer_Width/2.,
                                      m_ThicknessBOX[i][j]/2.);
      
      G4double BOX_PCB_Slot_Width;
      G4double BOX_PCB_Slot_Deepness;
      G4double BOX_PCB_Slot_Position;
      G4double BOX_DetectorSpacing;
      
      if(m_ThicknessPAD[i][j]>0){ //PAD Case
        BOX_PCB_Slot_Width = BOX_PCB_Slot_Width2;
        BOX_PCB_Slot_Deepness = BOX_PCB_Slot_Deepness2;
        BOX_PCB_Slot_Position = BOX_PCB_Slot_Position2;
        BOX_DetectorSpacing =  BOX_DetectorSpacing2;
      }
      
      else{ // No Pad Case
        BOX_PCB_Slot_Width = BOX_PCB_Slot_Width1;
        BOX_PCB_Slot_Deepness = BOX_PCB_Slot_Deepness1;
        BOX_PCB_Slot_Position = BOX_PCB_Slot_Position1;
        BOX_DetectorSpacing = BOX_DetectorSpacing1 ;
      }
      
      G4Box*  SlotShape = new G4Box("SlotShape",
                                    BOX_PCB_Slot_Width/2.,
                                    BOX_PCB_Width/2.+0.1*mm,
                                    BOX_PCB_Slot_Deepness);
     

      G4ThreeVector Box_Wafer_Offset ;
      if(m_Z[i]<0 ){
       Box_Wafer_Offset = G4ThreeVector(BOX_Wafer_Length_Offset1, BOX_Wafer_Width_Offset1,0 );
      }

      else{
        Box_Wafer_Offset = G4ThreeVector(BOX_Wafer_Length_Offset2, BOX_Wafer_Width_Offset2,0 );   
      }

      G4SubtractionSolid* PCB1 = new G4SubtractionSolid("PCB", PCBFull, SlotShape,new G4RotationMatrix,G4ThreeVector(BOX_PCB_Slot_Position, 0,0.5*BOX_PCB_Thickness));
      G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCB1, WaferShape,new G4RotationMatrix,Box_Wafer_Offset);
      
      // Master Volume
      G4LogicalVolume* logicBoxDetector =
        new G4LogicalVolume(PCB1,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
      logicBoxDetector->SetVisAttributes(G4VisAttributes::Invisible);
      // Sub Volume PCB
      G4LogicalVolume* logicPCB =
        new G4LogicalVolume(PCB,m_MaterialPCB,"logicPCB", 0, 0, 0);
      logicPCB->SetVisAttributes(PCBVisAtt);
      
      // Sub Volume Wafer
      G4LogicalVolume* logicWafer =
        new G4LogicalVolume(Wafer,m_MaterialSilicon,"logicWafer", 0, 0, 0);
      G4LogicalVolume* logicActiveWafer =
        new G4LogicalVolume(ActiveWafer,m_MaterialSilicon,"logicActiveWafer", 0, 0, 0);

      logicWafer->SetVisAttributes(SiliconVisAtt);
      logicActiveWafer->SetVisAttributes(SiliconVisAtt);

      // Place the sub volume in the master volume
      new G4PVPlacement(new G4RotationMatrix(0,0,0),
                        G4ThreeVector(0,0,0),
                        logicPCB,"Box_PCB",logicBoxDetector,false,DetNbr);
      
      if(m_ThicknessBOX[i][j]>0){
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          Box_Wafer_Offset+G4ThreeVector(0,0,0.5*BOX_PCB_Thickness-0.5*m_ThicknessBOX[i][j]),
                          logicWafer,"Box_Wafer",logicBoxDetector,false,DetNbr);
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          G4ThreeVector(0,0,0),
                          logicActiveWafer,"Box_ActiveWafer",logicWafer,false,DetNbr);
      
        logicActiveWafer->SetSensitiveDetector(m_BOXScorer);
      }
            
      // create the PAD
      // Make a single detector geometry
      G4LogicalVolume* logicPADDetector = NULL;
      
      G4ThreeVector PAD_Wafer_Offset =
      G4ThreeVector(PAD_Wafer_Length_Offset, PAD_Wafer_Width_Offset,0 );
      if(m_ThicknessPAD[i][j]>0){
        G4Box*  PADDetector = new G4Box("PADDetector"  ,
                                        PAD_PCB_Length/2.,
                                        PAD_PCB_Width/2.,
                                        PAD_PCB_Thickness/2.);
        
        G4Box*  PADPCBFull = new G4Box("PCBFull"  ,
                                       PAD_PCB_Length/2.,
                                       PAD_PCB_Width/2.,
                                       PAD_PCB_Thickness/2.);
        
        G4Box*  PADWaferShape = new G4Box("PADWaferShape",
                                          PAD_Wafer_Length/2.,
                                          PAD_Wafer_Width/2.,
                                          PAD_PCB_Thickness/2.+0.1*mm);
        
        G4Box*  PADWafer       = new G4Box("PADWafer",
                                           PAD_Wafer_Length/2.,
                                           PAD_Wafer_Width/2.,
                                           m_ThicknessPAD[i][j]/2.);
        
        G4SubtractionSolid* PADPCB = new G4SubtractionSolid("PADPCB", PADPCBFull, PADWaferShape,new G4RotationMatrix,PAD_Wafer_Offset);
        
        // Master Volume
        logicPADDetector =
        new G4LogicalVolume(PADDetector,m_MaterialVacuum,"logicPADDetector", 0, 0, 0);
        logicPADDetector->SetVisAttributes(G4VisAttributes::Invisible);
        
        // Sub Volume PCB
        G4LogicalVolume* logicPADPCB =
        new G4LogicalVolume(PADPCB,m_MaterialPCB,"logicPADPCB", 0, 0, 0);
        logicPADPCB->SetVisAttributes(PADVisAtt);
        
        // Sub Volume Wafer
        G4LogicalVolume* logicPADWafer =
        new G4LogicalVolume(PADWafer,m_MaterialSilicon,"logicPADWafer", 0, 0, 0);
        logicPADWafer->SetVisAttributes(SiliconVisAtt);
        
        logicPADWafer->SetSensitiveDetector(m_PADScorer);
        
        // Place the sub volume in the master volume
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          G4ThreeVector(0,0,0),
                          logicPADPCB,"PAD_PCB",logicPADDetector,false,DetNbr);
        
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          PAD_Wafer_Offset-G4ThreeVector(0,0,0.5*PAD_PCB_Thickness-0.5*m_ThicknessPAD[i][j]),
                          logicPADWafer,"PAD_Wafer",logicPADDetector,false,DetNbr);
      }
      ///////////////////////////////////////////////////////////////////////////////////
      // Place the detector in the world
      // Position of the center of the PCB
      
      G4ThreeVector DetectorPosition;
      if(m_ThicknessPAD[i][j]>0){ //PAD Case
        DetectorPosition = G4ThreeVector(-BOX_CenterOffset2,-Box_Wafer_Offset.y(),0);
      }
      
      else{ // No Pad Case
        DetectorPosition = G4ThreeVector(-BOX_CenterOffset1,-Box_Wafer_Offset.y(),0);
      }
      
      // Distance of the PCB to the target
      G4ThreeVector DetectorSpacing =
        -G4ThreeVector(0,0,BOX_DetectorSpacing);
      
      DetectorPosition+=DetectorSpacing;
      
      G4ThreeVector PADDetectorPosition = DetectorPosition ;
      G4ThreeVector PADDetectorSpacing =
        -G4ThreeVector(0,0,0.5*BOX_PCB_Thickness+0.5*PAD_PCB_Thickness);
      
    
      PADDetectorPosition += PADDetectorSpacing;
      
      G4RotationMatrix* DetectorRotation= new G4RotationMatrix;
      // The Rotation Matrix is different for each detector
      if(j==0){
        DetectorRotation->rotateX(90*deg);
      }
      else if(j==1){
        DetectorRotation->rotateZ(180*deg);
        DetectorRotation->rotateX(-90*deg);
      }
      else if(j==2){
        DetectorRotation->rotateX(90*deg);
        DetectorRotation->rotateZ(90*deg);
      }
      else if(j==3){
        DetectorRotation->rotateX(90*deg);
        DetectorRotation->rotateZ(-90*deg);
      }
    
      double Z = 0;
      if(m_Z[i]<0){
        DetectorRotation->rotateY(180*deg);
        Z = m_Z[i] -2*mm;
      }

      else
        Z= m_Z[i]+2*mm;

      DetectorPosition.transform(*DetectorRotation);
      DetectorPosition+=G4ThreeVector(0,0,Z);
      
      PADDetectorPosition.transform(*DetectorRotation);
      PADDetectorPosition+=G4ThreeVector(0,0,Z);
      
      new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition), logicBoxDetector,"Box",world,false,DetNbr);
      
      if(m_ThicknessPAD[i][j]>0){
        new G4PVPlacement(G4Transform3D(*DetectorRotation, PADDetectorPosition),
                          logicPADDetector,"PAD",world,false,DetNbr);
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::ConstructQQQDetector(G4LogicalVolume* world){
  // create the QQQ
  for(unsigned int i = 0 ; i < m_Pos.size() ; i++){
    int DetNbr = 0;
    if(m_Pos[i].z()<0) DetNbr = i+1;
    else               DetNbr = i+1 + 8;
    // Make the a single detector geometry
    G4Tubs*  QQQDetector = new G4Tubs("QQQDetector"  ,
                                      QQQ_PCB_Inner_Radius,
                                      QQQ_PCB_Outer_Radius,
                                      QQQ_PCB_Thickness*0.5,
                                      0.,
                                      M_PI/2.);
    
    G4Tubs*  PCBFull = new G4Tubs("PCBFull"  ,
                                  QQQ_PCB_Inner_Radius,
                                  QQQ_PCB_Outer_Radius,
                                  QQQ_PCB_Thickness*0.5,
                                  0.,
                                  M_PI*0.5);
    
    G4Tubs*  WaferShape = new G4Tubs("WaferShape"  ,
                                     QQQ_Wafer_Inner_Radius,
                                     QQQ_Wafer_Outer_Radius,
                                     QQQ_PCB_Thickness*0.5+0.1*mm,
                                     QQQ_Wafer_Starting_Phi,
                                     QQQ_Wafer_Stopping_Phi);
    
    G4Tubs*  Wafer = new G4Tubs("Wafer"  ,
                                QQQ_Wafer_Inner_Radius,
                                QQQ_Wafer_Outer_Radius,
                                m_ThicknessQQQ[i]*0.5,
                                QQQ_Wafer_Starting_Phi,
                                QQQ_Wafer_Stopping_Phi);
    
    G4SubtractionSolid* PCB = 
      new G4SubtractionSolid("PCB", PCBFull, WaferShape,new G4RotationMatrix,
                              G4ThreeVector(0, 0,0));
    
    // Master Volume
    G4LogicalVolume* logicQQQDetector =
    new G4LogicalVolume(QQQDetector,m_MaterialVacuum,"logicQQQDetector", 0, 0, 0);
    logicQQQDetector->SetVisAttributes(G4VisAttributes::Invisible);
    // Sub Volume PCB
    G4LogicalVolume* logicPCB =
    new G4LogicalVolume(PCB,m_MaterialPCB,"logicPCB", 0, 0, 0);
    logicPCB->SetVisAttributes(PCBVisAtt);
    
    // Sub Volume Wafer
    G4LogicalVolume* logicWafer =
    new G4LogicalVolume(Wafer,m_MaterialSilicon,"logicWafer", 0, 0, 0);
    logicWafer->SetVisAttributes(SiliconVisAtt);
    
    logicWafer->SetSensitiveDetector(m_QQQScorer);
    
    
    // Place the sub volume in the master volume
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                      G4ThreeVector(0,0,0),
                      logicPCB,"QQQ_PCB",logicQQQDetector,false,DetNbr);
    
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                      G4ThreeVector(0,0,0),
                      logicWafer,"QQQ_Wafer",logicQQQDetector,false,DetNbr);
    
    // Place the masters volume in the world
    
    new G4PVPlacement(new G4RotationMatrix(0,0,m_Pos[i].y()),
                      G4ThreeVector(0,0,m_Pos[i].z()),
                      logicQQQDetector,"QQQ",world,false,DetNbr);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Sharc::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("Sharc", "TSharcData", &m_Event) ;
  pTree->SetBranchAddress("Sharc", &m_Event) ;

}
     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Sharc::ReadSensitive(const G4Event* event){
  m_Event->Clear();
  
  ///////////
  // BOX
  G4THitsMap<G4double*>* BOXHitMap;
  std::map<G4int, G4double**>::iterator BOX_itr;
  
  G4int BOXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_BOXScorer/SharcBOX");
  BOXHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(BOXCollectionID));
  
  // Loop on the BOX map
  for (BOX_itr = BOXHitMap->GetMap()->begin() ; BOX_itr != BOXHitMap->GetMap()->end() ; BOX_itr++){
    
    G4double* Info = *(BOX_itr->second);
    
    double Energy = Info[0];
    
    if(Energy>EnergyThreshold){
      double Time       = Info[1];
      int DetNbr        = (int) Info[7];
      int StripFront    = (int) Info[8];
      int StripBack     = (int) Info[9];

      m_Event->SetFront_DetectorNbr(DetNbr);
      m_Event->SetFront_StripNbr(StripFront);
      m_Event->SetFront_Energy(RandGauss::shoot(Energy, ResoEnergy));
      m_Event->SetFront_TimeCFD(RandGauss::shoot(Time, ResoTime));
      m_Event->SetFront_TimeLED(RandGauss::shoot(Time, ResoTime));
      
      m_Event->SetBack_DetectorNbr(DetNbr);
      m_Event->SetBack_StripNbr(BOX_Wafer_Back_NumberOfStrip-StripBack+1);
      m_Event->SetBack_Energy(RandGauss::shoot(Energy, ResoEnergy));
      m_Event->SetBack_TimeCFD(RandGauss::shoot(Time, ResoTime));
      m_Event->SetBack_TimeLED(RandGauss::shoot(Time, ResoTime));
      
      // Interraction Coordinates
      ms_InterCoord->SetDetectedPositionX(Info[2]) ;
      ms_InterCoord->SetDetectedPositionY(Info[3]) ;
      ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
      ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
      ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
      
    }
  }
  // clear map for next event
  BOXHitMap->clear();
  
  ///////////
  // PAD
  G4THitsMap<G4double*>*     PADHitMap;
  std::map<G4int, G4double**>::iterator    PAD_itr;
  
  G4int PADCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_PADScorer/SharcPAD");
  PADHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PADCollectionID));
  
  // Loop on the BOX map
  for (PAD_itr = PADHitMap->GetMap()->begin() ; PAD_itr != PADHitMap->GetMap()->end() ; PAD_itr++){
    
    G4double* Info = *(PAD_itr->second);
    
    double Energy =  Info[0];
    if(Energy>EnergyThreshold){
      double Time  = Info[1];
      int DetNbr =     (int) Info[7];
      
      m_Event->SetPAD_DetectorNbr(DetNbr);
      m_Event->SetPAD_Energy(RandGauss::shoot(Energy, ResoEnergy));
      m_Event->SetPAD_TimeCFD(RandGauss::shoot(Time, ResoTime));
      m_Event->SetPAD_TimeLED(RandGauss::shoot(Time, ResoTime));
    }
  }
  
  // clear map for next event
  PADHitMap->clear();
  
  ///////////
  // QQQ
  G4THitsMap<G4double*>* QQQHitMap;
  std::map<G4int, G4double**>::iterator QQQ_itr;
  
  G4int QQQCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_QQQScorer/SharcQQQ");
  QQQHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(QQQCollectionID));
  
  // Loop on the BOX map
  for (QQQ_itr = QQQHitMap->GetMap()->begin() ; QQQ_itr != QQQHitMap->GetMap()->end() ; QQQ_itr++){
    
    G4double* Info = *(QQQ_itr->second);
    
    double Energy =  Info[0];
    if(Energy>EnergyThreshold){
      double Time  = Info[1];
      int DetNbr =     (int) Info[7];
      int StripFront = (int) Info[8];
      int StripBack =  (int) Info[9];
      
      m_Event->SetFront_DetectorNbr(DetNbr);
      m_Event->SetFront_StripNbr(QQQ_Wafer_NumberOf_AnnularStrip-StripFront+1); // Order is reverse (1 is outtermost strip)

      m_Event->SetFront_Energy(RandGauss::shoot(Energy, ResoEnergy));
      m_Event->SetFront_TimeCFD(RandGauss::shoot(Time, ResoTime));
      m_Event->SetFront_TimeLED(RandGauss::shoot(Time, ResoTime));
      
      m_Event->SetBack_DetectorNbr(DetNbr);
      m_Event->SetBack_StripNbr(StripBack);
      m_Event->SetBack_Energy(RandGauss::shoot(Energy, ResoEnergy));
      m_Event->SetBack_TimeCFD(RandGauss::shoot(Time, ResoTime));
      m_Event->SetBack_TimeLED(RandGauss::shoot(Time, ResoTime));
      
      // Interraction Coordinates
      ms_InterCoord->SetDetectedPositionX(Info[2]) ;
      ms_InterCoord->SetDetectedPositionY(Info[3]) ;
      ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
      ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
      ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
    }
  }
  
  // clear map for next event
  QQQHitMap->clear();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::InitializeScorers(){
  
  //   Silicon Associate Scorer
  bool already_exist = false;
  m_BOXScorer = CheckScorer("Sharc_BOXScorer",already_exist);
  m_PADScorer = CheckScorer("Sharc_PADScorer",already_exist);
  m_QQQScorer = CheckScorer("Sharc_QQQScorer",already_exist);
 
  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;
 
  G4VPrimitiveScorer* BOXScorer =
  new  SILICONSCORERS::PS_Silicon_Rectangle("SharcBOX",0,
                                   BOX_ActiveWafer_Length,
                                   BOX_ActiveWafer_Width,
                                   BOX_Wafer_Front_NumberOfStrip ,
                                   BOX_Wafer_Back_NumberOfStrip);
  
  G4VPrimitiveScorer* PADScorer =
  new  SILICONSCORERS::PS_Silicon_Rectangle("SharcPAD",0,
                                   PAD_Wafer_Length,
                                   PAD_Wafer_Width,
                                   1 ,
                                   1);
  
  G4VPrimitiveScorer* QQQScorer =
  new  SILICONSCORERS::PS_Silicon_Annular("SharcQQQ",0,
                                 QQQ_Wafer_Inner_Radius,
                                 QQQ_Wafer_Outer_Radius,
                                 QQQ_Wafer_Starting_Phi,
                                 QQQ_Wafer_Stopping_Phi,
                                 QQQ_Wafer_NumberOf_AnnularStrip,
                                 QQQ_Wafer_NumberOf_RadialStrip,1);
  
  //and register it to the multifunctionnal detector
  m_BOXScorer->RegisterPrimitive(BOXScorer);
  m_PADScorer->RegisterPrimitive(PADScorer);
  m_QQQScorer->RegisterPrimitive(QQQScorer);
  
  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_BOXScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_PADScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_QQQScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void Sharc::InitializeMaterial(){
  m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
  m_MaterialPCB = MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");

}


 ////////////////////////////////////////////////////////////////////////////////
 //            Construct Method to be pass to the DetectorFactory              //
 ////////////////////////////////////////////////////////////////////////////////
 NPS::VDetector* Sharc::Construct(){
  return  (NPS::VDetector*) new Sharc();
 }

 ////////////////////////////////////////////////////////////////////////////////
 //            Registering the construct method to the factory                 //
 ////////////////////////////////////////////////////////////////////////////////
 extern"C" {
 class proxy_nps_sharc{
   public:
    proxy_nps_sharc(){
      NPS::DetectorFactory::getInstance()->AddToken("Sharc","Sharc");
      NPS::DetectorFactory::getInstance()->AddDetector("Sharc",Sharc::Construct);
    }
};

 proxy_nps_sharc p_nps_sharc;
 }
