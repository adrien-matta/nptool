#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
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
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This Class manage the virtual detector and call their method.            *
 *  Every time a new detector is added to the project, a few line in the     *
 *  ReadConfigurationFile method are needed in order to detect the associate *
 *  token.                                                                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
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

// NPTool headers
#include "VDetector.hh"

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

private:

   // Logical volumes
   G4LogicalVolume* world_log      ;

   // Physical volumes
   G4VPhysicalVolume* world_phys   ;

   // Virtual Detector Vector
   vector<VDetector*> m_Detectors   ;


private: // Data about the target
   G4double m_TargetThickness ;
   G4double m_TargetRadius    ;
   G4double m_TargetX         ;
   G4double m_TargetY         ;
   G4double m_TargetZ         ;

public:  // Getter and Setter
   G4double GetTargetThickness()	{return m_TargetThickness;}
   G4double GetTargetRadius()    	{return m_TargetRadius;}
   G4double GetTargetX() 		{return m_TargetX;}
   G4double GetTargetY() 		{return m_TargetY;}
   G4double GetTargetZ() 		{return m_TargetZ;}
};

#endif

