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
 *             derive all different modules from the Helios detector.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "HeliosModule.hh"
#include "RootOutput.h"


THeliosData *HeliosModule::ms_Event = 0;



HeliosModule::HeliosModule()
{
   if (ms_Event == 0) ms_Event = new THeliosData();

   InitializeRootOutput();
   InitializeIndex();
}



HeliosModule::~HeliosModule()
{
}



void HeliosModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("HELIOS"))
      pTree->Branch("HELIOS", "THeliosData", &ms_Event);
}



void HeliosModule::InitializeIndex()
{
  //   m_index["Square"]     =    0;
  //   m_index["Trapezoid"]  =  100;
  //   m_index["Annular"]    =  200;
   m_index["DummyShape"] = 1000;
}
