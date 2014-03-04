#include "Analysis.h"
#include<bitset>
using namespace std;
#include "TTriggerData.h"

int main(int argc, char** argv){
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
  double EGammaDC[4];
  RootOutput::getInstance()->GetTree()->Branch("EGammaDC", &EGammaDC,"EGammaDC[4]/D");
  double GATCONF[16];
  RootOutput::getInstance()->GetTree()->Branch("GATCONF",&GATCONF,"GATCONF[16]/I");
  float beta=0.18441; //This beta is used in "EGammaDC" ----- the beta used in "DopplerCorrectedEnergy" is definded in ./config/beta.txt  !!!!!!!!!

  // Get the formed Chained Tree and Treat it
  TChain* Chain = RootInput:: getInstance() -> GetChain();
  TExogamPhysics *Exo 	 = (TExogamPhysics*)  myDetector -> GetDetector("EXOGAM")	;

  // Branching Gatconf
  TTriggerData* trigger = new TTriggerData();
  Chain->SetBranchStatus("TRIG", true); 
  Chain->SetBranchStatus("fTRIG_1", true);
  Chain->SetBranchStatus("fTRIG_2", false);
  Chain->SetBranchStatus("fTRIG_3", false);
  Chain->SetBranchStatus("fTRIG_4", false);
  Chain->SetBranchStatus("fTRIG_5", false);
  Chain->SetBranchAddress("TRIG", &trigger);
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
    int gatconf_integer = trigger->GetTRIG1();
    // Create a bitset of 16 bit,
    // each bit is set to one if the corresponding GMT entry has been fired
    bitset<16> gatconf(gatconf_integer);     

    // Fill the GATCONF array:
    for(unsigned int i = 0 ; i < 16 ; i++)
       GATCONF[i]=gatconf[i];
    


    // example of usage:
    // Let say I want to perform an action if TiaraBarrel has been fired
    // and that Tiara Barrel is wired to entry 8 of GATCONF
    // Then I just have to take the entry 8th element (index 7) of the Gatconf
    // array
    if(GATCONF[7])
      cout<<"Entry 8 of GMT has been fired" << endl;

    myDetector->ClearEventPhysics();
    myDetector->BuildPhysicalEvent();

    /************************************************/

    for(int i=0; i<4; i++) 
      EGammaDC[i]=0;
    
    for(int i=0; i<Exo->TotalEnergy_lab.size(); i++){
      float mytheta=Exo->GetSegmentAngleTheta(Exo->CloverNumber[i], Exo->CristalNumber[i], Exo->SegmentNumber[i]);
      float myphi=Exo->GetSegmentAnglePhi(Exo->CloverNumber[i], Exo->CristalNumber[i], Exo->SegmentNumber[i]);	
      //myv(Exo->TotalEnergy_lab[i]*sin(mytheta)*sin(myphi),Exo->TotalEnergy_lab[i]*sin(mytheta)*cos(myphi) , Exo->TotalEnergy_lab[i]*cos(mytheta),(Exo->TotalEnergy_lab)[i]); 
      //cout<<Exo->TotalEnergy_lab[i]<<endl;
      EGammaDC[i]=Exo->TotalEnergy_lab[i]*(1-beta*cos(mytheta*TMath::DegToRad()))/sqrt(1-beta*beta);
      //TVector3 boost= imp4.BoostVector(); 
      //myv.Boost(-boost);
      //DopplerCorrectedEnergy.push_back(myv.T());
      //h_DopplerCorrectedEnergy->Fill(myv.T());
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
