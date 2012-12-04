#include "Analysis.h"


int main(int argc,char** argv)
{	
   RootOutput::getInstance("Analysis/Must2_AnalysedData", "AnalysedTree");

   // get input files from NPOptionManager
   string reactionfileName    = myOptionManager->GetReactionFile();
   string detectorfileName    = myOptionManager->GetDetectorFile();
   string OutputfileName      = myOptionManager->GetOutputFile();

   // Instantiate some Reaction
   cout << endl << "/////////// Event generator ///////////" << endl;
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);
    
   // Instantiate the detector using a file 
   NPA::DetectorManager* myDetector = new DetectorManager();
   myDetector->ReadConfigurationFile(detectorfileName);

   // Calculate beam energy at target middle
   // Target informations
   cout << endl;
   cout << "/////////// Target information ///////////" << endl;
   cout << "Thickness (um): " << myDetector->GetTargetThickness() << endl;
   // Get nominal beam energy
   Double_t BeamEnergyNominal = myReaction->GetBeamEnergy() * MeV;
   cout << "Nominal beam energy (MeV): " << BeamEnergyNominal << endl;
   // Slow beam at target middle
   Double_t BeamEnergy = Fe60TargetCD2.Slow(BeamEnergyNominal, myDetector->GetTargetThickness()/2 * micrometer, 0);
   cout << "Middle-target beam energy (MeV): " << BeamEnergy << endl;
   // Set energy beam at target middle
   myReaction->SetBeamEnergy(BeamEnergy);
   myReaction->Print();
      
   // Get the formed Chained Tree and Treat it
   TChain* Chain = RootInput::getInstance()->GetChain();
      
   // Connect TInitialConditions branch
   TInitialConditions *initCond = 0;
   Chain->SetBranchAddress("InitialConditions", &initCond);
            
   cout << endl << "///////// Starting Analysis ///////// "<< endl;
   int nentries = Chain->GetEntries();
   cout << " Number of Event to be treated : " << nentries << endl;
   clock_t begin = clock();
   clock_t end = begin;


   for (int i = 0; i < nentries; i++) {
	   if (i%10000 == 0 && i!=0) 	{	
	      cout.precision(5);
	      end = clock();										
	      double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
	      double percent = (double)i/nentries ;
	      double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
	      cout  << "                                                                                                "<< flush;
	      cout	<< "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i==nentries-1) 	cout << "\r Progression:" << " 100% " <<endl;
	  
      // Clear previous event
     
      // Get new raw data
      Chain->GetEntry(i);

     // Loop over hit
      for (int hit = 0; hit < M2->Si_E.size(); hit++) {
      } // end of for loop over hit
	      
      RootOutput::getInstance()->GetTree()->Fill();      
   } // end of for loop over events
      
      
   cout << " A total of " << nentries << " event has been analysed " << endl ;
   cout << endl << " ///////////////////////////////////// "<< endl<< endl ;
   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();
   
   return 0;
}

