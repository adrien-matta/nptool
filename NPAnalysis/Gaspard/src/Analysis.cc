#include "ObjectManager.hh"

using namespace std;


int main(int argc,char** argv)
{	
   // test if number of arguments is correct
   if (argc != 4) {
      cout << 
         "you need to specify both a Reaction file and a Detector file such as : Analysis myReaction.reaction myDetector.detector runToRead.run" 
           << endl;
      return 0;
   }

   // get arguments
   string reactionfileName  = argv[1];
   string detectorfileName  = argv[2];
   string runToReadfileName = argv[3];

   // Instantiate RootInput and RootOutput singleton classes
   RootInput:: getInstance(runToReadfileName);
   RootOutput::getInstance("Analysis/Gaspard_AnalyzedData", "AnalyzedTree");

   // Initialize the reaction
   NPL::Reaction* myReaction = new Reaction();
   myReaction->ReadConfigurationFile(reactionfileName);

   // Initialize the detector
   DetectorManager* myDetector = new DetectorManager;
   myDetector	->	ReadConfigurationFile(detectorfileName);

   // Attach more branch to the output
   double Ex = 0 ; double EE = 0 ; double TT = 0 ; double X = 0 ; double Y = 0 ; int det ;
   RootOutput::getInstance()->GetTree()->Branch("ExcitationEnergy",&Ex,"Ex/D") ;
   RootOutput::getInstance()->GetTree()->Branch("E",&EE,"EE/D") ;
   RootOutput::getInstance()->GetTree()->Branch("A",&TT,"TT/D") ;
   RootOutput::getInstance()->GetTree()->Branch("X",&X,"X/D") ;
   RootOutput::getInstance()->GetTree()->Branch("Y",&Y,"Y/D") ;

   // Get GaspardTracker pointer
   GaspardTracker* GPDTrack = (GaspardTracker*) myDetector->m_Detector["GASPARD"];

   // Get the TChain and treat it
   TChain* chain = RootInput:: getInstance() -> GetChain();

   // Analysis is here!
   int nentries = chain->GetEntries();
   cout << "Number of entries to be analysed: " << nentries << endl;

   for (int i = 0; i < nentries; i ++) {
      if (i%10000 == 0 && i!=0) cout << i << " analysed events" << endl;
      chain -> GetEntry(i);

      // Treat Gaspard event
      myDetector->ClearEventPhysics();
      myDetector->BuildPhysicalEvent();

      // Get total energy and coordinates of interaction
      double   E = GPDTrack->GetEnergyDeposit();
      TVector3 A = GPDTrack->GetPositionOfInteraction();

      // Calculate scattering angle
      double Theta = ThetaCalculation (A ,TVector3(0,0,1));

      // Calculate excitatioin energy
      if (E > -1000) Ex = myReaction->ReconstructRelativistic(E, Theta);
      else Ex = -100;

      EE = E ; TT = Theta/deg;
      if (E>-1000) {
         X = A . X();
         Y = A . Y();
      }
      else {
         X = -1000 ; Y = -1000;
      }

      // Fill output tree
      RootOutput::getInstance()->GetTree()->Fill();
   }

   // delete singleton classes
   RootOutput::getInstance()->Destroy();
   RootInput::getInstance()->Destroy();

   return 0;
}


double ThetaCalculation (TVector3 A , TVector3 B)
{
   double Theta = acos( (A.Dot(B)) / (A.Mag()*B.Mag()) );
   return Theta ;
}

