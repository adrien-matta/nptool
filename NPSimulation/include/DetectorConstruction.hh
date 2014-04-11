#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : 11/04/14  (L. Evitts)                                    *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class manage the virtual detector and call their method.            *
 *  Every time a new detector is added to the project, a few line in the     *
 *  ReadConfigurationFile method are needed in order to detect the associate *
 *  token.                                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    11/04/14: Added Magnetic Field  (evitts@triumf.ca)                     *
 *    08/04/14: Added ANU detector  (evitts@triumf.ca)                       *
 *    25/08/10: Added Shield around PARIS   (marc.labiche@stfc.ac.uk)        *
 *    15/01/10: Added Chamber (marc.labiche@stfc.ac.uk)                      *
 *    04/12/09: Added PARIS detector (marc.labiche@stfc.ac.uk)               *
 *****************************************************************************/
class G4LogicalVolume;
class G4VPhysicalVolume;

// C++ headers
#include <vector>
#include <cmath>

// G4 headers
#include "G4VUserDetectorConstruction.hh"
#include "G4Event.hh"
#include "G4MagneticField.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"

// NPTool headers
#include "VDetector.hh"
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


public:
   void ReadConfigurationFile(string);
   void AddDetector(VDetector*);
   void ReadAllSensitive(const G4Event*);
   void DefineMagneticField(string);

private:
   // Logical volumes
   G4LogicalVolume* world_log;
  //G4LogicalVolume* Chamber_log;

   // Physical volumes
   G4VPhysicalVolume* world_phys;
  //G4VPhysicalVolume* Chamber_phys;

   // Virtual Detector Vector
   vector<VDetector*> m_Detectors;


private: // Data about the target
   Target*  m_Target;
   Chamber* m_Chamber;

public:  // Getter and Setter
   Target*  GetTarget()    {return m_Target;}
   Chamber* GetChamber()   {return m_Chamber;}
   
protected:
  G4MagneticField*        fMagneticField;
  G4FieldManager*         fFieldManager;
  G4Mag_UsualEqRhs*       fEquation; 
  G4MagIntegratorStepper* fStepper;
  G4ChordFinder*          fChordFinder;
};

#endif

