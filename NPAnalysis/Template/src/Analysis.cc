#include "ObjectManager.hh"

using namespace std;

int main(int argc, char** argv)
{
   // Parse command line
   NPOptionManager* myOptionManager = NPOptionManager::getInstance(argc, argv);
   string detectorfileName          = myOptionManager->GetDetectorFile();
   string reactionfileName          = myOptionManager->GetCalibrationFile();
   string calibrationfileName       = myOptionManager->GetCalibrationFile();
   string runToReadfileName         = myOptionManager->GetRunToReadFile();

   // First of All instantiate RootInput and Output
   // Detector will be attached later
   RootInput:: getInstance(runToReadfileName);
   RootOutput::getInstance("Analysis/Template_AnalyzedData", "AnalyzedTree");

   // Instantiate a Reaction
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Instantiate the detector using a file
   NPA::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);

   // Instantiate the Calibration Manger using a file
   CalibrationManager* myCalibration = CalibrationManager::getInstance(calibrationfileName);

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
   return 0 ;
}
