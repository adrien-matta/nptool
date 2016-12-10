/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 16/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define the S1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  + 11/10/09: Change scorer philosophy, one scorer for the detector number *
 *              added (N. de Sereville)                                      *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// Geant4 
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "Spice.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "NPOptionManager.h"
// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Spice::Spice(){
  m_LogicalDetector=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Spice::~Spice(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Spice::ConstructVolume(){
  if(!m_LogicalDetector){
    m_gdmlparser.Read("Spice/Spice.gdml");
    m_LogicalDetector= m_gdmlparser.GetVolume("World");
    // Set a few visual attribute
    G4VisAttributes* MagnetVisAtt = new G4VisAttributes(G4Colour(0.2,0.2, 1, 0.5)) ;
    G4VisAttributes* PhotonShieldVisAtt = new G4VisAttributes(G4Colour(0.2,1, 0.2, 0.5)) ;

    G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
    G4VisAttributes* ColdFingerVisAtt = new G4VisAttributes(G4Colour(0.8, 0.3, 0.3)) ;
    G4VisAttributes* TargetMechanismVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.3)) ;

    G4VisAttributes* ChamberVisAtt
      = new G4VisAttributes(G4Colour(0.0,0.4,0.5,0.2));

    // World box
    m_LogicalDetector->SetVisAttributes(G4VisAttributes::Invisible); 

    // chamber
    m_gdmlparser.GetVolume("electro_box_log")->SetVisAttributes(ChamberVisAtt);
    m_gdmlparser.GetVolume("target_chamber_front_ring_log")->SetVisAttributes(ChamberVisAtt);
    m_gdmlparser.GetVolume("target_chamber_front_cone_log")->SetVisAttributes(ChamberVisAtt);
    m_gdmlparser.GetVolume("target_chamber_sphere_log")->SetVisAttributes(ChamberVisAtt);
    m_gdmlparser.GetVolume("target_chamber_cylinder_down_log")->SetVisAttributes(ChamberVisAtt);

    // Cold Finger
    m_gdmlparser.GetVolume("cold_finger_log")->SetVisAttributes(ColdFingerVisAtt);


    // Magnet
    m_gdmlparser.GetVolume("magnet_log")->SetVisAttributes(MagnetVisAtt);  
    m_gdmlparser.GetVolume("magnet_clamp_chamber_log")->SetVisAttributes(MagnetVisAtt);  
    m_gdmlparser.GetVolume("magnet_clamp_shield_log")->SetVisAttributes(MagnetVisAtt);  

    // Photon Shield
    m_gdmlparser.GetVolume("photon_shield_layer_one_log")->SetVisAttributes(PhotonShieldVisAtt);  
    m_gdmlparser.GetVolume("photon_shield_layer_two_log")->SetVisAttributes(PhotonShieldVisAtt);  
    m_gdmlparser.GetVolume("photon_shield_layer_three_log")->SetVisAttributes(PhotonShieldVisAtt);  
    m_gdmlparser.GetVolume("ps_detector_clamp_log")->SetVisAttributes(PhotonShieldVisAtt);  
    m_gdmlparser.GetVolume("ps_target_clamp_log")->SetVisAttributes(PhotonShieldVisAtt);  


    // Target Mechanism
    m_gdmlparser.GetVolume("target_wheel_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("first_gear_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("second_gear_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("third_gear_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("gear_plate_one_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("gear_plate_two_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("gear_stick_log")->SetVisAttributes(TargetMechanismVisAtt);
    m_gdmlparser.GetVolume("target_mount_plate_log")->SetVisAttributes(TargetMechanismVisAtt);




    // S3 color
    for(unsigned int ring = 1 ; ring < 24 ; ring++){  
      ostringstream os;
      os << "siDetS3Ring_" << ring << "_Log";
      m_gdmlparser.GetVolume(os.str())->SetVisAttributes(SiliconVisAtt); 
    }

    //Spice color
    for(unsigned int ring = 0 ; ring < 10 ; ring++){  
      ostringstream os;
      os << "siDetSpiceRing_" << ring << "_Log";
      m_gdmlparser.GetVolume(os.str())->SetVisAttributes(SiliconVisAtt); 
    }
    m_gdmlparser.GetVolume("innerGuardRing")->SetVisAttributes(SiliconVisAtt);    
    m_gdmlparser.GetVolume("outerGuardRing")->SetVisAttributes(SiliconVisAtt);   
  }

  return m_LogicalDetector;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Spice::ReadConfiguration(NPL::InputParser){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Spice::ConstructDetector(G4LogicalVolume* world){
  ConstructVolume();
  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      G4ThreeVector(0,0,0),
      m_LogicalDetector,"Spice",world,false,1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void Spice::InitializeRootOutput(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Spice::ReadSensitive(const G4Event* event){
  if(event)
    event = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Initilize the Scorer use to read out the sensitive volume 
void Spice::InitializeScorers(){
  m_Scorer = 0;
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Spice::Construct(){
  return (NPS::VDetector*) new Spice();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_nps_spice{
  public:
    proxy_nps_spice(){
      NPS::DetectorFactory::getInstance()->AddToken("Spice","Spice");
      NPS::DetectorFactory::getInstance()->AddDetector("Spice",Spice::Construct);
    }
};

proxy_nps_spice p_nps_spice;
}


