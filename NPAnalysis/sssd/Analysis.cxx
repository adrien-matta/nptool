#include "Analysis.h"
using namespace std;

int main(int argc, char** argv)
{
   // command line parsing
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc,argv);

   // Instantiate RootInput
   string runToReadfileName = myOptionManager->GetRunToReadFile();
   RootInput:: getInstance(runToReadfileName);

   // if input files are not given, use those from TAsciiFile
   if (myOptionManager->IsDefault("DetectorConfiguration")) {
      string name = RootInput::getInstance()->DumpAsciiFile("DetectorConfiguration");
      myOptionManager->SetDetectorFile(name);
   }

   // get input files from NPOptionManager
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string OutputfileName      = myOptionManager->GetOutputFile();
   
   // Instantiate RootOutput
   RootOutput::getInstance("Analysis/"+OutputfileName, "AnalysedTree");
   
   // Instantiate the detector using a file
   NPA::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput:: getInstance() -> GetChain();

   // Get number of events to treat
   cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;

   // main loop on entries
   for (int i = 0; i < nentries; i++) {
      if (i%10000 == 0 && i!=0)  {
         cout.precision(5);
         end = clock();
         double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
         double percent = (double)i/nentries;
         double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
         cout  << "                                                                                                "<< flush;
         cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i == nentries-1)  cout << "\r Progression:" << " 100% " <<endl;

      // get data
      Chain -> GetEntry(i);

      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      /************************************************

      Put your code here

      ************************************************/
      RootOutput::getInstance()->GetTree()->Fill();
   }

   cout << "A total of " << nentries << " event has been annalysed " << endl ;

   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();

   return 0 ;
}
