/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Hyde tracker.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "HydeTrackerModule.hh"
#include "RootOutput.h"


THydeTrackerData *HydeTrackerModule::ms_Event = 0;



HydeTrackerModule::HydeTrackerModule()
{
   if (ms_Event == 0) ms_Event = new THydeTrackerData();

   InitializeRootOutput();
   InitializeIndex();
}



HydeTrackerModule::~HydeTrackerModule()
{
}



void HydeTrackerModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("HYDE"))
      pTree->Branch("HYDE", "THydeTrackerData", &ms_Event);
}



void HydeTrackerModule::InitializeIndex()
{
   m_index["Square"]     =    0;
   m_index["Trapezoid"]  =  100;
   m_index["Annular"]    =  200;
   m_index["DummyShape"] = 1000;
}
