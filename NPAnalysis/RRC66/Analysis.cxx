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
   string calibrationfileName = myOptionManager->GetCalibrationFile();
   string OutputfileName      = myOptionManager->GetOutputFile();
   
   // Instantiate RootOutput
   RootOutput::getInstance("Analysis/"+OutputfileName, "AnalysedTree");
   
   // Instantiate the detector using a file
   NPA::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput:: getInstance() -> GetChain();

   ////////////////////////////////////////////////////////
   // Load Value for Thickness correction in SSSD
   // Array of Correction for the SSSD
   vector< vector< vector<double> > > CorrectionMust2;
   vector< vector< vector<double> > > CorrectionSSSD;

   string buffer;
   double BufferMust2, BufferSSSD;
   vector<double> LineBufferMust2,LineBufferSSSD;
   vector< vector<double> > BufferCorrectionMust2,BufferCorrectionSSSD;
   ifstream correction_file;

   for(int k = 0 ; k < 4 ; k++)
      {   
         BufferCorrectionMust2.clear();
         BufferCorrectionSSSD.clear();
         TString filename = Form("Si%d_ThicknessCorrection.dat", k+1);
         cout << " Loading " << filename << endl ;
         correction_file.open(filename.Data());
         
         for(int i = 0 ; i < 128 ; i++)
            {
              LineBufferMust2.clear();LineBufferSSSD.clear();
              for(int j = 0 ; j<  128 ; j++)
                {
                  correction_file >> buffer >> buffer >> BufferSSSD >> BufferMust2;
                  LineBufferMust2.push_back(BufferMust2);
                  LineBufferSSSD.push_back(BufferSSSD);
                }

              BufferCorrectionMust2.push_back(LineBufferMust2);
              BufferCorrectionSSSD.push_back(LineBufferSSSD);
            }
         correction_file.close();
         CorrectionMust2.push_back(BufferCorrectionMust2);
         CorrectionSSSD.push_back(BufferCorrectionSSSD);
      }

   // Attach new branch
   InitOutputBranch();
   
   ////////////////////////////////////////////////////////
   
   // Get pointer to the different detector
   
   TMust2Physics* M2  = (TMust2Physics*)    myDetector -> GetDetector("MUST2");
   TSSSDPhysics* SSSD = (TSSSDPhysics*)    myDetector -> GetDetector("SSSD");

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

      /************************************************/
      //Part 1 : Thickness Correction
      Energy_Must2 = -1 ; Energy_SSSD  = -1;
      if(M2->Si_E.size()==1)
         {
            for(unsigned int countMust2 = 0 ; countMust2 < M2->Si_E.size() ; countMust2++){
               for(unsigned int countSSSD = 0 ; countSSSD < SSSD->Energy.size() ; countSSSD++){
                  if(M2->TelescopeNumber[countMust2]==SSSD->DetectorNumber[countSSSD]){
                     Energy_Must2 = M2->Si_E[countMust2] + CorrectionMust2[M2->TelescopeNumber[countMust2]-1][M2->Si_X[countMust2]-1][M2->Si_Y[countMust2]-1];
                     Energy_SSSD = SSSD->Energy[countSSSD] + CorrectionSSSD[M2->TelescopeNumber[countMust2]-1][M2->Si_X[countMust2]-1][M2->Si_Y[countMust2]-1];
                     }
               }
            }
         }
     /************************************************/
      RootOutput::getInstance()->GetTree()->Fill();
   }

   cout << "A total of " << nentries << " event has been annalysed " << endl ;

   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();

   return 0 ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void InitOutputBranch() 
   {
      RootOutput::getInstance()->GetTree()->Branch("Energy_Must2",&Energy_Must2,"Energy_Must2/F");
      RootOutput::getInstance()->GetTree()->Branch("Energy_SSSD",&Energy_SSSD,"Energy_SSSD/F");
   }
