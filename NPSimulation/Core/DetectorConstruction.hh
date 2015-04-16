#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : January 2010                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class manage the virtual detector and call their method.            *
 *  Every time a new detector is added to the project, a few line in the     *
 *  ReadConfigurationFile method are needed in order to detect the associate *
 *  token.                                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   January 2010: added chamber (marc.labiche@stfc.ac.uk)                   *
 *                                                                           *
 *****************************************************************************/
class G4LogicalVolume;
class G4VPhysicalVolume;

// C++ headers
#include <vector>
#include <cmath>

// G4 headers
#include "G4VUserDetectorConstruction.hh"
#include "G4Event.hh"
#include "DetectorMessenger.hh"
// NPTool headers
#include "NPSVDetector.hh"
#include "Target.hh"
#include "Chamber.hh"

// ROOT headers
#include "TTree.h"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
   DetectorConstruction();
   virtual ~DetectorConstruction();

   G4VPhysicalVolume* Construct();
   G4LogicalVolume* GetWorldLogic();
   // Delete every existing bit
   void ClearGeometry();
   // Reload the file configuration
   void RedefineGeometry(string file="");
 
public:
   G4VPhysicalVolume* ReadConfigurationFile();
   void AddDetector(NPS::VDetector*);
   void ReadAllSensitive(const G4Event*);


private:
   // Logical volumes
   G4LogicalVolume* world_log;
  //G4LogicalVolume* Chamber_log;

   // Physical volumes
   G4VPhysicalVolume* world_phys;
  //G4VPhysicalVolume* Chamber_phys;

   // Virtual Detector Vector
   vector<NPS::VDetector*> m_Detectors;


private: // Data about the target
   Target*  m_Target;
   Chamber* m_Chamber;
   DetectorMessenger* m_Messenger;
public:  // Getter and Setter
   Target*  GetTarget()    {return m_Target;}
   Chamber* GetChamber()   {return m_Chamber;}
};

#endif

