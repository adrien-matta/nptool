/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    : 02/07/14                                                         *
 *---------------------------------------------------------------------------*
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Nana detector.         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "NanaModule.hh"
#include "RootOutput.h"


TNanaData *NanaModule::ms_Event = 0;



NanaModule::NanaModule()
{
   if (ms_Event == 0) ms_Event = new TNanaData();

   InitializeRootOutput();
   InitializeIndex();
}



NanaModule::~NanaModule()
{
}



void NanaModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("NANA"))
      pTree->Branch("NANA", "TNanaData", &ms_Event);
}



void NanaModule::InitializeIndex()
{
  m_index["LaBr"]     =    0;  // 24 LaBr max
}
