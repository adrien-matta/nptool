/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@atfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 25/08/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Shield detector.         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "ShieldModule.hh"
#include "RootOutput.h"


TShieldData *ShieldModule::ms_Event = 0;



ShieldModule::ShieldModule()
{
   if (ms_Event == 0) ms_Event = new TShieldData();

   InitializeRootOutput();
   InitializeIndex();
}



ShieldModule::~ShieldModule()
{
}



void ShieldModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("SHIELD"))
      pTree->Branch("SHIELD", "TShieldData", &ms_Event);
}



void ShieldModule::InitializeIndex()
{
  m_index["ShieldCl"]=200; // CsI shield
  m_index["ShieldPh"]=300; // CsI shield
}
