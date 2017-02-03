#ifndef VDectector_H
#define VDectector_H
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  All detector added in the project should derive from this virtual class  *
 *  A vector ofNPS::VDetector object is manage in the DetectorConstruction class *
 *  and call the virtual method of this class implemented in the daughter    *
 *  class object.                                                            *
 *  This inheritance insure homogeneity and modularity of the code           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Detector inheriting fromNPS::VDetector should follow this Naming convention   *
 *    for logical volume:                                                    *
 *    DetectorNameX_SubPart                                                  *
 *                                                                           *
 *    -DetectorName is the name of the detector, like "MUST2Array",          * 
 *    note that use of _ within the name is prohibitted.                     *
 *                                                                           *
 *    -X is the detector Number, for example if you have two plastic then you*
 *    will have Plastic1 and Plastic2                                        *
 *                                                                           *
 *    -SubPart is the optionnal sub part, like for examples Si and CsI in    *
 *      MUST2. Note that you can add as many as _SubPart you need            *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4LogicalVolume.hh"
#include "G4Event.hh"
#include "G4MultiFunctionalDetector.hh"
// NPLib header
#include "TInteractionCoordinates.h"
#include "NPInputParser.h"

using namespace std;
using namespace CLHEP;

namespace NPS{
class VDetector{
  public:
    //  Default Constructor and destructor
    VDetector()   ;
    virtual ~VDetector()   ;

    // Read stream at Configfile to pick-up parameters of detector (Position,...)
    // Called in DetectorConstruction::ReadDetectorConfiguration Method
    virtual void ReadConfiguration(NPL::InputParser){};

    // Construct detector and inialise sensitive part.
    // Called After DetecorConstruction::AddDetector Method
    virtual void ConstructDetector(G4LogicalVolume*) = 0;

    // Read sensitive part and fill the Root tree.
    // Called at in the EventAction::EndOfEventAvtion
    virtual void ReadSensitive(const G4Event*) = 0;

    // Add Detector branch to the EventTree.
    // Called After DetecorConstruction::AddDetector Method
    virtual void InitializeRootOutput() ;

    // Add Detector branch to the EventTree.
    // Called After DetecorConstruction::AddDetector Method
    virtual void InitializeScorers() {};

    // Check if the scorer already exist, if yes, return it
    // else create one and return it
    // used for updating geometry
    static G4MultiFunctionalDetector* CheckScorer(string ScorerName, bool& exist);

  public:
    TInteractionCoordinates*     GetInterCoordPointer()  {return ms_InterCoord;};

  protected:
    // Class to store the interaction data (i.e. X,Y,Z,theta,phi)
    static TInteractionCoordinates*      ms_InterCoord;
};
}
#endif
