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
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  All detector added in the project should derive from this virtual class  *
 *  A vector of VDetector object is manage in the DetectorConstruction class *
 *  and call the virtual method of this class implemented in the daughter    *
 *  class object.                                                            *
 *  This inheritance insure homogeneity and modularity of the code           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "VDetector.hh"
#include "RootOutput.h"


TInteractionCoordinates *VDetector::ms_InterCoord = 0;


// Constructor
VDetector::VDetector()
{
   if (ms_InterCoord == 0) ms_InterCoord = new TInteractionCoordinates();

   InitializeRootOutput();
}


// Destructor
VDetector::~VDetector()
{
}


void VDetector::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("InteractionCoordinates"))
      pTree->Branch("InteractionCoordinates", "TInteractionCoordinates", &ms_InterCoord);
}
