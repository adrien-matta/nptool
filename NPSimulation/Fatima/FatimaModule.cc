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
 *             derive all different modules from the Fatima detector.         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "FatimaModule.hh"
#include "RootOutput.h"


TFatimaData *FatimaModule::ms_Event = 0;



FatimaModule::FatimaModule()
{
   if (ms_Event == 0) ms_Event = new TFatimaData();

   InitializeRootOutput();
   InitializeIndex();
}



FatimaModule::~FatimaModule()
{
}



void FatimaModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("FATIMA"))
      pTree->Branch("FATIMA", "TFatimaData", &ms_Event);
}



void FatimaModule::InitializeIndex()
{
  m_index["LaBr"]     =    0;  // 24 LaBr max
}
