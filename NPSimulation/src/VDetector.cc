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
