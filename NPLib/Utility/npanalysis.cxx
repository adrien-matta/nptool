// NPL
#include"NPDetectorManager.h"
#include"NPOptionManager.h"
#include"NPVAnalysis.h"
#include"NPAnalysisFactory.h"
#include"RootInput.h"
#include"RootOutput.h"

// STL
#include<iostream>
#include<stdio.h>
#include<dlfcn.h>
#include<stdlib.h>

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

void ProgressDisplay(clock_t&,clock_t&,unsigned int&, unsigned int&, int&, double&, int&);

int main(int argc , char** argv){
  // command line parsing
  NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);
  string inputfilename = myOptionManager->GetRunToReadFile();
  // if input files are not given, use those from TAsciiFile
  if (myOptionManager->IsDefault("DetectorConfiguration")) {
    string name = RootInput::getInstance(inputfilename)->DumpAsciiFile("DetectorConfiguration");
    myOptionManager->SetDetectorFile(name);
    cout << "\033[1;33mWarning: No Detector file given, using Input tree one\033[0m"<<endl;;
  }

  if (myOptionManager->IsDefault("EventGenerator")) {
    string name = RootInput::getInstance(inputfilename)->DumpAsciiFile("EventGenerator");
    myOptionManager->SetReactionFile(name);
    cout << "\033[1;33mWarning: No Event file given, using Input tree one\033[0m"<<endl;;
  }

  // get input files from NPOptionManager
  string detectorfileName    = myOptionManager->GetDetectorFile();
  string OutputfileName      = myOptionManager->GetOutputFile();

  // Instantiate RootOutput
  string TreeName="NPTool_Tree";

  // User decided of the name
  if(!myOptionManager->IsDefault("TreeName")){
    TreeName=myOptionManager->GetOutputTreeName();
  }

  // Case of a Physics tree produced
  else if(!myOptionManager->GetInputPhysicalTreeOption()){ //
    TreeName="PhysicsTree";
    if(myOptionManager->IsDefault("OutputFileName"))
      OutputfileName="PhysicsTree";
  }
  
  // Case of Result tree produced
  else{
    TreeName="ResultTree";
    if(myOptionManager->IsDefault("OutputFileName"))
      OutputfileName="ResultTree";
  }

  RootOutput::getInstance("Analysis/"+OutputfileName,TreeName);
  TTree* tree= RootOutput::getInstance()->GetTree();

  // Instantiate the detector using a file
  NPA::DetectorManager* myDetector = new NPA::DetectorManager();
  myDetector->ReadConfigurationFile(detectorfileName);

  // Attempt to load an analysis
  NPA::VAnalysis* UserAnalysis = NULL;
  string libName = "./libNPAnalysis" + SHARED_LIB_EXTENSION;
  dlopen(libName.c_str(),RTLD_NOW);
  char* error = dlerror();
  if(error==NULL){
    UserAnalysis = NPA::AnalysisFactory::getInstance()->Construct(); 
    UserAnalysis->SetDetectorManager(myDetector);
    UserAnalysis->Init();
  }
  else{
    std::string str_error=error;
    if(str_error.find("image not found")!=std::string::npos ||str_error.find("No such file or directory")!=std::string::npos )
      std::cout << "\033[1;33m**** INFO: No User analysis found, building Physical tree ****\033[0m" << std::endl;
    else{
      std::cout << "\033[1;31m**** ERROR: Failure to load libNPAnalysis ****" << std::endl << error << "\033[0m" <<std::endl;
      exit(1);
    }
  }

  std::cout << std::endl << "///////// Starting Analysis ///////// "<< std::endl;
  TChain* Chain = RootInput:: getInstance()->GetChain();
  myOptionManager->GetNumberOfEntryToAnalyse();
  int nentries = Chain->GetEntries();
  if(nentries> myOptionManager->GetNumberOfEntryToAnalyse() && myOptionManager->GetNumberOfEntryToAnalyse()>0)
    nentries = myOptionManager->GetNumberOfEntryToAnalyse() ;  
  std::cout << " Number of Event to be treated : " << nentries << std::endl;

  unsigned int inter = 0;
  unsigned int treated = 0;
  double mean_rate =0;
  int displayed=0;
  clock_t end;
  clock_t begin = clock();

  bool IsPhysics = myOptionManager->GetInputPhysicalTreeOption();

  if(UserAnalysis==NULL){ 
    if(!IsPhysics){
      for (unsigned int i = 0 ; i < nentries; i++) { 
        // Get the raw Data
        Chain -> GetEntry(i);
        // Build the current event
        myDetector->BuildPhysicalEvent();
        // Fill the tree
        tree->Fill();
        ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed);
      }
    }
    
    else{
     cout << "\033[1;31m ERROR: You are requesting to rebuild a Physics Tree without any User Analysis, nothing to be done\033[0m" <<endl;
     exit(1);
    }
  }

  else{
    if(!IsPhysics){ 
      for (unsigned int i = 0 ; i < nentries; i++) { 
        // Get the raw Data
        Chain -> GetEntry(i);
        // Build the current event
        myDetector->BuildPhysicalEvent();
        // User Analysis
        UserAnalysis->TreatEvent();
        // Fill the tree      
        tree->Fill();
        ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed);
      }
    }

    else{
      for (unsigned int i = 0 ; i < nentries; i++) { 
        // Get the Physics Data
        Chain -> GetEntry(i);
        // User Analysis
        UserAnalysis->TreatEvent();
        // Fill the tree      
        tree->Fill();
        ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed);
      }
    }
    UserAnalysis->End();
  }

#if __cplusplus > 199711L
  myDetector->StopThread();
#endif
  ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed);
  if(myOptionManager->GetGenerateHistoOption())
    myDetector->WriteSpectra();
  
  RootOutput::getInstance()->Destroy();
  RootInput::getInstance()->Destroy();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ProgressDisplay(clock_t& begin, clock_t& end, unsigned int& treated,unsigned int& inter,int& total,double& mean_rate,int& displayed){
  end = clock();
  if((end-begin)>CLOCKS_PER_SEC||treated>=total ){
    displayed++;
    long double elapsed =(long double) (end-begin)/CLOCKS_PER_SEC;
    double event_rate = inter/elapsed;
    mean_rate += (event_rate-mean_rate)/(displayed);
    double percent = 100*treated/total;
    double remain = (total-treated)/mean_rate;

    char* timer;
    double check;
    if(remain>60)
      check=asprintf(&timer,"%dmin",(int)(remain/60.));
    else
      check=asprintf(&timer,"%ds",(int)(remain));

    if(treated!=total)
      printf("\r \033[1;31m ******* Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s *******\033[0m", percent,mean_rate/1000.,timer);

    else{
      printf("\r                                                                                                                    ");  
      printf("\r \033[1;32m ******* Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s *******\033[0m", percent,mean_rate/1000.,timer);
    }
    fflush(stdout);
    inter=0;
    begin = clock() ;
  }

  treated++;
  inter++;
}
