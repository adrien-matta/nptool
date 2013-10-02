#include "GaspardTrackerModule.hh"
#include "RootOutput.h"


TGaspardTrackerData *GaspardTrackerModule::ms_Event = 0;



GaspardTrackerModule::GaspardTrackerModule()
{
   if (ms_Event == 0) ms_Event = new TGaspardTrackerData();

   InitializeRootOutput();
}



GaspardTrackerModule::~GaspardTrackerModule()
{
}



void GaspardTrackerModule::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   // if the branch does not exist yet, create it
   if (!pTree->GetBranch("GASPARD"))
      pTree->Branch("GASPARD", "TGaspardTrackerData", &ms_Event);
}
