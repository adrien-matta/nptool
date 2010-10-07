/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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

#include "TGaspardTrackerPhysicsNew.h"

// C++ headers
#include <iostream>	
#include <fstream>
#include <string>
#include <cmath>
#include <stdlib.h>

// NPL headers
#include "RootInput.h"
#include "RootOutput.h"

// ROOT headers
#include "TChain.h"

// Gaspard headers
#include "TGaspardTrackerDummyShape.h"
#include "TGaspardTrackerTrapezoid.h"

using namespace std ;	




TGaspardTrackerPhysicsNew::TGaspardTrackerPhysicsNew()
	: m_EventData (new TGaspardTrackerData()),
	  m_EventPhysics (this)
{
}	



TGaspardTrackerPhysicsNew::~TGaspardTrackerPhysicsNew()
{
   delete m_EventData;
   delete m_EventPhysics;
}	



void TGaspardTrackerPhysicsNew::Clear()
{
}



// Read stream at ConfigFile to pick-up parameters of detector (Position,...) using Token
void TGaspardTrackerPhysicsNew::ReadConfiguration(string Path) 	
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   bool GPDTrkSquare     = false;
   bool GPDTrkTrapezoid  = false;
   bool GPDTrkAnnular    = false;
   bool GPDTrkDummyShape = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 9, "GPDSquare") == 0  &&  GPDTrkSquare == false) {
         GPDTrkSquare = true;
/*
         // instantiate a new "detector" corresponding to the Square elements
         TGaspardTrackerModule* myDetector = new TGaspardTrackerSquare();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());
*/      }
      else if (LineBuffer.compare(0, 12, "GPDTrapezoid") == 0  &&  GPDTrkTrapezoid == false) {
         GPDTrkTrapezoid = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         TGaspardTrackerModule* myDetector = new TGaspardTrackerTrapezoid(m_ModulesMap);

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());
      }
      else if (LineBuffer.compare(0, 10, "GPDAnnular") == 0  &&  GPDTrkAnnular == false) {
         GPDTrkAnnular = true;
/*
         // instantiate a new "detector" corresponding to the Trapezoid elements
         TGaspardTrackerModule* myDetector = new TGaspardTrackerAnnular();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());
*/      }
      else if (LineBuffer.compare(0, 13, "GPDDummyShape") == 0  &&  GPDTrkDummyShape == false) {
         GPDTrkDummyShape = true;

         // instantiate a new "detector" corresponding to the Shape elements
         // The GaspardTrackerSquare class should be replaced by the
         // GaspardTrackerShape class you need to define
         TGaspardTrackerModule* myDetector = new TGaspardTrackerDummyShape(m_ModulesMap);

         // read part of the configuration file corresponding to shape elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());
      }
   }

   DumpMap();
}


// Read stream at Path and pick-up calibration parameter using Token
// If argument is "Simulation" no change calibration is loaded
void TGaspardTrackerPhysicsNew::ReadCalibrationFile(string Path)	
{
/*   // Order of Polynom function used for calibration
   int Calibration_Si_E_Order;
   int Calibration_Si_T_Order;
   int Calibration_SiLi_E_Order;
   int Calibration_CsI_E_Order;

   // Calibration_Si_X_E[DetectorNumber][StripNumber][Order of Coeff]
   vector< vector< vector< double > > >	Calibration_Si_X_E	;
   vector< vector< vector< double > > >	Calibration_Si_X_T	;
   vector< vector< vector< double > > >	Calibration_Si_Y_E	;
   vector< vector< vector< double > > >	Calibration_Si_Y_T	;

   // Calibration_SiLi_E[DetectorNumber][PadNumber][Order of Coeff]
   vector< vector< vector< double > > >	Calibration_SiLi_E	;

   // Calibration_SiLi_E[DetectorNumber][CrystalNumber][Order of Coeff]
   vector< vector< vector< double > > >	Calibration_CsI_E	;

   if (Path == "Simulation") {   // Simulation case: data already calibrated
      Calibration_Si_E_Order   = 1;
      Calibration_Si_T_Order   = 1;
      Calibration_SiLi_E_Order = 1;
      Calibration_CsI_E_Order  = 1;

      vector<double> Coef;
      // Order 0				Order 1
      Coef.push_back(0) ; Coef.push_back(1) 	;

      vector< vector<double> > StripLine 		;
      StripLine.resize( 128 , Coef)			;

      Calibration_Si_X_E.resize( m_NumberOfModule , StripLine)	;
      Calibration_Si_X_T.resize( m_NumberOfModule , StripLine)	;
      Calibration_Si_Y_E.resize( m_NumberOfModule , StripLine)	;
      Calibration_Si_Y_T.resize( m_NumberOfModule , StripLine)	;
				
      Calibration_SiLi_E.resize( m_NumberOfModule , StripLine)	;
      Calibration_CsI_E .resize( m_NumberOfModule , StripLine)	;
   }
   else {
   }
*/
}		

	

// Activated associated Branches and link it to the private member DetectorData address
// In this method mother Branches (Detector) AND daughter leaf (fDetector_parameter) have to be activated
void TGaspardTrackerPhysicsNew::InitializeRootInput() 		
{
   TChain* inputChain = RootInput::getInstance()->GetChain();
   inputChain->SetBranchStatus("GASPARD", true);
   inputChain->SetBranchStatus("fGPD*", true);
   inputChain->SetBranchAddress("GASPARD", &m_EventData);
}



// Create associated branches and associated private member DetectorPhysics address
void TGaspardTrackerPhysicsNew::InitializeRootOutput() 	
{
   TTree* outputTree = RootOutput::getInstance()->GetTree();
   outputTree->Branch("GASPARD", "TGaspardTrackerPhysicsNew", &m_EventPhysics);
}



// This method is called at each event read from the Input Tree. Aime is to build treat Raw dat in order to extract physical parameter. 
void TGaspardTrackerPhysicsNew::BuildPhysicalEvent()		
{
/*   if (m_ModulesMap.size() > 0) {
      for (UInt_t i = 0; i < m_ModulesMap.size(); i++) {
         m_ModulesMap[i]->BuildPhysicalEvent();
      }
   }*/
}



void TGaspardTrackerPhysicsNew::DumpMap()
{
   cout << "TGaspardTrackerPhysicsNew::DumpMap()" << endl;

   map<int, TGaspardTrackerModule*>::iterator it;

   for (it = m_ModulesMap.begin(); it != m_ModulesMap.end(); ++it) {
      cout << "Module " << it->first << " -> Address " << it->second << endl;
   }
}



// Same as above, but only the simplest event and/or simple method are used (low multiplicity, faster algorythm but less efficient ...).
// This method aimed to be used for analysis performed during experiment, when speed is requiered.
// NB: This method can eventually be the same as BuildPhysicalEvent.
void TGaspardTrackerPhysicsNew::BuildSimplePhysicalEvent()	
{
   BuildPhysicalEvent();
}



double TGaspardTrackerPhysicsNew::GetEnergyDeposit()		
{ 
/*   if (m_EventPhysics->TotalEnergy.size() > 0)
      return m_EventPhysics->TotalEnergy[0]; 
   else*/ 
      return -1000;
}



TVector3 TGaspardTrackerPhysicsNew::GetPositionOfInteraction()
{
   TVector3 Position = TVector3(-1000,-1000,-1000);

/*   if (m_EventPhysics->ModuleNumber.size() == 1) {
      Position = TVector3(GetStripPositionX(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0]),
                          GetStripPositionY(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0]),
                          GetStripPositionZ(m_EventPhysics->ModuleNumber[0], m_EventPhysics->FirstStage_X[0], m_EventPhysics->FirstStage_Y[0])); 
   }
*/
   return(Position);
}
