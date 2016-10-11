/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
 *             derive all different modules from the Hyde2 tracker.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "Hyde2TrackerModule.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "RootOutput.h"


THyde2TrackerData *Hyde2TrackerModule::ms_Event = 0;



Hyde2TrackerModule::Hyde2TrackerModule()
{
   if (ms_Event == 0) ms_Event = new THyde2TrackerData();

   InitializeRootOutput();
   InitializeIndex();
}



Hyde2TrackerModule::~Hyde2TrackerModule()
{
}



void Hyde2TrackerModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("HYDE2"))
      pTree->Branch("HYDE2", "THyde2TrackerData", &ms_Event);

    pTree->SetBranchAddress("HYDE2", &ms_Event);

}



void Hyde2TrackerModule::InitializeIndex()
{
   m_index["Square1"]     =  2000;
   m_index["Trapezoid1"]  =  2200;
   m_index["Trapezoid2"]  =  2300;
}



void Hyde2TrackerModule::InitializeMaterial()
{
   m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
   m_MaterialVacuum  = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}

