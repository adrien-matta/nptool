// NPL
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"NPVAnalysis.h"
#include"NPAnalysisFactory.h"
#include"RootInput.h"
#include"RootOutput.h"

// STL
#include<iostream>
#include <dlfcn.h>

#ifndef SHARED_LIB_EXTENSION_GLOB
#define SHARED_LIB_EXTENSION_GLOB
#ifdef __linux__
std::string SHARED_LIB_EXTENSION = ".so";
#endif

#ifdef __FreeBSD__
std::string SHARED_LIB_EXTENSION = ".so";
#endif

#ifdef __APPLE__
std::string SHARED_LIB_EXTENSION = ".dylib";
#endif
#endif

int main(int argc , char** argv){
  // command line parsing
  NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);
  
  // if input files are not given, use those from TAsciiFile
  if (myOptionManager->IsDefault("DetectorConfiguration")) {
    string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
    myOptionManager->SetDetectorFile(name);
  }

  // get input files from NPOptionManager
  string detectorfileName    = myOptionManager->GetDetectorFile();
  string OutputfileName      = myOptionManager->GetOutputFile();

  // Instantiate RootOutput
  RootOutput::getInstance("Analysis/"+OutputfileName, "ResultTree");
  TTree* tree= RootOutput::getInstance()->GetTree();
  
  // Instantiate the detector using a file
  NPA::DetectorManager* myDetector = new DetectorManager();
  myDetector->ReadConfigurationFile(detectorfileName);

  // Attempt to load an analysis
  NPA::VAnalysis* UserAnalysis = NULL;
  string libName = "libNPAnalysis" + SHARED_LIB_EXTENSION;
  dlopen(libName.c_str(),RTLD_NOW);
  char* error = dlerror();
  if(error==NULL){
    UserAnalysis = NPA::AnalysisFactory::getInstance()->Construct(); 
    UserAnalysis->SetDetectorManager(myDetector);
    UserAnalysis->Init();
  }

  std::cout << std::endl << "///////// Starting Analysis ///////// "<< std::endl;
  TChain* Chain = RootInput:: getInstance()->GetChain();
  myOptionManager->GetNumberOfEntryToAnalyse();
  int nentries = Chain->GetEntries();
  if(nentries> myOptionManager->GetNumberOfEntryToAnalyse() && myOptionManager->GetNumberOfEntryToAnalyse()>0)
    nentries = myOptionManager->GetNumberOfEntryToAnalyse() ;
  std::cout << " Number of Event to be treated : " << nentries << std::endl;


  if(UserAnalysis==NULL){ 
    for (int i = 0 ; i < nentries; i++) { 
      // Get the raw Data
      Chain -> GetEntry(i);
      // Clear previous Event
      myDetector->ClearEventPhysics();
      // Build the current event
      myDetector->BuildPhysicalEvent();
      // Fill the tree
      tree->Fill();
    }
  }

  else{
    for (int i = 0 ; i < nentries; i++) { 
      // Get the raw Data
      Chain -> GetEntry(i);
      // Clear previous Event
      myDetector->ClearEventPhysics();
      // Build the current event
      myDetector->BuildPhysicalEvent();
      // User Analysis
      UserAnalysis->TreatEvent();
      // Fill the tree
      tree->Fill();
    }
    UserAnalysis->End();
  }
 
  RootOutput::getInstance()->Destroy();
  return 0;
}
