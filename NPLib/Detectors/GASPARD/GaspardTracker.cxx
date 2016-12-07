/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 31/08/09                                                 *
 * Last update    : 10/09/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class is mainly an interface to the                      *
 *             TGaspardTrackerPhysics class and it deals with the geometrical*
 *             correspondance between strip number and absolute coordinates  *
 *             (X, Y, Z) of interaction.                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   + 10/09/09: Add support for Square and DummyShape shapes                *
 *               (N. de Sereville)                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "GaspardTracker.h"

// C++ headers
#include <iostream>   
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>

// NPL headers
#include "RootInput.h"
#include "RootOutput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

// ROOT headers
#include "TChain.h"

// Gaspard headers
#include "GaspardTrackerDummyShape.h"
#include "GaspardTrackerTrapezoid.h"
#include "GaspardTrackerAnnular.h"
#include "GaspardTrackerSquare.h"
#include "GaspardTrackerRectangle.h"

using namespace std ;   
////////////////////////////////////////////////////////////////////////////////
GaspardTracker::GaspardTracker(){
  m_EventData= new TGaspardTrackerData();
  m_EventPhysics=new TGaspardTrackerPhysics();
}   


////////////////////////////////////////////////////////////////////////////////
GaspardTracker::~GaspardTracker(){
  delete m_EventData;
  delete m_EventPhysics;
}   


////////////////////////////////////////////////////////////////////////////////
void GaspardTracker::Clear(){
}


////////////////////////////////////////////////////////////////////////////////
// Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void GaspardTracker::ReadConfiguration(NPL::InputParser parser ){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GaspardTracker");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 


  bool GPDTrkSquare = false;
  bool GPDTrkAnnular= false;
  bool GPDTrkTrapezoid= false;
  bool GPDTrkDummyShape= false;
  bool GPDTrkRectangle= false;

  for(unsigned int i = 0 ; i < blocks.size() ; i++){

    if(blocks[i]->GetMainValue() == "Square" && !GPDTrkSquare){
      GPDTrkSquare = true;

      // instantiate a new "detector" corresponding to the Square elements
      GaspardTrackerModule* myDetector = new GaspardTrackerSquare(m_ModulesMap, m_EventPhysics);

      // Pass the data object to the GaspardTracker*** object
      myDetector->SetGaspardDataPointer(m_EventData);

      // read part of the configuration file corresponding to square elements
      myDetector->ReadConfiguration(parser);
    }
    else if(blocks[i]->GetMainValue() == "Annular" && !GPDTrkAnnular){
      GPDTrkAnnular= true;

      // instantiate a new "detector" corresponding to the Square elements
      GaspardTrackerModule* myDetector = new GaspardTrackerAnnular(m_ModulesMap, m_EventPhysics);

      // Pass the data object to the GaspardTracker*** object
      myDetector->SetGaspardDataPointer(m_EventData);

      // read part of the configuration file corresponding to square elements
      myDetector->ReadConfiguration(parser);
    }
    else if(blocks[i]->GetMainValue() == "Trapezoid" &&! GPDTrkTrapezoid){
      GPDTrkTrapezoid= true;

      // instantiate a new "detector" corresponding to the Square elements
      GaspardTrackerModule* myDetector = new GaspardTrackerTrapezoid(m_ModulesMap, m_EventPhysics);

      // Pass the data object to the GaspardTracker*** object
      myDetector->SetGaspardDataPointer(m_EventData);

      // read part of the configuration file corresponding to square elements
      myDetector->ReadConfiguration(parser);
    }
    else if(blocks[i]->GetMainValue() == "Rectangle" && !GPDTrkRectangle){
      GPDTrkRectangle= true;

      // instantiate a new "detector" corresponding to the Square elements
      GaspardTrackerModule* myDetector = new GaspardTrackerRectangle(m_ModulesMap, m_EventPhysics);

      // Pass the data object to the GaspardTracker*** object
      myDetector->SetGaspardDataPointer(m_EventData);

      // read part of the configuration file corresponding to square elements
      myDetector->ReadConfiguration(parser);
    }
    else if(blocks[i]->GetMainValue() == "DummyShape" && !GPDTrkDummyShape){
      GPDTrkDummyShape= true;

      // instantiate a new "detector" corresponding to the Square elements
      GaspardTrackerModule* myDetector = new GaspardTrackerDummyShape(m_ModulesMap, m_EventPhysics);

      // Pass the data object to the GaspardTracker*** object
      myDetector->SetGaspardDataPointer(m_EventData);

      // read part of the configuration file corresponding to square elements
      myDetector->ReadConfiguration(parser);
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
// Read stream at Path and pick-up calibration parameter using Token
// If argument is "Simulation" no change calibration is loaded
void GaspardTracker::ReadCalibrationFile(string Path){
  Path.c_str();   // remove warning at compilation time

  /*   // Order of Polynom function used for calibration
       int Calibration_Si_E_Order;
       int Calibration_Si_T_Order;
       int Calibration_SiLi_E_Order;
       int Calibration_CsI_E_Order;

  // Calibration_Si_X_E[DetectorNumber][StripNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_Si_X_E   ;
  vector< vector< vector< double > > >   Calibration_Si_X_T   ;
  vector< vector< vector< double > > >   Calibration_Si_Y_E   ;
  vector< vector< vector< double > > >   Calibration_Si_Y_T   ;

  // Calibration_SiLi_E[DetectorNumber][PadNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_SiLi_E   ;

  // Calibration_SiLi_E[DetectorNumber][CrystalNumber][Order of Coeff]
  vector< vector< vector< double > > >   Calibration_CsI_E   ;

  if (Path == "Simulation") {   // Simulation case: data already calibrated
  Calibration_Si_E_Order   = 1;
  Calibration_Si_T_Order   = 1;
  Calibration_SiLi_E_Order = 1;
  Calibration_CsI_E_Order  = 1;

  vector<double> Coef;
  // Order 0            Order 1
  Coef.push_back(0) ; Coef.push_back(1)    ;

  vector< vector<double> > StripLine       ;
  StripLine.resize( 128 , Coef)         ;

  Calibration_Si_X_E.resize( m_NumberOfModule , StripLine)   ;
  Calibration_Si_X_T.resize( m_NumberOfModule , StripLine)   ;
  Calibration_Si_Y_E.resize( m_NumberOfModule , StripLine)   ;
  Calibration_Si_Y_T.resize( m_NumberOfModule , StripLine)   ;

  Calibration_SiLi_E.resize( m_NumberOfModule , StripLine)   ;
  Calibration_CsI_E .resize( m_NumberOfModule , StripLine)   ;
  }
  else {
  }
  */
}      

////////////////////////////////////////////////////////////////////////////////
// Activated associated Branches and link it to the private member DetectorData address
// In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void GaspardTracker::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("GASPARD", true);
  inputChain->SetBranchStatus("fGPD*", true);
  inputChain->SetBranchAddress("GASPARD", &m_EventData);
}

////////////////////////////////////////////////////////////////////////////////
void GaspardTracker::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchAddress("GASPARD" , &m_EventPhysics);
}

////////////////////////////////////////////////////////////////////////////////
// Create associated branches and associated private member DetectorPhysics address
void GaspardTracker::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch("GASPARD", "TGaspardTrackerPhysics", &m_EventPhysics);
}

////////////////////////////////////////////////////////////////////////////////
// This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
void GaspardTracker::BuildPhysicalEvent(){
  unsigned int mysize = m_EventData->GetGPDTrkFirstStageFrontEMult() ;

  for (unsigned int i = 0; i < mysize; i++) {
    UShort_t detecNbr = m_EventData->GetGPDTrkFirstStageFrontEDetectorNbr(i);
    m_ModulesMap[detecNbr]->BuildPhysicalEvent();
  }
}

////////////////////////////////////////////////////////////////////////////////
// Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
// This method aimed to be used for analysis performed during experiment, when speed is requiered.
// NB: This method can eventually be the same as BuildPhysicalEvent.
void GaspardTracker::BuildSimplePhysicalEvent(){
  BuildPhysicalEvent();
}

////////////////////////////////////////////////////////////////////////////////
void GaspardTracker::DumpModulesMap(){
  cout << "GaspardTracker::DumpMap()" << endl;

  map<int, GaspardTrackerModule*>::iterator it;

  for (it = m_ModulesMap.begin(); it != m_ModulesMap.end(); ++it) {
    cout << "Module " << it->first << " -> Address " << it->second << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
void GaspardTracker::DumpStrippingScheme(int moduleNumber){
  cout << "GaspardTracker::DumpStrippingScheme()" << endl;

  for (int i = 1; i < 65; i++) {   // front part
    for (int j = 1; j < 65; j++) {   // back part
      cout << "strips X, Y: " << i << "  " << j << "\t--->\t (X,Y,Z) mm: "
        << m_ModulesMap[moduleNumber]->GetStripPositionX(moduleNumber, i, j) << "\t"
        << m_ModulesMap[moduleNumber]->GetStripPositionY(moduleNumber, i, j) << "\t"
        << m_ModulesMap[moduleNumber]->GetStripPositionZ(moduleNumber, i, j) << endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
double GaspardTracker::GetEnergyDeposit()      { 
  if (m_EventPhysics->GetEventMultiplicity() > 0) {
    return m_EventPhysics->GetTotalEnergy(0); 
  }
  else
    return -1000;
}

////////////////////////////////////////////////////////////////////////////////
TVector3 GaspardTracker::GetPositionOfInteraction(){
  TVector3 Position = TVector3(-1000,-1000,-1000);

  if (m_EventPhysics->GetEventMultiplicity() == 1) {
    Int_t ModuleNbr = m_EventPhysics->GetModuleNumber(0);
    Int_t FrontPos  = m_EventPhysics->GetFirstStageFrontPosition(0);
    Int_t BackPos   = m_EventPhysics->GetFirstStageBackPosition(0);
    Position = TVector3(m_ModulesMap[ModuleNbr]->GetStripPositionX(ModuleNbr, FrontPos, BackPos),
        m_ModulesMap[ModuleNbr]->GetStripPositionY(ModuleNbr, FrontPos, BackPos),
        m_ModulesMap[ModuleNbr]->GetStripPositionZ(ModuleNbr, FrontPos, BackPos));
  }

  return Position;
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* GaspardTracker::Construct(){
  return (NPL::VDetector*) new GaspardTracker();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_gaspard{
  public:
    proxy_gaspard(){
      NPL::DetectorFactory::getInstance()->AddToken("GaspardTracker","GASPARD");
      NPL::DetectorFactory::getInstance()->AddDetector("GaspardTracker",GaspardTracker::Construct);
    }
};

proxy_gaspard p_gaspard;
}

