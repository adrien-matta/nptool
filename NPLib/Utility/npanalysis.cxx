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
#include<unistd.h>
#include<sys/time.h>

// Root
#include"TKey.h"
#include"TEnv.h" 
#include"TROOT.h"
void ProgressDisplay(clock_t&,clock_t&,unsigned long&, unsigned long&, unsigned long&, double&, unsigned long&, int&, int&);
void ProgressDisplay(struct timeval& begin, struct timeval& end, unsigned long& treated,unsigned long& inter,unsigned long& total,double& mean_rate,unsigned long& displayed, int& current_tree, int& total_tree);
////////////////////////////////////////////////////////////////////////////////
int main(int argc , char** argv){
  // command line parsing
  NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);
  std::string inputfilename = myOptionManager->GetRunToReadFile();
  // if input files are not given, use those from TAsciiFile
  if (myOptionManager->IsDefault("DetectorConfiguration")) {
    std::string name = RootInput::getInstance(inputfilename)->DumpAsciiFile("DetectorConfiguration");
    if(name!="fail"){
      myOptionManager->SetDetectorFile(name);
     std::cout << "\033[1;33mInfo: No Detector file given, using Input tree one \033[0m" << std::endl;;
    }  
  }

  if (myOptionManager->IsDefault("EventGenerator")) {
    std::string name = RootInput::getInstance(inputfilename)->DumpAsciiFile("EventGenerator");
    if(name!="fail"){
    myOptionManager->SetReactionFile(name);
    std::cout << "\033[1;33mInfo: No Event file given, using Input tree one \033[0m" << std::endl;;
    }
  }

  // get input files from NPOptionManager
  std::string detectorfileName    = myOptionManager->GetDetectorFile();
  std::string OutputfileName      = myOptionManager->GetOutputFile();

  // Instantiate RootOutput
  std::string TreeName="NPTool_Tree";

  // User decided of the name
  if(!myOptionManager->IsDefault("TreeName")){
    TreeName=myOptionManager->GetOutputTreeName();
  }

  // Case of a Physics tree produced
  else if(!myOptionManager->GetInputPhysicalTreeOption()){ 
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
  NPL::DetectorManager* myDetector = new NPL::DetectorManager();
  myDetector->ReadConfigurationFile(detectorfileName);
  myDetector->InitializeRootInput();
  myDetector->InitializeRootOutput();
  // Attempt to load an analysis
  NPL::VAnalysis* UserAnalysis = NULL;
  std::string libName = "./libNPAnalysis" + myOptionManager->GetSharedLibExtension();
  dlopen(libName.c_str(),RTLD_NOW | RTLD_GLOBAL);
  char* error = dlerror();
  if(error==NULL){
    UserAnalysis = NPL::AnalysisFactory::getInstance()->Construct(); 
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

  if(myOptionManager->GetOnline()){
    // Request Detector manager to give the Spectra to the server
    myDetector->SetSpectraServer(); 
  }

  std::cout << std::endl << "///////// Starting Analysis ///////// "<< std::endl;
  TChain* Chain = RootInput:: getInstance()->GetChain();
  myOptionManager->GetNumberOfEntryToAnalyse();

	unsigned long first_entry = myOptionManager->GetFirstEntryToAnalyse(); // defaults to zero
  unsigned long nentries = Chain->GetEntries();
  if(nentries> myOptionManager->GetNumberOfEntryToAnalyse() && myOptionManager->GetNumberOfEntryToAnalyse()>0)
    nentries = myOptionManager->GetNumberOfEntryToAnalyse() ;
	if(nentries + first_entry > Chain->GetEntries()) {nentries = first_entry+Chain->GetEntries();}
	

  TString ChainName = Chain->GetName();
  std::cout << " Number of Event to be treated : " << nentries << " on chain " << ChainName << std::endl;

  unsigned long inter = 0;
  unsigned long treated = 0;
  double mean_rate =0;
  unsigned long displayed=0;
  clock_t end;
  clock_t begin = clock();
  struct timeval tv_end;
  struct timeval tv_begin; gettimeofday(&tv_begin,NULL);
  unsigned long new_nentries = 0 ;
  int current_tree = 0 ;
  int total_tree = Chain->GetNtrees();

  bool IsPhysics = myOptionManager->GetInputPhysicalTreeOption();

  if(UserAnalysis==NULL){ 
    if(!IsPhysics){
      for (unsigned long i = first_entry ; i < nentries + first_entry; i++) { 
        // Get the raw Data
        Chain -> GetEntry(i);
        // Build the current event
        myDetector->BuildPhysicalEvent();
        // Fill the tree
        tree->Fill();

        current_tree = Chain->GetTreeNumber()+1;
        //ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        ProgressDisplay(tv_begin,tv_end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        if(myOptionManager->GetOnline() && i%10000==0){
          myDetector->CheckSpectraServer();

          bool first = true;
          if(!Chain || first){
            first = false;
           RootInput::getInstance()->GetFile()->ReadKeys(kTRUE);

            Chain = (TChain*)  RootInput::getInstance()->GetFile()->FindKeyAny(ChainName)->ReadObj();    
            new_nentries = Chain->GetEntries();
            if(new_nentries!=nentries){
              RootInput::getInstance()->SetChain(Chain);
              myDetector->InitializeRootInput();
              nentries = Chain->GetEntries();
            }
            else{
              first = true;
            }
          }
        }
      }
    }

    else{
      std::cout << "\033[1;31m ERROR: You are requesting to rebuild a Physics Tree without any User Analysis, nothing to be done\033[0m" << std::endl;
      // Quit without error
      exit(0);
    }
  }

  else{
    if(!IsPhysics){ 
      for (unsigned long i = first_entry ; i < nentries + first_entry; i++) { 
        // Get the raw Data
        Chain -> GetEntry(i);
        // Build the current event
        myDetector->BuildPhysicalEvent();
        // User Analysis
        UserAnalysis->TreatEvent();
        // Fill the tree      
        tree->Fill();
      
        current_tree = Chain->GetTreeNumber()+1;
        //ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        ProgressDisplay(tv_begin,tv_end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        if(myOptionManager->GetOnline() && i%10000==0){
          myDetector->CheckSpectraServer();
          bool first = true;
          while(!Chain || first){
            first = false;
            RootInput::getInstance()->GetFile()->ReadKeys(kTRUE);

            Chain = (TChain*)  RootInput::getInstance()->GetFile()->FindKeyAny(ChainName)->ReadObj();    
            new_nentries = Chain->GetEntries();
            if(new_nentries!=nentries){
              RootInput::getInstance()->SetChain(Chain);
              myDetector->InitializeRootInput();
              nentries = Chain->GetEntries();
            }
            else{
              first = true;
            }
          }
        }      
      }
    }

    else{
			for (unsigned long i = first_entry ; i < nentries + first_entry; i++) { 
        // Get the Physics Data
        Chain -> GetEntry(i);
        // User Analysis
        UserAnalysis->TreatEvent();
        // Fill the tree      
        tree->Fill();

        current_tree = Chain->GetTreeNumber()+1;
        //ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        ProgressDisplay(tv_begin,tv_end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);
        if(myOptionManager->GetOnline() && i%10000==0){
          myDetector->CheckSpectraServer();

          bool first = true;
          while(!Chain || first){
            first = false;
            RootInput::getInstance()->GetFile()->ReadKeys(kTRUE);

            Chain = (TChain*)  RootInput::getInstance()->GetFile()->FindKeyAny(ChainName)->ReadObj();    
            new_nentries = Chain->GetEntries();
            if(new_nentries!=nentries){
              RootInput::getInstance()->SetChain(Chain);
              myDetector->InitializeRootInput();
              nentries = Chain->GetEntries();
            }
            else{
              first = true;
            }
          }
        }      
      }
    }
    UserAnalysis->End();
  }

#if __cplusplus > 199711L && NPMULTITHREADING
  myDetector->StopThread();
#endif

  current_tree = Chain->GetTreeNumber()+1;
  //ProgressDisplay(begin,end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);

  ProgressDisplay(tv_begin,tv_end,treated,inter,nentries,mean_rate,displayed,current_tree,total_tree);

  if(myOptionManager->GetOnline()){
    myDetector->CheckSpectraServer(); 
  }


  if(myOptionManager->GetGenerateHistoOption())
    myDetector->WriteSpectra();

  RootOutput::Destroy();
  RootInput::Destroy();
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
void ProgressDisplay(struct timeval& begin, struct timeval& end, unsigned long& treated,unsigned long& inter,unsigned long& total,double& mean_rate,unsigned long& displayed, int& current_tree, int& total_tree){
  gettimeofday(&end,NULL);
  long double elapsed= end.tv_sec-begin.tv_sec+(end.tv_usec-begin.tv_usec)*1e-6;
  if(elapsed>1||treated>=total ){
    displayed++;
    double event_rate = inter/elapsed;
    mean_rate += (event_rate-mean_rate)/(displayed);
    double percent = 100*treated/total;
    double remain = (total-treated)/mean_rate;

    char* timer;
    double check;
    int minutes = remain/60.;
    int seconds = remain -60*minutes;
    if(remain>60)
      check=asprintf(&timer,"%dmin%ds",minutes,seconds);
    else
      check=asprintf(&timer,"%ds",(int)(remain));
    
    static char star[10];
    if(displayed%2==0 || treated==total)
      sprintf(star,"*******");
    else
      sprintf(star,"-------");

    if(treated!=total)
      printf("\r \033[1;31m %s Progress: \033[1;36m%.1f%% \033[1;31m| Rate: %.1fk evt/s | Remain: %s | Tree: %d/%d %s   \033[0m         ", star,percent,mean_rate/1000.,timer, current_tree,total_tree,star);

    else{
      printf("\r \033[1;32m %s Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s | Tree: %d/%d %s   \033[0m         ", star,percent,mean_rate/1000.,timer, current_tree, total_tree,star);
    }
    fflush(stdout);
    inter=0;
    gettimeofday(&begin,NULL);
  }

  treated++;
  inter++;
}
////////////////////////////////////////////////////////////////////////////////
void ProgressDisplay(clock_t& begin, clock_t& end, unsigned long& treated,unsigned long& inter,unsigned long& total,double& mean_rate,unsigned long& displayed, int& current_tree, int& total_tree){
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
    int minutes = remain/60.;
    int seconds = remain -60*minutes;
    if(remain>60)
      check=asprintf(&timer,"%dmin%ds",minutes,seconds);
    else
      check=asprintf(&timer,"%ds",(int)(remain));
    
    static char star[10];
    if(displayed%2==0 || treated==total)
      sprintf(star,"*******");
    else
      sprintf(star,"-------");

    if(treated!=total)
      printf("\r \033[1;31m %s Progress: \033[1;36m%.1f%% \033[1;31m| Rate: %.1fk evt/s | Remain: %s | Tree: %d/%d %s   \033[0m         ", star,percent,mean_rate/1000.,timer, current_tree,total_tree,star);

    else{
      printf("\r \033[1;32m %s Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s | Tree: %d/%d %s   \033[0m         ", star,percent,mean_rate/1000.,timer, current_tree, total_tree,star);
    }
    fflush(stdout);
    inter=0;
    begin = clock() ;
  }

  treated++;
  inter++;
}
