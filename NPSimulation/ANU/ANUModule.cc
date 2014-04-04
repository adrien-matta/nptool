/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the ANU detector.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "ANUModule.hh"
#include "RootOutput.h"


TANUData *ANUModule::ms_Event = 0;



ANUModule::ANUModule()
{
   if (ms_Event == 0) ms_Event = new TANUData();

   InitializeRootOutput();
   InitializeIndex();
}



ANUModule::~ANUModule()
{
}



void ANUModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("ANU"))
      pTree->Branch("ANU", "TANUData", &ms_Event);
}



void ANUModule::InitializeIndex()
{
  //   m_index["Square"]     =    0;
  //   m_index["Trapezoid"]  =  100;
  //   m_index["Annular"]    =  200;
   m_index["DummyShape"] = 1000;
}
